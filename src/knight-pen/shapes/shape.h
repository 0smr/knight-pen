#pragma once

#include <memory>
#include <array>

#include "apoint.h"

namespace knightPen {
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

    shape(const QRectF &rect,
        const nanoPen &pen = nanoPen(),
        const bool &selected = false,
        float angle = 0.0f)
        : mBoundingBox(rect),
          mPen(pen),
          mSelected(selected),
          mAngle(angle){}

    shape(const QPointF p1,
        const QPointF p2,
        const nanoPen &pen = nanoPen(),
        const bool &selected = false,
        float angle = 0.0f)
        : shape(QRectF(p1, p2), pen, selected, angle) {}

    shape(const QLineF line,
        const nanoPen &pen = nanoPen(),
        const bool &selected = false,
        float angle = 0.0f)
        : shape(QRectF(line.p1(), line.p2()), pen, selected, angle) {}

    shape(const nanoPen &pen,
        const bool &selected = false,
        float angle = 0.0f)
        : shape(QRectF(), pen, selected, angle) {}

    ~shape() {};

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

    /**
     * @brief boundingPoints
     * @return 8 points over boundingbox like:
     *  0────1────2
     *  │         │
     *  7    ∙    3
     *  │         │
     *  6────5────4
     */
    std::array<QPointF, 8> boundingBoxHandles() const {
        const QRectF &bp = mBoundingBox;
        std::array<QPointF, 8> boundPoints {
            bp.topLeft(), bp.topRight(),
            bp.bottomRight(), bp.bottomLeft()
        };
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

    virtual float width() const { return mBoundingBox.width(); }
    virtual float height() const { return mBoundingBox.height(); }
    virtual QPointF center() const { return mBoundingBox.center(); }
    virtual PointState contains(const QPointF &point) const { Q_UNUSED(point) return PointState::None; }
    virtual ShapeType type() const { return ShapeType::Shape; }

    virtual bool isNull() const { return mBoundingBox.isNull(); }
    virtual const QRectF &updateBoundingBox() { return mBoundingBox; }
    virtual void setBoundingBox(const QRectF &boundingBox) { mBoundingBox = boundingBox; }

    /// setters
    void setSelected(bool newSelected) { mSelected = newSelected; }
    void setAngle(float newAngle) { mAngle = newAngle; }
    void setPen(const nanoPen &newPen) { mPen = newPen; }

    /// friends
    friend QDebug operator<<(QDebug debug, const shape &shape) {
        switch(shape.type()) {
        case shape::Ellipse:
            debug << "shape::ellipse(" << shape.boundingBox() << ")";
            break;
        case shape::Path:
            debug << "shape::path(" << shape.boundingBox() << ")";
            break;
        case shape::Line:
            debug << "shape::line(" << shape.boundingBox() << ")";
            break;
        case shape::Polygon:
            debug << "shape::polygon(" << shape.boundingBox() << ")";
            break;
        case shape::Rectangle:
            debug << "shape::rectangle(" << shape.boundingBox() << ")";
            break;
        default:
            break;
        }
        debug << "";
        return debug;
    }
    friend QDebug operator<<(QDebug debug, const std::shared_ptr<shape> shapePtr) {
        return (debug << *shapePtr);
    }

protected:
    QRectF mBoundingBox;
    nanoPen mPen;
    bool mSelected;
    float mAngle;
};
}
