#ifndef _GROUP_H_
#define _GROUP_H_

#include "Node.h"
#include "Matrix4.h"
#include "Geode.h"

#include <vector>

using namespace std;

class Group : public Node {
	public:
		vector<Node*> child;
		int nChild;

		Group();

		bool hasChild();
		int getNumChild();
		void addChild(Node*);
		bool removeChild(string);

		virtual void draw(Matrix4) = 0;
};

#endif