#pragma once
#include <ESPDash.h>
#include <pid.h>
#include <observerPatten.h>
class pidCardSet:public Observer
{
    
private:
    /* data */
    Card* pkp=nullptr;
    Card* pki=nullptr;
    Card* pkd=nullptr;
    ESPDash* dashboard;
    pid* pid_instance;
public:
    virtual void update(Subject *subject);
    pidCardSet(ESPDash* dashboard,pid* pid_instance);
    ~pidCardSet();
};

