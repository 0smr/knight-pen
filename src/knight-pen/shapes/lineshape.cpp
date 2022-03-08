#include "lineshape.h"

namespace knightPen {

lineShape::operator pathShape() const {
    pathShape path(*this, mPen);
    path.setSelected(selected());
    path[0].setSelected(mP1);
    path[1].setSelected(mP2);
    return path;
}

}
