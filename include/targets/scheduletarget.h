#ifndef SCHEDULETARGET_H
#define SCHEDULETARGET_H

#include "commontarget.h"
#include <string>
#include <map>
#include <list>
#include <chrono>

namespace  {

using std::string;
using std::map;
using std::list;

using time_point_sys = std::chrono::time_point<std::chrono::system_clock>;
using time_duration = std::chrono::duration<std::chrono::seconds::rep>;
using days_int = std::chrono::duration<std::chrono::hours::rep, std::ratio_multiply<std::chrono::hours::period ,std::ratio<24>>>;
using seconds = std::chrono::duration<double>;
using days_d = std::chrono::duration<double, std::ratio_multiply<std::ratio<86400>,seconds::period>>;

}

class ScheduleTarget : public CommonTarget
{
    time_point_sys nextCompliteDateTime;
    time_point_sys beginDateTime;//%d.%m.%Y
    time_duration repeatTime;
public:
    ScheduleTarget(const map<string, string> & trg, ITarget* parent = nullptr);
    bool isToday();
    bool isNowComplite();
    void checkState();
    bool isSomeday(std::string dateStr) const;
    void setNowComplite();
    void setNowUncomplite();
    time_point_sys getNextTime() const {return nextCompliteDateTime;}
    time_point_sys getBeginTime() const {return beginDateTime;}
    time_duration getRepeateTime() const {return repeatTime;}
    void setStatus(string stat) override;
    void setStatus(bool stat) override;

    virtual map<string, string> targetToMap() const override;

protected:
    void saveCompleteHist();
    void changeStatus(bool stat);
    void initWithMap(const std::map<std::string, std::string> &trg) override;
};

time_point_sys dateFromString(std::string timeStr, std::string format = "%d.%m.%Y");
string dateToString(time_point_sys date, std::string format = "%d.%m.%Y");

#endif // SCHEDULETARGET_H
