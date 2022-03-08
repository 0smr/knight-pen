#pragma once

#include "apoint.h"
#include "shape.h"

namespace knightPen {

/**
 * @brief The path shape class
 * NOTE: what if every thing was pathShape? ...
 * FIXME: change bounding box on point position update.
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
        : shape(line, pen),
          mPointSeries({line.p1(), line.p2()}),
          mClosed(false),
          mArrows(pathShape::Arows::None) {}
    pathShape(const QPointF &point, const nanoPen &pen = nanoPen())
        : shape(QRectF(point, point), pen),
          mPointSeries({point}),
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
     * TODO: Add following function:
     * void setBoundingBox(const QRectF &boundingBox) override {}
     */

    /**
     * @brief contains
     * check wheater if a point is inside a path or not.
     * if shape is not filled function only check the edges and anchors.
     * @param point
     * @return true if path contain point, otherwise return false.
     */
    PointState contains(const QPointF &point) const override;

    void clear() { mPointSeries.clear(); }
    void pushPoint(APointF point) {
        point = revTransformer().map(point);
        mPointSeries.push_back(point);

        const qreal x1 = std::min(mBoundingBox.left(), point.x());
        const qreal y1 = std::min(mBoundingBox.top(), point.y());
        const qreal x2 = std::max(mBoundingBox.right(), point.x());
        const qreal y2 = std::max(mBoundingBox.bottom(), point.y());

        if(size() == 1) {
            mBoundingBox = QRectF(point, point + QPointF(1.0, 1.0));
        } else {
            mBoundingBox.setCoords(x1, y1, x2, y2);
        }
    }
    QPointF center() const override { return mBoundingBox.center(); }

    /// setters
    void setPointSeries(const std::vector<APointF> &newPointSeries) {
        mPointSeries = newPointSeries;
        updateBoundingBox();
    }
    void setClosed(bool newClosed) { mClosed = newClosed; }
    void setArrow(pathShape::Arows newArrows) { mArrows = newArrows; }

    /// getters
    bool empty() const { return mPointSeries.empty(); }
    bool closed() const { return mClosed; }
    bool isSinglePoint() const { return size() == 1; }
    pathShape::Arows arrow() const { return mArrows; }
    const std::vector<APointF> &pointSeries() const { return mPointSeries; }
    size_t size() const { return mPointSeries.size(); }

private:
    std::vector<APointF> mPointSeries;
    bool mClosed;
    pathShape::Arows mArrows;
};
}
