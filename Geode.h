#ifndef _GEODE_H_
#define _GEODE_H_

#include "Node.h"
#include "Matrix4.h"

using namespace std;

class Geode : public Node {
	public:
		Matrix4 modelview;

		Geode();
		Geode(Matrix4);

		void setModelView(Matrix4);
		Matrix4 getModelView();
		virtual void render(Matrix4) = 0;
		void draw(Matrix4);
		void draw2(Matrix4);
};

#endif