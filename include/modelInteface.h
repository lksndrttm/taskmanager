#pragma once

#include<string>
#include<sstream>
#include<list>
#include<vector>
#include<map>
#include<memory>
#include<iterator>
#include<algorithm>
#include <iostream>
//#include "sqlite3.h"

using std::string;
using std::list;
using std::map;
using std::vector;


class IModel;
using IModelPtr = std::shared_ptr<IModel>;
//using IModelPtr = IModel * ;
class IModel
{
protected:
	~IModel() = default;
	IModel() = default;

public:
	virtual list<map<string,string>> readSubTargets(string hierarchyAddr) = 0;
	virtual void deleteSubTargets(string hierarchyAddr) = 0;

	virtual void addTarget(map<string,string> trg) = 0;
    virtual map<std::string, std::string> getTargetbyName(string name) = 0;
    virtual void modifyTarget(map<string,string> modifiedTrg) = 0;
	virtual void deleteTarget(string hierarchyAddr) = 0;

    virtual void updateStatus(string status, string hierarchyAddr) = 0;

	virtual map<string, string> getTargetById(int id) = 0;
    virtual map<string, string> getTargetByAddr(string hierarchyAddr) = 0;
	virtual map<string, string> getLastTarget() = 0;

	virtual void createList(map<string, string> targetList) = 0;
	virtual void deleteTargetList(string listName) = 0;
	virtual void clearList(string listName) = 0;
	virtual list<map<string, string>> readList(string listName) = 0;

	virtual list<string> getAllTargetListsNames() = 0;

	static IModelPtr create(string name);

	//virtual list<map<string, string>> readTargetList(string listName) = 0;
	//virtual void saveTargetListSetting(map<string, string> listSetting) = 0;
	virtual map<string, string> getTargetListSetting(string name) = 0;
	virtual list<map<string, string>> readNotComplitedTargets(string listName) = 0;
	virtual list<map<string, string>> readComplitedTargets(string listName) = 0;

    //ITERABLETASKS
    virtual list<map<string,string>> getIterableTasks() = 0;
    virtual void saveIterableTask(map<string,string> taskMap) = 0;
    //virtual void modifyIterableTask(map<string,string> modifiedTask) = 0;
    virtual void delIterableTask(string name) = 0;

    //schedules
    virtual list<map<std::string, std::string>> getScheduleCompliteHistElem(string hierarchyAddr) = 0;
    virtual void saveScheduleCompliteHist(map<string,string> taskMap) = 0;
    virtual void delScheduleCompliteHist(string hierarchyAddr) = 0;
};

