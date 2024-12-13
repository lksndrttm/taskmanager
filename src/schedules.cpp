#include "schedules.h"
#include <algorithm>
#include <QDebug>
Schedule::Schedule(map<string, string> & listSetting) : TargetList (listSetting)
{

}

list<ITargetPtr> Schedule::getTargetList()
{
    list<ITargetPtr> allTargetsList = TargetList::getTargetList();
    list<ITargetPtr> todayList;
    std::copy_if(allTargetsList.begin(),allTargetsList.end(),std::back_inserter(todayList), [](ITargetPtr item){
        return std::dynamic_pointer_cast<ScheduleTarget>(item)->isToday();
    });
    return  todayList;
}

list<ITargetPtr> Schedule::getAllTargets()
{
    return TargetList::getTargetList();
}

list<ITargetPtr> Schedule::getTargetsByDate(std::string date)
{
    list<ITargetPtr> allTargetsList = getAllTargets();
    list<ITargetPtr> dateList;
    //auto dateT =  dateFromString(date, "%d.%m.%Y");//,"%d.%m.%Y");
    std::copy_if(allTargetsList.begin(),allTargetsList.end(),std::back_inserter(dateList), [date](ITargetPtr item){
        return std::dynamic_pointer_cast<ScheduleTarget>(item)->isSomeday(date);
    });
    return  dateList;
}
