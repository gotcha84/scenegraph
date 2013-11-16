#include "Group.h"

#include <vector>
#include <iostream>

using namespace std;

Group::Group() {
	child.clear();
	nChild = 0;
}

bool Group::hasChild() {
	return (nChild > 0) ? true : false;
}

void Group::addChild(Node *n) {
	child.push_back(n);
	n->setParent(this);
	//cout << n->getParent() << '\n';
	nChild++;
}

bool Group::removeChild(string n) {
	for (vector<Node*>::iterator it=child.begin(); it!=child.end(); it++) {
		if (n == (*it)->name) {
			child.erase(it);
			nChild--;
			return true;
		}
	}
	return false;
}

int Group::getNumChild() {
	return nChild;
}