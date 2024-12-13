#ifndef COMMONTARGET_H
#define COMMONTARGET_H

#include "Target.h"

class CommonTarget : public SimpleTarget
{
public:
    CommonTarget(const map<string, string> & trg, ITarget* parent = nullptr);
    CommonTarget() = default;
    virtual map<string, string> targetToMap() const override;
    const string& getStartDatetime() const {return startDatetime; }
    const string& getEndDatetime() const {return endDatetime; }
    //friend ITargetPtr ITarget::create(const map<string, string> & trg);

protected:
    string startDatetime;
    string endDatetime;

protected:
    virtual void initWithMap(const std::map<string, string> & trg) override;
};

#endif // COMMONTARGET_H
