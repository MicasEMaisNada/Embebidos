#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "display.h"
int main(int argc, char *argv[])
{   QCoreApplication app(argc, argv);
    Display display;
    display.get_info_database();
    return app.exec();
/*    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
 //   const QUrl url(QStringLiteral("qrc:/main.qml"));
*//*    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);*/
/*    engine.rootContext()->setContextProperty("display",display);
    engine.load(QUrl(QString("qrc:/main.qml")));
    if(engine.rootObjects().isEmpty())
        return -1;
    return app.exec();*/
}
