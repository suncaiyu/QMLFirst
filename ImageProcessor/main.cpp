#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "imageprocessor.h"
#include <QQmlContext>
#include <QMetaType>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    qmlRegisterType<ImageProcessor>("an.qt.ImageProcessor", 1, 0, "ImageProcessor");
    qRegisterMetaType<ImageProcessor::Handletype>("Handletype");

    QQmlApplicationEngine engine;
//    engine.rootContext()->setContextProperty("processor", new ImageProcessor);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

return app.exec();
}
