#pragma once

#include "qnanoquickitempainter.h"
#include "qnanopainter.h"
#include "nanocanvas.h"

namespace nanoknight {

class nanoCanvas;

class nanoPainter : public QNanoQuickItemPainter {
public:
    nanoPainter();
    void paint(QNanoPainter *p) override;
    void setCanvas(const nanoCanvas* canvas);

private:
    const nanoCanvas* mCanvas;
};
}
