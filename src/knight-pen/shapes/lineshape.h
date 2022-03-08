#pragma once

#include "shape.h"
#include "pathshape.h"

namespace knightPen {

class pathShape;

/**
 * @brief The lineShape struct
 */
class lineShape : public QLineF, public shape {
public:
    lineShape(const QLineF &line = QLineF(), const nanoPen &pen = nanoPen())
        : QLineF(line), shape(line, pen), mP1(false), mP2(false) {}
    lineShape(const QPointF &p1, const QPointF &p2, const nanoPen &pen = nanoPen())
        : lineShape(QLineF(p1, p2), pen) {}

    operator pathShape() const;

    void setBoundingBox(const QRectF &boundingBox) override {
        shape::setBoundingBox(boundingBox);
        setP1(boundingBox.topLeft());
        setP2(boundingBox.bottomRight());
    }

    shape::ShapeType type() const override { return ShapeType::Line; }
    bool isNull() const override { return QLineF::isNull(); }

    /**
     * @brief contains
     * check if point is over line or not.
     * @def
     * for p1, p2 and p3, there is a p3 between p1 and p2
     * if distance p1 and p3 plus to distance p3 and p2 are equal to
     * distance p1 and p2.
     *
     *  p1-------p3--------p2
     *
     * @param point
     * @return
     */
    PointState contains(const QPointF &point) const override {
        return APointF(point).isBetween(p1(), p2()) ? PointState::Edge : PointState::None;
    }

    /// setters
    void setP1(const QPointF &p1) {
        QLineF::setP1(p1);
        mBoundingBox.setTopLeft(p1);
    }
    void setP2(const QPointF &p2) {
        QLineF::setP2(p2);
        mBoundingBox.setBottomRight(p2);
    }
    void selectP1(bool set = true) { mP1 = set; }
    void selectP2(bool set = true) { mP2 = set; }

    /// getters
    bool p1Selected() const { return mP1; }
    bool p2Selected() const { return mP2; }

private:
    bool mP1, mP2;
};
}
