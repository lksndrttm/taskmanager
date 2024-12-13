#ifndef RESOURCESMANAGER_H
#define RESOURCESMANAGER_H

//#include "idatabase.h"
#include "executor.h"
#include <string>
#include <memory>

//using DBPointer = std::shared_ptr<db::IDatabase>;

class ResourcesManager
{
public:
    ResourcesManager();
    ~ResourcesManager();
    std::string reservate(const std::string& filepath);
    bool unreservate(const std::string& filepath);
    bool isReserved(const std::string& filepath);

private:
    class Private;
    std::unique_ptr<Private> p;

    std::string saveResourceToDB(const std::string& filepath);//return new filename unique for db
    bool deleteResourceFromDB(const std::string& filepath);

    bool copyResourcesToMedia();
    bool copyResourceToMedia(const std::string & filepath);
    bool deleteResourceFromMedia(const std::string & filepath);
    void deleteTempDir();
};

#endif // RESOURCESMANAGER_H
