#include <QQmlApplicationEngine>
#include <QGuiApplication>
#include <QSettings>

#include "utils.h"
#include "updater.h"
#include "knightcanvas.h"
#include "maskwindow.h"

int main(int argc, char *argv[]) {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    // Warning: This will slowdown anay process but fix issues on black screen flashes on CPU render.
    // QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);
    // qputenv("QSG_INFO", "1"); // Will log graphic backend information on console.

    QCoreApplication::setApplicationVersion(QStringLiteral(APP_VERSION));
    QCoreApplication::setOrganizationDomain(QStringLiteral("https://smr76.github.io"));
    QGuiApplication::setApplicationDisplayName(QStringLiteral("Knight Pen"));
    // QCoreApplication::co

    QSettings settings("config.ini", QSettings::IniFormat);

    if(settings.contains("Application/scaleFactor")) {
        QByteArray userDefinedScaleFactor =  settings.value("Application/scaleFactor").toByteArray();
        qputenv("QT_SCALE_FACTOR", userDefinedScaleFactor); // must be called before the app constructor.
    }

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if(!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
