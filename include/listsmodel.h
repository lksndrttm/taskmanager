#ifndef LISTSMODEL_H
#define LISTSMODEL_H

#include <QAbstractListModel>
#include <QStandardPaths>
#include <QFile>
#include <vector>
#include <QVariant>
#include "executor.h"

using ListInfoType = std::pair<std::string, std::string>;

class ListsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    ListsModel();
    static void registerMe(const std::string& moduleName);
    int rowCount(const QModelIndex& parent = {}) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole)const override;
    bool insertRowData(int row, db::ListInfoType listInfo);
    QHash<int,QByteArray> roleNames() const override;

    Q_INVOKABLE bool addList(QString listname, QString type);
    Q_INVOKABLE bool delList(QString listname, int row);
    Q_INVOKABLE bool createDatabaseCopy();
private:
    std::vector<ListInfoType> m_lists;//<name,type>
    std::vector<ListInfoType> getLists();
    db::Executor executor;
    enum ListsRoles{
        ListName = Qt::UserRole,
        ListType
    };
};

#endif // LISTSMODEL_H
