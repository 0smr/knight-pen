#pragma once

#include "qnanoquickitempainter.h"
#include "qnanopainter.h"
#include "knightcanvas.h"

namespace knightPen {

class knightCanvas;

class nanoPainter : public QNanoQuickItemPainter {
public:
    nanoPainter();
    void paint(QNanoPainter *p) override;
    void setCanvas(const knightCanvas* canvas);

private:
    const knightCanvas* mCanvas;
};
}
