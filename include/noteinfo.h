#ifndef NOTEINFO_H
#define NOTEINFO_H

#include <QObject>
#include <string>
#include <map>
#include <QString>
#include <QStringList>
#include <QDebug>

class NoteInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int index READ getId NOTIFY noteChanged)
    Q_PROPERTY(bool favorite READ isFavorite NOTIFY noteChanged)
    Q_PROPERTY(QString title READ getTitle NOTIFY noteChanged)
    Q_PROPERTY(QString content READ getText NOTIFY noteChanged)
    Q_PROPERTY(QString text READ getRawText NOTIFY noteChanged)
    Q_PROPERTY(QString category READ getCategory NOTIFY noteChanged)
    Q_PROPERTY(QString createDateTime READ getCreateDateTime NOTIFY noteChanged)
    Q_PROPERTY(QString lastUpdateDateTime READ getLastUpdateDateTime NOTIFY noteChanged)
    Q_PROPERTY(QString image READ getImagePath NOTIFY noteChanged)
    Q_PROPERTY(QStringList tags READ getTags NOTIFY noteChanged)
public:
    explicit NoteInfo(const std::map<std::string,std::string>& note,QObject *parent = nullptr);

    int getId() const;
    bool isFavorite() const;
    QString getText() const;
    QString getRawText() const;
    QString getTitle() const;
    QString getCategory() const;
    QString getCreateDateTime() const;
    QString getLastUpdateDateTime() const;
    QString getImagePath() const;
    QStringList getTags() const;

private:
    std::map<std::string,std::string> m_noteMap;

signals:
    void noteChanged();
};

#endif // NOTEINFO_H
