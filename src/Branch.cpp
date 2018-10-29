#include "Branch.h"

Branch::Branch() : head(new BranchNode) {
	receptivity = this;
}

Branch::~Branch() {
	BranchNode *node = head;
	while (node) {
		BranchNode *temp = node;
		node = node->nextBranch;
		delete temp->initial;
		delete temp;
	}
}

void Branch::add() {
	BranchNode *temp = head;
	while (temp->nextBranch) {
		temp=temp->nextBranch;
	}
	temp->nextBranch = new BranchNode;
}

bool Branch::validate() {
	BranchNode *branch = head;
	bool completed = true;
	while ( branch ) {
		if ( !handle(branch) ) completed = false; //handle return false if branch is not completed
		branch = branch->nextBranch;
	}
	if ( completed ) {
		resetBranch();
		//Serial.println("all branch completed");
	}
	return completed;
}

bool Branch::handle(BranchNode* node) {
	if ( node->completed ) return true; //do not execute a branch that have benn executed
	if (!node->activeStep) node->activeStep = node->initial; //set brach to initial at start
	if (node->activeStep->action) {
		node->activeStep->doAction();
	}
	if (!node->activeStep->receptivity) return false;
	if (!node->activeStep->receptivity->validate()) return false;
	node->activeStep->reset();
	node->activeStep = node->activeStep->receptivity->nextStep;
	if ( !node->activeStep ) node->completed = true; //mark branch as completed when no more step
	return node->completed;
}

Branch::BranchNode& Branch::branch(int _node) {
	BranchNode *node = head;
	for (int i = 0; i < _node; i++ ) {
		if (node) node = node->nextBranch; //safe gard
	}
	return *node;
}

void Branch::resetBranch() {
	BranchNode* node = head;
	while ( node ) {
		node->completed = false;
		node = node->nextBranch;
	}
}