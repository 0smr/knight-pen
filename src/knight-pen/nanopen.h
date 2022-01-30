#pragma once

#include <QPen>
#include "qnanopainter.h"

namespace knightPen {
/**
 * @brief The nanoPen struct
 */
struct nanoPen {
    QNanoPainter::PathWinding mWinding : 2;
    QNanoPainter::LineCap mCap : 2;
    QNanoPainter::LineJoin mJoin : 3;
    QNanoPainter::TextAlign mTextAlign : 3;
    QNanoPainter::TextBaseline mTextBaseLine : 7;
    QNanoPainter::PixelAlign mPixelAlign : 2;
    QNanoPainter::CompositeOperation mComposite : 4;
    QNanoPainter::BlendFactor mBlend : 11;

    bool mFill = false;
    bool mStroke = true;
    float mWidth = 1.0f;
    float mMiter = 10.0f;
    QColor mFillColor = Qt::black;
    QColor mStrokeColor = Qt::black;

    void  setToPainter(QNanoPainter *painter) const {
        /**
         * TODO: add pattern as style.
         */
        painter->setFillStyle(QNanoColor::fromQColor(mFillColor));
        painter->setStrokeStyle(QNanoColor::fromQColor(mStrokeColor));
        painter->setMiterLimit(mMiter);
        painter->setLineWidth(mWidth);
        painter->setLineCap(mCap);
        painter->setLineJoin(mJoin);
    }

    void setJoin(const Qt::PenJoinStyle &join) { mJoin = fromQtJoin(join); }
    void setJoin(const QNanoPainter::LineJoin &joinStyle) { mJoin = joinStyle; }
    void setCap(const Qt::PenCapStyle &capStyle) { mCap = fromQtCap(capStyle); }
    void setCap(const QNanoPainter::LineCap &capStyle) { mCap = capStyle; }

    operator Qt::PenCapStyle() const { return nanoPen::toQtCap(mCap); }
    operator Qt::PenJoinStyle() const { return nanoPen::toQtJoin(mJoin); }

    static QNanoPainter::LineCap fromQtCap(Qt::PenCapStyle capStyle) {
        switch(capStyle) {
        case Qt::PenCapStyle::FlatCap:
            return QNanoPainter::LineCap::CAP_BUTT;
        case Qt::PenCapStyle::RoundCap:
            return QNanoPainter::LineCap::CAP_ROUND;
        case Qt::PenCapStyle::SquareCap:
            return QNanoPainter::LineCap::CAP_SQUARE;
        default:
            return QNanoPainter::LineCap::CAP_BUTT;
        }
    }

    static Qt::PenCapStyle toQtCap(QNanoPainter::LineCap capStyle) {
        switch(capStyle) {
        case QNanoPainter::LineCap::CAP_BUTT:
            return Qt::PenCapStyle::FlatCap;
        case QNanoPainter::LineCap::CAP_ROUND:
            return Qt::PenCapStyle::RoundCap;
        case QNanoPainter::LineCap::CAP_SQUARE:
            return Qt::PenCapStyle::SquareCap;
        default:
            return Qt::PenCapStyle::MPenCapStyle;
        }
    }

    static QNanoPainter::LineJoin fromQtJoin(Qt::PenJoinStyle joinStyle) {
        switch(joinStyle) {
        case Qt::PenJoinStyle::RoundJoin:
            return QNanoPainter::LineJoin::JOIN_ROUND;
        case Qt::PenJoinStyle::BevelJoin:
            return QNanoPainter::LineJoin::JOIN_BEVEL;
        case Qt::PenJoinStyle::MiterJoin:
            return QNanoPainter::LineJoin::JOIN_MITER;
        default:
            return QNanoPainter::LineJoin::JOIN_BEVEL;
        }
    }

    static Qt::PenJoinStyle toQtJoin(QNanoPainter::LineJoin joinStyle) {
        switch(joinStyle) {
        case QNanoPainter::LineJoin::JOIN_ROUND:
            return Qt::PenJoinStyle::RoundJoin;
        case QNanoPainter::LineJoin::JOIN_BEVEL:
            return Qt::PenJoinStyle::BevelJoin;
        case QNanoPainter::LineJoin::JOIN_MITER:
            return Qt::PenJoinStyle::MiterJoin;
        default:
            return Qt::PenJoinStyle::MiterJoin;
        }
    }
};
}
