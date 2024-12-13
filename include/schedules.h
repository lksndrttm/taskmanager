#ifndef Schedule_H
#define Schedule_H

#include "TargetList.h"
#include "targets/scheduletarget.h"
#include <string>
#include <map>
#include <list>
#include <algorithm>


using std::string;
using std::map;
using std::list;

class Schedule : public TargetList
{

public:
    Schedule(map<string, string> & listSetting);
    virtual list<ITargetPtr> getTargetList() override;//return only today targets
    list<ITargetPtr> getAllTargets();
    list<ITargetPtr> getTargetsByDate(string date/*dd.mm.yyyy*/);
};

#endif // Schedule_H
