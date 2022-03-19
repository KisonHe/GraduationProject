#pragma once
#include <ESPDash.h>
#include <observerPatten.h>
//#include "" get info about motor

class motorCardSet
{
private:
    /* data */
    Card* rpm = nullptr;
    Card* distance = nullptr;
    Card* distanceControl = nullptr;
    ESPDash* dashboard;
public:
    motorCardSet(ESPDash* dashboard);
    ~motorCardSet();
};


