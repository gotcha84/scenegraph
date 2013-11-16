#include "Sphere.h"

#include <stdlib.h>
#include <GL/glut.h>
#include <iostream>

using namespace std;

void Sphere::render(Matrix4 m) {

	glPushMatrix();
		glColor3f(1,0,0);
		glLoadMatrixf(m.getGLMatrix());
		glutSolidSphere(1, 10, 10);
		//cout << "drew Sphere\n";
	glPushMatrix();
}