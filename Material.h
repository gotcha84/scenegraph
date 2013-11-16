#ifndef _MATERIAL_H
#define _MATERIAL_H_

#include <string>
#include <GL/glut.h>

using namespace std;

class Material
{
	public:
		int light_num;
		GLenum frontback; // front and/or back

		Material();
		Material(GLenum);

		void setAmbient(GLfloat*);
		void setDiffuse(GLfloat*);
		void setAmbientAndDiffuse(GLfloat*);
		void setSpecular(GLfloat*);
		void setShininess(GLfloat*);
		void setEmission(GLfloat*);
		void setColorIndexes(GLfloat*);
		
};

#endif