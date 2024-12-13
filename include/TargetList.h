#pragma once
#include "Target.h"
#include "targets/commontarget.h"

using std::list;

class BaseTargetList;
using ITargetListPtr = shared_ptr<BaseTargetList>;

class BaseTargetList
{
protected:
    std::unique_ptr<db::Executor> executor;
    std::vector<ITarget*> targetList;
	bool isNameUnique(string name);
    std::string name, type;
    bool isListInitialized;
    ITarget* rootItem;
    std::vector<db::ListInfoType> getLists();
public:
    explicit BaseTargetList(std::string listName = "null_hollow_list");
    virtual ~BaseTargetList();
    const std::vector<ITarget*>& getTargetList(bool isGetNew = false);
    void initList(std::string listName);

    bool addTarget(map<string,string> & trg);
    bool delTargetByRow(size_t row);
    void delTarget(int id);
    void delTargetByName(string name);
    void delTargetByAddr(string hierarchyAddr);

    const list<ITarget*> getTargetsCreatedAfterDate(string datetime/*hh:mm dd.MM.YYYY*/);
    const list<ITarget*> getTargetsCreatedBeforDate(string datetime/*hh:mm dd.MM.YYYY*/);

    ITarget* getTargetById(int id);
    ITarget* getTargetByName(string name);
    ITarget* getTargetByAddr(string hierarchyAddr);
    list<ITarget*> getTargetsByDate(string date/*dd.mm.yyyy*/);
    ITarget* getRootItem() {return rootItem;}

    map<string, string> getlistSetting();

private:
    void fillRootSubTargets();
    void clearRoot();
};


