#pragma once

#include <cmath>

#include "shape.h"
#include "pathshape.h"

namespace knightPen {
/**
 * @brief The rectangular Shapes
 */
class rectShape: public QRectF, public shape {
public:
    rectShape(const QRectF &rect = QRectF(), const nanoPen &pen = nanoPen())
        : QRectF(rect), shape(pen), mCornerRadius({}), mSelectedAnchors({}) {}
    rectShape(const QPointF &topLeft, const QPointF &bottomRight, const nanoPen &pen = nanoPen())
        : rectShape(QRectF(topLeft, bottomRight), pen) {}

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

    PointState contains(const QPointF &point) const override {
        APointF rp = revTransformer().map(point);
        if(radius() == 0.0f) {
            const float off = 2.0f; ///> 2x offset for edges area.
            bool inside = adjusted(-off, -off, off, off).contains(rp) && mPen.mFill;
            bool overEdges = rp.isBetween(topLeft(), topRight()) ||
                             rp.isBetween(topRight(), bottomRight()) ||
                             rp.isBetween(bottomRight(), bottomLeft()) ||
                             rp.isBetween(bottomLeft(), topLeft());

            return overEdges ? PointState::Edge :
                     inside  ? PointState::Inside :
                               PointState::None;
        } else {
            /**
             * FIXME: detect if rounded rect contains point.
             * https://stackoverflow.com/questions/401847/circle-rectangle-collision-detection-intersection
             */
            qErrnoWarning("method rectshape::contains() can't handle rounded rectangles.");
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
    QPointF center() const override { return QRectF::center(); }
    float radius() const {
        for(const auto rad : mCornerRadius)
            if(rad != mCornerRadius[0])
                return std::nanf("");
        return mCornerRadius[0];
    }
    const std::array<float, 4> &cornerRadius() const { return mCornerRadius; }
    pathShape selectedAnchors() const { return *this; }

private:
    std::array<float, 4> mCornerRadius;
    std::array<bool, 8> mSelectedAnchors;
};
}

