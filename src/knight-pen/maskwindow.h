#pragma once

#include <QQmlApplicationEngine>
#include <QQuickPaintedItem>
#include <QQuickWindow>
#include <QQuickItem>
#include <QBitmap>
#include <QPixmap>
#include <QRegion>
#include <QRgb>

class maskWindow : public QQuickWindow {
    Q_OBJECT
    Q_PROPERTY(QSize sectorSize READ sector WRITE setSector NOTIFY sectorChanged)
public:
    maskWindow() : mSector(28, 28) {
        setFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Widget);
        setColor(Qt::transparent);
    }

    void resizeEvent(QResizeEvent *event) override {
        update();
        QQuickWindow::resizeEvent(event);
    }

    const QSize &sector() const { return mSector; }
    void setSector(const QSize &newSectorSize) {
        if(mSector == newSectorSize)
            return;
        mSector = newSectorSize;
        emit sectorChanged();
        update();
    }

public slots:
    void update() {
        updateMask();
        QQuickWindow::update();
    }

    void updateMask() {
        if(mSector.isEmpty() || !isActive()) return;

        QRegion region(width() - 28, 0, mSector.width(), mSector.height());
        QImage windowImage = grabWindow().scaled(width(), height());

        int sectorWidth = mSector.width(), sectorHeight = mSector.height();
        int rows = width() / sectorWidth, cols = height() / sectorHeight;

        for(int i = 0; i < rows; ++i) {
            for(int j = 0 ; j < cols; ++j) {
                QPoint topLeft(sectorWidth * i, sectorHeight * j);
                QPoint center = (topLeft + QPoint(sectorWidth/2, sectorHeight/2));

                if(qAlpha(windowImage.pixel(center.x(), center.y()))) {
                    region = region.united(QRegion(QRect(topLeft, mSector)));
                }
            }
        }
        setMask(region);
    }

signals:
    void sectorChanged();

private:
    QSize mSector;
};

static void registerKPMaskWindowType() {
    qmlRegisterType<maskWindow>("knight.pen.window", 1, 0, "MaskWindow");
}
Q_COREAPP_STARTUP_FUNCTION(registerKPMaskWindowType)
