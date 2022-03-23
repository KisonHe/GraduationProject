#include "motorCardSet.h"
#include <functional>

// void callBack(bool value){
//     Serial.println("cb called");
//     return;
// }

// std::function<void(bool)> distanceControlcbf = callBack;

motorCardSet::motorCardSet(ESPDash* dashboard, canMotors::motor* motor):dashboard(dashboard),motorInstance(motor)
{
    distanceControl = new Card(dashboard, BUTTON_CARD, "路程控制");
    rpm = new Card(dashboard, SLIDER_CARD, "转速","RPM",-6000,6000);
    distance = new Card(dashboard, SLIDER_CARD, "路程","角度",-18000,18000);
    
    distanceControl->attachCallback([this](bool value){
        if (value){// spd to pos control
            this->motorInstance->Angle_Set(this->motorInstance->GetSoftAngle());//设置当前值为目标值防止乱转
            this->distance->update(this->motorInstance->GetSoftAngle());
            this->isPosCtl = true;
        }
        else{   // pos to spd control
            this->motorInstance->Speed_Set(0);
            this->rpm->update(0);
            this->isPosCtl = false;
        }
        this->distanceControl->update(value);
    });
    // distanceControl->attachCallback(distanceControlcbf);
    rpm->attachCallback([this](int value){
        if (!this->isPosCtl){
            this->rpm->update(value);
            this->motorInstance->Speed_Set(value);
        }
    });
    distance->attachCallback([this](int value){
        if (this->isPosCtl){
            this->distance->update(value);
            this->motorInstance->Angle_Set(value);
        }
    });
    
}

void motorCardSet::update(){
    if (isPosCtl){
        rpm->update(motorInstance->GetRealSpeed());
    }else{
        distance->update(motorInstance->GetSoftAngle());
    }
    
    
}


motorCardSet::~motorCardSet()
{
}