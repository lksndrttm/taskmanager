#include "../include/noteinfo.h"

NoteInfo::NoteInfo(const std::map<std::string,std::string>& note,QObject *parent) : QObject(parent)
{
    m_noteMap = note;
}

int NoteInfo::getId() const
{
    return atoi(m_noteMap.at("id").c_str());
}

bool NoteInfo::isFavorite() const
{
    return m_noteMap.at("isFavorite") == "1";
}

QString NoteInfo::getText() const
{
    QRegExp imgRe {R"(<img [^>]*src[ ]*=[ ]*["']([^"']*)["'])"};

    QString res {m_noteMap.at("content").c_str()};

    int pos = 0;

    while ((pos = imgRe.indexIn(res, pos)) != -1) {
            res.insert(imgRe.pos(1),"file:");
            pos += imgRe.matchedLength() + 5;
    }

    return res;
}

QString NoteInfo::getRawText() const
{
    return {m_noteMap.at("content").c_str()};
}

QString NoteInfo::getTitle() const
{
    return QString(m_noteMap.at("title").c_str());
}

QString NoteInfo::getCategory() const
{
    return QString(m_noteMap.at("category").c_str());
}

QString NoteInfo::getCreateDateTime() const
{
    return QString(m_noteMap.at("createDateTime").c_str());
}

QString NoteInfo::getLastUpdateDateTime() const
{
    return QString(m_noteMap.at("lastEditDateTime").c_str());
}

QString NoteInfo::getImagePath() const
{
    QString text {m_noteMap.at("content").c_str()};
    QRegExp rxFirstImg("<img[^>]* src[ ]*=[ ]*['\"]([^\'\">]*)['\"].*>");
    int pos = rxFirstImg.indexIn(text);
    if (pos > -1) {
        return "file:" + rxFirstImg.cap(1);
    }
    return {};
}

QStringList NoteInfo::getTags() const
{
    return QString(m_noteMap.at("tags").c_str()).split(",");
}
