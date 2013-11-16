#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "Matrix4.h"

#include <string>
#include <GL/glut.h>

using namespace std;

class Light
{
	public:
		int light_num;
		GLenum light; // GL_LIGHT#
		
		GLfloat ambient[4];
		GLfloat diffuse[4];
		GLfloat specular[4];

		Light();
		Light(int);

		void enable();
		void disable();

		void setAmbient(GLfloat*);
		void setDiffuse(GLfloat*);
		void setSpecular(GLfloat*);

		void setPosition(GLfloat*);
		void setMatrixPosition(GLfloat*);
		void setSpotDirection(GLfloat*);
		void setMatrixDirection(GLfloat*);
		void setSpotExponent(GLfloat);
		void setSpotCutoff(GLfloat);
		void setConstantAttenuation(GLfloat);
		void setLinearAttenuation(GLfloat);
		void setQuadraticAttenuation(GLfloat);
		
};

#endif