#include "knightcanvas.h"

namespace knightPen {
knightCanvas::knightCanvas(QQuickItem *parent) : QNanoQuickItem(parent) {
    mSelectPen.mStrokeColor = 0xff4f7fff;
    mSelectPen.mFillColor = 0xff4f7fff;
    mSelectPen.mWidth = 0.6f;
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
