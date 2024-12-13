#pragma once
#include <chrono>
#include <string>
#include <iostream>

namespace itime {
    using timePoint = std::chrono::time_point<std::chrono::system_clock>;
    using rangePair = std::pair<timePoint,timePoint>;
    using days = std::chrono::duration<std::chrono::hours::rep, std::ratio_multiply<std::chrono::hours::period ,std::ratio<24>>>;
    using hours = std::chrono::hours;
    using minutes = std::chrono::minutes;
}

class TasksTimer
{
public:
    static itime::timePoint nextTime(std::string dateTimeString, size_t msStep, std::string format = "%H:%M %d.%m.%Y");
    static itime::timePoint prevTime(std::string dateTimeString, size_t msStep, size_t identNumber = 1, std::string format ="%H:%M %d.%m.%Y");
    static itime::timePoint stringToDatetime(std::string datetime, std::string format = "%H:%M %d.%m.%Y");
    static std::string datetimeToString(itime::timePoint datetime, std::string format = "%H:%M %d.%m.%Y");
    static itime::timePoint currentTime();
    static bool isLater(itime::timePoint t1, itime::timePoint t2);
    static itime::timePoint toMidnight(itime::timePoint datetime);
    static itime::timePoint floorHours(itime::timePoint datetime);//12:50 ... -> 12:00 ...
    template<class T1, class T2>
    static bool isDurationBigger(T1 t1,T2 t2){
        std::cout << std::chrono::duration_cast<std::chrono::seconds>(t1).count() << " - " << std::chrono::duration_cast<std::chrono::seconds>(t2).count() << "\n";
        return std::chrono::duration_cast<std::chrono::seconds>(t1).count() > std::chrono::duration_cast<std::chrono::seconds>(t2).count();
    }

};
