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

#include <cmath>
#include <map>

class utils : public QObject {
    Q_OBJECT
public:
    enum CursorShapes {
        SHAPE_SELECT,
        POINT_SELECT,
    };
    Q_ENUM(CursorShapes)

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

    Q_INVOKABLE QString timeToShortStr(float milisecond) {
        std::vector<std::pair<QString, float>> timeTable {
            {"day", 86400000}, {"hour", 3600000}, {"minute", 60000}, {"second", 1000}, {"milisec", 1},
        };

        for(const auto &timeFormat : timeTable) {
            if(milisecond > timeFormat.second) {
                return QString("%1 %2").arg(std::floor(milisecond/timeFormat.second))
                                       .arg(timeFormat.first);
            }
        }

        return "a bit";
    }

    static utils* getInstance() {
        return instance != nullptr ? instance : new utils();
    }

    static QObject* qmlSingletoProvider(QQmlEngine *engine, QJSEngine *scriptEngine) {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)
        return utils::getInstance();
    }

private:
    explicit utils(QObject *parent = nullptr) : QObject{parent} {
        mScreens = QGuiApplication::screens();
    }

    static inline utils* instance = nullptr;

    std::map<utils::CursorShapes, QCursor> mCursorShapes;
    QList<QScreen *> mScreens;
    QPixmap mScreenPixmap;
};

static void registerKPUtilsType() {
    qmlRegisterSingletonType<utils>("knight.pen.utils", 1, 0, "Utils", utils::qmlSingletoProvider);
}
Q_COREAPP_STARTUP_FUNCTION(registerKPUtilsType)
