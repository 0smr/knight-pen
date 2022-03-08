#pragma once

#include <QLineF>

#include <cmath>

#include "qnanopainter.h"
#include "nanopen.h"

namespace knightPen {

enum PointProxy {
    Center,
    TopLeft,
    Top,
    TopRight,
    Right,
    BottomRight,
    Bottom,
    BottomLeft,
    Left,
};

enum PointState {
    Inside,
    Edge,
    None
};

/**
 * @brief The nanoPoint struct
 * represent selectable anchor point.
 */
class APointF : public QPointF {
public:
    constexpr APointF(const QPointF &p = QPointF(),
        const QPointF &cp1 = QPointF(),
        const QPointF &cp2 = QPointF())
        : QPointF(p), mControl1(cp1), mControl2(cp2), mSelected(false) {}
    constexpr APointF(const APointF &p)
        : QPointF(p.x(), p.y()),
          mControl1(p.mControl1),
          mControl2(p.mControl2),
          mSelected(p.mSelected) {}
    constexpr APointF(float x, float y,
        bool selected = false,
        const QPointF &cp1 = QPointF(),
        const QPointF &cp2 = QPointF())
        : QPointF(x, y), mControl1(cp1), mControl2(cp2), mSelected(selected) {}

    const APointF &operator =(const APointF &in) {
        setX(in.x());
        setY(in.y());
        mControl2 = in.mControl2;
        mControl1 = in.mControl1;
        mSelected = in.mSelected;
        return *this;
    }

    const APointF &operator =(const QPointF &in) {
        setX(in.x());
        setY(in.y());
        return *this;
    }

    bool isBeizer() const {
        return mControl1.isNull() == false || mControl1.isNull() == false;
    }

    bool isBetween(const QPointF &p1, const QPointF &p2, float trolance = 0.5f) {
        const float distToPoint = QLineF(p1, *this).length() + QLineF(*this, p2).length();
        return std::abs(distToPoint - QLineF(p1, p2).length()) <= trolance;
    }

    APointF transformed(const QTransform &transform) const {
        APointF result = transform.map(*this);
        result.mControl1 = transform.map(mControl1);
        result.mControl2 = transform.map(mControl2);
        result.mSelected = mSelected;
        return result;
    }

    const APointF &transform(const QTransform &transform) {
        *this = transformed(transform);
        return *this;
    }

    APointF &rotate(float angle, const QPointF &origin = QPointF(0.0, 0.0)) {
        APointF::rotate(*this, angle, origin);
        return *this;
    }

    static QPointF rotate(const QPointF &p, float angle, const QPointF &origin = QPointF(0.0, 0.0)) {
        QTransform transform = QTransform()
                                   .translate(origin.x(), origin.y())
                                   .rotate(angle)
                                   .translate(-origin.x(), -origin.y());
        return transform.map(p);
    }

    QPointF control1() const { return mControl1; }
    QPointF control2() const { return mControl2; }
    bool selected() const { return mSelected; }

    void setControl1(QPointF newControl1) { mControl1 = newControl1; }
    void setControl2(QPointF newControl2) { mControl2 = newControl2; }
    void setSelected(bool newSelected) { mSelected = newSelected; }

private:
    QPointF mControl1, mControl2;  // Control points 1 & 2
    bool mSelected;
};
}
