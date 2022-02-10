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
#include <algorithm>

#include "nanopainter.h"
#include "nanopen.h"
#include "nanoshapes.h"
#include "painthelper.h"

namespace knightPen {

class knightCanvas : public QNanoQuickItem {
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
    Q_PROPERTY(bool drawing READ drawing CONSTANT)

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

    knightCanvas(QQuickItem *parent = nullptr);

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
        paintHelper::GlobalScale = mScaleFactor;

        /**
         * @brief draws all shapes.
         * Draw main shape.
         */
        for(const auto &shape : mShapes) {
            paintHelper::drawShape(painter, shape, shape->pen());
        }

        /**
         * @brief draws selection outlines.
         * Draw shape selection outlines (using selected pen).
         */
        for(const auto &shape : mSelectedShapes) {
            if(shape->isNull() == false) {
                /// If path is a single point.
                if(shape->type() == shape::Path) {
                    auto path = std::dynamic_pointer_cast<pathShape>(shape);
                    if(path->size() == 1) {
                        paintHelper::drawPoint(painter, (*path)[0], mSelectPen);
                    } else {
                        paintHelper::drawShape(painter, shape, mSelectPen);
                    }
                } else {
                    paintHelper::drawShape(painter, shape, mSelectPen);
                }
            }
        }

        /// @brief draws pathShape draw guide.
        if(mCurrentShape && mCurrentShape->type() == shape::Path) {
            auto path = std::dynamic_pointer_cast<pathShape>(mCurrentShape);
            QPointF last = path->pointSeries().back();
            paintHelper::drawLine(painter, QLineF(last, mMouse), mSelectPen);
        }

        /**
         * @brief draws shape drawAnchors.
         * Draw shape anchors only if Tool is direct selection or shape currently is painting.
         */
        if(mSelectedTool != Tools::Selection) {
            for(const auto &shape : mSelectedShapes) {
                if(shape->isNull() == false) {
                    /// If "current shape" fill anchor.
                    paintHelper::drawAnchors(painter, shape, mSelectPen, shape == mCurrentShape);
                }
            }
        }

        /**
         * @brief draws selected shapes bounding box.
         */
        if(mSelectedTool == Tools::Selection && mSelectedShapes.empty() == false) {
            if(mSelectedShapes.size() == 1) {
                paintHelper::drawBoundingBox(painter, mSelectedShapes[0], mSelectPen);
            } else {
                QRectF selectedBoundingBox;
                for(const auto &shape : mSelectedShapes) {
                    selectedBoundingBox = selectedBoundingBox.united(shape->boundingBox());
                }
                paintHelper::drawBoundingBox(painter, selectedBoundingBox, mSelectPen);
            }
        }

        /**
         * TODO: draw pixel-based paths in the QFrameBufferObject.
         *  pixel-based path: paths are not continuous
         * and points has more than one-pixel distance to each other.
         * It's better to draw them pixel based on framebuffer.
         */
    }

    Q_INVOKABLE void eraseSelected() {
        for(const auto &selectedShape: mSelectedShapes) {
            std::remove(mShapes.begin(), mShapes.end(), selectedShape);
        }
    }

    Q_INVOKABLE bool joinSelected() {
        return mSelectedTool == Tools::Selection ? joinSelectedPahtes() : joinSelectedAnchors();
    }
    Q_INVOKABLE bool joinSelectedPahtes() { return false; }
    Q_INVOKABLE bool joinSelectedAnchors() { return false; }

    Q_INVOKABLE bool translateSelecttion(const QPointF &offset) { Q_UNUSED(offset) return false; }
    Q_INVOKABLE bool translateSelectedAnchors(const QPointF &offset) { Q_UNUSED(offset) return false; }
    Q_INVOKABLE bool translateSelectedShapes(const QPointF &offset) { Q_UNUSED(offset) return false; }

    /**
     * @brief shape selection
     * Shape selection functions.
     * Includes:
     * @li select at specific point.
     * @li select at specific circle area.
     * @li select at specific rectangle area.
     */
    Q_INVOKABLE bool selectAt(const QPointF &point) {
        return mSelectedTool == Tools::Selection ? selectShapesAt(point) : selectAnchorsAt(point);
    }
    Q_INVOKABLE bool selectAnchorsAt(const QPointF &point) { Q_UNUSED(point) return false; }
    Q_INVOKABLE bool selectShapesAt(const QPointF &point) { Q_UNUSED(point) return false; }

    Q_INVOKABLE bool selectAtArea(const QRectF &area) { Q_UNUSED(area) return false; }
    Q_INVOKABLE bool selectAnchorsAtArea(const QRectF &area) { Q_UNUSED(area) return false; }
    Q_INVOKABLE bool selectShapesAtArea(const QRectF &area) { Q_UNUSED(area) return false; }

    Q_INVOKABLE bool selectAtArea(const QPointF &area, float radius) { Q_UNUSED(area) Q_UNUSED(radius) return false; }
    Q_INVOKABLE bool selectAnchorsAtArea(const QPointF &area, float radius) { Q_UNUSED(area) Q_UNUSED(radius) return false; }
    Q_INVOKABLE bool selectShapesAtArea(const QPointF &area, float radius) { Q_UNUSED(area) Q_UNUSED(radius) return false; }

    /**
     * @brief getter functions.
     */
    const Tools &selectedTool() const { return mSelectedTool; }
    const QPointF &mouse() const { return mMouse; }
    const QColor &strokeColor() const { return mPen.mStrokeColor; }
    const QColor &fillColor() const { return mPen.mFillColor; }
    float strokeWidth() const { return mPen.mWidth; }
    float miterLimit() const { return mPen.mMiter; }
    Qt::PenCapStyle lineCap() const { return mPen; }
    Qt::PenJoinStyle lineJoin() const { return mPen; }
    float scaleFactor() const { return mScaleFactor; }
    bool drawing() const { return mCurrentShape.use_count() > 0; }

    /**
     * @brief setter functions.
     */
    void setSelectedTool(const Tools &newSelectedTool);
    void setStrokeWidth(float newWidth) {
        newWidth = std::max(0.0f, newWidth);
        if(mPen.mWidth == newWidth)
            return;
        mPen.mWidth = newWidth;
        emit strokeWidthChanged();
        emit penChanged();
        updateCanvas();
    }
    void setStrokeColor(const QColor &newColor) {
        if(mPen.mStrokeColor == newColor) return;
        mPen.mStrokeColor = newColor;
        emit strokeColorChanged();
        emit penChanged();
        updateCanvas();
    }
    void setFillColor(const QColor &newColor) {
        if(mPen.mFillColor == newColor) return;
        mPen.mFillColor = newColor;
        emit fillColorChanged();
        emit penChanged();
        updateCanvas();
    }
    void setMiterLimit(float newMiterLimit) {
        if(mPen.mMiter == newMiterLimit || newMiterLimit < 0) return;
        mPen.mMiter = newMiterLimit;
        emit miterLimitChanged();
        emit penChanged();
        updateCanvas();
    }
    void setLineJoin(const Qt::PenJoinStyle &newLineJoin) {
        if(mPen == newLineJoin) return;
        mPen.setJoin(newLineJoin);
        emit lineJoinChanged();
        emit penChanged();
        updateCanvas();
    }
    void setLineCap(const Qt::PenCapStyle &newLineCap) {
        if(mPen == newLineCap) return;
        mPen.setCap(newLineCap);
        emit lineCapChanged();
        emit penChanged();
        updateCanvas();
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
    }
    void setScaleFactor(float newScaleFactor) {
        if(qFuzzyCompare(mScaleFactor, newScaleFactor) || newScaleFactor < 0.2)
            return;
        mScaleFactor = newScaleFactor;
        mSelectPen.mWidth = 0.7 * mScaleFactor; // change selection pen width on scale
        emit scaleFactorChanged();
        updateCanvas();
    }

private:
    /**
     * private functions.
     */

    /**
     * @brief updateCanvas
     * call update slot if item was enabled.
     */
    void updateCanvas() {
        if(isEnabled() == true) {
            update();
        }
    }

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

            case shape::Line: {
                auto line = std::dynamic_pointer_cast<lineShape>(mCurrentShape);
                line->setP2(mMouse);
            } break;

            case shape::Polygon:
            case shape::Path:
            default:
                break;
        }
        updateCanvas();
    }

public slots:
    void addRect(const QPointF &point) {
        if(mCurrentShape == nullptr) {
            mCurrentShape = std::make_shared<rectShape>(point, mMouse);
            mCurrentShape->setPen(mPen);
            mCurrentShape->setSelected(true);
            mShapes.push_back(mCurrentShape);
            mSelectedShapes.push_back(mCurrentShape);
        }
        updateCanvas();
    }

    void addLine(const QPointF &point) {
        if(mCurrentShape == nullptr) {
            mCurrentShape = std::make_shared<lineShape>(point, mMouse);
            mCurrentShape->setPen(mPen);
            mCurrentShape->setSelected(true);
            mShapes.push_back(mCurrentShape);
            mSelectedShapes.push_back(mCurrentShape);
        }
        updateCanvas();
    }

    void addEllipse(const QPointF &point) {
        if(mCurrentShape == nullptr) {
            mCurrentShape = std::make_shared<ellipseShape>(point, mMouse);
            mCurrentShape->setPen(mPen);
            mCurrentShape->setSelected(true);
            mShapes.push_back(mCurrentShape);
            mSelectedShapes.push_back(mCurrentShape);
        }
        updateCanvas();
    }

    void addPathPoint(const QPointF &point) {
        if(mCurrentShape == nullptr) {
            mCurrentShape = std::make_shared<pathShape>(point);
            mCurrentShape->setPen(mPen);
            mCurrentShape->setSelected(true);
            mShapes.push_back(mCurrentShape);
            mSelectedShapes.push_back(mCurrentShape);
        } else if(mCurrentShape->type() == shape::Path) {
            auto path = std::dynamic_pointer_cast<pathShape>(mCurrentShape);
            path->pushPoint(point);
        }
        updateCanvas();
    }

    void setSelectedPen() {
        if(mCurrentShape) {
            mCurrentShape->setPen(mPen);
        }
        for(auto &shape : mSelectedShapes) {
            shape->setPen(mPen);
        }
    }

    void clearSelection() {
        for(auto &shape : mSelectedShapes) {
            shape->setSelected(false);
        }
        mSelectedShapes.clear();
        updateCanvas();
    }

    void clearCanvas() {
        mShapes.clear();
        mSelectedShapes.clear();
        mCurrentShape.reset();
        updateCanvas();
    }

    void stopDrawing() {
        if(mCurrentShape) {
            if(mCurrentShape->isNull()) {
                cancelDrawing();
            } else {
                mCurrentShape.reset();
            }
        }
        updateCanvas();
    }

    void cancelDrawing() {
        if(mShapes.empty() == false && mSelectedShapes.empty() == false && mShapes.back()->selected()) {
            mShapes.pop_back();
            mSelectedShapes.pop_back();
        }
        mCurrentShape.reset();
        updateCanvas();
    }

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
     *  @li frameBuffer for raster types.
     *  @li polygonShape.
     *  @li textShape.
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
