#include "pidCardSet.h"
#include <Arduino.h>

pidCardSet::pidCardSet(ESPDash* dashboard,pid* pid_instance):dashboard(dashboard),pid_instance(pid_instance)
{
    pkp = new Card(dashboard, SLIDER_CARD, "Kp*100","",-1000,1000);
    pki = new Card(dashboard, SLIDER_CARD, "Ki*100","",-100,100);
    pkd = new Card(dashboard, SLIDER_CARD, "Kd*100","",-100,100);

    pkp->attachCallback([pid_instance](int value){
        pid_instance->setArgs(PIDArgType::kP,value/100.0);
    });
    pki->attachCallback([pid_instance](int value){
        pid_instance->setArgs(PIDArgType::kI,value/100.0);
    });
    pkd->attachCallback([pid_instance](int value){
        pid_instance->setArgs(PIDArgType::kD,value/100.0);
    });
}

pidCardSet::~pidCardSet()
{
}

void pidCardSet::update(Subject *subject){
    pkp->update((int)(100*pid_instance->getArgs(PIDArgType::kP)));
    pki->update((int)(100*pid_instance->getArgs(PIDArgType::kI)));
    pkd->update((int)(100*pid_instance->getArgs(PIDArgType::kD)));

}