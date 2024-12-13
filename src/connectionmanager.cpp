#include "connectionmanager.h"
#include "dbtypes.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardPaths>
#include <QDebug>
#include <QString>
#include <QDir>
#include <vector>

namespace db {

namespace  {
    class DBCloser {
    public:
        void operator()(QSqlDatabase* db){
            db->close();
            QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
        }
    };
}



class ConnectionManager::CManagerPrivate {
public:
    std::unique_ptr<QSqlDatabase,DBCloser> m_database;

    DBTypes::DBState m_state {DBTypes::DBState::OK};
    QString m_dbPath;

    bool m_isValid {true};

    bool setup();
    bool setupWorkspace();
    bool setupTables();

    void setIsValid(bool isValid);
};

ConnectionManager &ConnectionManager::instance()
{
    static ConnectionManager inst {};
    return  inst;
}

ConnectionManager::~ConnectionManager()
{

}

ConnectionManager::ConnectionManager()
    : m_private {new CManagerPrivate {}}
{
    const bool setupResult {m_private->setup()};
    m_private->setIsValid(setupResult);
}

bool ConnectionManager::CManagerPrivate::setup()
{
    QString driver {"QSQLITE"};
    if(!QSqlDatabase::isDriverAvailable(driver)){
        m_state = DBTypes::DBState::NO_DRIVER_ERROR;
        qWarning() << "Driver " << driver << " is not available.";
        return false;
    }

    if(!setupWorkspace()){
        m_state = DBTypes::DBState::WORKSPACE_ERROR;
        qCritical() << "Workspace setup failed!";
        return false;
    }

    auto* db = new QSqlDatabase {QSqlDatabase::addDatabase(driver)};
    m_database.reset(db);
    m_database->setDatabaseName(m_dbPath);

    qDebug() << "Database name: " << m_database->databaseName();

    if (!m_database->open())
    {
        m_state = DBTypes::DBState::OPENNING_ERROR;
        qCritical() << "Error in opening DB " << m_database->databaseName()
                   << " reason: " <<  m_database->lastError().text();
        return false;
    }

    return setupTables();
}

bool ConnectionManager::CManagerPrivate::setupWorkspace()
{
    const QString dbname {"insei.db"};
    const QString location {QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)};
    const QString fullPath {location + "/" + dbname};

    m_dbPath = fullPath;
    QDir dbDirectory {location};
    if(!dbDirectory.exists()){
        const bool creationResult {dbDirectory.mkpath(location)};
                qWarning() << "DB directory not exist, creating result: "
                           << creationResult;
    }
    qDebug() << "Data path: " << fullPath;
    return dbDirectory.exists();

}

bool ConnectionManager::CManagerPrivate::setupTables()
{
    bool result {true};

    QSqlQuery targetsTableDescription { "CREATE TABLE IF NOT EXISTS targets(id INTEGER PRIMARY KEY AUTOINCREMENT, hierarchyAddr CHAR(1024),"
                                                    " name CHAR(256),description CHAR(1024), status CHAR(60), createTime CHAR(20),"
                                                    " targetType CHAR(60), startTime CHAR(20), endTime CHAR(20), prior INTEGER,"
                                            " nextCompliteDateTime CHAR(20),"
                                                    " beginDateTime CHAR(20), repeatTime INTEGER ,UNIQUE(name,hierarchyAddr));"};

    QSqlQuery listsTableDescription {"CREATE TABLE IF NOT EXISTS lists(id INTEGER PRIMARY KEY, name CHAR(256), type CHAR(60), UNIQUE(name));"};

//    QSqlQuery iterableTasksTableDescription {"CREATE TABLE IF NOT EXISTS iterators(id INTEGER PRIMARY KEY, name CHAR(256), lastDayStatus CHAR(7),"
//                                                  " createTime CHAR(20),"
//                                                          "continualFistDate CHAR(20), continualLastDate CHAR(20), currentDaysRange INTEGER ,"
//                                                  "maxContinualFistDate CHAR(20),"
//                                                          "maxContinualLastDate CHAR(20), maxDaysRange INTEGER ,UNIQUE(name));"};
    QSqlQuery iterableTasksTableDescription {"CREATE TABLE IF NOT EXISTS iterators_1(id INTEGER PRIMARY KEY, name CHAR(256), type CHAR(60),data TEXT, UNIQUE(name));"};

    QSqlQuery scheduleCompliteHistTable {"CREATE TABLE IF NOT EXISTS scheduleHist(id INTEGER PRIMARY KEY, hierarchyAddr CHAR(1024),"
                                              " name CHAR(256),description CHAR(1024), status CHAR(60), createTime CHAR(20),"
                                              " targetType CHAR(60), startTime CHAR(20), endTime CHAR(20), prior INTEGER,"
                                              " nextCompliteDateTime CHAR(20),"
                                              " beginDateTime CHAR(20), repeatTime INTEGER, compliteTime CHAR(20));"};

    QSqlQuery notesTableDescription {"CREATE TABLE IF NOT EXISTS notes(id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                     " title CHAR(256), createDateTime CHAR(20),"
                                     " lastEditDateTime CHAR(20), content TEXT, isFavorite BOOL,"
                                     " category CHAR(256), tags CHAR(1024));"};

    QSqlQuery notesCategoryTableDescription {"CREATE TABLE IF NOT EXISTS notes_cat(id INTEGER PRIMARY KEY,"
                                     " name CHAR(256) UNIQUE, color CHAR(8), image CHAR(256) DEFAULT '');"};

    std::vector<QSqlQuery> tablesList;
    tablesList.push_back(targetsTableDescription);
    tablesList.push_back(listsTableDescription);
    tablesList.push_back(iterableTasksTableDescription);
    tablesList.push_back(scheduleCompliteHistTable);
    tablesList.push_back(notesTableDescription);
    tablesList.push_back(notesCategoryTableDescription);

    for(auto& query : tablesList){
        if (!query.exec())
            {
                result = false;
                m_state = DBTypes::DBState::TABLES_ERROR;
                qWarning() << "Table creation failed. Reason: "
                           << query.lastError();
            }
    }

    return result;
}

void ConnectionManager::CManagerPrivate::setIsValid(bool isValid)
{
    this->m_isValid = isValid;
}


bool ConnectionManager::isValid() const
{
    return m_private->m_isValid && m_private->m_database->isValid();
}

}
