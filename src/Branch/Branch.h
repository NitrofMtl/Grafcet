#ifndef BRANCH_H
#define BRANCH_H

#include <Arduino.h>
#include "GrafcetBase\GrafcetBase.h"

class ConvergeTransition : public Receptivity
{
    public:
    template<class BoolCallable>
    ConvergeTransition(BoolCallable callable) : condition(new ConditionDerived<BoolCallable>(callable))
    {}
    private:
    //end of branch have to wait all branch
    bool validate() override { return false; }
    Condition* getConditionPtr() { return condition; }
    Condition* condition;
};

class Branch : public Step
{
    public:
    Branch() : Step([](){}), next(nullptr), first(nullptr)
    {}
    ~Branch();
    void add();
    Branch& operator[](unsigned int index);
    void setFirstStep(Step& firstStep);
    void operator=(Step& step);
    void setConvergeTransition(ConvergeTransition& converge);
    Receptivity& converge();
    private:
    void doAction() override;
    bool validate() override;
    void deactivate() override;
    Branch* next;
    Step* activeStep;
    Step* first;
};

#endif //BRANCH_H















/*


class Branch : public StepObj, public Receptivity {
private:

	class BranchNode {
	public:
		BranchNode() : nextBranch(NULL), initial(new Step), activeStep(NULL), completed(false) {};
		BranchNode *nextBranch;
		StepObj *initial;
		StepObj *activeStep;
		bool completed;
		BranchNode& operator=(void (*_action)()) { initial->action = _action; return *this; };
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

*/