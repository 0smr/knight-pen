#include "pathshape.h"

namespace knightPen {

PointState pathShape::contains(const QPointF &point) const {
    QPointF rtpoint = revTransformer().map(point);
    if(mBoundingBox.contains(rtpoint) == false || mPointSeries.size() < 2) {
        return PointState::None;
    } else {
        for(size_t i = 1; i < mPointSeries.size(); ++i) {
            if(APointF(rtpoint).isBetween(mPointSeries[i-1], mPointSeries[i]) == PointState::Edge) {
                return PointState::Edge;
            }
        }
        /// if path is closed.
        if(mClosed == true && APointF(rtpoint).isBetween(mPointSeries.front(), mPointSeries.back()) == PointState::Edge) {
            return PointState::Edge;
        }

        /// if shape is filled.
        if(mPen.mFill == true && mPointSeries.size() > 2) {
            ///
            QLineF horizontalLine(rtpoint.x(), rtpoint.y(), rtpoint.x(), boundingBox().right() + 1);
            size_t intersects = 0;
            const size_t count = mPointSeries.size();

            for(size_t i = 1; i < count + 1; ++i) {
                const QLineF line = QLineF(mPointSeries[(i - 1) % count], mPointSeries[i % count]);
                if(horizontalLine.intersects(line, nullptr) == QLineF::BoundedIntersection) {
                    intersects++;
                }
            }
            return intersects % 2 == 1 ? PointState::Inside : PointState::None;
        }
        return PointState::None;
    }
}
}
