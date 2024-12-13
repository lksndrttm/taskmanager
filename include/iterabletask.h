#ifndef ITERABLETASK_H
#define ITERABLETASK_H

#include <string>
#include <sstream>
#include <map>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <memory>
#include "executor.h"
#include "taskstimer.h"
#include <QJsonDocument>
#include <QJsonObject>

using std::string;
using std::map;

class IterableTask
{
protected:
    string name;
    string type;
    std::unique_ptr<db::Executor> executor;
    itime::timePoint createTime;
    size_t compliteCount;
    string levelIncType;
    size_t firstLevelScore;
    size_t level;

public:
    explicit IterableTask(map<string,string> taskMap);//{"id":int, "name":string, "type":string, "data":string(json)}
    virtual ~IterableTask() = default;
    virtual map<string,string> taskMap() const;
    virtual void updateTask(db::TargetRecordType taskMap, size_t id);
    string getName() const {return name;}
    string getType() const {return type;}
    size_t getLevel() const {return level;}
    size_t getCount() const {return compliteCount;}
    string getDataFieldValue(string key);


    virtual bool isComplited() const {return false;}//always compliteble

    void saveTask();
    size_t levelScore(size_t lvl) const;
    size_t upLevelScore(size_t lvl) const;

    virtual void complite();
    virtual void uncomplite();

    static map<string,string> unpackDataField(map<string,string> taskMap);//taskMap has field named "data"
    static map<string,string> packDataField(map<string,string> taskMap);
};


class IterableSeries : public IterableTask
{
public:
    explicit IterableSeries(map<string,string> taskMap);
    map<string,string> taskMap() const override;
    void complite() override;
    void uncomplite() override;
    bool isComplited() const override;
    void updateTask(db::TargetRecordType taskMap, size_t id) override;
    bool isMidnight() const {return  m_isMidnight;}

protected:
    size_t series;
    size_t maxSeries;
    itime::hours seriesTimeStep;
    itime::timePoint seriesBeginDateTime;
    itime::timePoint maxSeriesBeginDateTime;
    bool m_isMidnight;

    bool isSeriesContinue() const;
    void maxSeriesUpdate();
};

using IterableTaskPtr = std::shared_ptr<IterableTask>;
#endif // ITERABLETASK_H
