#pragma once

#include <QLine>
#include <QPen>
#include <QPoint>
#include <QRect>

#include <array>
#include <cmath>
#include <vector>
#include <memory>

#include "nanopen.h"
#include "nanoshapes.h"
namespace nanoknight {

class paintHelper
{
public:
    paintHelper();

    static void currentDraw() {

    }

    static void drawPath(QNanoPainter *painter, const std::shared_ptr<pathShape> &path, const nanoPen &pen) {
        pen.setToPainter(painter);
        painter->resetTransform();
        painter->transform(path->transformer());
        painter->beginPath();
        painter->moveTo((*path)[0]);

        for(size_t i = 1; i < path->size(); ++i) {
            painter->lineTo((*path)[i]);
        }

        if(pen.mFill == true) painter->fill();
        if(pen.mStroke == true) painter->stroke();
    }

    static void drawAnchors(QNanoPainter *painter, const std::shared_ptr<pathShape> &path, nanoPen pen, float revscale = 1.0) {
        pen.mFill = true;
        for(const auto &point: path->pointSeries()) {
            QRectF rect(point - QPointF(1.5, 1.5) * revscale, QSizeF(3.0, 3.0) * revscale);
            pen.mFillColor = point.selected() ? pen.mStrokeColor : QColor(255,255,255);
            drawRect(painter, rect, pen);
        }
    }

    static void drawAnchors(QNanoPainter *painter, const pathShape &path, nanoPen pen, float revscale = 1.0) {
        pen.mFill = true;
        for(const auto &point: path.pointSeries()) {
            QRectF rect(point - QPointF(1.5, 1.5) * revscale, QSizeF(3.0, 3.0) * revscale);
            pen.mFillColor = point.selected() ? pen.mStrokeColor : QColor(255,255,255);
            drawRect(painter, rect, pen);
        }
    }

    static void drawRect(QNanoPainter *painter, const std::shared_ptr<rectShape> &rect, const nanoPen &pen) {
        painter->resetTransform();
        painter->transform(rect->transformer());
        drawRect(painter, *rect, pen);
    }

    static void drawBoundingBox(QNanoPainter *painter, const std::shared_ptr<shape> &sh, const nanoPen &pen) {
        painter->resetTransform();
        painter->transform(sh->transformer());
        drawRect(painter, sh->boundingBox(), pen);
    }

    static void drawRect(QNanoPainter *painter, const QRectF &rect, const nanoPen &pen) {
        pen.setToPainter(painter);
        painter->beginPath();
        painter->rect(rect);
        if(pen.mFill == true) painter->fill();
        if(pen.mStroke == true) painter->stroke();
    }

    static void drawEllipse(QNanoPainter *painter, const std::shared_ptr<ellipseShape> &ellipse, const nanoPen &pen) {
        painter->resetTransform();
        painter->transform(ellipse->transformer());
        pen.setToPainter(painter);
        painter->beginPath();
        painter->ellipse(*ellipse);
        if(pen.mFill == true) painter->fill();
        if(pen.mStroke == true) painter->stroke();
    }

    static void drawLine(QNanoPainter *painter, const std::shared_ptr<lineShape> &line, const nanoPen &pen) {
        painter->resetTransform();
        painter->transform(line->transformer());
        drawLine(painter, *line, pen);
    }

    static void drawLine(QNanoPainter *painter, const QLineF &line, const nanoPen &pen) {
        pen.setToPainter(painter);
        painter->beginPath();
        painter->moveTo(line.p1());
        painter->lineTo(line.p2());
        if(pen.mFill == true) painter->fill();
        if(pen.mStroke == true) painter->stroke();
    }
};
}
