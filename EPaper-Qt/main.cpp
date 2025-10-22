#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "uicontroller.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);


    QQmlApplicationEngine engine;
    UiController uiController;
    engine.rootContext()->setContextProperty("uiController", &uiController);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("EPaper_Qt", "Main");

    return app.exec();
}
