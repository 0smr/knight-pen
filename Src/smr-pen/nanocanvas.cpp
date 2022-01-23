#include "nanocanvas.h"

namespace nanoknight {
nanoCanvas::nanoCanvas(QQuickItem *parent) : QNanoQuickItem(parent) {
    mSelectPen.mStrokeColor = QColor(79, 127, 255, 255);
    mSelectPen.mWidth = 0.6;
    mScaleFactor = 1.0f;

    connect(this, &nanoCanvas::penChanged, this, &nanoCanvas::setSelectedPen);
}

QNanoQuickItemPainter *nanoCanvas::createItemPainter() const {
    nanoPainter *qquickItem = new nanoPainter();
    qquickItem->setCanvas(this);
    return qquickItem;
}

void nanoCanvas::setSelectedTool(const Tools &newSelectedTool) {
    if(mSelectedTool == newSelectedTool)
        return;
    mSelectedTool = newSelectedTool;
    emit selectedToolChanged();
    update();
}
}
