#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QFontDatabase>
#include "listsmodel.h"
#include "tasklistmodel.h"
#include "iterabletaskmodel.h"
#include "notesmodel.h"
#include "richtexthandler.h"


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.addImportPath(":/qml");
    ListsModel::registerMe("ListsModel");
    TaskListModel::registerMe("TaskListModel");
    IterModel::registerMe("IterModel");
    NotesModel::registerMe("NotesModel");
    qmlRegisterType<RichTextHandler>("RichTextHandler", 1, 0, "RichTextHandler");//"DocumentHandler");

    if (QFontDatabase::addApplicationFont(":/fonts/fontello.ttf") == -1)
        qWarning() << "Failed to load fontello.ttf";

    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
