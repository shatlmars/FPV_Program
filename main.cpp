#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "gamepad/gamepadmanager.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    GamepadManager gamepad;
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("gamepad", &gamepad);
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    // engine.loadFromModule("FPV_GR", "main");

    engine.load(QUrl(QStringLiteral("qrc:/qt/qml/FPV_GR/qml/main.qml")));
    return QCoreApplication::exec();
}
