#include <Grafcet.h>

Grafcet::Grafcet() : activeStep(&initial) {
}

void Grafcet::setInitial(Receptivity &_receptivity){//remove just keep transition
	initial.receptivity = &_receptivity;
}

void Grafcet::handler() {
	if (activeStep->action) activeStep->doAction();
	if (!activeStep->receptivity) return;
	if (!activeStep->receptivity->validate()) return;
	if (activeStep->receptivity->nextStep) { //if remove this, grafcet lock in non complete graf... maybe better
		activeStep->reset();
		activeStep = activeStep->receptivity->nextStep;
	}
}
