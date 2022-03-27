#pragma once
#include <ESPDash.h>
#include <observerPatten.h>
#include "rm_can_motors.h"
//#include "" get info about motor

class motorCardSet
{
private:
    /* data */
    Card* rpm = nullptr;
    Card* distance = nullptr;
    Card* distanceControl = nullptr;
    ESPDash* dashboard;
    canMotors::motor* motorInstance = nullptr;
public:
    bool isPosCtl = false;
    void update();
    motorCardSet(ESPDash* dashboard, canMotors::motor* motor);
    ~motorCardSet();
};


