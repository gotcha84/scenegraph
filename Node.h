#ifndef _NODE_H_
#define _NODE_H_

#include "Matrix4.h"
#include <limits.h>
#include <string>
#include <math.h>

using namespace std;

class Node {
	public:
		string name;
		Node *parent;
		int boxx;
		int boxy;
		int boxz;
		int x_min;
		int x_max;
		int y_min;
		int y_max;
		int z_min;
		int z_max;
		float boxr;

		Node();
		Node(string);
		string getName();

		bool hasParent();
		Node* getParent();
		void setParent(Node*);

		void setBoundingBox(int, int, int, int, int, int);
		int getBoundingBoxX();
		int getBoundingBoxY();
		float getBoundingBoxR();
		virtual void draw2(Matrix4 m) = 0;
		virtual void draw(Matrix4 m) = 0;
};

#endif