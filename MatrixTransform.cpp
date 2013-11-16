#include "MatrixTransform.h"

#include <string>
#include <iostream>

using namespace std;

MatrixTransform::MatrixTransform() {
	Matrix4 i = Matrix4();
	i.identity();
	t = Matrix4(i);
}

MatrixTransform::MatrixTransform(Matrix4 m) {
	t = Matrix4(m);
}

void MatrixTransform::setTransformationMatrix(Matrix4 m) {
	t = Matrix4(m);
}

Matrix4 MatrixTransform::getTransformationMatrix() {
	return t;
}

void MatrixTransform::draw(Matrix4 c) {
	Matrix4 c_new = c.multiply(t);
	//c_new.print();

	for (int i=0; i<nChild; i++) {
		//cout << "drawing child " << i << "\n";
		child[i]->draw(c_new);
	}
}

void MatrixTransform::draw2(Matrix4 c) {
	Matrix4 c_new = c.multiply(t);
	//c_new.print();

	for (int i=0; i<nChild; i++) {
		//cout << "drawing child " << i << 5"\n";
		child[i]->draw2(c_new);
	}
}
