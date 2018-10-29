#include "Fork.h"

Fork::Fork() : head(new ForkNode) {}

Fork::~Fork() {
	ForkNode *node = head;
	while (node) {
		ForkNode *temp = node;
		node = node->nextNode;
		delete temp;
	}
}

void Fork::add() {
	ForkNode *node = head;
	while ( node->nextNode ) {
		node = node->nextNode;
	}
	node->nextNode = new ForkNode;
}

Receptivity& Fork::fork(int num) {
	ForkNode *node = head;
	for (int i = 0; i < num; i++ ) {
		if (node) node = node->nextNode; //safe garde
	}
	return *node;
}

bool  Fork::validate() {
	ForkNode *node = head;
	while ( node ) {
		if ( !node->transition ) continue;
		if ( node->transition() ) {
			nextStep = node->nextStep;
			return true;
		}
		node = node->nextNode;
	}
	return false;
}
