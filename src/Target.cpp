#include "Target.h"
#include <QDebug>

SimpleTarget::SimpleTarget(const map<string, string>& trg, ITarget* parent) : ITarget(parent)
{
	initWithMap(trg);
    targetType = "simple";
    getSubTargets();
}




void SimpleTarget::initWithMap(const std::map<string, string>& trg)
{
	if (!(trg.count("id") && trg.count("name") && trg.count("description") &&
        trg.count("hierarchyAddr") && trg.count("status") && trg.count("createTime"))){
		throw std::invalid_argument("Target init error.");
    }

	stringstream ss;
	ss << atoi(trg.at("id").c_str());
	ss >> id;
	name = trg.at("name");
	hierarchyAddr = trg.at("hierarchyAddr");
	status = trg.at("status");
	description = trg.at("description");
    createTime = trg.at("createTime");
    prior =  atoi(trg.at("prior").c_str());
}

void ITarget::updateTarget(const map<std::string, std::string> &trg)
{
    auto targetMap {targetToMap()};
    for(auto& [key,value] : trg){
        targetMap[key] = value;
    }
    initWithMap(targetMap);
    executor->update(atoi(targetMap.at("id").c_str()), targetMap, "targets");
}

map<string, string> SimpleTarget::targetToMap() const
{
    using sspair = std::pair<string, string>;
	map<string, string> resMap;
	std::stringstream ss;
	ss << getId();
    resMap.insert(sspair{"id", ss.str()});
    resMap.insert(sspair{"name", name});
    resMap.insert(sspair{"description", description});
    resMap.insert(sspair{"hierarchyAddr", hierarchyAddr});
    resMap.insert(sspair{"status", status});
    resMap.insert(sspair{"createTime", createTime});
	
    resMap.insert(sspair{"targetType", "simple"});

    ss.str("");
    ss << prior;
    resMap.insert(std::pair<string, string>("prior", ss.str()));

    return resMap;
}

ITarget* SimpleTarget::getHollowTarget()
{
    using sspair = std::pair<string, string>;
    map<string, string> resMap;
    std::stringstream ss;
//    (!(trg.count("id") && trg.count("name") && trg.count("description") &&
//            trg.count("hierarchyAddr") && trg.count("status") && trg.count("createTime")))
    resMap.insert(sspair{"id", "-1"});
    resMap.insert(sspair{"hierarchyAddr", ""});
    resMap.insert(sspair{"name", ""});
    resMap.insert(sspair{"description", ""});
    resMap.insert(sspair{"status", ""});
    resMap.insert(sspair{"createTime", ""});

    resMap.insert(sspair{"targetType", "simple"});

    resMap.insert(std::pair<string, string>("prior", ""));

    return  ITarget::create(resMap);
}


std::vector<ITarget*>& ITarget::getSubTargets()
{
    if(!isSubTrgInitialized){
        if(hierarchyAddr.empty()){
            isSubTrgInitialized = true;
            return subTargets;
        }
        std::string subTargetCondition = "hierarchyAddr like '"
                + hierarchyAddr + "\\_%' escape '\\'  and hierarchyAddr not like '"
                + hierarchyAddr + "\\_%\\_%' escape '\\'";
        auto subTargetsMaps = executor->select("targets",subTargetCondition);

        for (auto & trgMap : subTargetsMaps) {
            subTargets.push_back(ITarget::create(trgMap,this));
        }
        isSubTrgInitialized = true;
    }   
    return subTargets;
}

ITarget* ITarget::getSubTargetsByName(string name)
{
    //auto subTargetsList = getSubTargets();
    auto it = std::find_if(subTargets.begin(), subTargets.end(), [name](const ITarget* _it) {
        return _it->getName() == name;
    });

    if (it != subTargets.end()) {
        return *it;
    }
    return nullptr;
}

ITarget* ITarget::getSubTargetsByRow(int row)
{
    if(row >= static_cast<int>(subTargets.size()) || row < 0){
        qWarning() << "ITarget::getSubTargetsByRow  incorrect row";
        return nullptr;
    }
    return  subTargets.at(row);
}

void ITarget::addSubTarget(const map<string,string> & trg)
{
    //auto lastAddedTargets = executor->select("targets","TRUE",1,true);
    //auto lastAddedTarget = lastAddedTargets.empty() ? db::Record{} : lastAddedTargets.front();
    //auto lastAddedTarget = executor->getLastTarget();
    db::Index lastIndex = executor->lastIndex("targets");
    int nextId = lastIndex ? lastIndex + 1 : 0;

	stringstream ss;
	ss << nextId;
	map<string, string> subTrg = trg;
	subTrg["hierarchyAddr"] = hierarchyAddr + "_" + ss.str();

    {
        auto nowTime = std::chrono::system_clock::now();
        auto tt (std::chrono::system_clock::to_time_t(nowTime));
        auto loct (std::localtime(&tt));
        stringstream ss;
        ss << std::put_time(loct,"%H:%M %d.%m.%Y");
        subTrg["createTime"] = ss.str();
    }
	
	size_t currentDeep = std::count(subTrg["hierarchyAddr"].begin(), subTrg["hierarchyAddr"].end(), '_');
	if (currentDeep >= ITarget::MaxSubTargetDeep) { 
		std::cout << "ITarget::MaxSubTargetDeep =" << ITarget::MaxSubTargetDeep << " tagre has max deep.\n";
		return;
	}

	subTrg["id"] = ss.str();
    //subTrg.at("targetType") = "simple";
    ITarget* newSubTarget = ITarget::create(subTrg, this);
	subTargets.push_back(newSubTarget);

    executor->insert(subTrg,"targets");
}

void ITarget::delSubTarget(int id)
{
	auto subTargets = getSubTargets();
	
	if (subTargets.empty()) return;

    auto it = std::find_if(subTargets.begin(), subTargets.end(), [id](const ITarget* _it) {
		return _it->getId() == id;
	});

	if (it != subTargets.end()) {
        executor->deleteRecordByField("hierarchyAddr",(*it)->getHierarchyAddr(), "targets");
		subTargets.erase(it);
	}

}

bool ITarget::delSubTargetByRow(size_t row)
{
    if(row > subTargets.size()){
        qWarning("Attemp to delete not exist row");
        return false;
    }
    bool res {executor->deleteRecordByField("hierarchyAddr",subTargets.at(row)->getHierarchyAddr(),"targets")};
    delete subTargets.at(row);
    subTargets.erase(subTargets.begin()+row);
    return res;
}

void ITarget::delSubTargetByName(std::string name)
{
    auto subTargets = getSubTargets();

    if (subTargets.empty()) return;

    auto it = std::find_if(subTargets.begin(), subTargets.end(), [name](const ITarget* _it) {
        return _it->getName() == name;
    });

    if (it != subTargets.end()) {
        executor->deleteRecordByField("hierarchyAddr",(*it)->getHierarchyAddr(), "targets");
        subTargets.erase(it);
    }
}

ITarget::ITarget(ITarget* parent) : parent {parent}, isSubTrgInitialized{false}
{
    executor.reset(new db::Executor{});
}

ITarget::~ITarget()
{
    qDeleteAll(subTargets.begin(),subTargets.end());
}

ITarget* ITarget::getParent() const
{
    return parent;
}

void ITarget::rewriteTarget()
{
    auto targetMap = targetToMap();
    executor->update(atoi(targetMap.at("id").c_str()), targetMap, "targets");
}

void ITarget::setStatus(std::string stat)
{
     status = (stat == "complite" || stat == "in_process") ? stat : status;
     auto targetMap = targetToMap();
     targetMap["status"] = status;
     executor->update(atoi(targetMap.at("id").c_str()), targetMap, "targets");
}

void ITarget::setStatus(bool stat)
{
    status = stat ? "complite" : "in_process";
    auto targetMap = targetToMap();
    targetMap["status"] = status;
    executor->update(atoi(targetMap.at("id").c_str()), targetMap, "targets");
}



ostream & operator<<(ostream & stream,const ITarget & trg)
{
	auto target = trg.targetToMap();
	for (auto &[key, value] : target) {
		stream << value << "|";
	}
	stream << "\n";
	return stream;
}
