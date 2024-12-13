#ifndef CATEGORYINFO_H
#define CATEGORYINFO_H

#include <QObject>
#include <QString>
#include "executor.h"
#include <memory>

class CategoryInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int index READ getId NOTIFY catChanged)
    Q_PROPERTY(int count READ getCount NOTIFY catChanged)
    Q_PROPERTY(QString name READ getName NOTIFY catChanged)
    Q_PROPERTY(QString color READ getColor NOTIFY catChanged)
    Q_PROPERTY(QString image READ getImage NOTIFY catChanged)

public:
    explicit CategoryInfo(const std::map<std::string,std::string>& cat,QObject *parent = nullptr);
    int getId() const;
    int getCount() const;
    QString getName() const;
    QString getColor() const;
    QString getImage() const;

signals:
    void catChanged();

private:
    std::map<std::string,std::string> m_catmap;
    std::unique_ptr<db::Executor> m_executor;

};

#endif // CATEGORYINFO_H
