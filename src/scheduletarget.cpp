#include "targets/scheduletarget.h"
#include <QDebug>
ScheduleTarget::ScheduleTarget(const map<string, string> & trg, ITarget* parent) : CommonTarget (trg, parent)
{
    initWithMap(trg);
    targetType = "schedule";
    checkState();

}

bool ScheduleTarget::isToday()
{
    double durationDays = std::chrono::duration_cast<days_d>(nextCompliteDateTime - std::chrono::system_clock::now()).count();
    if(durationDays < -1){
        while (std::chrono::duration_cast<days_d>(nextCompliteDateTime - std::chrono::system_clock::now()).count() < -1) {
            nextCompliteDateTime += repeatTime;
        }
        rewriteTarget();
    }

    if(durationDays < 0 && durationDays > -1)
        return true;
    return false;
}

bool ScheduleTarget::isSomeday(std::string dateStr) const
{
    time_point_sys date = dateFromString(dateStr,"%d.%m.%Y");
    double diff = std::chrono::duration_cast<days_d>(date - nextCompliteDateTime).count();
    if(diff < 1 && diff > -1)
        return true;
    long long diffInt = std::chrono::duration_cast<days_int>(date - nextCompliteDateTime).count();
    long long repeatCount = std::chrono::duration_cast<days_int>(repeatTime).count();
    if(repeatCount < 1)
        return true;
    if(diffInt % repeatCount == 0)
        return true;
    return false;

}

void ScheduleTarget::setNowComplite()
{
    //saveCompleteHist();
    if(isNowComplite())
        return;
    if(isToday()){
        nextCompliteDateTime += repeatTime;
        changeStatus(true);
    }
}

void ScheduleTarget::setNowUncomplite()
{
    changeStatus(false);
    if(isNowComplite()){
        nextCompliteDateTime -= repeatTime;
    }
    //executor->delScheduleCompliteHist(hierarchyAddr);
}

void ScheduleTarget::setStatus(std::string stat)
{
    if(stat == "complite")
        setNowComplite();
    else {
        setNowUncomplite();
    }
}

void ScheduleTarget::setStatus(bool stat)
{
    if(stat)
        setNowComplite();
    else {
        setNowUncomplite();
    }
}


map<std::string, std::string> ScheduleTarget::targetToMap() const
{
    map<string, string> resMap;
    std::stringstream ss;
    ss << getId();
    resMap.insert(std::pair<string, string>("id", ss.str()));
    resMap.insert(std::pair<string, string>("name", name));
    resMap.insert(std::pair<string, string>("description", description));
    resMap.insert(std::pair<string, string>("hierarchyAddr", hierarchyAddr));
    resMap.insert(std::pair<string, string>("status", status));
    resMap.insert(std::pair<string, string>("startTime", startDatetime));
    resMap.insert(std::pair<string, string>("endTime", endDatetime));
    resMap.insert(std::pair<string, string>("createTime", createTime));
    ss.str("");
    ss << prior;
    resMap.insert(std::pair<string, string>("prior", ss.str()));

    resMap["nextCompliteDateTime"] = dateToString(nextCompliteDateTime);
    resMap["beginDateTime"] = dateToString(beginDateTime,"%d.%m.%Y");
    ss.str("");
    ss << repeatTime.count();
    resMap["repeatTime"] = ss.str();

    resMap.insert(std::pair<string, string>("targetType", "schedule"));


    return resMap;
}

void ScheduleTarget::saveCompleteHist()
{
    map<string,string> shHist{targetToMap()};
    shHist["compliteTime"] = dateToString(nextCompliteDateTime);
    //shHist["hierarchyAddr"] = hierarchyAddr;
    //executor->saveScheduleCompliteHist(shHist);
    throw;//TODO NOT IMPLEMENTED

}

void ScheduleTarget::changeStatus(bool stat)
{
    std::string strStat = stat ? "complite" : "in_process";
    throw;//TODO
    //executor->updateTargetStatus(strStat, hierarchyAddr);
}

bool ScheduleTarget::isNowComplite()
{
//    auto lastCompliteList {executor->getScheduleCompliteHistElem(hierarchyAddr)};
//    string lastComplite;

//    if(lastCompliteList.empty()){
//        return false;
//    }
//    else {
//        lastComplite = lastCompliteList.front().at("compliteTime");
//    }

//    time_point_sys lastCompliteTime = dateFromString(lastComplite);
//    time_point_sys now = std::chrono::system_clock::now();

//    double diff = std::chrono::duration_cast<seconds>((now - lastCompliteTime)).count();
//    return diff < std::chrono::duration_cast<seconds>(repeatTime).count();
    throw;//NOT IMPLEMENTED
    return {};
}

void ScheduleTarget::checkState()
{
    if(!isNowComplite()){
        changeStatus(false);
        //auto subTargets = getSubTargets();
        //for(auto sub : subTargets){
        //    sub->setStatus("in_process");
        //}
    }
}

void ScheduleTarget::initWithMap(const std::map<std::string, std::string> &trg)
{
    if (!(trg.count("startTime") && trg.count("endTime") && trg.count("prior")))
        throw std::invalid_argument("Target init error. [ScheduleTarget::initWithMap]");

    repeatTime = std::chrono::seconds(86400);//atoi(trg.at("repeatTime").c_str()));
    beginDateTime = dateFromString(trg.at("beginDateTime"),"%d.%m.%Y");
    nextCompliteDateTime = beginDateTime + std::chrono::seconds(86400);//dateFromString(trg.at("nextCompliteDateTime"),"%d.%m.%Y");

}

time_point_sys dateFromString(std::string timeStr, std::string format)
{
    std::tm tm = {};
    std::stringstream ss(timeStr);
    ss >> std::get_time(&tm, format.c_str());
    return  std::chrono::system_clock::from_time_t(std::mktime(&tm));
}

std::string dateToString(time_point_sys date, std::string format)
{
    std::stringstream ss;
    const auto tt { std::chrono::system_clock::to_time_t(date)};
    const auto ltime {std::localtime(&tt)};
    ss <<  std::put_time(ltime,format.c_str());
    return ss.str();

}
