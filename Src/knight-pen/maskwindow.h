#pragma once

#include <QQuickPaintedItem>
#include <QQuickWindow>
#include <QQuickItem>
#include <QPainter>
#include <QBitmap>
#include <QPixmap>
#include <QRgb>

class maskWindow : public QQuickWindow {
    Q_OBJECT
public:
    maskWindow() {
        setFlag(Qt::FramelessWindowHint, true);
        setFlag(Qt::WindowStaysOnTopHint, true);
        setFlag(Qt::Tool, true);
        setColor(Qt::white);
    }

    /**
     * TODO: transparent mouse & keyboard input on specific spots.
     *  https://stackoverflow.com/questions/20401896/qt-global-mouse-listener
     *  https://stllug.sluug.org/meeting_notes/1997/0619/xkey.html
     *  https://stackoverflow.com/questions/8629474/how-to-globally-capture-every-mouse-click-in-x11
     */
signals:
};
