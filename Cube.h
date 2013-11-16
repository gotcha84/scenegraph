#ifndef _CUBE_H_
#define _CUBE_H_

#include "Geode.h"
#include "Matrix4.h"

using namespace std;

class Cube : public Geode {
	public:
		void render(Matrix4);
		//void draw2(Matrix4);
};

#endif