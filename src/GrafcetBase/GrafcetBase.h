#ifndef GRAFCET_BASE
#define GRAFCET_BASE

#include <Arduino.h>
#include "Condition\Condition.h"
#include "Action\Action.h"

class Step;

class Receptivity
{
	public:
	virtual ~Receptivity() {}
	virtual void setNextStep(Step& next) { nextStep = &next; }
	protected:
	virtual bool validate() = 0;
	virtual Step* getNextStep() { return nextStep; }
	virtual Condition* getConditionPtr() { return nullptr; } //use only by ConvergeTransition
    Step* nextStep;
	private:
	friend class Grafcet;
	friend class Step;
	friend class Fork;
	friend class Branch;
};

class Transition : public Receptivity
{
    public:
	template<class BoolCallable>  
    Transition(BoolCallable callable) :
	condition(new ConditionDerived<BoolCallable>(callable))
	{}
	~Transition() { if(condition) delete condition; }
	protected:
    bool validate() { return condition->validate(); }
	Step* getNextStep() { return nextStep; }
    Condition* condition;
};

class Step
{
    public:
	template<class Callable>
	Step(Callable callable) :
	state(false), action(new ActionDerived<Callable>(callable)),
	receptivity(nullptr), activeStamp(0)
	{}
	virtual ~Step() { if (action) delete action; }
	void setTransition(Receptivity& next) { receptivity = &next; }
	bool isActive() { return state; }
	unsigned long activeTime()
	{
		if ( !activeStamp ) return 0;
		return millis() - activeStamp;
	}

    private:
	friend class Grafcet;
	friend class Branch;
	Step* handleStep() {		
		doAction();
		state = true;
		if ( !activeStamp ) activeStamp = millis();
#ifndef GRAFCET_DEPLOYD
		if ( !receptivity ) {
			Serial.println("[[GRAFCETBASE ERROR]] No transition !!!");
			return this;
		}
#endif //GRAFCET_DEPLOYD
		if ( !validate() ) return this;
#ifndef GRAFCET_DEPLOYD
		if (!receptivity->getNextStep()) {
			Serial.println("[[GRAFCETBASE ERROR]] Transition without next step: non-close Grafcet !!!");
			return this;
		}
#endif //GRAFCET_DEPLOYD		
		state = false;
		activeStamp = 0;
		deactivate();
		return receptivity->getNextStep();
	}
	virtual void doAction() { if(!action) return; action->doAction(); }
	virtual bool validate() { return receptivity->validate(); }
	virtual void deactivate() { }

	protected:
	bool state;
	Action* action;
	Receptivity* receptivity;
	unsigned long activeStamp;
};

class StepRising : public Step
{
	public:
	template<class Callable>
	StepRising(Callable callable) : Step(callable)
	{}
	private:
	void doAction() { if(!action || state) return; action->doAction(); }
};

class StepFalling : public Step
{
	public:
	template<class Callable>
	StepFalling(Callable callable) : Step(callable)
	{}
	private:
	void doAction() {}
	void deactivate() { if(!action) return; action->doAction(); }
};

class StepOnChange : public Step
{
	public:
	template<class Callable, class CallableOff>
	StepOnChange(Callable callable, CallableOff deAct) :
	Step(callable), deActivateAction(new ActionDerived<CallableOff>(deAct))
	{}
	private:
	void doAction() { if(state || !action) return; action->doAction(); }
	void deactivate() { if(!deActivateAction) return; deActivateAction->doAction(); }
	Action* deActivateAction;
};

#endif //GRAFCET_BASE

