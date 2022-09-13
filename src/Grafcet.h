#ifndef Grafcet_h
#define Grafcet_h

#include <Arduino.h>

#include "GrafcetBase\GrafcetBase.h"
#include "Fork\Fork.h"
#include "Branch\Branch.h"
#include "Timer\Timer.h"
#include "Counter\Counter.h"
#include "SyncMemory\SyncMerory.h"

#ifndef GRAFCET_DEPLOYD
#define GRAFCET_ERROR "[[GRAFCET ERROR]] "
#define P_ERROR(msg) Serial.println(GRAFCET_ERROR msg)
#else//GRAFCET_DEPLOYD
#define P_ERROR(msg)
#endif//GRAFCET_DEPLOYD

class Grafcet {
public:
	Grafcet(); //reset variable, add Transition to initial, new initial obj
	void handler();
	void setInitial(Transition& initialReceptivity); //step one is to set initial transition
	Step& backToInitial();// { return initial; };
private:
	Step initial;//initial will have a blank action, 
	Step *activeStep;
};

const bool TRUE();

void WAIT();

#endif //Grafcet_h


//add template transition and step
//restructure file system

//add set rise and drop
//change time for simple timer

//add step wait

//add active step transition synchronisation

//add step activeTime counter