#pragma once

#include <QQuickItem>
#include <QGuiApplication>

#include <QFontDatabase>
#include <QScreen>
#include <QCursor>
#include <QPixmap>
#include <QImage>
#include <QIcon>
#include <QRgb>

class utils : public QObject {
    Q_OBJECT
public:
    enum CursorShapes {
        SHAPE_SELECT,
        POINT_SELECT,
    };
    Q_ENUM(CursorShapes)

    explicit utils(QObject *parent = nullptr) : QObject{parent} {
        mScreens = QGuiApplication::screens();
    }

    /**
     * @brief pickColorAt
     * @param x
     * @param y
     * @return screen color a (x,y) coordinate.
     * TODO: create an image provider.
     */
    Q_INVOKABLE QColor pickColorAt(int x, int y) {
        mScreenPixmap = mScreens.at(0)->grabWindow(0, x, y, 1, 1);
        QRgb pixel = mScreenPixmap.toImage().pixel(0, 0);
        return pixel;
    }

    /**
     * @brief takeScreenShot
     * @return true if successful otherwise return false.
     */
    Q_INVOKABLE bool takeScreenShot() const {
        QPixmap screen = mScreens.at(0)->grabWindow(0);
        return screen.save ("image", "JPEG");
    }

    Q_INVOKABLE QString systemFixedFont() const {
        return QFontDatabase::systemFont(QFontDatabase::FixedFont).family();
    }

    Q_INVOKABLE void setCursorShape(utils::CursorShapes cursor) const {
        QIcon("qrc:/Resources/Cursor/DirectSelection.svg").pixmap(1);
        QGuiApplication::setOverrideCursor(mCursorShapes.at(cursor));
    }

    Q_INVOKABLE QString applicationVersion() {
        return QGuiApplication::applicationVersion();
    }

private:
    std::map<utils::CursorShapes, QCursor> mCursorShapes;
    QList<QScreen *> mScreens;
    QPixmap mScreenPixmap;
};
