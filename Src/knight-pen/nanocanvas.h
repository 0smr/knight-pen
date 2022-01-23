#pragma once

#include "qnanopainter.h"
#include "qnanoquickitem.h"
#include "qnanoquickitempainter.h"

#include <qnamespace.h>

#include <QLine>
#include <QPen>
#include <QPoint>
#include <QRect>

#include <array>
#include <cmath>
#include <vector>
#include <memory>

#include "nanopainter.h"
#include "nanopen.h"
#include "nanoshapes.h"
#include "painthelper.h"

namespace nanoknight {

class nanoCanvas : public QNanoQuickItem {
    Q_OBJECT
    Q_PROPERTY(Tools selectedTool READ selectedTool WRITE setSelectedTool NOTIFY selectedToolChanged)
    Q_PROPERTY(QColor strokeColor READ strokeColor WRITE setStrokeColor NOTIFY strokeColorChanged)
    Q_PROPERTY(QColor fillColor READ fillColor WRITE setFillColor NOTIFY fillColorChanged)
    Q_PROPERTY(Qt::PenCapStyle lineCap READ lineCap WRITE setLineCap NOTIFY lineCapChanged)
    Q_PROPERTY(Qt::PenJoinStyle lineJoin READ lineJoin WRITE setLineJoin NOTIFY lineJoinChanged)
    Q_PROPERTY(float strokeWidth READ strokeWidth WRITE setStrokeWidth NOTIFY strokeWidthChanged)
    Q_PROPERTY(float miterLimit READ miterLimit WRITE setMiterLimit NOTIFY miterLimitChanged)
    Q_PROPERTY(float scaleFactor READ scaleFactor WRITE setScaleFactor NOTIFY scaleFactorChanged)
    Q_PROPERTY(QPointF mouse READ mouse WRITE setMouse NOTIFY mouseChanged)
public:
    Q_ENUMS(PointProxy)
    /**
     * @brief The ExportType enum
     */
    enum ExportType {
        Png = 0x1,
        Jpg,
        Svg,
    };
    Q_ENUMS(ExportType)

    /**
     * @brief The DrawType enum
     */
    enum DrawType {
        Raster = 0x4,
        Vector,
    };
    Q_ENUMS(DrawType)

    /**
     * @brief The Tools enum
     */
    enum Tools {
        None = 0x0,
        Line,
        Arrow,
        Ellipse,
        Rectangle,
        Path,
        Polygon,
        Selection,
        DirectSelection,
        GroupSelectionToggle,
        GroupSelectionInclude,
        GroupSelectionExclude,
    };
    Q_ENUMS(Tools)

    nanoCanvas(QQuickItem *parent = nullptr);

    /**
     * @brief createItemPainter
     * @return
     */
    QNanoQuickItemPainter *createItemPainter() const override;

    /**
     * @brief paintHelper
     * @param painter
     */
    void painter(QNanoPainter *painter) const {
        painter->scale(mScaleFactor);

        for(const auto &shape : mShapes) {
            const auto &pen = shape->pen();
            switch(shape->type()) {
                case shape::Path:
                    paintHelper::drawPath(painter, std::dynamic_pointer_cast<pathShape>(shape), pen);
                    if(shape->selected()) {
                        auto path = std::dynamic_pointer_cast<pathShape>(shape);
                        paintHelper::drawPath(painter, path, mSelectPen);
                        paintHelper::drawAnchors(painter, *path, mSelectPen, 1/mScaleFactor);
                    }
                    break;

                case shape::Line:
                    break;

                case shape::Polygon:
                    break;

                case shape::Ellipse:
                    paintHelper::drawEllipse(painter, std::dynamic_pointer_cast<ellipseShape>(shape), pen);
                    if(shape->selected()) {
                        paintHelper::drawEllipse(painter, std::dynamic_pointer_cast<ellipseShape>(shape), mSelectPen);
                    }
                    break;

                case shape::Rectangle:
                    paintHelper::drawRect(painter, std::dynamic_pointer_cast<rectShape>(shape), pen);
                    if(shape->selected()) {
                        paintHelper::drawRect(painter, std::dynamic_pointer_cast<rectShape>(shape), mSelectPen);
                    }
                    break;

                case shape::Shape:
                default:
                    break;
            }
        }

        /// draw path draw guide.
        if(mCurrentShape && mCurrentShape->type() == shape::Path) {
            auto path = std::dynamic_pointer_cast<pathShape>(mCurrentShape);
            QPointF last = path->pointSeries().back();
            paintHelper::drawLine(painter, QLineF(last, mMouse), mSelectPen);
        }

        /**
         * TODO: draw pixel-based paths in the QFrameBufferObject.
         *  pixel-based path: paths are not continuous
         * and points has more than one-pixel distance to each other.
         * It's better to draw them pixel based on framebuffer.
         */
    }

    /**
     * TODO: erase shapes.
     * Q_INVOKABLE void erase(const QPointF &point, const float &radius = 1,
     *                        DrawType etype = DrawType::Vector) {
     * }
     */

    QPointF revScale(const QPointF &point) { return point / mScaleFactor; }

    const Tools &selectedTool() const { return mSelectedTool; }
    const float &strokeWidth() const { return mPen.mWidth; }
    const QColor &strokeColor() const { return mPen.mStrokeColor; }
    const QColor &fillColor() const { return mPen.mFillColor; }
    const float &miterLimit() const { return mPen.mMiter; }
    Qt::PenCapStyle lineCap() const { return mPen; }
    Qt::PenJoinStyle lineJoin() const { return mPen; }
    QPointF mouse() const { return mMouse; }
    float scaleFactor() const { return mScaleFactor; }

    void setSelectedTool(const Tools &newSelectedTool);

    void setStrokeWidth(const float &newWidth) {
        if(mPen.mWidth == newWidth || newWidth < 0) return;
        mPen.mWidth = newWidth;
        emit strokeWidthChanged();
        emit penChanged();
        update();
    }

    void setStrokeColor(const QColor &newColor) {
        if(mPen.mStrokeColor == newColor) return;
        mPen.mStrokeColor = newColor;
        emit strokeColorChanged();
        emit penChanged();
        update();
    }

    void setFillColor(const QColor &newColor) {
        if(mPen.mFillColor == newColor) return;
        mPen.mFillColor = newColor;
        emit fillColorChanged();
        emit penChanged();
        update();
    }

    void setMiterLimit(const float &newMiterLimit) {
        if(mPen.mMiter == newMiterLimit || newMiterLimit < 0) return;
        mPen.mMiter = newMiterLimit;
        emit miterLimitChanged();
        emit penChanged();
        update();
    }

    void setLineJoin(const Qt::PenJoinStyle &newLineJoin) {
        if(mPen == newLineJoin) return;
        mPen.setJoin(newLineJoin);
        emit lineJoinChanged();
        emit penChanged();
        update();
    }

    void setLineCap(const Qt::PenCapStyle &newLineCap) {
        if(mPen == newLineCap) return;
        mPen.setCap(newLineCap);
        emit lineCapChanged();
        emit penChanged();
        update();
    }

    void setMouse(const QPointF &newMouse) {
        const QPointF &rscale = newMouse / mScaleFactor;  // reverse scaled point
        if(mMouse == rscale) return;
        mMouse = rscale;
        emit mouseChanged();

        /**
         * @brief mouseMoves
         * visualize current draw on mouse move.
         */
        mouseMoves();

        update();
    }

    void setScaleFactor(float newScaleFactor) {
        if(qFuzzyCompare(mScaleFactor, newScaleFactor) || newScaleFactor < 0.2)
            return;
        mScaleFactor = newScaleFactor;
        mSelectPen.mWidth = 0.7 * mScaleFactor; // change selection pen width on scale
        emit scaleFactorChanged();
        update();
    }

private slots:
    void mouseMoves() {
        if(mCurrentShape.use_count() == 0)
            return;

        switch(mCurrentShape->type()) {
            case shape::Rectangle: {
                auto rect = std::dynamic_pointer_cast<rectShape>(mCurrentShape);
                rect->setBottomRight(mMouse);
            } break;

            case shape::Ellipse: {
                auto ellipse = std::dynamic_pointer_cast<ellipseShape>(mCurrentShape);
                ellipse->setBottomRight(mMouse);
            } break;

            case shape::Polygon:
            case shape::Line:
            case shape::Path:
            default:
                break;
        }
    }

public slots:
    void addRect(const QPointF &point) {
        if(mCurrentShape == nullptr) {
            mCurrentShape = std::make_shared<rectShape>(point, mMouse);
            mCurrentShape->setPen(mPen);
            mCurrentShape->setSelected(true);
            mShapes.push_back(mCurrentShape);
        }
        update();
    }

    void addLine(const QPointF &point) {
        if(mCurrentShape == nullptr) {
            mCurrentShape = std::make_shared<lineShape>(point, mMouse);
            mCurrentShape->setPen(mPen);
            mCurrentShape->setSelected(true);
            mShapes.push_back(mCurrentShape);
        }
        update();
    }

    void addEllipse(const QPointF &point) {
        if(mCurrentShape == nullptr) {
            mCurrentShape = std::make_shared<ellipseShape>(point, mMouse);
            mCurrentShape->setPen(mPen);
            mCurrentShape->setSelected(true);
            mShapes.push_back(mCurrentShape);
        }
        update();
    }

    void addPathPoint(const QPointF &point) {
        if(mCurrentShape == nullptr) {
            mCurrentShape = std::make_shared<pathShape>(point);
            mCurrentShape->setPen(mPen);
            mCurrentShape->setSelected(true);
            mShapes.push_back(mCurrentShape);
        } else if(mCurrentShape->type() == shape::Path) {
            auto path = std::dynamic_pointer_cast<pathShape>(mCurrentShape);
            path->pushPoint(point);
        }
        update();
    }

    void setSelectedPen() {
        if(mCurrentShape) {
            mCurrentShape->setPen(mPen);
        }
        for(auto &shape : mSelectedShapes)
            shape->setPen(mPen);
    }

    void clearSelection() {
        for(auto &shape : mSelectedShapes)
            shape->setSelected(false);
        mSelectedShapes.clear();
        update();
    }

    void clearCanvas() {
        mShapes.clear();
        mSelectedShapes.clear();
        mCurrentShape.reset();
        update();
    }

    void stopDrawing() {
        if(mCurrentShape) {
            mSelectedShapes.push_back(mCurrentShape);
            mCurrentShape.reset();
        }
        update();
    }

    void cancelDrawing() {
        if(mShapes.empty() == false && mShapes.back()->selected()) {
            mShapes.pop_back();
        }
        mCurrentShape.reset();
        update();
    }

    bool joinSelected() {
        return mSelectedTool == Tools::Selection ? joinSelectedPahtes() : joinSelectedAnchors();
    }
    bool joinSelectedPahtes() { return false; }
    bool joinSelectedAnchors() { return false; }

    bool selectAt(const QPointF &point) {
        return mSelectedTool == Tools::Selection ? selectShapesAt(point) : selectAnchorsAt(point);
    }
    bool selectAnchorsAt(const QPointF &point) { Q_UNUSED(point) return false; }
    bool selectShapesAt(const QPointF &point) { Q_UNUSED(point) return false; }

    bool selectArea(const QRectF &area) { Q_UNUSED(area) return false; }
    bool selectAnchorsAtArea(const QRectF &area) { Q_UNUSED(area) return false; }
    bool selectShapesAtArea(const QRectF &area) { Q_UNUSED(area) return false; }

    bool transferSelecttion(const QPointF &offset) { Q_UNUSED(offset) return false; }
    bool transferSelectedAnchors(const QPointF &offset) { Q_UNUSED(offset) return false; }
    bool transferSelectedShapes(const QPointF &offset) { Q_UNUSED(offset) return false; }

signals:
    void selectedToolChanged();
    void strokeWidthChanged();
    void strokeColorChanged();
    void fillColorChanged();
    void miterLimitChanged();
    void lineJoinChanged();
    void lineCapChanged();
    void penChanged();
    void mouseChanged();
    void scaleFactorChanged();

private:
    /// TODO add selected shape and selected anchors.

    /**
     * TODO: classified snap points.
     *  std::vector<QPointF> mSnapPointCentersX;
     *  std::vector<QPointF> mSnapPointIntersectsX;
     *  std::vector<QPointF> mSnapPointSortedX;
     *  std::vector<QPointF> mSnapPointSortedY;
     *
     *  nanoPen mSnapGuidePen;
     *
     *  TODO: other type of shapes.
     *  \li frameBuffer for raster types.
     *  \li polygonShape.
     *  \li textShape.
     *
     *  add groups.
     *
     *  polygonShape mTempTriangle;
     *
     *  mAboutToClosePath -> bool
     *  mAboutToJoin -> bool
     *  mNearCorners -> pair<shape, APoint>
     *  mNearEdges -> pair<shape, APoints>
     */
    std::vector<std::shared_ptr<shape>> mShapes;
    std::vector<std::shared_ptr<shape>> mSelectedShapes;
    std::shared_ptr<shape> mCurrentShape;
    lineShape mPathGuide;

    QPointF mMouse;
    nanoPen mPen;
    nanoPen mSelectPen;

    Tools mSelectedTool;

    float mScaleFactor;
    std::vector<QRectF> mArtwork;
};
}
