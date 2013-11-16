#ifndef _TETRAHEDRON_H_
#define _TETRAHEDRON_H_

#include "Geode.h"
#include "Matrix4.h"

using namespace std;

class Tetrahedron : public Geode {
	public:
		void draw(Matrix4);
};

#endif