#include "qtsqlitemodel.h"
#include <fstream>

QtSQLiteModel::QtSQLiteModel()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);
    if(!db.open()){
        qDebug() << db.lastError().text();
        throw std::invalid_argument("Open database error. Databese name: "+ dbName.toStdString() + "\nerror text: " + db.lastError().text().toStdString() + "\n");
    }
    list<QString> tablesList;
    //targets models descriptors
    tablesList.push_back(mostFullTargetsTableDescription);
    //lists models descriptors
    tablesList.push_back(mostFullListsTableDescription);
    //iterableTasks
    tablesList.push_back(mostFullIterableTasksTableDescription);
    tablesList.push_back(scheduleCompliteHistTable);
    initTables(tablesList);
}

QtSQLiteModel::~QtSQLiteModel()
{
    db.close();
}

list<map<std::string, std::string> > QtSQLiteModel::readSubTargets(std::string hierarchyAddr)
{
    auto columnNames = getColumnsNames("targets");
    //string query = "SELECT * FROM targets WHERE hierarchyAddr LIKE '" + hierarchyAddr + "\\_%' ESCAPE '\\';";
    string query = "select * from targets where hierarchyAddr like '" + hierarchyAddr + "\\_%' escape '\\'  and hierarchyAddr not like '" + hierarchyAddr + "\\_%\\_%' escape '\\';";
    auto res = convertSQLSelectResultToContainer(query);
    return res;
}

void QtSQLiteModel::deleteSubTargets(std::string hierarchyAddr)
{
    QString queryString =  "delete from targets where hierarchyAddr like '"+ QString::fromStdString(hierarchyAddr)
            +"\\_%' escape '\\' and hierarchyAddr not like '" + QString::fromStdString(hierarchyAddr) +"\\_%\\_%' escape '\\';";
    QSqlQuery query;
    if(!query.exec(queryString)){
        throw std::invalid_argument("query error in QtSQLiteModel::deleteSubTargets. Query: "+ queryString.toStdString() + "\n");
    }
}

void QtSQLiteModel::addTarget(map<std::string, std::string> trg)
{
    string queryString = "INSERT INTO targets ";
    string keys = "(";
    string values = "(";
    for (auto &[key, value] : trg) {
        keys += key + ", ";
        values += "'" + value + "'" + ", ";
    }
    keys.assign(keys.begin(), keys.end() - 2);//delete last ", "
    values.assign(values.begin(), values.end() - 2);//delete last ", "
    keys += ")";
    values += ")";

    queryString += keys + "VALUES " + values + ";";
    QSqlQuery query;
    if(!query.exec(QString::fromStdString(queryString))){
        qDebug() << "Query error\n";
        throw std::invalid_argument("query error in QtSQLiteModel::addTarget. Query: "+ queryString + "\n");
    }
}

void QtSQLiteModel::modifyTarget(map<std::string, std::string> modifiedTrg)
{
    string queryString = "UPDATE targets SET ";
    for (auto [colName,colValue] : modifiedTrg) {
        if(colName == "id" || colName == "hierarchyAddr" || colName == "createTime" || colName == "status" || colName == "targetType")//close to modify fields
            continue;
        queryString += colName + " = '" + colValue + "', ";
    }
    queryString.assign(queryString.begin(), queryString.end() - 2);//delete last ", "
    queryString += " WHERE hierarchyAddr = '" + modifiedTrg.at("hierarchyAddr") + "';";

    QSqlQuery query;
    if(!query.exec(QString::fromStdString(queryString))){
        qDebug() << "Query error\n";
        throw std::invalid_argument("query error in QtSQLiteModel::modifyTarget. Query: "+ queryString + "\n");
    }

}

void QtSQLiteModel::deleteTarget(std::string hierarchyAddr)
{
    QString queryString = "DELETE FROM targets WHERE hierarchyAddr LIKE '" + QString::fromStdString(hierarchyAddr) + "%';";
    QSqlQuery query;
    if(!query.exec(queryString)){
        throw std::invalid_argument("query error in QtSQLiteModel::deleteTarget. Query: "+ queryString.toStdString() + "\n");
    }
}

void QtSQLiteModel::updateStatus(std::string status, std::string hierarchyAddr)
{
    QString queryString = "UPDATE targets SET status = '" + QString::fromStdString(status) + "' WHERE hierarchyAddr = '" + QString::fromStdString(hierarchyAddr) + "';";
    QSqlQuery query;
    if(!query.exec(queryString)){
        throw std::invalid_argument("query error in QtSQLiteModel::updateStatus. Query: "+ queryString.toStdString() + "\n");
    }
}

map<std::string, std::string> QtSQLiteModel::getTargetById(int id)
{
    std::stringstream ss;
    string query = "SELECT * FROM targets WHERE id = ";
    ss << query << id;
    query = ss.str();
    query += ";";
    auto selectRes = convertSQLSelectResultToContainer(query);
    if (!selectRes.empty())
        return selectRes.front();
    else
        return map<string, string>();
}

map<std::string, std::string> QtSQLiteModel::getTargetByAddr(std::string hierarchyAddr)
{
    string query = "SELECT * FROM targets WHERE hierarchyAddr = '" + hierarchyAddr + "';";
    auto selectRes = convertSQLSelectResultToContainer(query);
    if (!selectRes.empty())
        return selectRes.front();
    else
        return map<string, string>();
}

map<std::string, std::string> QtSQLiteModel::getLastTarget()
{
    string query = "SELECT * FROM targets ORDER BY id DESC LIMIT 1;";
    auto selectRes = convertSQLSelectResultToContainer(query);
    if (!selectRes.empty())
        return selectRes.front();
    else
        return map<string, string>();
}

map<std::string, std::string> QtSQLiteModel::getTargetbyName(std::string name)
{
    string query = "SELECT * FROM targets WHERE hierarchyAddr not LIKE '%\\_%\\_%' escape '\\' and name = '" + name + "';";
    auto selectRes = convertSQLSelectResultToContainer(query);
    if (!selectRes.empty())
        return selectRes.front();
    else
        return map<string, string>();
}

void QtSQLiteModel::createList(map<std::string, std::string> targetList)
{
    string query = "INSERT INTO lists ";
    string keys = "(";
    string values = "(";
    for (auto &[key, value] : targetList) {
        keys += key + ", ";
        values += "'" + value + "'" + ", ";
    }
    keys.assign(keys.begin(), keys.end() - 2);//delete last ", "
    values.assign(values.begin(), values.end() - 2);//delete last ", "
    keys += ")";
    values += ")";

    query += keys + "VALUES " + values + ";";
    QSqlQuery resQuery;
    if(!resQuery.exec(QString::fromStdString(query))){
        throw std::invalid_argument("query error in QtSQLiteModel::createList. Query: "+ query + "\n");
    }
}

void QtSQLiteModel::deleteTargetList(std::string listName)
{
    clearList(listName);
    QString query = "DELETE FROM lists WHERE name='" + QString::fromStdString(listName) + "';";
    QSqlQuery resQuery;
    if(!resQuery.exec(query)){
        throw std::invalid_argument("query error in QtSQLiteModel::deleteTargetList. Query: "+ query.toStdString() + "\n");
    }

}

void QtSQLiteModel::clearList(std::string listName)
{
    if (listName.empty()) return;
    QString query = "DELETE FROM targets WHERE hierarchyAddr LIKE '" + QString::fromStdString(listName) + "%';";
    QSqlQuery resQuery;
    if(!resQuery.exec(query)){
        throw std::invalid_argument("query error in QtSQLiteModel::clearList. Query: "+ query.toStdString() + "\n");
    }
}

list<map<std::string, std::string> > QtSQLiteModel::readList(std::string listName)
{
    string query = "SELECT * FROM targets WHERE hierarchyAddr not LIKE '"+ listName +"\\_%\\_%' ESCAPE '\\' and hierarchyAddr LIKE '"+ listName +"%';";
    return convertSQLSelectResultToContainer(query);
}

list<std::string> QtSQLiteModel::getAllTargetListsNames()
{
    list<string> res;
    //auto colNames = getColumnsNames("lists");
    string query = "SELECT * FROM lists;";
    auto lists = convertSQLSelectResultToContainer(query);

    if(lists.empty()) {
        qDebug() << "Retuned empty list\n";
        return list<string>();
    }

    for(auto list : lists){
        res.push_back(list.at("name"));
    }
    return res;
}

map<std::string, std::string> QtSQLiteModel::getTargetListSetting(std::string name)
{
    string query = "SELECT * FROM lists WHERE name='" + name + "';";
    auto queryRes = convertSQLSelectResultToContainer(query);
    auto it = std::find_if(queryRes.begin(), queryRes.end(), [name](map<string,string> targetListSettings) {
        return targetListSettings.at("name") == name;
    });
    if (it == queryRes.end()) throw std::invalid_argument("list with name '"+name+"' does not exist.");
    return *it;
}

list<map<std::string, std::string> > QtSQLiteModel::readNotComplitedTargets(std::string listName)
{
    string query = "SELECT * FROM targets WHERE hierarchyAddr not LIKE '" + listName + "\\_%\\_%' ESCAPE '\\' and hierarchyAddr LIKE '" + listName + "%' and status='in_process';";
    return convertSQLSelectResultToContainer(query);
}

list<map<std::string, std::string> > QtSQLiteModel::readComplitedTargets(std::string listName)
{
    string query = "SELECT * FROM targets WHERE hierarchyAddr not LIKE '" + listName + "\\_%\\_%' ESCAPE '\\' and hierarchyAddr LIKE '" + listName + "%' and status='complite';";
    return convertSQLSelectResultToContainer(query);
}

list<map<std::string, std::string> > QtSQLiteModel::getIterableTasks()
{
    string query = "SELECT * FROM iterators;";
    return convertSQLSelectResultToContainer(query);
}

void QtSQLiteModel::saveIterableTask(map<std::string, std::string> taskMap)
{
    string query = "insert or replace into iterators ";
    string keys = "(id, ";
    string values = "((select id from iterators where name = '"+ taskMap.at("name") +"'), ";
    for (auto &[key, value] : taskMap) {
        keys += key + ", ";
        values += "'" + value + "'" + ", ";
    }
    keys.assign(keys.begin(), keys.end() - 2);//delete last ", "
    values.assign(values.begin(), values.end() - 2);//delete last ", "
    keys += ")";
    values += ")";

    query += keys + " VALUES " + values + ";";
    QSqlQuery resQuery;
    if(!resQuery.exec(QString::fromStdString(query))){
        throw std::invalid_argument("query error in QtSQLiteModel::saveIterableTask. Query: "+ query + "\n");
    }
}

void QtSQLiteModel::delIterableTask(std::string name)
{
    string query = "DELETE from iterators WHERE name = '" + name + "';";
    QSqlQuery resQuery;
    if(!resQuery.exec(QString::fromStdString(query))){
        throw std::invalid_argument("query error in QtSQLiteModel::delIterableTask. Query: "+ query + "\n");
    }
}

void QtSQLiteModel::saveScheduleCompliteHist(map<std::string, std::string> taskMap)
{
    string query = "insert into scheduleHist ";
    string keys = "(";
    string values = "(";
    for (auto &[key, value] : taskMap) {
        if(key == "id") continue;
        keys += key + ", ";
        values += "'" + value + "'" + ", ";
    }
    keys.assign(keys.begin(), keys.end() - 2);//delete last ", "
    values.assign(values.begin(), values.end() - 2);//delete last ", "
    keys += ")";
    values += ")";

    query += keys + " VALUES " + values + ";";
    QSqlQuery resQuery;
    if(!resQuery.exec(QString::fromStdString(query))){
        throw std::invalid_argument("query error in QtSQLiteModel::saveScheduleCompliteHist. Query: "+ query + "\n");
    }
}

void QtSQLiteModel::delScheduleCompliteHist(string hierarchyAddr)
{
    string query = "DELETE FROM scheduleHist WHERE id = (SELECT MAX(id) FROM scheduleHist WHERE hierarchyAddr = '"+ hierarchyAddr +"');";
    QSqlQuery resQuery;
    if(!resQuery.exec(QString::fromStdString(query))){
        throw std::invalid_argument("query error in QtSQLiteModel::delScheduleCompliteHist. Query: "+ query + "\n");
    }
}

list<map<std::string, std::string>> QtSQLiteModel::getScheduleCompliteHistElem(std::string hierarchyAddr)
{
    string query = "SELECT * FROM scheduleHist WHERE id = (SELECT MAX(id) FROM scheduleHist WHERE hierarchyAddr = '"+ hierarchyAddr +"');";
    return convertSQLSelectResultToContainer(query);
}


list<map<std::string, std::string>> QtSQLiteModel::convertSQLSelectResultToContainer(std::string &sqlSelectQuery)
{
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
            rowMap.insert(std::make_pair(colName,value));
        }
        resultList.push_back(rowMap);
    }

    return resultList;
}

vector<std::string> QtSQLiteModel::getColumnsNames(std::string tableName)
{
    QString queryString = "PRAGMA table_info('"+ QString::fromStdString(tableName) +"');";
    vector<string> res;
    QSqlQuery query;
    if(!query.exec(queryString))
        throw std::invalid_argument("SQL error in QtSQLiteModel::getColumnsNames : " + queryString.toStdString());

    while (query.next())
    {
        std::stringstream ss;
        ss << query.value(1).toString().toStdString();
        res.push_back(ss.str());
    }

    return res;
}

void QtSQLiteModel::initTables(list<QString> tablesList)
{
    for(auto table : tablesList){
        QSqlQuery query;
        if(!query.exec("CREATE TABLE IF NOT EXISTS " + table))
            throw std::invalid_argument("SQL error in QtSQLiteModel::initTables : CREATE TABLE IF NOT EXISTS " + table.toStdString());
    }
}
