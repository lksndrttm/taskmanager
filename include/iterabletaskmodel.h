#pragma once
#include <QAbstractListModel>
#include <vector>
#include <QVariant>
#include "executor.h"
#include "iterabletask.h"

class IterModel : public QAbstractListModel
{
    Q_OBJECT
public:
    IterModel();
    static void registerMe(const std::string& moduleName);
    int rowCount(const QModelIndex& parent = {}) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole)const override;
    QHash<int,QByteArray> roleNames() const override;

    Q_INVOKABLE bool addIterator(QVariantMap task);
    Q_INVOKABLE bool delIterator(int row);
    Q_INVOKABLE bool delIterator(QString name);
    Q_INVOKABLE void changeDaylyStatus(QString name);
    Q_INVOKABLE void complite(QString name);
    Q_INVOKABLE void uncomplite(QString name);
    Q_INVOKABLE void updateTask(QVariantMap task, int id);
private:
    db::TargetRecordType toTargetRecord(QVariantMap task);

    std::vector<IterableTaskPtr> m_iterators;
    db::Executor executor;
    enum IterRoles{
        IterName = Qt::UserRole,
        Type,
        CompliteCount,
        Status,
        Level,
        UpLevelScore,
        CreateTime,
        SeriesBeginDateTime,
        MaxSeriesBeginDateTime,
        Series,
        MaxSeries,
        SeriesTimeStep,
        NextTime,
        IsMidnight
    };
};

