#ifndef _CONE_H_
#define _CONE_H_

#include "Geode.h"
#include "Matrix4.h"

using namespace std;

class Cone : public Geode {
	public:
		void render(Matrix4);
};

#endif