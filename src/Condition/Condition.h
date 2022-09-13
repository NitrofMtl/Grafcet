#ifndef CONDITION_H
#define CONDITION_H

#include <Arduino.h>

class Condition
{
	protected:
	friend class Transition;
	friend class Branch;
	Condition() = default;
	virtual ~Condition() = default;
	virtual bool validate() = 0;
};

template<class BoolCallable>
class ConditionDerived : public Condition
{
	private:
	friend class Transition;
	friend class ConvergeTransition;
	ConditionDerived(BoolCallable callable) : condition(callable) {}
	~ConditionDerived() = default;
	bool validate() override { return condition(); }
	BoolCallable condition;
};

#endif // CONDITION_H