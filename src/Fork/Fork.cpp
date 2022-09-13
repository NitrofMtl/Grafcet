#include "Fork.h"

Fork::~Fork()
{
	Fork* f = this->next;
	while (f) {
		Fork* n = f->next;
		delete f;
		f = n;
	}
}

void Fork::add()
{
	Fork* addedFork = this;
	while (addedFork->next) {
		addedFork = addedFork->next;
	}
	addedFork->next = new Fork();
}

Fork& Fork::operator[](unsigned int index)
{
	Fork* ptr = this;
	while (index && ptr->next) {
		ptr = ptr->next;
		index--;
	}
	return *ptr;
}

bool Fork::validate()
{ 
	validatedNextStep = nullptr;
	Fork* ptr = this;
	while(ptr) {
		if( ptr->receptivity->validate() ) {
			validatedNextStep = ptr->receptivity->nextStep;
			return true;
		}
		ptr = ptr->next;
	}
	return false;
}

Step* Fork::getNextStep()
{
	return validatedNextStep;
}

void Fork::operator=(Receptivity& recep)
{
	receptivity = &recep;
}