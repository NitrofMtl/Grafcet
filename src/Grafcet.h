#ifndef Grafcet_h
#define Grafcet_h

#include <Arduino.h>
#include <Fork.h>
#include <GrafcetTimer.h>
#include <TimeOut.h>


typedef bool transition_t;

class StepObj;


class Receptivity {
public:
	Receptivity() : transition(NULL), nextStep(NULL) {};
	void setNextStep(StepObj &_nextStep) { nextStep = &_nextStep; };
	Receptivity operator=(transition_t (*t)()) { this->transition = t; };
private:
	friend class Grafcet;
	friend class Fork;
	friend class Branch;	
	virtual bool validate() { if (transition) return transition(); };
	transition_t (*transition)();
protected:
	StepObj *nextStep;	
};


class StepObj {
public:
	StepObj() : receptivity(NULL), action(NULL) {};
	Receptivity *receptivity;
private:
	friend class Grafcet;
	friend class Branch;
	virtual void doAction() {};
	virtual void reset() {};
protected:
	void (*action)(); // action run on step activation
};


class Step : public StepObj {
public:
	Step() : ran(false) {};	
	Step operator=(void (*_action)()) { this->action = _action; };
private:	
	void doAction() { if(!ran) {ran = true; action();} };
	void reset() { ran = false; };
	bool ran;
};

class StepContinuous : public StepObj {
public:
	StepContinuous operator=(void (*_action)()) { this->action = _action; };
private:	
	void doAction() { action(); };
};


class Grafcet {
public:
	Grafcet(); //reset variable, add Transition to initial, new initial obj
	void handler();
	void setInitial(Receptivity &_receptivity); //step one is to set initial transition
	StepObj& backToInitial() { return initial; };
private:
	StepObj initial; //initial will have no action, 
	StepObj *activeStep;
};


class Fork : public Receptivity {
public:
	Fork();
	~Fork();
	void add();
	Receptivity& fork(int num); 
	bool validate();
private:

	class ForkNode : public Receptivity {
	public:
		Receptivity& operator=(transition_t (*t)()) { this->transition = t; };
	private:
		ForkNode() : nextNode(NULL) {};
		ForkNode * nextNode;
		friend class Fork;
		int node = 0;
	};

	ForkNode *head;
};


class Branch : public StepObj, public Receptivity {
private:

	class BranchNode {
	public:
		BranchNode() : nextBranch(NULL), initial(new Step), activeStep(NULL), completed(false) {};
		BranchNode *nextBranch;
		StepObj *initial;
		StepObj *activeStep;
		bool completed;
		BranchNode& operator=(void (*_action)()) { initial->action = _action; };
	};

	BranchNode* head;
	void setActiveBranch();
	bool handle(BranchNode* node);
	void resetBranch();
	bool validate();
public:
	Branch();
	~Branch();
	void add();
	void converge(StepObj& step) { receptivity->nextStep = &step; };	
	BranchNode& branch(int _node);
};


class TON : public StepObj, public Receptivity, private TimeOut {
public:
	TON() : _timeOut(false) { receptivity = this; };
	void delay(uint32_t time) { _delay = time; };
	TON operator=(void (*_action)()) { this->action = _action; };
private:
	bool validate() { if (!_timeOut) return _timeOut; _timeOut = false; ran = false; return true; };
	transition_t _timeOut;
	void doAction() { if(!ran) {ran = true; action(); timeOut(_delay, NULL); } };
	unsigned long _delay;
	void TO_callbackCaller() {  _timeOut = true; };
	bool ran;
};

class TOFF : public StepObj, public Receptivity, private TimeOut {
public:
	TOFF() : _timeOut(false) { receptivity = this; };
	void delay(uint32_t time) { _delay = time; };
	TON operator=(void (*_action)()) { this->action = _action; };
private:
	bool validate() { if (!_timeOut) return _timeOut; _timeOut = false; ran = false; return true; };
	transition_t _timeOut;
	void doAction() { if(!ran) {ran = true; timeOut(_delay, NULL); } };
	unsigned long _delay;
	void TO_callbackCaller() {  action(); _timeOut = true; };
	bool ran;
};

#endif




