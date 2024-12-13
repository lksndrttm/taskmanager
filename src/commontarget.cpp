#include "targets/commontarget.h"

CommonTarget::CommonTarget(const map<string, string> & trg, ITarget* parent) : SimpleTarget (trg, parent)
{
    initWithMap(trg);
    targetType = "common";
}

map<std::string, std::string> CommonTarget::targetToMap() const
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


    resMap.insert(std::pair<string, string>("targetType", "common"));


    return resMap;
}

void CommonTarget::initWithMap(const std::map<std::string, std::string> &trg)
{
    if (!(trg.count("startTime") && trg.count("endTime") && trg.count("prior")))
        throw std::invalid_argument("Target init error. [CommonTarget::initWithMap]");

    startDatetime = trg.at("startTime");
    endDatetime = trg.at("endTime");
}


