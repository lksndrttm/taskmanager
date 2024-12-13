#ifndef NOTESMODEL_H
#define NOTESMODEL_H

#include <QAbstractItemModel>
#include <QObject>
#include <QList>
#include <QVariant>
#include <memory>
#include <QQmlEngine>
#include "executor.h"
#include "noteinfo.h"
#include "categoryinfo.h"

class NotesModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant notes READ reversed_notes NOTIFY notesListChanged)
    Q_PROPERTY(QVariant categories READ categories NOTIFY catsListChanged)
    Q_PROPERTY(QVariant categories_counts READ categories_counts NOTIFY catsCountsChanged)
public:
    NotesModel(QObject* parent = nullptr);
    ~NotesModel();
    QVariant notes() const;
    QVariant reversed_notes() const;

    QVariant categories() const;
    QVariant categories_counts() const;

    static void registerMe(const std::string& moduleName);

public slots:
    void showCategory(QString cat);
//    void showTag(QString cat);
    void showFavorite();
    void showAll();

    bool addNote(QString title, QString data, QString createDTime, QString category, QString tags, bool favorite);
    bool updateNote(int id ,QString title, QString data, QString updateDTime, QString category, QString tags, bool favorite=false);
    bool deleteNote(int id);
    bool changeNoteFavorite(int id);

    bool createCategory(QString name, QString color, QString image = "");
    bool updateCategory(int id, QString name, QString color,  bool updateRelatedNotes = true, QString image = "");
    bool deleteCategory(int id);

signals:
    void notesListChanged();
    void catsListChanged();
    void catsCountsChanged();

private:
    QList<QObject*> m_notes;
    QList<QObject*> m_cats;
    std::unique_ptr<db::Executor> m_executor;
    void refreshNotes();
    int notesCount() const;
    int favNotesCount() const;


    QString reservateImages(const QString & text);
    bool unreservateImages(const QString & text);
    bool unreservateUnneededImages(const QString & old, const QString & updated);
    std::vector<QString> extractImages(const QString & text);
};

#endif // NOTESMODEL_H
