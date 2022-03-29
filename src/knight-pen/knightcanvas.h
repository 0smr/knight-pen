#pragma once

#include "qnanopainter.h"
#include "qnanoquickitem.h"
#include "qnanoquickitempainter.h"

#include <qnamespace.h>

#include <QQmlApplicationEngine>
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
#include "shapes/shapes.h"
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
        Line = 0x1,
        Arrow = 0x2,
        Ellipse = 0x4,
        Rectangle = 0x8,
        Path = 0x10,
        Polygon = 0x20,
        Pencil = 0x40,
        DrawTools = 0x7F,
        Selection,
        DirectSelection,
        GroupSelectionToggle,
        GroupSelectionInclude,
        GroupSelectionExclude,
        SelectionTools = 0x0
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

    /// TODO: complete following functions.
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
    Q_INVOKABLE bool selectAt(const QPointF &point, bool discardSelected = false) {
        return mSelectedTool == Tools::Selection ? selectShapesAt(point, discardSelected)
                                                 : selectAnchorsAt(point, discardSelected);
    }
    Q_INVOKABLE bool selectAnchorsAt(const QPointF &point, bool discardSelected) {
        Q_UNUSED(point)
        Q_UNUSED(discardSelected)
        return false;
    }
    Q_INVOKABLE bool selectShapesAt(const QPointF &point, bool discardSelected) {
        if(!discardSelected) clearSelection();
        for(auto shapeIter = mShapes.rbegin(); shapeIter != mShapes.rend(); ++shapeIter) {
            if((*shapeIter)->contains(point) != PointState::None) {
                mSelectedShapes.push_back(*shapeIter);

                emit selectedShapesChanged();
                updatePen();
                return true;
            }
        }

        return false;
    }

    Q_INVOKABLE bool selectAtArea(const QRectF &area) { Q_UNUSED(area) return false; }
    Q_INVOKABLE bool selectAnchorsAtArea(const QRectF &area) { Q_UNUSED(area) return false; }
    Q_INVOKABLE bool selectShapesAtArea(const QRectF &area) { Q_UNUSED(area) return false; }

    Q_INVOKABLE bool selectAtArea(const QPointF &area, float radius) { Q_UNUSED(area) Q_UNUSED(radius) return false; }
    Q_INVOKABLE bool selectAnchorsAtArea(const QPointF &area, float radius) { Q_UNUSED(area) Q_UNUSED(radius) return false; }
    Q_INVOKABLE bool selectShapesAtArea(const QPointF &area, float radius) { Q_UNUSED(area) Q_UNUSED(radius) return false; }

    /// @brief getter functions.
    const Tools &selectedTool() const { return mSelectedTool; }
    const QPointF &mouse() const { return mMouse; }
    QColor strokeColor() const { return mPen.mStrokeColor; }
    QColor fillColor() const { return mPen.mFillColor; }
    float strokeWidth() const { return mPen.mWidth; }
    float miterLimit() const { return mPen.mMiter; }
    Qt::PenCapStyle lineCap() const { return mPen; }
    Qt::PenJoinStyle lineJoin() const { return mPen; }
    float scaleFactor() const { return mScaleFactor; }
    bool drawing() const { return mCurrentShape.use_count() > 0; }

    /// @brief setter functions.
    void setSelectedTool(const Tools &newSelectedTool);
    void setStrokeWidth(float newWidth) {
        newWidth = std::max(0.0f, newWidth);
        if(mPen.mWidth == newWidth)
            return;
        mPen.mWidth = newWidth;
        emit strokeWidthChanged();
        emit penChanged();
        update();
    }
    void setStrokeColor(const QColor &newColor) {
        if(mPen.mStrokeColor == newColor.rgb()) return;
        mPen.mStrokeColor = newColor.rgb();
        emit strokeColorChanged();
        emit penChanged();
        update();
    }
    void setFillColor(const QColor &newColor) {
        if(mPen.mFillColor == newColor.rgb()) return;
        mPen.mFillColor = newColor.rgb();
        emit fillColorChanged();
        emit penChanged();
        update();
    }
    void setMiterLimit(float newMiterLimit) {
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
        visualizeCurrentShape();
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
    /// private functions.
    void updatePen() {
        if(mSelectedShapes.empty() == false) {
            nanoPen sPen = mSelectedShapes.front()->pen(),
                    temp = mPen;
            mPen = sPen;

            if(temp.mStrokeColor != sPen.mStrokeColor ) emit strokeColorChanged();
            if(temp.mFillColor != sPen.mFillColor) emit fillColorChanged();
            if(temp.mWidth != sPen.mWidth) emit strokeWidthChanged();
            if(temp.mMiter != sPen.mMiter) emit miterLimitChanged();
            if(temp.mJoin != sPen.mJoin) emit lineJoinChanged();
            if(temp.mCap != sPen.mCap) emit lineCapChanged();

            emit penChanged();
        }
    }

    void visualizeCurrentShape() {
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
        update();
    }

public slots:
    void update() {
        if(isEnabled() == true) {
            QQuickItem::update();
        }
    }

    void addRect(const QPointF &point) {
        if(mCurrentShape == nullptr) {
            mCurrentShape = std::make_shared<rectShape>(point, mMouse);
            mCurrentShape->setPen(mPen);
            mCurrentShape->setSelected(true);
            mShapes.push_back(mCurrentShape);
            mSelectedShapes.push_back(mCurrentShape);

            emit selectedShapesChanged();
        }
        update();
    }

    void addLine(const QPointF &point) {
        if(mCurrentShape == nullptr) {
            mCurrentShape = std::make_shared<lineShape>(point, mMouse);
            mCurrentShape->setPen(mPen);
            mCurrentShape->setSelected(true);
            mShapes.push_back(mCurrentShape);
            mSelectedShapes.push_back(mCurrentShape);

            emit selectedShapesChanged();
        }
        update();
    }

    void addEllipse(const QPointF &point) {
        if(mCurrentShape == nullptr) {
            mCurrentShape = std::make_shared<ellipseShape>(point, mMouse);
            mCurrentShape->setPen(mPen);
            mCurrentShape->setSelected(true);
            mShapes.push_back(mCurrentShape);
            mSelectedShapes.push_back(mCurrentShape);

            emit selectedShapesChanged();
        }
        update();
    }

    void addPathPoint(const QPointF &point) {
        if(mCurrentShape == nullptr) {
            mCurrentShape = std::make_shared<pathShape>(point);
            mCurrentShape->setPen(mPen);
            mCurrentShape->setSelected(true);
            mShapes.push_back(mCurrentShape);
            mSelectedShapes.push_back(mCurrentShape);

            emit selectedShapesChanged();
        } else if(mCurrentShape->type() == shape::Path) {
            auto path = std::dynamic_pointer_cast<pathShape>(mCurrentShape);
            /// pathes can be closed in two condition:
            /// 1. path must contain more than 2 point.
            /// 2. distance between tail and head must be lower than 3 pixel.
            if(QLineF(point, path->at(0)).length() < 3 && path->size() >= 3) {
                path->setClosed(true);
                stopDrawing();
            } else {
                path->pushPoint(point);
            }
        }
        update();
    }

    void setSelectedPen() {
        if(mCurrentShape) {
            mCurrentShape->setPen(mPen);
        }
        for(auto &shape : mSelectedShapes) {
            shape->setPen(mPen);
        }
    }

    void eraseSelection() {
        for(const auto &selectedShape: mSelectedShapes) {
            auto end = std::remove(mShapes.begin(), mShapes.end(), selectedShape);
            mShapes.resize(std::distance(mShapes.begin(), end));
        }
        mSelectedShapes.clear();

        emit selectedShapesChanged();
        update();
    }

    void clearSelection() {
        for(auto &shape : mSelectedShapes) {
            shape->setSelected(false);
        }
        mSelectedShapes.clear();

        emit selectedShapesChanged();
        update();
    }

    void clearCanvas() {
        mShapes.clear();
        mSelectedShapes.clear();
        mCurrentShape.reset();

        emit selectedShapesChanged();
        update();
    }

    void stopDrawing() {
        if(mCurrentShape) {
            if(mCurrentShape->isNull()) {
                cancelDrawing();
            } else {
                mCurrentShape.reset();
            }
        }
        update();
    }

    void cancelDrawing() {
        if(mShapes.empty() == false && mSelectedShapes.empty() == false && mShapes.back()->selected()) {
            mShapes.pop_back();
            mSelectedShapes.pop_back();
        }
        mCurrentShape.reset();

        emit selectedShapesChanged();
        update();
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
    void selectedShapesChanged();
    void shapeSelected();

private:
    /**
     * TODO: classified snap points.
     *  nanoPen mSnapGuidePen;
     *
     *  TODO: other type of shapes.
     *  @li frameBuffer for raster types.
     *  @li polygonShape.
     *  @li textShape.
     *
     *  add groups.
     *
     *  mAboutToClosePath -> bool
     *  mAboutToJoin -> bool
     *  mNearCorners -> pair<shape, APoint>
     *  mNearEdges -> pair<shape, APoints>
     */
    std::vector<std::shared_ptr<shape>> mShapes;
    std::vector<std::shared_ptr<shape>> mSelectedShapes;
    std::shared_ptr<shape> mCurrentShape;

    std::vector<QRectF> mArtwork;

    QPointF mMouse;
    nanoPen mPen;
    nanoPen mSelectPen;

    Tools mSelectedTool;

    bool mHover;
    float mScaleFactor;
};
}

static void registerKnightCanvasType() {
    qmlRegisterType<knightPen::knightCanvas>("knight.pen.canvas", 1, 0, "KnightCanvas");
}
Q_COREAPP_STARTUP_FUNCTION(registerKnightCanvasType)
