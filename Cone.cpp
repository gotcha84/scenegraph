#include "Cone.h"

#include <stdlib.h>
#include <GL/glut.h>
#include <iostream>

using namespace std;

void Cone::render(Matrix4 m) {

	glPushMatrix();
		glLoadMatrixf(m.getGLMatrix());
		glutSolidCone(1, 2, 10, 10);
		//cout << "drew Cone ";
	glPopMatrix();
}