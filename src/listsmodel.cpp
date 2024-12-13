#include "listsmodel.h"
#include <QQmlEngine>
#include <QDebug>
#include <algorithm>

ListsModel::ListsModel()
{    
    m_lists = this->getLists();
}

void ListsModel::registerMe(const std::string &moduleName)
{
    qmlRegisterType<ListsModel>(moduleName.c_str(), 1, 0, "ListsModel");
}

int ListsModel::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return m_lists.size();
}

QVariant ListsModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || index.row() > rowCount()){
        return {};
    }
    switch (role) {
        case ListsRoles::ListName: {
            return QVariant::fromValue(QString::fromStdString(m_lists.at(index.row()).first));
        }
        case ListsRoles::ListType: {
            return QVariant::fromValue(QString::fromStdString(m_lists.at(index.row()).second));
        }
        default:{
            return {};
        }

    }
}

bool ListsModel::insertRowData(int row, db::ListInfoType listInfo)
{
    qDebug() << "name: " << QString::fromStdString(listInfo.first);
    qDebug() << "type: " << QString::fromStdString(listInfo.second);

    //rename list type for compatibility
    if(listInfo.second == "tasklist"){
        listInfo.second = "targetList";
    }

    db::Record recToInsert;
    recToInsert["name"] = listInfo.first;
    recToInsert["type"] = listInfo.second;

    bool insertionStat = executor.insert(recToInsert,"lists");
    if(insertionStat){
       beginInsertRows(QModelIndex{} ,row, row);
       m_lists.push_back(std::move(listInfo));
       endInsertRows();
       return true;
    }
    return false;
}

QHash<int, QByteArray> ListsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ListsRoles::ListName] = "name";
    roles[ListsRoles::ListType] = "type";
    return roles;
}

bool ListsModel::addList(QString listname, QString type)
{
    return insertRowData(rowCount(),std::make_pair(listname.toStdString(),type.toStdString()));
}

bool ListsModel::delList(QString listname, int row)
{
    bool deletionStat = executor.deleteRecordByField("name", listname.toStdString(), "lists");
    if(deletionStat){
        beginRemoveRows(QModelIndex{} ,row, row);
        m_lists.erase(m_lists.begin()+row);
        endRemoveRows();
        return true;
    }
    return false;
}

bool ListsModel::createDatabaseCopy()
{
    const QString dbname {"insei.db"};
    const QString location {QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)};
    const QString fullPath {location + "/" + dbname};

    QString newLocation {QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)};
    QString newName {"cta71r-qeeW24-ert-1234ro-op1891"};
    QString newPath = {newLocation + "/" + newName};

    qDebug() << "Database copied to " << newPath;
    return QFile::copy(fullPath,newPath);
}

std::vector<ListInfoType> ListsModel::getLists()
{
    using std::vector, std::map, std::string;
    vector<std::pair<string, string>> res;

    auto lists = this->executor.select("lists");

    if(lists.empty()) {
        qDebug() << "Retuned empty list\n";
        return {};
    }

    for(auto &&list : lists){
        res.push_back(std::make_pair(list.at("name"),list.at("type")));
    }
    return res;
}
