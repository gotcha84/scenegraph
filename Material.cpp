#include "Material.h"

#include <GL/glut.h>

using namespace std;

Material::Material() {
	frontback = GL_FRONT_AND_BACK;
}

Material::Material(GLenum fb) {
	frontback = fb;
}

void Material::setAmbient(GLfloat *v) {
	glMaterialfv(frontback, GL_AMBIENT, v);
}

void Material::setDiffuse(GLfloat *v) {
	glMaterialfv(frontback, GL_DIFFUSE, v);
}

void Material::setAmbientAndDiffuse(GLfloat *v) {
	glMaterialfv(frontback, GL_AMBIENT_AND_DIFFUSE, v);
}

void Material::setSpecular(GLfloat *v) {
	glMaterialfv(frontback, GL_SPECULAR, v);
}

void Material::setShininess(GLfloat *v) {
	glMaterialfv(frontback, GL_SHININESS, v);
}

void Material::setEmission(GLfloat *v) {
	glMaterialfv(frontback, GL_EMISSION, v);
}

void Material::setColorIndexes(GLfloat *v) {
	glMaterialfv(frontback, GL_COLOR_INDEXES, v);
}