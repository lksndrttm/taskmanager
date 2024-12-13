#pragma once
#include <string>
#include <map>
#include <list>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <memory>
#include <chrono>
#include <iomanip>
#include "executor.h"
#include <vector>


using std::string;
using std::stringstream;
using std::list;
using std::map;
using std::ostream;
using std::istream;
using std::shared_ptr; 

class ITarget;
using ITargetPtr = shared_ptr<ITarget>;
//using ITargetPtr = ITarget * ;
class ITarget 
{
protected:
    //virtual ~ITarget() = default;
    ITarget(ITarget* parent = nullptr);
    int id;
    string hierarchyAddr, name, status, createTime, targetType;//status {"complite" || "in_process" || [...]};
    ITarget* parent;
    std::vector<ITarget*> subTargets;
    std::unique_ptr<db::Executor> executor;
    bool isSubTrgInitialized;
    virtual void initWithMap(const std::map<string, string> & trg) = 0;
public:
    virtual ~ITarget();
    //virtual ~ITarget() = 0;
    virtual std::map<string, string> targetToMap() const = 0;
    ITarget* getParent() const;
    std::vector<ITarget*>& getSubTargets();
    ITarget* getSubTargetsByName(string name);
    ITarget* getSubTargetsByRow(int row);
    void addSubTarget(const map<string, string> & trg);
    void delSubTarget(int id);
    bool delSubTargetByRow(size_t row);
    void delSubTargetByName(string name);
    //virtual bool modifySubTarget(int id, std::map<string, string> & modifications) = 0; //map<field, value>; return fail or complite
    void rewriteTarget();
    void updateTarget(const map<string, string> & trg);
    const string& getTargetType() const { return targetType; }
    const string& getHierarchyAddr() const { return hierarchyAddr; }
    int getId() const { return id; }
    const string& getCreateTime() const {return createTime;}
    virtual void setStatus(string stat);
    virtual void setStatus(bool stat);
    const string& getName() const { return name; }
    static ITarget* create(const map<string,string> & trg, ITarget* parent = nullptr);

    static const size_t MaxSubTargetDeep = 10; //max deep of sub targets tree

    friend ostream& operator<< (ostream& stream,const ITarget & trg);

};


class SimpleTarget :public ITarget
{	
protected:
    string description;//status {"complite" || "in_process" || [...]}
    size_t prior;

	virtual void initWithMap(const std::map<string, string> & trg) override;
public:
    const string& getDescription() const {return description;}
    size_t getPrior() const {return prior;}
	virtual map<string, string> targetToMap() const override;        

	//virtual bool modifySubTarget(int id, std::map<string, string> & modifications) override; //map<field, value>; return fail or complite
	friend ostream& operator<< (ostream& stream,const ITarget & trg);

    //friend ITarget* ITarget::create(const map<string, string> & trg);

    static ITarget* getHollowTarget();
    SimpleTarget(const map<string, string> & trg, ITarget* parent = nullptr);
	SimpleTarget() = default;
};

