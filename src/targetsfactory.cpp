#include "Target.h"
#include "targets/commontarget.h"
#include "targets/scheduletarget.h"


ITarget* ITarget::create(const map<string, string> & trg, ITarget* parent)
{
    if (trg.at("targetType") == "simple") {
        ITarget* pnewTrg = new SimpleTarget(trg, parent);
        return pnewTrg;
    }
    else if(trg.at("targetType") == "common"){
        ITarget* pnewTrg = new CommonTarget(trg,parent);
        return pnewTrg;
    }
    else if(trg.at("targetType") == "schedule"){
        ITarget* pnewTrg = new ScheduleTarget(trg,parent);
        return pnewTrg;
    }
    throw std::invalid_argument("no has target with type " + trg.at("targetType"));
}
