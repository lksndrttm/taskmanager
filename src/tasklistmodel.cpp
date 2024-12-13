#include "tasklistmodel.h"
#include "executor.h"
#include <QQmlEngine>
#include <memory>
#include <QDebug>

TaskListModel::TaskListModel()
    :list{new BaseTargetList{}}
{

}

void TaskListModel::setSelectedList(QString listName)
{
    beginResetModel();
    list->initList(listName.toStdString());
    endResetModel();
}

QVariant TaskListModel::getTimePeriod(const QModelIndex &index)
{
    if(!index.isValid())
        return {QString{""}};
    QString startTime {data(index,TaskRoles::StartTime).toString()};
    QString endTime {data(index,TaskRoles::EndTime).toString()};
    if(startTime.isEmpty() || endTime.isEmpty())
        return {QString{""}};

    return QVariant::fromValue(startTime + " - " + endTime);
}

QVariant TaskListModel::getStartTime(const QModelIndex &index)
{
    if(!index.isValid())
        return {QString{""}};
    return data(index,TaskRoles::StartTime);
}

QVariant TaskListModel::getEndTime(const QModelIndex &index)
{
    if(!index.isValid())
        return {QString{""}};
    return data(index,TaskRoles::EndTime);
}

QVariant TaskListModel::getPrior(const QModelIndex &index)
{
    if(!index.isValid())
        return {QString{""}};
    return data(index,TaskRoles::Prior);
}

QVariant TaskListModel::getStatus(const QModelIndex &index)
{
    if(!index.isValid())
        return {QString{""}};
    return data(index,TaskRoles::Status);
}

QVariant TaskListModel::getName(const QModelIndex &index)
{
    if(!index.isValid())
        return {QString{""}};
    return data(index,TaskRoles::Name);
}

QVariant TaskListModel::getDescription(const QModelIndex &index)
{
    if(!index.isValid())
        return {QString{""}};
    return data(index,TaskRoles::Description);
}

QVariant TaskListModel::getType(const QModelIndex &index)
{
    if(!index.isValid())
        return {QString{""}};
    return data(index,TaskRoles::TargetType);
}

QVariant TaskListModel::getListType()
{
    return QVariant::fromValue(QString::fromStdString(list->getlistSetting().at("type")));
}

QVariant TaskListModel::getCreateTime(const QModelIndex &index)
{
    if(!index.isValid())
        return {QString{""}};
    return data(index,TaskRoles::CreateTime);
}

QList<QVariant> TaskListModel::getSubTasks(const QModelIndex &parent)
{
    if(!parent.isValid())
        return {};
    ITarget *item = static_cast<ITarget*>(parent.internalPointer());
    auto subTasks = item->getSubTargets();
    QList<QVariant> res;
    for(size_t i = 0; i< subTasks.size(); i++){
        res.append(QVariant::fromValue(index(i,0,parent)));
    }
    return res;
}

void TaskListModel::setTaskState(bool state, const QModelIndex &index)
{
    ITarget *item = static_cast<ITarget*>(index.internalPointer());
    item->setStatus(state);
}

void TaskListModel::addTask(QVariantMap task, const QModelIndex &parent)
{
    ITarget* parentItem;

    if(task.empty()){
        qWarning() << "add task error task map empty.";
        return;
    }

    if(!parent.isValid())
        parentItem = list->getRootItem();
    else
        parentItem = static_cast<ITarget*>(parent.internalPointer());


    auto trgMap = convertFromQmlMap(task);

    if(parentItem == list->getRootItem()){
        beginInsertRows(QModelIndex(),parentItem->getSubTargets().size(),parentItem->getSubTargets().size());
        list->addTarget(trgMap);
        endInsertRows();
        return;
    }

    beginInsertRows(parent,parentItem->getSubTargets().size(),parentItem->getSubTargets().size());
    parentItem->addSubTarget(trgMap);
    endInsertRows();
}

void TaskListModel::delTask(const QModelIndex &index)
{
    if(!index.isValid()){
        qWarning() << "Delete error. Index not valid.";
        return;
    }
    ITarget* parentItem;
    if(!index.parent().isValid())
        parentItem = list->getRootItem();
    else
        parentItem = static_cast<ITarget*>(index.parent().internalPointer());

    if(static_cast<int>(parentItem->getSubTargets().size()) <= index.row() || index.row()<0){
        qWarning() << "delTask error incorrect index.";
        return;
    }

    if(parentItem == list->getRootItem()){
        beginRemoveRows(QModelIndex(),index.row(),index.row());
        list->delTargetByRow(index.row());
        endRemoveRows();
        return;
    }

    beginRemoveRows(index.parent(),index.row(),index.row());
    parentItem->delSubTargetByRow(index.row());
    endRemoveRows();
    qDebug() << "ENDDELETEPROCESSS";
}

void TaskListModel::updateTask(QVariantMap task, const QModelIndex &index)
{
    qDebug() << "StartUpdate";
    if(!index.isValid()){
        qWarning() << "changeTaskName error. Index not valid.";
        return;
    }
    ITarget* parentItem = static_cast<ITarget*>(index.internalPointer());
    auto trgMap = convertFromQmlMap(task);
    qDebug() << "Converted";
    //beginResetModel();
    parentItem->updateTarget(trgMap);
    //endResetModel();
    //qDebug() << "Updated";
    emit dataChanged(index,index);
}

void TaskListModel::changeTaskName(QString name, const QModelIndex &index)
{
    if(!index.isValid()){
        qWarning() << "changeTaskName error. Index not valid.";
        return;
    }
    ITarget* parentItem = static_cast<ITarget*>(index.internalPointer());
    auto trgMap {parentItem->targetToMap()};
    trgMap["name"] = name.toStdString();
    parentItem->updateTarget(trgMap);
    emit dataChanged(index,index,{TaskRoles::Name});
    qDebug() << "Renamed " << QString::fromStdString(trgMap.at("hierarchyAddr"));
}

void TaskListModel::changeTaskDescription(QString description, const QModelIndex &index)
{
    if(!index.isValid()){
        qWarning() << "changeTaskDescription error. Index not valid.";
        return;
    }
    ITarget* parentItem = static_cast<ITarget*>(index.internalPointer());
    auto trgMap {parentItem->targetToMap()};
    trgMap["description"] = description.toStdString();
    parentItem->updateTarget(trgMap);
    emit dataChanged(index,index,{TaskRoles::Description});
    qDebug() << "Rediscribed";
}

void TaskListModel::changeTaskPrior(QString prior, const QModelIndex &index)
{
    if(!index.isValid()){
        qWarning() << "changeTaskPrior error. Index not valid.";
        return;
    }
    ITarget* parentItem = static_cast<ITarget*>(index.internalPointer());
    auto trgMap {parentItem->targetToMap()};
    trgMap["prior"] = prior.toStdString();
    parentItem->updateTarget(trgMap);
    emit dataChanged(index,index, {TaskRoles::Prior});
    qDebug() << "Repriored";
}

void TaskListModel::changeSubTask(QString name, QString prior, const QModelIndex &index)
{
    if(!index.isValid()){
        qWarning() << "changeSubTask error. Index not valid.";
        return;
    }
    ITarget* parentItem = static_cast<ITarget*>(index.internalPointer());
    auto trgMap {parentItem->targetToMap()};
    trgMap["name"] = name.toStdString();
    trgMap["prior"] = prior.toStdString();
    parentItem->updateTarget(trgMap);
    emit dataChanged(index,index, {TaskRoles::Prior,TaskRoles::Name});
}

void TaskListModel::registerMe(const std::string &moduleName)
{
    qmlRegisterType<TaskListModel>(moduleName.c_str(), 1, 0, "TaskListModel");
}

QModelIndex TaskListModel::index(int row, int column, const QModelIndex &parent) const
{

    if (!hasIndex(row, column, parent))
        return QModelIndex();

    ITarget *parentItem;

    if (!parent.isValid())
        parentItem = list->getRootItem();
    else
        parentItem = static_cast<ITarget*>(parent.internalPointer());

    ITarget *childItem = ItemAdapter::itemSubTargets(parentItem).at(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex TaskListModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    ITarget *childItem = static_cast<ITarget*>(index.internalPointer());
    ITarget *parentItem = childItem->getParent();

    if (parentItem == list->getRootItem() || !childItem)
        return QModelIndex();

    auto getParentRow = [](ITarget* parentItem)->int{
        auto rootList = parentItem->getParent()->getSubTargets();
        for(size_t i = 0; i < rootList.size(); i++){
          if(rootList.at(i)->getHierarchyAddr() == parentItem->getHierarchyAddr())
              return  i;
        }
        qDebug() << "TaskListModel::parent parent row not finded";
        return -1;
    };
    return createIndex(getParentRow(parentItem), 0, parentItem);
}

int TaskListModel::rowCount(const QModelIndex &parent) const
{
    ITarget *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = list->getRootItem();
    else
        parentItem = static_cast<ITarget*>(parent.internalPointer());

    return parentItem->getSubTargets().size();
}

int TaskListModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant TaskListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section);
    Q_UNUSED(orientation);
    if(role == TaskRoles::Name)
        return QString::fromStdString(list->getlistSetting().at("name"));
    return {};
}

QVariant TaskListModel::data(const QModelIndex &index, int role) const
{
    ITarget *item = static_cast<ITarget*>(index.internalPointer());
    db::TargetRecordType currentTask {item->targetToMap()};


    if(!index.isValid() || index.row() > rowCount(index.parent())){
        return {};
    }
    switch (role) {
        case TaskRoles::Name: {
            return QVariant::fromValue(QString::fromStdString(currentTask.at("name")));
        }
        break;
        case TaskRoles::TargetType: {
            return QVariant::fromValue(QString::fromStdString(currentTask.at("targetType")));
        }
        break;
        case TaskRoles::Prior: {
            return QVariant::fromValue(QString::fromStdString(currentTask.at("prior")));
        }
        break;
        case TaskRoles::Status: {
            return QVariant::fromValue(QString::fromStdString(currentTask.at("status")));
        }
        break;
        case TaskRoles::EndTime: {
            return QVariant::fromValue(QString::fromStdString(currentTask.count("endTime") ? currentTask.at("endTime"): ""));
        }
        case TaskRoles::StartTime: {
            return QVariant::fromValue(QString::fromStdString(currentTask.count("startTime") ? currentTask.at("startTime"): ""));
        }
        break;
        case TaskRoles::CreateTime: {
            return QVariant::fromValue(QString::fromStdString(currentTask.at("createTime")));
        }
        break;
        case TaskRoles::RepeatTime: {
            return QVariant::fromValue(QString::fromStdString(currentTask.count("repeatTime") ? currentTask.at("repeatTime"): ""));
        }
        break;
        case TaskRoles::Description: {
            return QVariant::fromValue(QString::fromStdString(currentTask.count("description") ? currentTask.at("description"): ""));
        }
        case TaskRoles::BeginDateTime: {
            return QVariant::fromValue(QString::fromStdString(currentTask.count("beginDateTime") ? currentTask.at("beginDateTime"): ""));
        }
        break;
        case TaskRoles::HierarchyAddr: {
            return QVariant::fromValue(QString::fromStdString(currentTask.at("hierarchyAddr")));
        }
        break;
        case TaskRoles::NextCompliteDateTime: {
            return QVariant::fromValue(QString::fromStdString(currentTask.count("nextCompliteDateTime") ? currentTask.at("nextCompliteDateTime"): ""));
        }
        break;
        case TaskRoles::ListName: {
            return QVariant::fromValue(QString::fromStdString(list->getlistSetting().at("name")));
        }
        break;
        case TaskRoles::ListType: {
            return QVariant::fromValue(QString::fromStdString(list->getlistSetting().at("type")));
        }
        break;
        default:{
            return {};
        }
        break;

    }
}

QHash<int, QByteArray> TaskListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[TaskRoles::Name] = "name";
    roles[TaskRoles::Prior] = "prior";
    roles[TaskRoles::Status] = "status";
    roles[TaskRoles::EndTime] = "endTime";
    roles[TaskRoles::StartTime] = "startTime";
    roles[TaskRoles::CreateTime] = "createTime";
    roles[TaskRoles::RepeatTime] = "repeatTime";
    roles[TaskRoles::TargetType] = "type";
    roles[TaskRoles::Description] = "description";
    roles[TaskRoles::BeginDateTime] = "beginDateTime";
    roles[TaskRoles::HierarchyAddr] = "hierarchyAddr";
    roles[TaskRoles::NextCompliteDateTime] = "nextCompliteDateTime";
    roles[TaskRoles::ListName] = "listname";
    roles[TaskRoles::ListType] = "listtype";

    return roles;
}

db::TargetRecordType TaskListModel::convertFromQmlMap(QVariantMap qmlmap)
{
    db::TargetRecordType res;
    for(auto &[key,val] : qmlmap.toStdMap()){
        res[key.toStdString()] = val.toString().toStdString();
        //qDebug() << "key: " << key << " value:" << val;
    }
    return  res;
}



std::vector<ITarget*> TaskListModel::ItemAdapter::itemSubTargets(ITargetListPtr item)
{
    return item->getTargetList();
}

std::vector<ITarget*> TaskListModel::ItemAdapter::itemSubTargets(ITargetPtr item)
{
    return item->getSubTargets();
}

std::vector<ITarget*> TaskListModel::ItemAdapter::itemSubTargets(ITarget *item)
{
    return item->getSubTargets();
}
