#include "Torus.h"

#include <stdlib.h>
#include <GL/glut.h>
#include <iostream>

using namespace std;

void Torus::draw(Matrix4 m) {
	glPushMatrix();
		glLoadMatrixf(m.getGLMatrix());
		glutSolidTorus(1, 2, 10, 10);
		cout << "drew Tauros ";
	glPopMatrix();
}