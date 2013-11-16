#include "Tetrahedron.h"

#include <stdlib.h>
#include <GL/glut.h>
#include <iostream>

using namespace std;

void Tetrahedron::draw(Matrix4 m) {
	glPushMatrix();
		glLoadMatrixf(m.getGLMatrix());
		glutSolidTetrahedron();
		cout << "drew Tetrahedron ";
	glPopMatrix();
}