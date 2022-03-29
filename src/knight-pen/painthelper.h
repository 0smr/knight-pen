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
#include "shapes/shapes.h"

namespace knightPen {

class paintHelper
{
public:
    static inline float GlobalScale = 1.0;
    paintHelper();

    /**
     * @brief drawShape
     * @param painter
     * @param shape
     * @param pen
     *
     * Draw shapes depend on their types.
     */
    static void drawShape(QNanoPainter *painter, const std::shared_ptr<shape> &shape, const nanoPen &pen) {
        if(shape->isNull() == false) {
            switch(shape->type()) {
            case shape::Path:
                drawPath(painter, std::dynamic_pointer_cast<pathShape>(shape), pen);
                break;
            case shape::Line:
                drawLine(painter, std::dynamic_pointer_cast<lineShape>(shape), pen);
                break;
            case shape::Ellipse:
                drawEllipse(painter, std::dynamic_pointer_cast<ellipseShape>(shape), pen);
                break;
            case shape::Rectangle:
                drawRect(painter, std::dynamic_pointer_cast<rectShape>(shape), pen);
                break;
            case shape::Polygon:
            case shape::Shape:
            default:
                break;
            }
        }
    }

    /**
     * @brief drawPath
     * @param painter
     * @param path
     * @param pen
     *
     * Draw path shape.
     */
    static void drawPath(QNanoPainter *painter, const std::shared_ptr<pathShape> &path, const nanoPen &pen) {
        pen.setToPainter(painter);
        painter->resetTransform();
        painter->transform(path->transformer());
        painter->beginPath();
        painter->moveTo((*path)[0]);

        for(size_t i = 1; i < path->size(); ++i) {
            painter->lineTo((*path)[i]);
        }

        if(path->closed()) {
            painter->lineTo(path->at(0));
        }

        if(pen.mFill == true) painter->fill();
        if(pen.mStroke == true) painter->stroke();
    }

    /**
     * @brief drawAnchors
     * @param painter
     * @param shape
     * @param pen
     * @param forceFill
     *
     * Draw anvhors over a shape depended on it's type.
     */
    static void drawAnchors(QNanoPainter *painter, const std::shared_ptr<shape> &shape, nanoPen pen, bool forceFill = false)  {
        if(shape->type() == shape::Path)
            drawAnchors(painter, *std::dynamic_pointer_cast<pathShape>(shape), pen, forceFill);
        else if(shape->type() == shape::Line)
            drawAnchors(painter, *std::dynamic_pointer_cast<lineShape>(shape), pen, forceFill);
        else if(shape->type() == shape::Ellipse)
            drawAnchors(painter, *std::dynamic_pointer_cast<ellipseShape>(shape), pen, forceFill);
        else if(shape->type() == shape::Rectangle)
            drawAnchors(painter, *std::dynamic_pointer_cast<rectShape>(shape), pen, forceFill);
    }

    /**
     * @brief drawAnchors
     * @param painter
     * @param path
     * @param pen
     * @param forceFill
     *
     *  Draw anchors over a pathShape point series.
     */
    static void drawAnchors(QNanoPainter *painter, const pathShape &path, nanoPen pen, bool forceFill = false)  {
        const float reverseScale = 1/GlobalScale;
        pen.mFill = true;
        for(const auto &point: path.pointSeries()) {
            QRectF rect(point - QPointF(1.5, 1.5) * reverseScale, QSizeF(3.0, 3.0) * reverseScale);

            /// If @a forceSelectedStyle is set stroke color.
            pen.mFillColor = point.selected() || forceFill ? pen.mStrokeColor : 0xffffffff;
            drawRect(painter, rect, pen);
        }
    }

    /**
     * @brief drawRect
     * @param painter
     * @param rect
     * @param pen
     *
     * Draw a rectangle.
     */
    static void drawRect(QNanoPainter *painter, const std::shared_ptr<rectShape> &rect, const nanoPen &pen) {
        painter->resetTransform();
        painter->transform(rect->transformer());
        drawRect(painter, *rect, pen);
    }

    /**
     * @brief drawBoundingBox
     * @param painter
     * @param shape
     * @param pen
     *
     * Draw shape pointer boundig box.
     */
    static void drawBoundingBox(QNanoPainter *painter, const std::shared_ptr<shape> &shape, const nanoPen &pen) {
        drawBoundingBox(painter, shape->boundingBox(), pen, shape->transformer());
    }

    /**
     * @brief drawBoundingBox
     * @param painter
     * @param rect
     * @param pen
     * @param transorm
     *
     * Draw bounding box over input rect.
     */
    static void drawBoundingBox(QNanoPainter *painter, const QRectF &rect,
                                const nanoPen &pen, const QTransform &transorm = QTransform()) {
        pathShape ps;
        ps.setPointSeries(std::vector<APointF>{
            rect.topLeft(), (rect.topLeft() + rect.topRight())/2,
            rect.topRight(), (rect.topRight() + rect.bottomRight())/2,
            rect.bottomRight(), (rect.bottomRight() + rect.bottomLeft())/2,
            rect.bottomLeft(), (rect.bottomLeft() + rect.topLeft())/2,
        });

        painter->resetTransform();
        for(size_t i = 0 ; i < ps.size(); ++i) {
            ps[i] = transorm.map(ps[i]);
        }

        drawRect(painter, rect, pen); ///> draw bounding box rect.
        drawAnchors(painter, ps, pen); ///> draw anchors around bounding box.
        /// draw a point in center of bounding box if shape was big enough..
        if(rect.normalized().width() > 10 && rect.normalized().height() > 10) {
            pen.setToPainter(painter);
            painter->beginPath();
            painter->circle(rect.center(), 1);
            painter->fill();
        }
    }

    /**
     * @brief drawRect
     * @param painter
     * @param rect
     * @param pen
     *
     * Draw given QRectF.
     */
    static void drawRect(QNanoPainter *painter, const QRectF &rect, const nanoPen &pen) {
        pen.setToPainter(painter);
        painter->beginPath();
        painter->rect(rect);
        if(pen.mFill == true) painter->fill();
        if(pen.mStroke == true) painter->stroke();
    }

    /**
     * @brief drawEllipse
     * @param painter
     * @param ellipse
     * @param pen
     *
     * Draw given ellipseShape pointer.
     */
    static void drawEllipse(QNanoPainter *painter, const std::shared_ptr<ellipseShape> &ellipse, const nanoPen &pen) {
        painter->resetTransform();
        painter->transform(ellipse->transformer());
        pen.setToPainter(painter);
        painter->beginPath();
        painter->ellipse(*ellipse);
        if(pen.mFill == true) painter->fill();
        if(pen.mStroke == true) painter->stroke();
    }

    /**
     * @brief drawLine
     * @param painter
     * @param line
     * @param pen
     *
     * Draw given lineShape pointer.
     */
    static void drawLine(QNanoPainter *painter, const std::shared_ptr<lineShape> &line, const nanoPen &pen) {
        painter->resetTransform();
        painter->transform(line->transformer());
        drawLine(painter, *line, pen);
    }

    /**
     * @brief drawLine
     * @param painter
     * @param line
     * @param pen
     *
     * Draw given lineShape pointer.
     */
    static void drawLine(QNanoPainter *painter, const QLineF &line, const nanoPen &pen) {
        pen.setToPainter(painter);
        painter->beginPath();
        painter->moveTo(line.p1());
        painter->lineTo(line.p2());
        if(pen.mFill == true) painter->fill();
        if(pen.mStroke == true) painter->stroke();
    }

    /**
     * @brief drawPoint
     * @param painter
     * @param point
     * @param pen
     *
     * Draw given QPointF.
     * Two crossing lines for single point.
     */
    static void drawPoint(QNanoPainter *painter, const QPointF &point, const nanoPen &pen) {
        const float reverseScale = 1/GlobalScale;

        /// @brief draw two crossing lines for single point.
        QLineF vLine(point - QPointF(2, 2) * reverseScale, point + QPointF(2, 2) * reverseScale);
        QLineF hLine(point - QPointF(2, -2) * reverseScale, point + QPointF(2, -2) * reverseScale);
        drawLine(painter, vLine, pen);
        drawLine(painter, hLine, pen);
    }
};
}
