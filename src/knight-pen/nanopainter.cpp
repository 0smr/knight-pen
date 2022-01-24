#include "nanopainter.h"

namespace nanoknight {

nanoPainter::nanoPainter() {}

void nanoPainter::paint(QNanoPainter *p) {
    mCanvas->painter(p);
}

void nanoPainter::setCanvas(const nanoCanvas *canvas) {
    mCanvas = canvas;
}

}
