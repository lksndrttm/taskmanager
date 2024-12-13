#include "../include/notesmodel.h"
#include "../include/resourcesmanager.h"
#include <QRegExp>

NotesModel::NotesModel(QObject* parent) : QObject(parent)
{
    m_executor.reset(new db::Executor{});
    auto notesMap = m_executor->select("notes");
    for(auto& note : notesMap){
        m_notes.append(new NoteInfo(note));
    }

    auto catsMap = m_executor->select("notes_cat");
    for(auto& cat : catsMap){
        m_cats.append(new CategoryInfo(cat));
    }
    QObject::connect(this, SIGNAL(notesListChanged()), this, SIGNAL(catsListChanged()));

    QObject::connect(this, SIGNAL(notesListChanged()), this, SIGNAL(catsCountsChanged()));
    QObject::connect(this, SIGNAL(catsListChanged()), this, SIGNAL(catsCountsChanged()));
}

NotesModel::~NotesModel()
{
    qDeleteAll(m_notes);
    qDeleteAll(m_cats);
}

QVariant NotesModel::notes() const
{
    return QVariant::fromValue(m_notes);
}

QVariant NotesModel::reversed_notes() const
{
    QList<QObject*> revNotes;
    revNotes.reserve(m_notes.size());
    std::reverse_copy(m_notes.begin(), m_notes.end(), std::back_inserter(revNotes));
    return QVariant::fromValue(revNotes);
}

QVariant NotesModel::categories() const
{
    return QVariant::fromValue(m_cats);
}

QVariant NotesModel::categories_counts() const
{
    QList<int> counts;
    for(int i = 0; i < m_cats.size(); i++){
        counts.append(qobject_cast<CategoryInfo*>(m_cats.at(i))->getCount());
    }
    counts.append(favNotesCount());
    counts.append(notesCount());
    return QVariant::fromValue(counts);
}

void NotesModel::registerMe(const std::string &moduleName)
{
    qmlRegisterType<NotesModel>(moduleName.c_str(), 1, 0, "NotesModel");
}

void NotesModel::showCategory(QString cat)
{
    qDeleteAll(m_notes);
    m_notes.clear();

    auto catNotes = m_executor->select("notes","category = '" + cat.toStdString() + "'");
    for( auto &cat : catNotes){
        m_notes.append(new NoteInfo(cat));
    }
    emit notesListChanged();
//    for(auto note : m_notes){
//        emit qobject_cast<NoteInfo*>(note)->noteChanged();
    //    }
}

void NotesModel::showFavorite()
{
    qDeleteAll(m_notes);
    m_notes.clear();

    auto notesMap = m_executor->select("notes","isFavorite = '1'");
    for(auto& note : notesMap){
        m_notes.append(new NoteInfo(note));
    }
    emit notesListChanged();
}

void NotesModel::showAll()
{
    qDeleteAll(m_notes);
    m_notes.clear();
    auto notesMap = m_executor->select("notes");
    for(auto& note : notesMap){
        m_notes.append(new NoteInfo(note));
    }
    emit notesListChanged();
}

bool NotesModel::addNote(QString title, QString data, QString createDTime, QString category, QString tags, bool favorite)
{
    db::NoteRecordType newNote;
    newNote["title"] = title.toStdString();
    newNote["createDateTime"] = createDTime.toStdString();
    newNote["lastEditDateTime"] = createDTime.toStdString();
    newNote["content"] = reservateImages(data).toStdString();
    newNote["isFavorite"] = "0";
    newNote["category"] = category.toStdString();
    newNote["tags"] = tags.toStdString();
    newNote["isFavorite"] = favorite ? "1" : "0";

    bool res = m_executor->insert(newNote,"notes");

    if(res){
        auto lastNote = new NoteInfo(m_executor->selectOne("notes","TRUE", true));
        m_notes.append(lastNote);
        emit notesListChanged();
    }

    return res;
}

bool NotesModel::updateNote(int id, QString title, QString data, QString updateDTime, QString category, QString tags, bool favorite)
{
    db::NoteRecordType newNote;
    int reversedId = m_notes.size() - 1 - id;
    auto infoObj = qobject_cast<NoteInfo*>(m_notes.at(reversedId));
    int dbId = infoObj->getId();

    newNote["title"] = title.toStdString();
    newNote["lastEditDateTime"] = updateDTime.toStdString();
    newNote["content"] = reservateImages(data).toStdString();
    newNote["category"] = category.toStdString();
    newNote["tags"] = tags.toStdString();
    newNote["isFavorite"] = favorite ? "1" : "0";

    bool res = m_executor->update(dbId, newNote,"notes");

    unreservateUnneededImages(infoObj->getRawText(), data);

    if(res){
        delete  m_notes[reversedId];
        m_notes[reversedId] = new NoteInfo(m_executor->selectOne("notes","id = " + std::to_string(dbId)));
//        emit infoObj->noteChanged();
        emit notesListChanged();
    }

    return res;
}

bool NotesModel::deleteNote(int id)
{
    int reversedId = m_notes.size() - 1 - id;
    NoteInfo* noteToDel {qobject_cast<NoteInfo*>(m_notes.at(reversedId))};
    int dbId = noteToDel->getId();

    bool res =  m_executor->deleteRecord(dbId,"notes");

    if(res) {
        unreservateImages(noteToDel->getRawText());
        delete  m_notes[reversedId];
        noteToDel = nullptr;
        m_notes.removeAt(reversedId);
        emit notesListChanged();
    }
    return res;
}

bool NotesModel::changeNoteFavorite(int id)
{
    int reversedId = m_notes.size() - 1 - id;
    auto infoObj = qobject_cast<NoteInfo*>(m_notes.at(reversedId));
    bool res = this->updateNote(id,infoObj->getTitle(),infoObj->getRawText(), infoObj->getLastUpdateDateTime(),
                     infoObj->getCategory(),infoObj->getTags().join(","), !infoObj->isFavorite());
    if(res){
        emit notesListChanged();
    }
    return res;
}

bool NotesModel::createCategory(QString name, QString color, QString image)
{
    db::Record catRecord;
    catRecord["name"] = name.toStdString();
    catRecord["color"] = color.toStdString();
    catRecord["image"] = image.toStdString();

    bool res = m_executor->insert(catRecord, "notes_cat");
    if(res){
        auto addedCat = m_executor->selectOne("notes_cat","TRUE",true);
        m_cats.append(new CategoryInfo(addedCat));
        emit catsListChanged();
    }
    return res;
}

bool NotesModel::updateCategory(int id, QString name, QString color, bool updateRelatedNotes, QString image)
{
    auto catToUpdate = qobject_cast<CategoryInfo*>(m_cats.at(id));
    int dbId = catToUpdate->getId();

    db::Record catRecord;
    catRecord["name"] = name.toStdString();
    catRecord["color"] = color.toStdString();
    catRecord["image"] = image.toStdString();

    bool res = m_executor->update(dbId, catRecord, "notes_cat");

    if(res){
        db::Record updatedCat;
        updatedCat["category"] = name.toStdString();

        m_executor->updateSet("category = '" + catToUpdate->getName().toStdString() + "'", updatedCat, "notes");//update category name in related notes

        auto allNotes = m_executor->select("notes");
        for(auto &note : allNotes){
            if(note.at("category") == catToUpdate->getName().toStdString()){
                note["category"] = name.toStdString();
                m_executor->update(atoi(note.at("id").c_str()), std::move(note),"notes");
            }
        }
        delete  m_cats[id];
        m_cats[id] = new CategoryInfo(m_executor->selectOne("notes_cat","id = " + std::to_string(dbId)));
        emit catsListChanged();
        refreshNotes();
    }

    return res;
}

bool NotesModel::deleteCategory(int id)
{
    auto catToDelete = qobject_cast<CategoryInfo*>(m_cats.at(id));
    int dbId = catToDelete->getId();
    bool res =  m_executor->deleteRecord(dbId,"notes_cat");

    if(res) {
        auto allNotes = m_executor->select("notes");
        for(auto &note : allNotes){
            if(note.at("category") == catToDelete->getName().toStdString()){
                note["category"] = "";
                m_executor->update(atoi(note.at("id").c_str()), std::move(note),"notes");
            }
        }
        delete  m_cats[id];
        m_cats.removeAt(id);
        emit catsListChanged();
    }
    return res;
}

void NotesModel::refreshNotes()
{
    for(int i = 0; i  < m_notes.size(); i++){
        auto tempNote = m_executor->selectOne("notes","id = " + std::to_string(qobject_cast<NoteInfo*>(m_notes.at(i))->getId()));
        delete m_notes[i];
        m_notes[i] = new NoteInfo(tempNote);
    }
    emit notesListChanged();
}

int NotesModel::notesCount() const
{
    return m_executor->select("notes").size();
}

int NotesModel::favNotesCount() const
{
    return m_executor->select("notes","isFavorite = '1'").size();
}

QString NotesModel::reservateImages(const QString &text)
{
    ResourcesManager rm;
    QRegExp imgRe {R"(<img [^>]*src[ ]*=[ ]*["']([^"']*)["'])"};

    QString res {text};

    int pos = 0;

    while ((pos = imgRe.indexIn(res, pos)) != -1) {
        QString resourcePath = imgRe.cap(1);

        if(!rm.isReserved(resourcePath.toStdString())){
            QString newPath {rm.reservate(resourcePath.toStdString()).c_str()};
            res.replace(imgRe.pos(1), resourcePath.length(),newPath);
            pos += newPath.length();
        }
        else {
            pos += imgRe.matchedLength();
        }
    }

    return res;
}

bool NotesModel::unreservateImages(const QString &text)
{
    ResourcesManager rm;
    QRegExp imgRe {R"(<img [^>]*src[ ]*=[ ]*["']([^"']*)["'])"};

    QString txt {text};
    bool res {true};

    int pos = 0;

    while ((pos = imgRe.indexIn(txt, pos)) != -1) {
        QString resourcePath = imgRe.cap(1);
        if(resourcePath.startsWith("file:"))
            resourcePath = resourcePath.mid(5);

        res = rm.unreservate(resourcePath.toStdString()) && res;

        pos += imgRe.matchedLength();
    }

    return res;
}

bool NotesModel::unreservateUnneededImages(const QString & old, const QString & updated)
{
    ResourcesManager rm;

    auto oldImages {extractImages(old)};
    auto updatedImages {extractImages(updated)};

    bool res = true;

    for(auto&& image : oldImages){
        if(std::find(updatedImages.begin(),updatedImages.end(),image) == updatedImages.end()){
            res = rm.unreservate(image.toStdString()) && res;
        }
    }
    return res;
}


std::vector<QString> NotesModel::extractImages(const QString &text)
{
    ResourcesManager rm;
    QRegExp imgRe {R"(<img [^>]*src[ ]*=[ ]*["']([^"']*)["'])"};

    QString txt {text};
    std::vector<QString> res;

    int pos = 0;

    while ((pos = imgRe.indexIn(txt, pos)) != -1) {
        QString resourcePath = imgRe.cap(1);
        if(resourcePath.startsWith("file:"))
            resourcePath = resourcePath.mid(5);

        res.push_back(resourcePath);

        pos += imgRe.matchedLength();
    }

    return res;
}
