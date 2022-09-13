#include <Grafcet.h>


Grafcet::Grafcet() : initial(Step([](){;})) , activeStep(&initial) {
}

void Grafcet::setInitial(Transition& initialReceptivity){//remove just keep transition
	initial.setTransition(initialReceptivity);
}

void Grafcet::handler() {
	if (!activeStep) {
		P_ERROR("NO ACTIVE STEP !!!");
		return;
	}
	activeStep = activeStep->handleStep();
}

Step& Grafcet::backToInitial()
{ 
	return initial; 
}

const bool TRUE()
{
	return true;
}

void WAIT() {}