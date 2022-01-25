#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "utils.h"
#include "nanocanvas.h"
#include "maskwindow.h"

int main(int argc, char *argv[]) {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    qmlRegisterType<utils>("knight.pen.utils", 1, 0, "Utils");
    qmlRegisterType<tileMaskWindow>("knight.pen.window", 1, 0, "TileMaskWindow");
    qmlRegisterType<nanoknight::nanoCanvas>("knight.pen.canvas", 1, 0, "NanoCanvas");

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
