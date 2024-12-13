#include "iterabletaskmodel.h"
#include <sstream>
#include <QQmlEngine>
#include <QDebug>
#include <algorithm>

IterableTaskPtr createIterTask(db::TargetRecordType taskMap){//fabric func
    auto unpackedMap {IterableTask::unpackDataField(taskMap)};
    if(unpackedMap.at("type")=="counter")
        return std::make_shared<IterableTask>(taskMap);
    else if(unpackedMap.at("type")=="series")
        return std::make_shared<IterableSeries>(taskMap);

    qWarning() << "Fail create iterable task. Undefined type.";
    return nullptr;
}

IterModel::IterModel()
{
    //m_iterators =
    auto iterTaskData {executor.select("iterators_1")};
    for(auto &iterTask : iterTaskData){
        m_iterators.push_back(createIterTask(iterTask));
    }
}

void IterModel::registerMe(const std::string &moduleName)
{
    qmlRegisterType<IterModel>(moduleName.c_str(), 1, 0, "IterModel");
}

int IterModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_iterators.size();
}

QVariant IterModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || index.row() > rowCount()){
        return {};
    }
    auto itertask {m_iterators.at(index.row())};
    switch (role) {
        case IterRoles::IterName: {
            return QVariant::fromValue(QString::fromStdString(itertask->getName()));
        }
        break;
        case IterRoles::Status: {
            return QVariant::fromValue(itertask->isComplited());
        }
        break;
        case IterRoles::Type: {
            return  QVariant::fromValue(QString::fromStdString(itertask->getType()));
        }
        break;
        case IterRoles::Series: {
            if(itertask->getType()=="counter"){
                return QVariant::fromValue(QString::fromStdString(""));
            }
            else if(itertask->getType()=="series"){
                return QVariant::fromValue(QString::fromStdString(itertask->getDataFieldValue("series")));
            }
        }
        break;
        case IterRoles::MaxSeries: {
            if(itertask->getType()=="counter"){
                return QVariant::fromValue(QString::fromStdString(""));
            }
            else if(itertask->getType()=="series"){
                return QVariant::fromValue(QString::fromStdString(itertask->getDataFieldValue("maxSeries")));
            }
        }
        break;
        case IterRoles::CreateTime: {
            return QVariant::fromValue(QString::fromStdString(itertask->getDataFieldValue("cerateTime")));
        }
        break;
        case IterRoles::CompliteCount: {
            return QVariant::fromValue(QString::fromStdString(itertask->getDataFieldValue("compliteCount")));
        }
        break;
        case IterRoles::SeriesTimeStep: {
            if(itertask->getType()=="counter")
                return QVariant::fromValue(QString::fromStdString(""));
            else if(itertask->getType()=="series"){
                return QVariant::fromValue(QString::fromStdString(itertask->getDataFieldValue("seriesTimeStep")));
            }
        }
        break;
        case IterRoles::SeriesBeginDateTime: {
            if(itertask->getType()=="counter")
                return QVariant::fromValue(QString::fromStdString(""));
            else if(itertask->getType()=="series"){
                return QVariant::fromValue(QString::fromStdString(itertask->getDataFieldValue("seriesBeginDateTime")));
            }
        }
        break;
        case IterRoles::MaxSeriesBeginDateTime: {
            if(itertask->getType()=="counter")
                return QVariant::fromValue(QString::fromStdString(""));
            else if(itertask->getType()=="series"){
                return QVariant::fromValue(QString::fromStdString(itertask->getDataFieldValue("maxSeriesBeginDateTime")));
            }
        }
        break;
        case IterRoles::Level: {
            return QVariant::fromValue(itertask->getLevel());
        }
        break;
        case IterRoles::UpLevelScore: {
            return QVariant::fromValue(itertask->upLevelScore(itertask->getLevel()));
        }
        break;
        case IterRoles::NextTime: {
            if(itertask->getType()=="counter")
                return QVariant::fromValue(QString::fromStdString(""));
            else if(itertask->getType()=="series"){
                auto taskMap {IterableTask::unpackDataField(itertask->taskMap())};
                std::string beginTime {taskMap.at("seriesBeginDateTime")};
                auto timeStep {itime::hours(atoi(taskMap.at("seriesTimeStep").c_str()))};
                auto seriesDuration {(atoi(taskMap.at("series").c_str())+1) * timeStep};
                std::string timeformat;
                if(timeStep.count() >= 24 && taskMap.at("isMidnight") == "true"){
                    timeformat = "%d.%m.%Y";// : "%H:%M %d.%m.%Y";
                }
                else {
                    timeformat = "%H:%M %d.%m.%Y";
                }
                std::string nextTime {TasksTimer::datetimeToString(TasksTimer::stringToDatetime(beginTime) + seriesDuration,timeformat)};
                return QVariant::fromValue(QString::fromStdString(nextTime));
            }
        }
        break;
        case IterRoles::IsMidnight: {
            if(itertask->getType()=="counter")
                return QVariant::fromValue(QString::fromStdString(""));
            else if(itertask->getType()=="series"){
                auto taskMap {IterableTask::unpackDataField(itertask->taskMap())};
                return QVariant::fromValue(QString::fromStdString(taskMap.at("isMidnight")));
            }
        }
    break;
        default:{
            return {};
        }

    }
    return {};
}


QHash<int, QByteArray> IterModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IterRoles::IterName] = "name";
    roles[IterRoles::Status] = "stat";
    roles[IterRoles::CompliteCount] = "compliteCount";
    roles[IterRoles::MaxSeries] = "maxseries";
    roles[IterRoles::CreateTime] = "createtime";
    roles[IterRoles::Type] = "type";
    roles[IterRoles::Series] = "series";
    roles[IterRoles::SeriesTimeStep] = "seriesTimeStep";
    roles[IterRoles::SeriesBeginDateTime] = "seriesBeginDateTime";
    roles[IterRoles::MaxSeriesBeginDateTime] = "maxSeriesBeginDateTime";
    roles[IterRoles::Level] = "level";
    roles[IterRoles::UpLevelScore] = "upLevelScore";
    roles[IterRoles::NextTime] = "nextTime";
    roles[IterRoles::IsMidnight] = "isMidnight";
    return roles;
}

bool IterModel::addIterator(QVariantMap task)
{
    std::map<string,string> resMap;
    for(auto it = task.begin();it != task.end();it++){
        resMap[it.key().toStdString()] = it.value().toString().toStdString();
    }
//    resMap["name"] = task.value("name").toString().toStdString();
//    resMap["type"] = task.value("type").toString().toStdString();
    resMap["compliteCount"] = "0";
    resMap["levelIncType"] = "const";
    //resMap["firstLevelScore"] = task.value("firstLevelScore").toString().toStdString();
    resMap["level"] = "0";
    resMap["series"] = "0";
    resMap["maxSeries"] = "0";
    //resMap["seriesTimeStep"] = task.value("seriesTimeStep").toString().toStdString();
    resMap["seriesBeginDateTime"] = TasksTimer::datetimeToString(TasksTimer::floorHours(TasksTimer::currentTime()));
    resMap["maxSeriesBeginDateTime"] = TasksTimer::datetimeToString(TasksTimer::floorHours(TasksTimer::currentTime()));
    resMap["createTime"] = TasksTimer::datetimeToString(TasksTimer::currentTime());
    resMap = IterableTask::packDataField(resMap);

    bool addStat = executor.insert(resMap,"iterators_1");

    if(addStat){
       beginInsertRows(QModelIndex{} ,rowCount(), rowCount());
       m_iterators.push_back(createIterTask(resMap));
       endInsertRows();
       return true;
    }
    return false;
}

bool IterModel::delIterator(int row)
{
    return delIterator(QString::fromStdString(m_iterators.at(row)->getName()));
}

bool IterModel::delIterator(QString name)
{
    size_t row = 0;
    for(auto && itertask : m_iterators){
        if(itertask->getName() == name.toStdString()){
            break;
        }
        row++;
    }

    beginRemoveRows({},row,row);
    m_iterators.erase(m_iterators.begin()+row);
    bool res = executor.deleteRecordByField("name",name.toStdString(),"iterators_1");
    endRemoveRows();
    return res;
}

void IterModel::changeDaylyStatus(QString name)
{
    size_t row = 0;
    for(auto && itertask : m_iterators){
        if(itertask->getName() == name.toStdString()){
            break;
        }
        row++;
    }
    if(m_iterators.at(row)->isComplited())
        m_iterators.at(row)->uncomplite();
    else
        m_iterators.at(row)->complite();
    emit dataChanged(index(row),index(row));
}

void IterModel::complite(QString name)
{
    size_t row = 0;
    for(auto && itertask : m_iterators){
        if(itertask->getName() == name.toStdString()){
            break;
        }
        row++;
    }
    m_iterators.at(row)->complite();
    emit dataChanged(index(row),index(row));
}

void IterModel::uncomplite(QString name)
{
    size_t row = 0;
    for(auto && itertask : m_iterators){
        if(itertask->getName() == name.toStdString()){
            break;
        }
        row++;
    }
    m_iterators.at(row)->uncomplite();
    emit dataChanged(index(row),index(row));
}

void IterModel::updateTask(QVariantMap task, int id)
{
    auto taskMap {IterableTask::packDataField(toTargetRecord(task))};
    size_t row = 0;
    std::string name {task.value("name").toString().toStdString()};
    for(auto && itertask : m_iterators){
        if(itertask->getName() == name){
            break;
        }
        row++;
    }
    qDebug() << QString::fromStdString(name) << "  " << row;
    m_iterators.at(id)->updateTask(taskMap,id);
    emit dataChanged(index(id),index(id));
}

db::TargetRecordType IterModel::toTargetRecord(QVariantMap task)
{
    map<std::string, std::string> resMap;
    for(auto it = task.begin();it!=task.end();it++){
        resMap[it.key().toStdString()] = it.value().toString().toStdString();
    }
    return resMap;
}

