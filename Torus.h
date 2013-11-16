#ifndef _TORUS_H_
#define _TORUS_H_

#include "Geode.h"
#include "Matrix4.h"

using namespace std;

class Torus : public Geode {
	public:
		void draw(Matrix4);
};

#endif