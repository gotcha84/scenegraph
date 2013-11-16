#include "Cube.h"

#include <stdlib.h>
#include <GL/glut.h>
#include <iostream>

using namespace std;

bool blue = false;

void Cube::render(Matrix4 m) {

	glPushMatrix();
		(blue) ? glColor3f(0,0,1) : glColor3f(1,1,1);
		//blue = !blue;
		glLoadMatrixf(m.getGLMatrix());
		glutSolidCube(1);
		//cout << "drew Cube\n";
	glPopMatrix();
}