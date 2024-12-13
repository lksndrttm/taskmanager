#pragma once
#include <QObject>
//#include <QAbstractListModel>
#include <QAbstractItemModel>
#include <vector>
#include <QVariant>
#include <QString>
#include "TargetList.h"

class TaskListModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    TaskListModel();
    Q_INVOKABLE void setSelectedList(QString listName);
    Q_INVOKABLE QVariant getTimePeriod(const QModelIndex &index);
    Q_INVOKABLE QVariant getStartTime(const QModelIndex &index);
    Q_INVOKABLE QVariant getEndTime(const QModelIndex &index);
    Q_INVOKABLE QVariant getPrior(const QModelIndex &index);
    Q_INVOKABLE QVariant getStatus(const QModelIndex &index);
    Q_INVOKABLE QVariant getName(const QModelIndex &index);
    Q_INVOKABLE QVariant getDescription(const QModelIndex &index);
    Q_INVOKABLE QVariant getType(const QModelIndex &index);
    Q_INVOKABLE QVariant getListType();
    Q_INVOKABLE QVariant getCreateTime(const QModelIndex &index);
    Q_INVOKABLE QList<QVariant> getSubTasks(const QModelIndex &parent);

    Q_INVOKABLE void setTaskState(bool state, const QModelIndex &index);
    Q_INVOKABLE void addTask(QVariantMap task, const QModelIndex &parent);
    Q_INVOKABLE void delTask(const QModelIndex &index);
    Q_INVOKABLE void updateTask(QVariantMap task,const QModelIndex &index);

    Q_INVOKABLE void changeTaskName(QString name,const QModelIndex &index);
    Q_INVOKABLE void changeTaskDescription(QString description,const QModelIndex &index);
    Q_INVOKABLE void changeTaskPrior(QString prior,const QModelIndex &index);
    Q_INVOKABLE void changeSubTask(QString name, QString prior, const QModelIndex &index);

    static void registerMe(const std::string& moduleName);
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex& parent = {}) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole)const override;
    QHash<int,QByteArray> roleNames() const override;

private:
    db::TargetRecordType convertFromQmlMap(QVariantMap qmlmap);

    //переделать в шаблонный класс
    struct ItemAdapter {
        static std::vector<ITarget*> itemSubTargets(ITargetListPtr item);
        static std::vector<ITarget*> itemSubTargets(ITargetPtr item);
        static std::vector<ITarget*> itemSubTargets(ITarget* item);
    };
    ITargetListPtr list;

    enum TaskRoles{
        Name = Qt::UserRole,
        TargetType,
        HierarchyAddr,
        Description,
        Status,
        CreateTime,
        StartTime,
        EndTime,
        Prior,
        NextCompliteDateTime,
        BeginDateTime,
        RepeatTime,
        ListName,
        ListType,
    };
};

