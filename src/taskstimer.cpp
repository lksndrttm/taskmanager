#include "taskstimer.h"
#include <iomanip>
#include <sstream>

itime::timePoint TasksTimer::nextTime(std::string dateTimeString, size_t msStep, std::string format)
{
    return stringToDatetime(dateTimeString, format) + std::chrono::duration<std::chrono::milliseconds::rep>(msStep);
}

itime::timePoint TasksTimer::prevTime(std::string dateTimeString, size_t msStep, size_t identNumber, std::string format)
{
    return stringToDatetime(dateTimeString, format) - (std::chrono::duration<std::chrono::milliseconds::rep>(msStep)*identNumber);
}

itime::timePoint TasksTimer::stringToDatetime(std::string datetime, std::string format)
{
    std::tm tm = {};
    std::stringstream ss(datetime);
    ss >> std::get_time(&tm, format.c_str());
    return  std::chrono::system_clock::from_time_t(std::mktime(&tm));
}

std::string TasksTimer::datetimeToString(itime::timePoint datetime, std::string format)
{
    std::stringstream ss;
    const auto tt { std::chrono::system_clock::to_time_t(datetime)};
    const auto ltime {std::localtime(&tt)};
    ss <<  std::put_time(ltime,format.c_str());
    return ss.str();
}

itime::timePoint TasksTimer::currentTime()
{
    return  std::chrono::system_clock::now();
}

bool TasksTimer::isLater(itime::timePoint t1, itime::timePoint t2)
{
    if((t1 - t2).count() > 0)
        return true;
    else
        return false;
}

itime::timePoint TasksTimer::toMidnight(itime::timePoint datetime)
{
    return stringToDatetime(datetimeToString(datetime,"%d.%m.%Y"),"%d.%m.%Y");
}

itime::timePoint TasksTimer::floorHours(itime::timePoint datetime)
{
    return stringToDatetime(datetimeToString(datetime,"%H %d.%m.%Y"),"%H %d.%m.%Y");
}
