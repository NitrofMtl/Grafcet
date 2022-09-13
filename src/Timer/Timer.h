#ifndef GRAFCET_TIMER_H
#define GRAFCET_TIMER_H

#include <Arduino.h>
#include "GrafcetBase\GrafcetBase.h"
#include "TimeOut.h"


class GrafcetTimer : public Receptivity
{
	public:
	GrafcetTimer() : _delay(0)
	{}
	void set(Step& upstream, u_int32_t d);
	void reset();
	private:
	bool validate() override;
	uint32_t _delay;
	int validation = 0;
	TimeOut timer;
	Step* previousStep;
};

#endif //GRAFCET_TIMER_H