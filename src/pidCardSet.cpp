#include "pidCardSet.h"
#include <Arduino.h>

pidCardSet::pidCardSet(ESPDash* dashboard,pid* pid_instance):dashboard(dashboard),pid_instance(pid_instance)
{
    pkp = new Card(dashboard, SLIDER_CARD, "Kp*100","",-1000,1000);
    pki = new Card(dashboard, SLIDER_CARD, "Ki*100","",-100,100);
    pkd = new Card(dashboard, SLIDER_CARD, "Kd*100","",-100,100);

    pkp->attachCallback([pid_instance](int value){
        pid_instance->setArgs(pid::kP,value/100.0);
    });
    pki->attachCallback([pid_instance](int value){
        pid_instance->setArgs(pid::kI,value/100.0);
    });
    pkd->attachCallback([pid_instance](int value){
        pid_instance->setArgs(pid::kD,value/100.0);
    });
}

pidCardSet::~pidCardSet()
{
}

void pidCardSet::update(Subject *subject){
    // Serial.println("Update Called");
    pkp->update((int)(100*pid_instance->getArgs(pid::kP)));
    pki->update((int)(100*pid_instance->getArgs(pid::kI)));
    pkd->update((int)(100*pid_instance->getArgs(pid::kD)));

}