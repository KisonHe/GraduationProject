#include "motorCardSet.h"

motorCardSet::motorCardSet(ESPDash* dashboard):dashboard(dashboard)
{
    distanceControl = new Card(dashboard, BUTTON_CARD, "路程控制");
    rpm = new Card(dashboard, SLIDER_CARD, "转速","RPM",-18000,18000);
    distance = new Card(dashboard, SLIDER_CARD, "路程","转",-18000,18000);
    
    distanceControl->attachCallback([this](bool value){
        this->distanceControl->update(value);
    });
    rpm->attachCallback([this](int value){
        this->rpm->update(value);
    });
    distance->attachCallback([this](int value){
        this->distance->update(value);
    });
    
}

motorCardSet::~motorCardSet()
{
}