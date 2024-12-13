#ifndef LISTCONTROLLER_H
#define LISTCONTROLLER_H

#include "TargetList.h"
#include "schedules.h"

class ListController
{
public:
    ListController() = default;
    ~ListController() = default;
    list<string> getAllListsNames();
    ITargetListPtr getTargetListByName(string name);
    ITargetListPtr createList(map<string,string> listSetting);
    void deleteList(string name);
    void clearList(string name);
private:
    static IModelPtr model;
};

using ListControllerPtr = shared_ptr<ListController>;
#endif // LISTCONTROLLER_H
