#include "TargetList.h"
#include <algorithm>
#include <QDebug>



bool BaseTargetList::isNameUnique(string name)
{
    auto listInfo { getLists() };

    auto it = std::find_if(listInfo.begin(), listInfo.end(),[name](auto infoPair){
        return infoPair.first == name;
    });
    return it == listInfo.end();
}

std::vector<db::ListInfoType> BaseTargetList::getLists()
{
    using std::vector, std::map, std::string;
    vector<std::pair<string, string>> res;

    auto lists = executor->select("lists");

    if(lists.empty()) {
        qDebug() << "Retuned empty list\n";
        return {};
    }

    for(auto &&list : lists){
        res.push_back(std::make_pair(list.at("name"),list.at("type")));
    }
    return res;
}



map<string, string> BaseTargetList::getlistSetting()
{
    auto listInfo { getLists() };

    auto it = std::find_if(listInfo.begin(), listInfo.end(),[this](auto infoPair){
        return infoPair.first == name;
    });
    if (it == listInfo.end()){
        qWarning() << "List not exist.";
        return {};
    }

    return {{"name",it->first},{"type",it->second}};
}

void BaseTargetList::clearRoot()
{
    auto rootSubTargets = rootItem->getSubTargets();
    qDeleteAll(rootSubTargets.begin(),rootSubTargets.end());
    rootSubTargets.clear();
}

void BaseTargetList::fillRootSubTargets()
{
    for(auto trg : targetList){
        rootItem->getSubTargets().push_back(trg);
    }
}

bool BaseTargetList::addTarget(map<string, string> & trg)
{

//    auto it = std::find_if(targetList.begin(),targetList.end(),[&trg](auto target){
//        return trg.at("name") == target->getName();
//    });
//    if(it != BaseTargetList.end()){
//        std::cout << "name must be unique.\n";
//        return nullptr;
//    }


    auto lastAddedTargets = executor->select("targets","TRUE",1,true);
    auto lastAddedTarget = lastAddedTargets.empty() ? db::Record{} : lastAddedTargets.front();
    //auto lastAddedTarget = executor->getLastTarget();
	int nextId = lastAddedTarget.count("id") ? atoi(lastAddedTarget.at("id").c_str()) + 1 : 0;

    string nextIdStr = std::to_string(nextId);
	auto trgToAdd = trg;
	trgToAdd["id"] = nextIdStr;
	trgToAdd["hierarchyAddr"] = name + "_" + nextIdStr;

    {
        auto nowTime = std::chrono::system_clock::now();
        auto tt (std::chrono::system_clock::to_time_t(nowTime));
        auto loct (std::localtime(&tt));
        stringstream ss;
        ss << std::put_time(loct,"%H:%M %d.%m.%Y");
        trgToAdd["createTime"] = ss.str();
    }

    bool res = executor->insert(trgToAdd,"targets");
    ITarget* addedTarget = ITarget::create(trgToAdd,rootItem);
	targetList.push_back(addedTarget);
    rootItem->getSubTargets().push_back(addedTarget);

    return res;
}

bool BaseTargetList::delTargetByRow(size_t row)
{
    if(row > targetList.size()){
        qWarning("Attemp to delete not exist row");
        return false;
    }
    bool res = rootItem->delSubTargetByRow(row);
    targetList.erase(targetList.begin()+row);
    return res;
}

void BaseTargetList::delTarget(int id)
{
    //auto BaseTargetList = getTargetList();

	if (targetList.empty()) return;

    auto it = std::find_if(targetList.begin(), targetList.end(), [id](const ITarget* _it) {
		return _it->getId() == id;
	});

    if (it != targetList.end()) {
        executor->deleteRecordByField("hierarchyAddr",(*it)->getHierarchyAddr(), "targets");
		targetList.erase(it);
	}

}

void BaseTargetList::delTargetByName(std::string name)
{
    auto targetList = getTargetList();
    if (targetList.empty()) return;

    auto it = std::find_if(targetList.begin(), targetList.end(), [&name](const ITarget* _it) {
        return _it->getName() == name;
    });

    if (it != targetList.end()) {
        executor->deleteRecordByField("hierarchyAddr",(*it)->getHierarchyAddr(), "targets");
        targetList.erase(it);
    }
}

void BaseTargetList::delTargetByAddr(std::string hierarchyAddr)
{
    auto targetList = getTargetList();
    if (targetList.empty()) return;

    auto it = std::find_if(targetList.begin(), targetList.end(), [&hierarchyAddr](const ITarget* _it) {
        return _it->getHierarchyAddr() == hierarchyAddr;
    });

    if (it != targetList.end()) {
        executor->deleteRecordByField("hierarchyAddr",(*it)->getHierarchyAddr(), "targets");
        targetList.erase(it);
    }
}


const std::vector<ITarget*>& BaseTargetList::getTargetList(bool isGetNew)
{
    if(!isListInitialized || isGetNew){
        std::string selectConditioin = "hierarchyAddr not LIKE '"
                + name +"\\_%\\_%' ESCAPE '\\' and hierarchyAddr LIKE '"+ name +"%'";
        auto targets = executor->select("targets", std::move(selectConditioin));

        qDeleteAll(targetList.begin(),targetList.end());
        targetList.clear();
        rootItem->getSubTargets().clear();
        for (auto && trg : targets) {
            targetList.push_back(ITarget::create(trg,rootItem));
        }
        fillRootSubTargets();
        isListInitialized = true;
    }
    return targetList;
}

void BaseTargetList::initList(std::string listName)
{
    name = listName;
    this->type = getlistSetting().at("type");
    targetList = getTargetList(true);
}

const list<ITarget*> BaseTargetList::getTargetsCreatedAfterDate(std::string datetime)
{
    auto allListTargets = getTargetList();
    list<ITarget*> afterTargets;
    std::copy_if(allListTargets.begin(),allListTargets.end(), std::back_inserter(afterTargets), [&datetime] (ITarget* trg){
        std::tm tm = {};
        std::stringstream ss(datetime);
        ss >> std::get_time(&tm, "%H:%M %d.%m.%Y");
        auto tp1 = std::chrono::system_clock::from_time_t(std::mktime(&tm));
        ss.str(trg->getCreateTime());
        ss >> std::get_time(&tm, "%H:%M %d.%m.%Y");
        auto tp2 = std::chrono::system_clock::from_time_t(std::mktime(&tm));
        return tp2 >= tp1;
    });
    return afterTargets;
}

const list<ITarget*> BaseTargetList::getTargetsCreatedBeforDate(std::string datetime)
{
    auto allListTargets = getTargetList();
    list<ITarget*> afterTargets;
    std::copy_if(allListTargets.begin(),allListTargets.end(), std::back_inserter(afterTargets), [&datetime] (ITarget* trg){
        std::tm tm = {};
        std::stringstream ss(datetime);
        ss >> std::get_time(&tm, "%H:%M %d.%m.%Y");
        auto tp1 = std::chrono::system_clock::from_time_t(std::mktime(&tm));
        ss.str(trg->getCreateTime());
        ss >> std::get_time(&tm, "%H:%M %d.%m.%Y");
        auto tp2 = std::chrono::system_clock::from_time_t(std::mktime(&tm));
        return tp2 <= tp1;
    });
    return afterTargets;
}

ITarget* BaseTargetList::getTargetById(int id)
{
    auto it = std::find_if(targetList.begin(), targetList.end(), [id](ITarget* _trg) {
		return _trg->getId() == id;
	});
    if (it == targetList.end()) return nullptr;

    return *it;
}

ITarget* BaseTargetList::getTargetByName(std::string name)
{
    auto it = std::find_if(targetList.begin(), targetList.end(), [name](ITarget* _trg) {
        return _trg->getName() == name;
    });
    if (it == targetList.end()) return nullptr;

    return *it;
}

ITarget* BaseTargetList::getTargetByAddr(std::string hierarchyAddr)
{
    auto it = std::find_if(targetList.begin(), targetList.end(), [hierarchyAddr](ITarget* _trg) {
        return _trg->getHierarchyAddr() == hierarchyAddr;
    });
    if (it == targetList.end()) return nullptr;

    return *it;
}



BaseTargetList::BaseTargetList(std::string listName/*="null_hollow_list"*/)
    :name{listName}, isListInitialized{false}
{
    executor.reset(new db::Executor {});
    rootItem = SimpleTarget::getHollowTarget();


    if(listName == "null_hollow_list"){
        type = "hollow";
        targetList = {};
    }
    else{
        this->type = getlistSetting().at("type");
        targetList = getTargetList();
        fillRootSubTargets();
    }
}

BaseTargetList::~BaseTargetList()
{
    //qDeleteAll(targetList);
    delete rootItem;
}
