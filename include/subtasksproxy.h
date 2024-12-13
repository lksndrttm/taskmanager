#pragma once
#include <QSortFilterProxyModel>


class SubTasksProxyModel : QSortFilterProxyModel
{
public:
    void setCurrentTask(const QModelIndex &index);
};
