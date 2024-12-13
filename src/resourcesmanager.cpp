#include "../include/resourcesmanager.h"
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QByteArray>
#include <sstream>
#include <filesystem>
#include <QStandardPaths>

#include <QDebug>

class ResourcesManager::Private{
public:
    std::unique_ptr<db::Executor> database;
    const std::string RESOURCES_TABLE_NAME = "Resources";
    const std::string RESOURCE_NAME_FIELD = "filename";
    const std::string RESOURCE_DATA_FIELD = "file";
    std::string m_resourcesDir;

    const std::string RESOURCES_FILES_TABLE_SQL {"CREATE TABLE IF NOT EXISTS "
                                                 + RESOURCES_TABLE_NAME
                + "(id INTEGER PRIMARY KEY, filename CHAR(255) UNIQUE, file BLOB);"};

    void createResourcesTable();
    std::string createUniqueFilename(const std::string& originFilepath);
};


void ResourcesManager::Private::createResourcesTable()
{
    std::string createQuery {"CREATE TABLE IF NOT EXISTS "
                             + RESOURCES_TABLE_NAME
                             + "(id INTEGER PRIMARY KEY," + RESOURCE_NAME_FIELD
                             + " CHAR(255) UNIQUE, "+ RESOURCE_DATA_FIELD + " BLOB);"};

    bool isSuccess = database->execute(createQuery);

    assert(isSuccess && "Table create query not syccess");
}

std::string ResourcesManager::Private::createUniqueFilename(const std::string& originFilepath)
{
    QFileInfo finfo(originFilepath.c_str());
    std::string extension {finfo.completeSuffix().toStdString()};

    db::Index lastIndex = database->lastIndex(RESOURCES_TABLE_NAME);
    std::stringstream ss;
    ss << lastIndex + 1 << "." << extension;

    auto newPath {QDir(m_resourcesDir.c_str()).filePath(ss.str().c_str()).toStdString()};//append newFilename to temp dirPath

    //assert(!database->contains("filename", newPath, RESOURCES_TABLE_NAME) && "Filename must be unique");

    return newPath;
}




////////////////////////////////////

ResourcesManager::ResourcesManager() : p(std::make_unique<Private>())
{
    p->database.reset(new db::Executor{});
    p->createResourcesTable();

    auto appDir {QDir{QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)}};

    p->m_resourcesDir = appDir.filePath("media").toStdString();

    if(!QDir(p->m_resourcesDir.c_str()).exists()){
        appDir.mkdir("media");
    }
}

ResourcesManager::~ResourcesManager()
{

}

std::string ResourcesManager::reservate(const std::string &filepath)
{
    std::string newFilePath = filepath;
    if(!isReserved(filepath)){
        newFilePath = saveResourceToDB(filepath);
        copyResourceToMedia(newFilePath);
    }
    return newFilePath;
}

bool ResourcesManager::unreservate(const std::string &filepath)
{
    bool result = deleteResourceFromMedia(filepath);
    result = deleteResourceFromDB(filepath) || result;
    return result;
}

std::string ResourcesManager::saveResourceToDB(const std::string &filepath)
{
    QFile file(filepath.c_str());
    if (!file.open(QIODevice::ReadOnly)){
        return filepath;
    }
    QByteArray inByteArray = file.readAll();
    file.close();

    std::string newName {p->createUniqueFilename(filepath)};
    db::Record resRecord {{p->RESOURCE_NAME_FIELD,newName}, {"file",inByteArray.toStdString()}};

    bool result = p->database->insert(std::move(resRecord), p->RESOURCES_TABLE_NAME, {p->RESOURCE_DATA_FIELD});

    assert(result && "[DEBUG] Resource not saved");

    if(!result){
        return filepath;
    }
    return newName;
}

bool ResourcesManager::deleteResourceFromDB(const std::string &filepath)
{
    db::Record resourceToDel = p->database->selectOne(p->RESOURCES_TABLE_NAME, p->RESOURCE_NAME_FIELD + " = '" + filepath + "'");

    if(resourceToDel.empty()){
        qWarning() << "Attemp to delete unexist resource -> " << filepath.c_str();
        return false;
    }

    return p->database->deleteRecord(atoi(resourceToDel.at("id").c_str()),p->RESOURCES_TABLE_NAME);
}

bool ResourcesManager::isReserved(const std::string &filepath)
{
    db::Record resourceToDel = p->database->selectOne(p->RESOURCES_TABLE_NAME, p->RESOURCE_NAME_FIELD + " = '" + filepath + "'");
    return !resourceToDel.empty();
}

bool ResourcesManager::copyResourcesToMedia()
{
    size_t resourcesCount {p->database->count(p->RESOURCES_TABLE_NAME)};
    bool copySuccess = true;

    for(size_t num = 1; num <= resourcesCount; num++){
        db::Record record = p->database->selectOne(p->RESOURCES_TABLE_NAME,"TRUE",false, num);

        if(record.empty()){
            qWarning() << "Returned Empty record in copyResourcesToMedia";
            continue;
        }

        QString resourceName {record.at(p->RESOURCE_NAME_FIELD).c_str()};

        if (!QFile::exists(resourceName)){
            QFile file(resourceName);
            if (!file.open(QIODevice::WriteOnly)){
                copySuccess = false;
                continue;
            }
            file.write(QByteArray::fromStdString(record.at(p->RESOURCE_DATA_FIELD)));
            file.close();
        }
    }
    return copySuccess;
}

bool ResourcesManager::copyResourceToMedia(const std::string & filepath)
{
    db::Record record = p->database->selectOne(p->RESOURCES_TABLE_NAME,p->RESOURCE_NAME_FIELD + "='" + filepath+"'");

    if(record.empty()){
        qWarning() << "Resource not in db. [ResourcesManager::copyResourceToMedia]. " << filepath.c_str();
        return false;
    }

    QString resourceName {record.at(p->RESOURCE_NAME_FIELD).c_str()};

    if (!QFile::exists(resourceName)){
        QFile file(resourceName);
        if (file.open(QIODevice::WriteOnly)){
            file.write(QByteArray::fromStdString(record.at(p->RESOURCE_DATA_FIELD)));
            file.close();
            return true;
        } else {
            qWarning() << "Cannot open file. " << resourceName;
            return false;
        }
    }
    return true;
}

bool ResourcesManager::deleteResourceFromMedia(const std::string &filepath)
{
    if (isReserved(filepath))
        return QFile::remove(filepath.c_str());
    return false;
}

void ResourcesManager::deleteTempDir()
{
    assert(QDir("") == QDir(p->m_resourcesDir.c_str()) && "atempt to delete app dir. Temp path must be safe to delete dir");
//    QDir(p->m_resourcesDir.c_str()).removeRecursively();
}

