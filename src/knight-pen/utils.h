#pragma once

#include <QQuickItem>
#include <QGuiApplication>

#include <QDesktopServices>
#include <QFontDatabase>
#include <QSettings>
#include <QProcess>
#include <QScreen>
#include <QCursor>
#include <QPixmap>
#include <QString>
#include <QImage>
#include <QIcon>
#include <QDir>
#include <QRgb>

#include <vector>
#include <cmath>
#include <map>

class utils : public QObject {
    Q_OBJECT
    Q_PROPERTY(Platform platform READ platform CONSTANT)

public:
    enum Platform {
        Windows,
        Linux,
        Mac,
        Other,
    };
    Q_ENUM(Platform) ///> Add platform enum type.

    enum CursorShapes {
        SHAPE_SELECT,
        POINT_SELECT,
    };
    Q_ENUM(CursorShapes)

    enum ThemeType {
        Light,
        Dark,
        None,
    };
    Q_ENUM(ThemeType)

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

    Q_INVOKABLE bool osLightModeEnabled() {
        if(PLATFORM == Platform::Windows) {
            QSettings setting("HKEY_URRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", QSettings::NativeFormat);
            return setting.value("SystemUsesLightTheme").toBool();
        }

        return false; //> in case of error or unknown os.
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

        return "a moment";
    }

    Q_INVOKABLE bool openFileInExplorer(QString filePath) {
        if(PLATFORM == Platform::Windows) {
            QStringList args;
            args << "/select," << QDir::toNativeSeparators(filePath);
            return QProcess::startDetached("explorer", args);
        } else if(PLATFORM == Platform::Mac) {
            QStringList args;
            args << "-e" << "tell application \"Finder\""
                 << "-e" << "activate"
                 << "-e" << "select POSIX file \"" + filePath + "\""
                 << "-e" << "end tell";
            return QProcess::startDetached("osascript", args);
        } else if(PLATFORM == Platform::Linux) {
            return QDesktopServices::openUrl(filePath);
        } else {
            return false;
        }
    }

    static utils* getInstance() {
        return instance != nullptr ? instance : new utils();
    }

    static QObject* qmlSingletoProvider(QQmlEngine *engine, QJSEngine *scriptEngine) {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)
        return utils::getInstance();
    }

    Platform platform() const { return PLATFORM; }

#if defined(WIN64) || defined(_WIN64) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    static inline const Platform PLATFORM = Platform::Windows;
#elif defined(linux) || defined(__linux) || defined(__linux__)
    static inline const Platform PLATFORM = Platform::Linux;
#elif defined(__APPLE__) || defined(__MACH__)
    static inline const Platform PLATFORM = Platform::Mac;
#else
    static inline const Platform PLATFORM = Platform::Other;
#endif
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
