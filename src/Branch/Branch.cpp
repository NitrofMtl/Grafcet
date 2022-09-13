#include "Branch.h"


Branch::~Branch()
{
	Branch* b = this->next;
	while (b) {
		Branch* n = b->next;
		if ( b->first ) delete b->first;
		delete b;
		b = n;
	}
}

Branch& Branch::operator[](unsigned int index)
{
	Branch* b = this;
		while(index && b) {
			b = b->next;
			index--;
		}
		return *b;
}

void Branch::add()
{
	Branch* addedBranch = this;
	while ( addedBranch->next ) {
		addedBranch = addedBranch->next;
	}
	addedBranch->next = new Branch();
}

void Branch::setFirstStep(Step& firstStep)
{
	first = &firstStep;
	activeStep = first;
}

void Branch::operator=(Step& step)
{
	setFirstStep(step);
}

void Branch::setConvergeTransition(ConvergeTransition& converge)
{
	receptivity = &converge;
}

Receptivity& Branch::converge()
{//set the same transition for each last step
	return *receptivity;
}

void Branch::doAction()
{   
	Branch* b = this;
	while ( b ) {
		b->activeStep = b->activeStep->handleStep();
		b = b->next;
	}
}

bool Branch::validate()
{//check for branch transition, compare if each branch step have converge transition as active
	Branch* b = this;
	while (b) {
		if ( b->activeStep->receptivity != receptivity ) return false;
		b = b->next;
	}
	if ( !receptivity->getConditionPtr()->validate() ) return false;
	return true;
}

void Branch::deactivate()
{//deactivate all step and reset each branch to first
	Branch* b = this;
	while (b) {
		b->activeStep->deactivate();
		b->activeStep->state = false;
		b->activeStep = b->first;
		b = b->next;
	}
}

