#include "../include/categoryinfo.h"

CategoryInfo::CategoryInfo(const std::map<std::string, std::string> &cat, QObject *parent) : QObject(parent)
{
    m_executor.reset(new db::Executor{});
    m_catmap = cat;
}

int CategoryInfo::getId() const
{
    return atoi(m_catmap.at("id").c_str());
}

int CategoryInfo::getCount() const
{
    return m_executor->select("notes","category = '" + getName().toStdString() + "'").size();
}

QString CategoryInfo::getName() const
{
    return QString(m_catmap.at("name").c_str());
}

QString CategoryInfo::getColor() const
{
    return QString(m_catmap.at("color").c_str());
}

QString CategoryInfo::getImage() const
{
    return QString(m_catmap.at("image").c_str());
}
