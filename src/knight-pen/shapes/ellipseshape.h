#pragma once

#include <array>

#include "shape.h"
#include "pathshape.h"

namespace knightPen {
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

    /**
     * @brief setBoundingBox
     * set bounding box and update ellipse center and radius.
     * @param boundingBox
     */
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

    /**
     * @brief contains
     * @param point
     * @return
     * @i PointState::Inside if ellipse is filled and point is inside ellipse.
     * @i PointState::Edge if point is over ellipse outline.
     * @i PointState::None if none of above are true.
     */
    PointState contains(const QPointF &point) const override {
        QPointF rp = revTransformer().map(point);
        const float eeq = std::pow(rp.x() - mCenter.x(), 2) / std::pow(mRadius.width(), 2) +
                          std::pow(rp.y() - mCenter.y(), 2) / std::pow(mRadius.height(), 2);
        return mPen.mFill && eeq <= 0.9 ? PointState::Inside :
               0.9 < eeq  && eeq <= 1.1 ? PointState::Edge : PointState::None;
    }

    /**
     * @brief intersects
     * @param otherEllipse
     * @return true if two ellipse has intersect.
     */
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
    void selectAnchor(int i, bool set = true) {
        mSelectedAnchors[i] = set;
    }

    /// getters
    QPointF center() const override { return mCenter; }
    const QSizeF &radius() const { return mRadius; }
    QPointF topLeft() const { return mBoundingBox.topLeft(); }
    QPointF bottomRight() const { return mBoundingBox.bottomRight(); }

private:
    QPointF mCenter;
    QSizeF mRadius;
    std::array<bool, 4> mSelectedAnchors;
};
}
