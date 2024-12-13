#ifndef EXECUTOR_H
#define EXECUTOR_H
#include <list>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <QString>
#include <QVariant>

namespace db {

using SelectResult = std::list<std::map<std::string, std::string>>;
using TargetRecordType = std::map<std::string, std::string>;
using NoteRecordType = std::map<std::string, std::string>;
using CatRecordType = std::map<std::string, std::string>;
using ListInfoType = std::pair<std::string, std::string>;

using Record = std::map<std::string, std::string>;
using Records = std::list<Record>;
using Field = std::pair<std::string, std::string>;

using Index = unsigned long;

class Executor
{
public:
    Executor();
    bool execute(const std::string & query);
    bool execute(const QString & query, const std::map<QString,QVariant> & placeholders);

    bool insert(const Record & newRecord, const std::string & table);
    bool insert(const Record & newRecord, const std::string & table, const std::vector<std::string> & blobFieldNames);
    bool insertOrReplace(const Record & newRecord, const std::string & table);
    bool update(Index id, const Record & changedFields, const std::string & table);
    bool updateSet(const std::string & conditioin, const Record & changedFields, const std::string & table);
    bool deleteRecord(Index id, const std::string & table);
    bool deleteRecordByField(const std::string & fieldName, const std::string & fieldVal, const std::string & table);
    Records select(const std::string & table, const std::string & condition = "TRUE",unsigned long count = 0, bool isReversed = false, unsigned long offset = 0, const std::string & fields = "*") const;
    Record selectOne(const std::string & table, const std::string & condition = "TRUE", bool isReversed = false, unsigned long offset = 0) const;

    unsigned long count(const std::string& table) const;

    Index lastIndex(const std::string & table);

private:
    //ConnectionManager& connectionManager;
    SelectResult convertSQLSelectResultToContainer(const std::string &sqlSelectQuery) const;
};

}
#endif // EXECUTOR_H
