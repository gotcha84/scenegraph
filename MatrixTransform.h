#ifndef _MATRIXTRANSFORM_H_
#define _MATRIXTRANSFORM_H_

#include "Group.h"
#include "Matrix4.h"

#include <string>

using namespace std;

class MatrixTransform : public Group {
	public:
		Matrix4 t;

		MatrixTransform();
		MatrixTransform(Matrix4);

		void setTransformationMatrix(Matrix4);
		Matrix4 getTransformationMatrix();
		virtual void draw2(Matrix4);
		virtual void draw(Matrix4);
};

#endif