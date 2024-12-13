#include "executor.h"
#include "dbtypes.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <QDebug>
#include <sstream>
#include <algorithm>
#include "connectionmanager.h"

namespace db {

Executor::Executor()
{
    //creat singleton that manage db connection & get instance
    auto& cm = ConnectionManager::instance();
    Q_UNUSED(cm);
}


bool Executor::execute(const QString& query, const std::map<QString,QVariant>& placeholders)
{
    QSqlQuery q;
    if (!q.prepare(query)){
        qWarning() << "Query prepare error in execute";
        return false;
    }

    for(auto&& [placeholder,value] : placeholders){
        q.bindValue(placeholder, value);
    }
    return q.exec();
}

bool Executor::execute(const std::string &query)
{
    QSqlQuery sqlQuery;
    if(!sqlQuery.exec(QString::fromStdString(query))){
        qWarning() << "Execution error\n" << " Query: " << query.c_str();
        return false;
    }
    return true;
}

bool Executor::insert(const Record &newRecord, const std::string &table)
{
    using std::string;
    string queryString = "INSERT INTO " + table + " ";
    string keys = "(";
    string values = "(";
    for (auto &&[key, value] : newRecord) {
        keys +=  key + ", ";

        auto validatedValue =  value;
        std::replace(validatedValue.begin(),validatedValue.end(),'\'','"');

        values += "'" + validatedValue + "'" + ", ";

    }

    keys.assign(keys.begin(), keys.end() - 2);//delete last ", "
    values.assign(values.begin(), values.end() - 2);//delete last ", "
    keys += ")";
    values += ")";

    queryString += keys + " VALUES " + values + ";";

    QSqlQuery query;
    if(!query.exec(QString::fromStdString(queryString))){
        qWarning() << "Insert error\n" << " Table: " << table.c_str();
        qWarning() << queryString.c_str();
        return false;
    }
    return true;
}

bool Executor::insert(const Record &newRecord, const std::string &table, const std::vector<std::string> &blobFieldNames)
{
    std::string queryString = "INSERT INTO " + table + " ";
    std::string keys = "(";
    std::string values = "(";
    std::map<QString,QVariant> placeholders; //<placeholder, placeholder value>
    for (auto &[key, value] : newRecord) {
        keys += key + ", ";
        values += ":" + key + ", ";

        if(std::find(blobFieldNames.begin(),blobFieldNames.end(),key) != blobFieldNames.end()){ //p->isFieldBLOB(key.c_str(), table.c_str())){
            QByteArray blobVal {QByteArray::fromStdString(value)};
            placeholders[":"+QString(key.c_str())] = QVariant::fromValue(std::move(blobVal));
        }
        else
            placeholders[":"+QString(key.c_str())] = QString(value.c_str());
    }
    keys.assign(keys.begin(), keys.end() - 2);//delete last ", "
    values.assign(values.begin(), values.end() - 2);//delete last ", "
    keys += ")";
    values += ")";

    queryString += keys + " VALUES " + values + ";";

    if(!execute(queryString.c_str(), placeholders)){
        return false;
    }
    return true;
}

bool Executor::insertOrReplace(const Record &newRecord, const std::string &table)
{
    std::string query = "insert or replace into " + table + " ";
    std::string keys = "( ";
    std::string values = "( ";
    for (auto &&[key, value] : newRecord) {
        keys += key + ", ";

        auto validatedValue =  value;
        std::replace(validatedValue.begin(),validatedValue.end(),'\'','"');

        values += "'" + validatedValue + "'" + ", ";
    }

    keys.assign(keys.begin(), keys.end() - 2);//delete last ", "
    values.assign(values.begin(), values.end() - 2);//delete last ", "
    keys += ")";
    values += ")";

    query += keys + " VALUES " + values + ";";
    QSqlQuery resQuery;
    if(!resQuery.exec(QString::fromStdString(query))){
        qWarning() << "insert or replace error. (insertOrReplace)";
        qWarning() << "query: " << QString::fromStdString(query);
        return false;
    }
    return true;
}

bool Executor::update(Index id, const Record &changedFields, const std::string &table)
{
    std::string queryString = "UPDATE " + table + " SET ";
    for (auto&& [colName,colValue] : changedFields) {
        auto validatedValue =  colValue;
        std::replace(validatedValue.begin(),validatedValue.end(),'\'','"');

        queryString += colName + " = '" + validatedValue + "', ";
    }
    queryString.assign(queryString.begin(), queryString.end() - 2);//delete last ", "
    queryString += " WHERE id = '" + std::to_string(id) + "';";

    QSqlQuery query;
    if(!query.exec(QString::fromStdString(queryString))){
        qWarning() << "update error\n" << " Table: " << QString(table.c_str());
        qWarning() << queryString.c_str();
        return false;
    }
    return true;
}

bool Executor::updateSet(const std::string &conditioin, const Record &changedFields, const std::string &table)
{
    std::string queryString = "UPDATE " + table + " SET ";
    for (auto&& [colName,colValue] : changedFields) {
        auto validatedValue =  colValue;
        std::replace(validatedValue.begin(),validatedValue.end(),'\'','"');

        queryString += colName + " = '" + validatedValue + "', ";
    }
    queryString.assign(queryString.begin(), queryString.end() - 2);//delete last ", "
    queryString += " WHERE " + conditioin + ";";

    QSqlQuery query;
    if(!query.exec(QString::fromStdString(queryString))){
        qWarning() << "update error\n" << " Table: " << QString(table.c_str());
        qWarning() << queryString.c_str();
        return false;
    }
    return true;
}

bool Executor::deleteRecord(Index id, const std::string &table)
{
    QString queryString = "DELETE FROM " + QString(table.c_str()) + " WHERE id = " + QString(std::to_string(id).c_str()) + ";";
    QSqlQuery query;
    if(!query.exec(queryString)){
        qWarning() << "delete record error." << " Table: " << QString(table.c_str());
        return false;
    }
    return true;
}

bool Executor::deleteRecordByField(const std::string &fieldName, const std::string &fieldVal, const std::string &table)
{
    QString queryString = "DELETE FROM " + QString(table.c_str()) + " WHERE " + QString(fieldName.c_str()) + " = '" + QString(fieldVal.c_str()) + "';";
    QSqlQuery query;
    if(!query.exec(queryString)){
        qWarning() << "delete record error. (deleteRecordByField)" << " Table: " << QString(table.c_str());
        qWarning() << queryString;
        return false;
    }
    return true;
}

Records Executor::select(const std::string &table, const std::string &condition, unsigned long count, bool isReversed, unsigned long offset, const std::string & fields) const
{
    std::string limit = "";
    std::string orderBy = "";
    std::string desc = "";
    std::string offsetStr = "";
    if(count){
        limit = " LIMIT " + std::to_string(count);
    }
    if(isReversed){
        orderBy = "ORDER BY id";
        desc = "DESC";
    }
    if(offset){
        offsetStr = "OFFSET " + std::to_string(offset);
    }
    std::string query {"SELECT " + fields + " FROM " + table + " WHERE " + condition + " " + orderBy + " " + desc + " " + limit + " " + offsetStr + " ;"};
    return convertSQLSelectResultToContainer(query);
}


Record Executor::selectOne(const std::string &table, const std::string &condition, bool isReversed, unsigned long offset) const
{
    Records selectResult = select(table,condition, 1, isReversed, offset);
    if(selectResult.empty()){
        return {};
    }
    return selectResult.front();
}

unsigned long Executor::count(const std::string &table) const
{
    std::string query {"SELECT id FROM "+ table +" WHERE TRUE;"};
    auto selectRes = this->convertSQLSelectResultToContainer(query);
    return selectRes.size();
}

Index Executor::lastIndex(const std::string &table)
{
    std::string query {"SELECT id FROM "+ table +" WHERE TRUE ORDER BY id DESC LIMIT 1;"};
    auto selectRes = this->convertSQLSelectResultToContainer(query);
    if(selectRes.empty()){
        return 0;
    }
    return atol(selectRes.front().at("id").c_str());
}


SelectResult Executor::convertSQLSelectResultToContainer(const std::string &sqlSelectQuery) const
{
    using std::list, std::map, std::string;

    if (!ConnectionManager::instance().isValid())
        {
            qCritical() << "Database is not valid, skip";
            return {};
        }
    QSqlQuery query;
    if(!query.exec(QString::fromStdString(sqlSelectQuery))){
        throw std::invalid_argument("query error in QtSQLiteModel::convertSQLSelectResultToContainer. Query: "+ sqlSelectQuery + "\n");
    }

    list<map<std::string, std::string>> resultList;
    int colCount = query.record().count();


    while (query.next()) {
        map<std::string, std::string> rowMap;
        for(int i = 0; i < colCount; i++){
            std::string colName = query.record().field(i).name().toStdString();
            std::string value = query.value(i).toString().toStdString();

            if(query.record().field(i).type() == QVariant::Type::ByteArray){
                QByteArray byteValue = query.value(i).toByteArray();
                value = byteValue.toStdString();
            } else {
                value = query.value(i).toString().toStdString();
            }

            rowMap.insert(std::make_pair(colName,value));
        }
        resultList.push_back(rowMap);
    }

    return resultList;
}

}
