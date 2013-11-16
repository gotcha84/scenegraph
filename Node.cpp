#include "Node.h"
#include <iostream>
using namespace std;

Node::Node() {
	name = "";
	x_min = INT_MAX;
	x_max = INT_MIN;
	y_min = INT_MAX;
	y_max = INT_MIN;
	z_min = INT_MAX;
	z_max = INT_MIN;
	parent = NULL;
}
Node::Node(string n) {
	name = n;
}

string Node::getName() {
	return name;
}

bool Node::hasParent() {
	if (parent != nullptr) {
		return true;
	}
	return false;
}

Node* Node::getParent() {
	return parent;
}

void Node::setParent(Node *p) {
	parent = p;
}

void Node::setBoundingBox(int x_min, int x_max, int y_min, int y_max, int z_min, int z_max) {
	this->boxx = (x_min + x_max)/2;
	this->boxy = (y_min + y_max)/2;
	this->boxz = (z_min + z_max)/2;

	int temp = max(boxx, boxy);
	temp = max(temp, boxz);
	this->boxr = 0.5*temp;
}

int Node::getBoundingBoxX() {
	return boxx;
}

int Node::getBoundingBoxY() {
	return boxy;
}

float Node::getBoundingBoxR() {
	return boxr;
}
