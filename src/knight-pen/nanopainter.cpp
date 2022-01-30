#include "nanopainter.h"

namespace knightPen {

nanoPainter::nanoPainter() {}

void nanoPainter::paint(QNanoPainter *p) {
    mCanvas->painter(p);
}

void nanoPainter::setCanvas(const knightCanvas *canvas) {
    mCanvas = canvas;
}

}
