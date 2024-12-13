#ifndef QTSQLITEMODEL_H
#define QTSQLITEMODEL_H

#include "modelInteface.h"
#include <QtSql>
#include <QString>

class QtSQLiteModel : public IModel
{
public:
    QtSQLiteModel();
    virtual ~QtSQLiteModel();
    virtual list<map<string, string>> readSubTargets(string hierarchyAddr) override;
    virtual void deleteSubTargets(string hierarchyAddr) override;

    virtual void addTarget(map<string, string> trg) override;
    virtual void modifyTarget(map<string,string> modifiedTrg) override;
    virtual void deleteTarget(string hierarchyAddr) override;

    virtual void updateStatus(string status, string hierarchyAddr) override;

    virtual map<string, string> getTargetById(int id) override;
    virtual map<string, string> getTargetByAddr(string hierarchyAddr) override;
    virtual map<string, string> getLastTarget() override;
    virtual map<std::string, std::string> getTargetbyName(string name) override;

    virtual void createList(map<string, string> targetList) override;
    virtual void deleteTargetList(string listName) override;
    virtual void clearList(string listName) override;
    virtual list<map<string, string>> readList(string listName) override;

    virtual list<string> getAllTargetListsNames() override;
    //virtual list<map<string, string>> readTargetList(string listName) override;///???? dublicate readList

    //virtual void saveTargetListSetting(map<string, string> listSetting) override;
    virtual map<string, string> getTargetListSetting(string name) override;

    virtual list<map<string, string>> readNotComplitedTargets(string listName) override;
    virtual list<map<string, string>> readComplitedTargets(string listName) override;

    //ITERABLETASKS
    virtual list<map<string,string>> getIterableTasks() override;
    virtual void saveIterableTask(map<string,string> taskMap) override;
    //virtual void modifyIterableTask(map<string,string> modifiedTask) override;
    virtual void delIterableTask(string name) override;

    void saveScheduleCompliteHist(map<string,string> taskMap) override;
    void delScheduleCompliteHist(string hierarchyAddr) override;
    list<map<std::string, std::string>> getScheduleCompliteHistElem(string hierarchyAddr) override;

    friend IModelPtr IModel::create(string name);

private:
    const QString mostFullTargetsTableDescription = "targets(id INTEGER PRIMARY KEY, hierarchyAddr CHAR(1024),"
                                                    " name CHAR(256),description CHAR(1024), status CHAR(60), createTime CHAR(20),"
                                                    " targetType CHAR(60), startTime CHAR(20), endTime CHAR(20), prior INTEGER, nextCompliteDateTime CHAR(20),"
                                                    " beginDateTime CHAR(20), repeatTime INTEGER ,UNIQUE(name,hierarchyAddr));";

    const QString mostFullListsTableDescription = "lists(id INTEGER PRIMARY KEY, name CHAR(256), type CHAR(60), UNIQUE(name));";

    const QString mostFullIterableTasksTableDescription = "iterators(id INTEGER PRIMARY KEY, name CHAR(256), lastDayStatus CHAR(7), createTime CHAR(20),"
                                                          "continualFistDate CHAR(20), continualLastDate CHAR(20), currentDaysRange INTEGER ,maxContinualFistDate CHAR(20),"
                                                          "maxContinualLastDate CHAR(20), maxDaysRange INTEGER ,UNIQUE(name));";

    const QString scheduleCompliteHistTable = "scheduleHist(id INTEGER PRIMARY KEY, hierarchyAddr CHAR(1024),"
                                              " name CHAR(256),description CHAR(1024), status CHAR(60), createTime CHAR(20),"
                                              " targetType CHAR(60), startTime CHAR(20), endTime CHAR(20), prior INTEGER, nextCompliteDateTime CHAR(20),"
                                              " beginDateTime CHAR(20), repeatTime INTEGER, compliteTime CHAR(20));";
    list<map<string, string>> convertSQLSelectResultToContainer(string &sqlSelectQuery);
    vector<string> getColumnsNames(string tableName);
    void initTables(list<QString> tablesList);
    QString dbName = "insei.db";
    QSqlDatabase db;
};

#endif // QTSQLITEMODEL_H
