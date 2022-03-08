#include "knightcanvas.h"

namespace knightPen {
knightCanvas::knightCanvas(QQuickItem *parent) : QNanoQuickItem(parent) {
    mSelectPen.mStrokeColor = QColor(79, 127, 255, 255);
    mSelectPen.mFillColor = QColor(79, 127, 255, 255);
    mSelectPen.mWidth = 0.6;
    mScaleFactor = 1.0f;

    connect(this, &knightCanvas::penChanged, this, &knightCanvas::setSelectedPen);
}

QNanoQuickItemPainter *knightCanvas::createItemPainter() const {
    nanoPainter *qquickItem = new nanoPainter();
    qquickItem->setCanvas(this);
    return qquickItem;
}

void knightCanvas::setSelectedTool(const Tools &newSelectedTool) {
    if(mSelectedTool == newSelectedTool)
        return;
    mSelectedTool = newSelectedTool;
    emit selectedToolChanged();
    update();
}
}
