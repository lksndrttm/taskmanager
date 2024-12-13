#include "iterabletask.h"
#include <QDebug>
#include <math.h>
///////////////////////////////////////////////////////////////////////////////////////////
 //  IterableTask                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////

IterableTask::IterableTask(map<std::string, std::string> taskMap)
{
    executor.reset(new db::Executor{});
    auto unpackedMap {unpackDataField(taskMap)};
    name = unpackedMap.at("name");
    type = unpackedMap.at("type");
    createTime = TasksTimer::stringToDatetime(unpackedMap.at("createTime"));
    compliteCount = atoi(unpackedMap.at("compliteCount").c_str());
    levelIncType = unpackedMap.at("levelIncType");
    firstLevelScore = atoi(unpackedMap.at("firstLevelScore").c_str());
    level = atoi(unpackedMap.at("level").c_str());
}

map<std::string, std::string> IterableTask::taskMap() const
{
    map<std::string, std::string> unpackedMap;
    unpackedMap["name"] = name;
    unpackedMap["type"] = type;
    unpackedMap["createTime"] = TasksTimer::datetimeToString(createTime);
    std::stringstream ss; ss << compliteCount;//convert to string
    unpackedMap["compliteCount"] = ss.str();
    unpackedMap["levelIncType"] = levelIncType;
    ss.str(""); ss << firstLevelScore;
    unpackedMap["firstLevelScore"] = ss.str();
    ss.str(""); ss << level;
    unpackedMap["level"] = ss.str();
    return packDataField(unpackedMap);
}

void IterableTask::updateTask(db::TargetRecordType taskMap, size_t id)
{
    map<std::string, std::string> unpackedMap {unpackDataField(taskMap)};
    name = unpackedMap.at("name");
    type = unpackedMap.at("type");
    compliteCount = atoi(unpackedMap.at("compliteCount").c_str());
    firstLevelScore = atoi(unpackedMap.at("firstLevelScore").c_str());
    level = atoi(unpackedMap.at("level").c_str());

    auto updatedTask = this->taskMap();
    updatedTask["id"] = std::to_string(id);
    executor->insertOrReplace(std::move(updatedTask),"iterators_1");
}

std::string IterableTask::getDataFieldValue(std::string key)
{
    return unpackDataField(taskMap()).at(key);
}

map<std::string, std::string> IterableTask::unpackDataField(map<std::string, std::string> taskMap)
{
    auto resultMap {taskMap};
    QJsonDocument jdoc = QJsonDocument::fromJson((QByteArray(taskMap.at("data").c_str())));
    if(!jdoc.isObject())
        qWarning() << "JSON incorrect";
    QJsonObject jobj = jdoc.object();
    auto it = jobj.begin();
    while (it != jobj.end()) {
        resultMap[it.key().toStdString()] = it.value().toString().toStdString();
        it++;
    }
    return resultMap;
}

map<std::string, std::string> IterableTask::packDataField(map<std::string, std::string> taskMap)
{
    map<std::string, std::string> resMap;
    std::string jsonData{"{"};
    for(auto &&[key,val] : taskMap){
        if(key == "data") continue;
        if(key == "name" || key == "id" || key == "type"){
            resMap[key] = val;
        }
        else{
            jsonData.append("\""+key+"\""+":"+"\""+val+"\""+",");
        }
    }
    jsonData.assign(jsonData.c_str(),jsonData.size()-1);//delete last ','
    jsonData.append("}");
    resMap["data"] = jsonData;
    return resMap;
}


void IterableTask::saveTask()
{
    auto task = taskMap();
    auto taskRecord = db::Record{};
    auto taskRecords = executor->select("iterators_1", "name = '" + task.at("name") + "'", 1);

    if(!taskRecords.empty()){
        taskRecord = taskRecords.front();
        task["id"] = taskRecord.at("id");
    }

    executor->insertOrReplace(task,"iterators_1");
}

size_t IterableTask::levelScore(size_t lvl) const
{
    if(levelIncType == "const"){
        return firstLevelScore*lvl;
    }
    else {
        return -1;
        qWarning() << "undefined level increment type";
    }
}

size_t IterableTask::upLevelScore(size_t lvl) const
{
    return  levelScore(lvl+1) - levelScore(lvl);
}

void IterableTask::complite()
{
    compliteCount++;
    level = levelScore(level+1) <= compliteCount ? level+1 : level;
    saveTask();
}

void IterableTask::uncomplite()
{
    if(compliteCount == 0)
        return;
    compliteCount--;
    level = levelScore(level) > compliteCount ? level-1 : level;
    saveTask();
}


///////////////////////////////////////////////////////////////////////////////////////////
 //  IterableSeries                                                                     //
///////////////////////////////////////////////////////////////////////////////////////////
IterableSeries::IterableSeries(map<std::string, std::string> taskMap) : IterableTask(taskMap)
{
    auto unpackedMap {unpackDataField(taskMap)};
    if(unpackedMap.count("isMidnight") == 0){
        unpackedMap["isMidnight"] = "false";
    }
    m_isMidnight = true;//unpackedMap.at("isMidnight") == "true";
    series = atoi(unpackedMap.at("series").c_str());
    maxSeries = atoi(unpackedMap.at("maxSeries").c_str());
    seriesBeginDateTime = TasksTimer::stringToDatetime(unpackedMap.at("seriesBeginDateTime"));
    maxSeriesBeginDateTime = TasksTimer::stringToDatetime(unpackedMap.at("maxSeriesBeginDateTime"));
    seriesTimeStep = itime::hours(atoi(unpackedMap["seriesTimeStep"].c_str()));
    if(!isSeriesContinue()){
        series = 0;
        saveTask();
    }
    if(!series){
        seriesBeginDateTime = TasksTimer::toMidnight(TasksTimer::currentTime());
    }
}

map<std::string, std::string> IterableSeries::taskMap() const
{
    auto unpackedMap {unpackDataField(IterableTask::taskMap())};
    std::stringstream ss; ss << series;//convert to string
    unpackedMap["series"] = ss.str();
    ss.str(""); ss << maxSeries;
    unpackedMap["maxSeries"] = ss.str();
    unpackedMap["seriesBeginDateTime"] = TasksTimer::datetimeToString(seriesBeginDateTime);
    unpackedMap["maxSeriesBeginDateTime"] = TasksTimer::datetimeToString(maxSeriesBeginDateTime);
    ss.str(""); ss << seriesTimeStep.count();
    unpackedMap["seriesTimeStep"] = ss.str();
    unpackedMap["isMidnight"] = isMidnight() ? "true" : "false";
    return  packDataField(unpackedMap);
}

void IterableSeries::complite()
{
    if(isComplited())//not time to be complited
        return;

    if(!isSeriesContinue())
        series = 0;

    if(series == 0){
        if(seriesTimeStep >= itime::hours(24) && isMidnight())
            seriesBeginDateTime = TasksTimer::toMidnight(TasksTimer::currentTime());//17:24->00:00
        else if(isMidnight())
            seriesBeginDateTime = TasksTimer::floorHours(TasksTimer::currentTime());//17:24->17:00
        else
            seriesBeginDateTime = TasksTimer::currentTime();
    }

    qDebug() << QString::fromStdString(TasksTimer::datetimeToString(seriesBeginDateTime)) << " BEGIN md:" << isMidnight();
    compliteCount++;
    series++;
    maxSeriesUpdate();

    if(series >= upLevelScore(level))
    {
        int mult = 1;
        while (series > upLevelScore(level)*mult) {
            mult++;
        }
        level = series - upLevelScore(level)*(mult) == 0  ? level+1 : level;
    }
    else
        level = upLevelScore(level) <= series ? level+1 : level;
    saveTask();
}

void IterableSeries::uncomplite()
{
    if(!isComplited()){//yet uncomplited
        return;
    }
    if(compliteCount != 0)
        compliteCount--;

    if(!isSeriesContinue())
        series = 0;
    if(series == 0){
        return;
    }
    series--;
    maxSeriesUpdate();
    if(level > 0){
    if(series >= upLevelScore(level)){
        int mult = 1;
        while (series > upLevelScore(level)*mult) {
            mult++;
        }
        level = upLevelScore(level-1)-1 == series%((mult-1)*upLevelScore(level-1)) ? level-1 : level;
    }
    else
        level = upLevelScore(level-1)-1 == series ? level-1 : level;
    }
    saveTask();
}

bool IterableSeries::isComplited() const
{
    return  (seriesBeginDateTime + (seriesTimeStep*series)) > TasksTimer::currentTime();
}

void IterableSeries::updateTask(db::TargetRecordType taskMap, size_t id)
{
    IterableTask::updateTask(taskMap,id);
    map<std::string, std::string> unpackedMap {unpackDataField(taskMap)};
    seriesTimeStep = itime::hours(atoi(unpackedMap.at("seriesTimeStep").c_str()));

    int seriesDiff = series - atoi(unpackedMap.at("series").c_str());
    series = atoi(unpackedMap.at("series").c_str());
    m_isMidnight = unpackedMap.at("isMidnight") == "true";
    if(m_isMidnight){
        seriesBeginDateTime = seriesTimeStep.count() < 24 ? TasksTimer::floorHours(seriesBeginDateTime) : TasksTimer::toMidnight(seriesBeginDateTime);
    }
    seriesBeginDateTime = seriesBeginDateTime - (-seriesDiff)*seriesTimeStep;
    maxSeries = atoi(unpackedMap.at("maxSeries").c_str());
    maxSeriesBeginDateTime = seriesBeginDateTime - maxSeries*seriesTimeStep;//just not equal seriesBeginTime
    maxSeriesUpdate();

    auto updatedTask = this->taskMap();
    updatedTask["id"] = std::to_string(id);
    executor->insertOrReplace(std::move(updatedTask),"iterators_1");
}

bool IterableSeries::isSeriesContinue() const
{
    return  (seriesBeginDateTime + (seriesTimeStep*(series+1))) > TasksTimer::currentTime();
}

void IterableSeries::maxSeriesUpdate()
{
    maxSeriesBeginDateTime = series > maxSeries ? seriesBeginDateTime : maxSeriesBeginDateTime;
    if(series > maxSeries)
        maxSeries = series;
    else if(series < maxSeries && maxSeriesBeginDateTime == seriesBeginDateTime){//current series - max. has been decrease
        maxSeries = series;
        maxSeriesBeginDateTime -= seriesTimeStep; //make maxDate not equal beginDate// TODO: save prev maxDate
    }
}

