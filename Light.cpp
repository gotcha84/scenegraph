#include "Light.h"

#include <iostream>
#include <GL/glut.h>

using namespace std;

Light::Light() {
	light_num = 0;
	light = GL_LIGHT0;
}

Light::Light(int num) {
	light_num = num;

	switch (num) {
		case 0:
			light = GL_LIGHT0;
			break;
		case 1:
			light = GL_LIGHT1;
			break;
		case 2:
			light = GL_LIGHT2;
			break;
		case 3:
			light = GL_LIGHT3;
			break;
		case 4:
			light = GL_LIGHT4;
			break;
		case 5:
			light = GL_LIGHT5;
			break;
		case 6:
			light = GL_LIGHT6;
			break;
		case 7:
			light = GL_LIGHT7;
			break;
	}

	this->enable();
}

void Light::enable() {
	glEnable(light);

	glLightfv(light, GL_AMBIENT, ambient);
	glLightfv(light, GL_DIFFUSE, diffuse);
	glLightfv(light, GL_SPECULAR, specular);

	cout << "enabled GL_LIGHT" << light_num << "\n";
}

void Light::disable() {
	glDisable(light);

	GLfloat black[] = {0, 0, 0, 0};
	glLightfv(light, GL_DIFFUSE, black);
	glLightfv(light, GL_SPECULAR, black);

	cout << "disabled GL_LIGHT" << light_num << "\n";
}

void Light::setAmbient(GLfloat *v) {
	for (int i=0; i<4; i++)
		ambient[i] = v[i];
	glLightfv(light, GL_AMBIENT, v);
}

void Light::setDiffuse(GLfloat *v) {
	for (int i=0; i<4; i++)
		diffuse[i] = v[i];
	glLightfv(light, GL_DIFFUSE, v);
}

void Light::setSpecular(GLfloat *v) {
	for (int i=0; i<4; i++)
		specular[i] = v[i];
	glLightfv(light, GL_SPECULAR, v);
}

void Light::setPosition(GLfloat *v) {
	glLightfv(light, GL_POSITION, v);
}

void Light::setSpotDirection(GLfloat *v) {
	glLightfv(light, GL_SPOT_DIRECTION, v);
}

void Light::setSpotExponent(GLfloat n) {
	glLightf(light, GL_SPOT_EXPONENT, n);
}

void Light::setSpotCutoff(GLfloat n) {
	glLightf(light, GL_SPOT_CUTOFF, n);
}

void Light::setConstantAttenuation(GLfloat n) {
	glLightf(light, GL_CONSTANT_ATTENUATION, n);
}

void Light::setLinearAttenuation(GLfloat n) {
	glLightf(light, GL_LINEAR_ATTENUATION, n);
}

void Light::setQuadraticAttenuation(GLfloat n) {
	glLightf(light, GL_QUADRATIC_ATTENUATION, n);
}