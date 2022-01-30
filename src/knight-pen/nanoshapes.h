#pragma once

#include <cmath>

#include "qnanopainter.h"
#include "nanopen.h"

struct pathShape;
struct ellipseShape;
struct lineShape;
struct rectShape;

namespace knightPen {

enum PointProxy {
    Center,
    TopLeft,
    Top,
    TopRight,
    Right,
    BottomRight,
    Bottom,
    BottomLeft,
    Left,
};

enum PointState {
    Inside,
    Edge,
    None
};

/**
 * @brief The nanoPoint struct
 * represent selectable anchor point.
 */
class APointF : public QPointF {
public:
    constexpr APointF(const QPointF &p = QPointF(),
        const QPointF &cp1 = QPointF(),
        const QPointF &cp2 = QPointF())
        : QPointF(p), mControl1(cp1), mControl2(cp2), mSelected(false) {}
    constexpr APointF(const APointF &p)
        : QPointF(p.x(), p.y()),
          mControl1(p.mControl1),
          mControl2(p.mControl2),
          mSelected(p.mSelected) {}
    constexpr APointF(float x, float y,
        bool selected = false,
        const QPointF &cp1 = QPointF(),
        const QPointF &cp2 = QPointF())
        : QPointF(x, y), mControl1(cp1), mControl2(cp2), mSelected(selected) {}

    const APointF &operator =(const APointF &in) {
        setX(in.x());
        setY(in.y());
        mControl2 = in.mControl2;
        mControl1 = in.mControl1;
        mSelected = in.mSelected;
        return *this;
    }

    const APointF &operator =(const QPointF &in) {
        setX(in.x());
        setY(in.y());
        return *this;
    }

    bool isBeizer() const {
        return (mControl1.isNull() && mControl1.isNull()) == false;
    }

    APointF transformed(const QTransform &transform) const {
        APointF result = transform.map(*this);
        result.mControl1 = transform.map(mControl1);
        result.mControl2 = transform.map(mControl2);
        result.mSelected = mSelected;
        return result;
    }

    const APointF &transform(const QTransform &transform) {
        *this = transformed(transform);
        return *this;
    }

    APointF &rotate(float angle, const QPointF &origin = QPointF(0.0, 0.0)) {
        APointF::rotate(*this, angle, origin);
        return *this;
    }

    static QPointF rotate(const QPointF &p, float angle, const QPointF &origin = QPointF(0.0, 0.0)) {
        QTransform transform = QTransform()
                                   .translate(origin.x(), origin.y())
                                   .rotate(angle)
                                   .translate(-origin.x(), -origin.y());
        return transform.map(p);
    }

    QPointF control1() const { return mControl1; }
    QPointF control2() const { return mControl2; }
    bool selected() const { return mSelected; }

    void setControl1(QPointF newControl1) { mControl1 = newControl1; }
    void setControl2(QPointF newControl2) { mControl2 = newControl2; }
    void setSelected(bool newSelected) { mSelected = newSelected; }

private:
    QPointF mControl1, mControl2;  // Control points 1 & 2
    bool mSelected;
};

class shape {
public:
    enum ShapeType {
        Shape,
        Path,
        Line,
        Polygon,
        Ellipse,
        Rectangle,
    };

    shape() = default;

    constexpr shape(const QRectF &rect,
        const nanoPen &pen = nanoPen(),
        const bool &selected = false,
        float angle = 0.0f)
        : mBoundingBox(rect),
          mPen(pen),
          mSelected(selected),
          mAngle(angle){}

    constexpr shape(const QPointF p1,
        const QPointF p2,
        const nanoPen &pen = nanoPen(),
        const bool &selected = false,
        float angle = 0.0f)
        : shape(QRectF(p1, p2), pen, selected, angle) {}

    constexpr shape(const nanoPen &pen,
        const bool &selected = false,
        float angle = 0.0f)
        : shape(QRectF(), pen, selected, angle) {}

    QTransform transformer() const {
        QPointF c = mBoundingBox.center();
        return QTransform()
            .translate(c.x(), c.y())
            .rotate(mAngle)
            .translate(-c.x(), -c.y());
    }

    QTransform revTransformer() const {
        QPointF c = mBoundingBox.center();
        return QTransform()
            .translate(c.x(), c.y())
            .rotate(-mAngle)
            .translate(-c.x(), -c.y());
    }

    std::array<QPointF, 8> boundingPoints() {
        const QRectF &bp = mBoundingBox;
        std::array<QPointF, 8> boundPoints(
            {bp.topLeft(), bp.topRight(), bp.bottomRight(), bp.bottomLeft()});
        QTransform t = transformer();
        for(auto &point : boundPoints){
            point  = t.map(point);
        }
        return boundPoints;
    }

    /// getters
    bool selected() const { return mSelected; }
    float angle() const { return mAngle; }
    const QRectF &boundingBox() const { return mBoundingBox; }
    const nanoPen &pen() const { return mPen; }

    virtual PointState contains(const QPointF &point) const { Q_UNUSED(point) return PointState::None; }
    virtual ShapeType type() const { return ShapeType::Shape; }

    /// setters
    void setSelected(bool newSelected) { mSelected = newSelected; }
    void setAngle(float newAngle) { mAngle = newAngle; }
    void setPen(const nanoPen &newPen) { mPen = newPen; }

    virtual void setBoundingBox(const QRectF &boundingBox) { mBoundingBox = boundingBox; }
    virtual bool isNull() const { return mBoundingBox.isNull(); }
    virtual const QRectF &updateBoundingBox() { return mBoundingBox; }

protected:
    QRectF mBoundingBox;
    nanoPen mPen;
    bool mSelected;
    float mAngle;
};

/**
 * @brief The freePath struct
 * NOTE: what if every thing was pathShape? ...
 * TODO: use https://gitlab.com/inkscape/inkscape/-/tree/master/src/path
 */
class pathShape : public shape {
public:
    /**
     * @brief The Arrows enum
     * line arrows.
     * start and end arrows can bind together. (i.e. StartCircle | EndFill)
     */
    enum Arows {
        None = 0x0,

        StartFill = 0x1,
        StartAngle = 0x2,
        StartOutline = 0x3,
        StartCircle = 0x4,
        StartLine = 0x5,

        EndFill = 0x16,
        EndAngle = 0x32,
        EndOutline = 0x48,
        EndCircle = 0x64,
    };

    pathShape(const nanoPen &pen = nanoPen(), bool closed = false)
        : shape(pen), mPointSeries({}), mClosed(closed), mArrows(pathShape::Arows::None) {}
    pathShape(const QLineF &line, const nanoPen &pen = nanoPen())
        : shape(line.p1(), line.p2(), pen),
          mPointSeries({line.p1(), line.p2()}),
          mClosed(false),
          mArrows(pathShape::Arows::None) {}
    pathShape(const QPointF &startPoint, const nanoPen &pen = nanoPen())
        : shape(pen),
          mPointSeries({startPoint}),
          mClosed(false),
          mArrows(pathShape::Arows::None) {}

    APointF &operator[](size_t index) { return mPointSeries.at(index); }
    const APointF &operator[](size_t index) const { return mPointSeries.at(index); }

    std::vector<APointF>::iterator begin() {
        return mPointSeries.begin();
    }

    std::vector<APointF>::iterator end() {
        return mPointSeries.end();
    }

    const QRectF &updateBoundingBox() override {
        if(mPointSeries.empty() == true)
            return mBoundingBox;
        mBoundingBox.setRect(center().x(), center().y(), 0.0, 0.0);
        for(auto &p : mPointSeries) {
            p.transform(transformer());
            mBoundingBox.setRect(std::min(mBoundingBox.left(), p.x()),
                                 std::min(mBoundingBox.top(), p.y()),
                                 std::max(mBoundingBox.right(), p.x()),
                                 std::max(mBoundingBox.bottom(), p.y()));
        }
        return mBoundingBox;
    }

    /**
     * @brief resetBoundingBox
     * Sets angle to zero, and changes points and thier controls depend on shape rotation.
     * NOTE: only pathShape can reset it's bounding box.
     */
    void resetBoundingBox() {
        if(mAngle != 0) {
            for(auto &point : mPointSeries) {
                point.transform(transformer());
            }
        }
        mAngle = 0;
        updateBoundingBox();
    }

    ShapeType type() const override { return ShapeType::Path; }
    /**
     * @brief isNull
     * smae as empty.
     */
    bool isNull() const override { return mPointSeries.empty(); }
    /**
     * TODO: Add following functions:
     * PointState contains(const QPointF &point) const override { return PointState::None; }
     * void setBoundingBox(const QRectF &boundingBox) override {}
     */

    void clear() { mPointSeries.clear(); }
    void pushPoint(const APointF &point) { mPointSeries.push_back(point); }

    /// setters
    void setPointSeries(const std::vector<APointF> &newPointSeries) {
        mPointSeries = newPointSeries;
    }
    void setClosed(bool newClosed) { mClosed = newClosed; }
    void setArrow(pathShape::Arows newArrows) { mArrows = newArrows; }

    /// getters
    bool empty() const { return mPointSeries.empty(); }
    bool closed() const { return mClosed; }
    bool isSinglePoint() const { return size() == 1; }
    pathShape::Arows arrow() const { return mArrows; }
    const std::vector<APointF> &pointSeries() const { return mPointSeries; }
    QPointF center() const { return mBoundingBox.center(); }
    size_t size() const { return mPointSeries.size(); }

private:
    std::vector<APointF> mPointSeries;
    bool mClosed;
    pathShape::Arows mArrows;
};

/**
 * @brief The ellipse struct
 */
class ellipseShape : public shape {
public:
    ellipseShape(const QPointF &center = QPointF(0.0, 0.0),
        const QSizeF &radius = QSizeF(0.0, 0.0),
        const nanoPen &pen = nanoPen())
        : shape(QRectF(center - QPointF(radius.width(), radius.height()), radius * 2), pen),
          mCenter(center),
          mRadius(radius),
          mSelectedAnchors({}) {}
    ellipseShape(const QRectF &rect, const nanoPen &pen = nanoPen())
        : shape(rect, pen),
          mCenter(rect.center()),
          mRadius(rect.size() / 2),
          mSelectedAnchors({}) {}
    ellipseShape(const QPointF &center,
        float radius = 1,
        const nanoPen &pen = nanoPen())
        : ellipseShape(center, QSizeF(radius, radius), pen) {}
    ellipseShape(const QPointF &topLeft,
        const QPointF &bottomRight,
        const nanoPen &pen = nanoPen())
        : ellipseShape(QRectF(topLeft, bottomRight), pen) {}

    /**
     * TODO: Add following functions:
     * operator = () const {}
     */

    operator pathShape() const {
        pathShape ps(mPen, true);

        APointF();

        ps.setPointSeries({
            (mBoundingBox.topLeft() + mBoundingBox.topRight()) / 2,
            (mBoundingBox.topRight() + mBoundingBox.bottomRight()) / 2,
            (mBoundingBox.bottomRight() + mBoundingBox.bottomLeft()) / 2,
            (mBoundingBox.bottomLeft() + mBoundingBox.topLeft()) / 2,
        });
        for(size_t i = 0; i < mSelectedAnchors.size(); ++i) {
            ps[i].setSelected(mSelectedAnchors[i]);
        }
        ps.setAngle(angle());
        ps.resetBoundingBox();
        return ps;
    }

    operator QRectF() const {
        return mBoundingBox;
    }

    void setBoundingBox(const QRectF &boundingBox) override {
        shape::setBoundingBox(boundingBox);
        mCenter = boundingBox.center();
        mRadius.setWidth(boundingBox.width()/2);
        mRadius.setHeight(boundingBox.height()/2);
    }

    const QRectF &updateBoundingBox() override {
        mBoundingBox.setTopLeft(QPointF(mCenter.x() - mRadius.width(), mCenter.y() - mRadius.height()));
        mBoundingBox.setSize(mRadius * 2);
        return mBoundingBox;
    }

    ShapeType type() const override { return ShapeType::Ellipse; }
    bool isNull() const override { return mRadius.isNull(); }

    PointState contains(const QPointF &point) const override {
        QPointF rtpoint = revTransformer().map(point);
        float dist = (rtpoint.x() - mCenter.x())/mRadius.width() + (rtpoint.x() - mCenter.x())/mRadius.width();
        return dist <= 0.99   ? PointState::Inside
               : dist <= 1.01 ? PointState::Edge
                              : PointState::None;
    }

    bool intersects(const ellipseShape &otherEllipse) const {
        if(isCircle() == true) {
            return QPointF::dotProduct(center(), otherEllipse.center()) <=
                   mRadius.width() + otherEllipse.mRadius.width();
        } else {
            /**
             *  FIXME: Add ellipse-ellipse collision.
             *  https://en.wikipedia.org/wiki/Ellipse
             */
            throw "not implemented";
            return false;
        }
    }

    /// TODO: constexpr.
    bool isCircle() const { return mRadius.width() == mRadius.height(); }
    float maxRadius() const { return std::max(mRadius.width(), mRadius.height()); }

    /// setters
    void setTopLeft(const QPointF &newPoint) {
        mBoundingBox.setTopLeft(newPoint);
        mCenter = mBoundingBox.center();
        mRadius = mBoundingBox.size()/2;
    }
    void setBottomRight(const QPointF &newPoint) {
        mBoundingBox.setBottomRight(newPoint);
        mCenter = mBoundingBox.center();
        mRadius = mBoundingBox.size()/2;
    }
    void setCenter(const QPointF &newCenter) {
        mCenter = newCenter;
        updateBoundingBox();
    }
    void setRadius(const QSizeF &newRadius) {
        mRadius = newRadius;
        updateBoundingBox();
    }
    void setRadius(float xrad, float yrad) {
        mRadius = QSizeF(xrad, yrad);
        updateBoundingBox();
    }
    void setRadius(float rad) {
        mRadius = QSize(rad, rad);
        updateBoundingBox();
    }

    /// getters
    const QPointF &center() const { return mCenter; }
    const QSizeF &radius() const { return mRadius; }
    QPointF topLeft() const { return mBoundingBox.topLeft(); }
    QPointF bottomRight() const { return mBoundingBox.bottomRight(); }

private:
    QPointF mCenter;
    QSizeF mRadius;
    std::array<bool, 4> mSelectedAnchors;
};

/**
 * @brief The rectangular Shapes
 */
class rectShape: public QRectF, public shape {
public:
    rectShape(const QRectF &rect = QRectF(), const nanoPen &pen = nanoPen())
        : QRectF(rect), shape(pen), mCornerRadius(), mSelectedAnchors() {}
    rectShape(const QPointF &topLeft, const QPointF &bottomRight, const nanoPen &pen = nanoPen())
        : QRectF(topLeft, bottomRight), shape(pen), mCornerRadius(), mSelectedAnchors() {}

    // rectShape &operator = (const rectShape &rect) TODO: Is this really necessary?

    operator pathShape() const {
        pathShape path(mPen, true);

        /**
         *  Position anchros depend on corner radiuses.
         *  FIXME: set corners handlers.
         *
         *    ╭─a1───a2─╮
         *   a0 r0   r1 a3
         *    │    ∙    │
         *   a7 r3   r2 a4
         *    ╰─a6───a5─╯
         */
        path.setPointSeries({
            mBoundingBox.topLeft() + QPointF(0, mCornerRadius[0]),
            mBoundingBox.topLeft() + QPointF(mCornerRadius[0], 0),
            mBoundingBox.topRight() + QPointF(-mCornerRadius[1], 0),
            mBoundingBox.topRight() + QPointF(0, mCornerRadius[1]),
            mBoundingBox.bottomLeft() + QPointF(0, -mCornerRadius[2]),
            mBoundingBox.bottomLeft() + QPointF(-mCornerRadius[2], 0),
            mBoundingBox.bottomRight() + QPointF(mCornerRadius[3], 0),
            mBoundingBox.bottomRight() + QPointF(0, -mCornerRadius[3]),
        });
        path.setAngle(this->angle());
        path.resetBoundingBox();

        return path;
    }

    const QRectF &updateBoundingBox() override {
        mBoundingBox.setTopLeft(topLeft());
        mBoundingBox.setBottomRight(bottomRight());
        return mBoundingBox;
    }

    /// getters
    ShapeType type() const override { return ShapeType::Rectangle; }
    bool isNull() const override { return QRectF::isNull(); }
    QPointF center() const { return QRectF::center(); }
    float radius() const {
        for(const auto rad : mCornerRadius)
            if(rad != mCornerRadius[0])
                return NAN;
        return mCornerRadius[0];
    }
    PointState contains(const QPointF &point) const override {
        QPointF rtpoint = revTransformer().map(point);
        if(radius() == 0.0f) {
            return QRectF::contains(rtpoint) ? PointState::Inside : PointState::None;
        } else {
            QRectF::contains(rtpoint);
            /**
             * FIXME: detect if rounded rect contains point.
             * https://stackoverflow.com/questions/401847/circle-rectangle-collision-detection-intersection
             */
            throw "not implemented";
            return PointState::None;
        }
    }

    /// setters
    void setBoundingBox(const QRectF &boundingBox) override {
        shape::setBoundingBox(boundingBox);
        setTopLeft(boundingBox.topLeft());
        setBottomRight(boundingBox.bottomRight());
    }
    void setTopLeft(const QPointF &p) {
        QRectF::setTopLeft(p);
        updateBoundingBox();
    }
    void setBottomRight(const QPointF &p) {
        QRectF::setBottomRight(p);
        updateBoundingBox();
    }
    void setRectFromCenter(const QPointF &center, const QPointF &bottomRight) {
        /// FIXME
        setTopLeft(center);
        setBottomRight(bottomRight);
    }
    void setRect(const QRectF &rect = QRectF()) {
        setTopLeft(rect.topLeft());
        setBottomRight(rect.bottomRight());
    }
    void setCornerRadius(const std::array<float, 4> &newCornerRadius) { mCornerRadius = newCornerRadius; }
    void setSelectedAnchors(const std::array<bool, 8> &newAnchors) { mSelectedAnchors = newAnchors; }

    /// getters
    const std::array<float, 4> &cornerRadius() const { return mCornerRadius; }
    pathShape selectedAnchors() const { return *this; }

private:
    std::array<float, 4> mCornerRadius;
    std::array<bool, 8> mSelectedAnchors;
};

/**
 * @brief The lineShape struct
 */
class lineShape : public QLineF, public shape {
public:
    lineShape(const QLineF &line = QLineF(), const nanoPen &pen = nanoPen())
        : QLineF(line), shape(pen), mP1(false), mP2(false) {}
    lineShape(const QPointF &p1, const QPointF &p2, const nanoPen &pen = nanoPen())
        : lineShape(QLineF(p1, p2), pen) {}

    operator pathShape() const {
        pathShape path(*this, mPen);
        path.setSelected(selected());
        path[0].setSelected(mP1);
        path[1].setSelected(mP2);
        return path;
    }

    void setBoundingBox(const QRectF &boundingBox) override {
        shape::setBoundingBox(boundingBox);
        setP1(boundingBox.topLeft());
        setP2(boundingBox.bottomRight());
    }

    ShapeType type() const override { return ShapeType::Line; }
    bool isNull() const override { return QLineF::isNull(); }
    PointState contains(const QPointF &point) const override {
        QPointF p = p1() - point;
        return qFuzzyCompare(p.x()/p.y(), dx()/dy()) ? PointState::Edge : PointState::None;
    }

    /// setters
    void selectP1(bool set = true) { mP1 = set; }
    void selectP2(bool set = true) { mP2 = set; }

    /// getters
    bool p1Selected() const { return mP1; }
    bool p2Selected() const { return mP2; }

private:
    bool mP1, mP2;
};

/**
 *  TODO: add polygon type.
 *  polygon can includes (triangle, rectangle, pentagon etc).
 *  all edges have same length.
 *  all corners have same angle.
 *
 *  class polygonShape : public pathShape {}
 */

/// TODO: detect if shape intersects other shape.
/// level 2: return intersection as pathShape.
static bool intersects(const ellipseShape &ellipse, const rectShape &rect) {
    Q_UNUSED(ellipse)
    Q_UNUSED(rect)
    throw "not implemented";
    return false;
}
static bool intersects(const ellipseShape &ellipse, const pathShape &path) {
    Q_UNUSED(ellipse)
    Q_UNUSED(path)
    throw "not implemented";
    return false;
}
}
