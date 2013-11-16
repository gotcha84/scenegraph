#include "Geode.h"
#include <iostream>

using namespace std;

Geode::Geode() {

}

Geode::Geode(Matrix4  m) {
	modelview = Matrix4(m);
	
	modelview.identity();
}

void Geode::setModelView(Matrix4 m) {
	modelview = Matrix4(m);
}

Matrix4 Geode::getModelView() {
	return modelview;
}

void Geode::draw (Matrix4 m) {
	render(m);
}

void Geode::draw2 (Matrix4 m) {
	if (m.get(0, 3) - m.get(0, 0) < x_min) {
		x_min = m.get(0, 3) - m.get(0, 0);
	}
	if (m.get(0, 3) + m.get(0, 0) > x_max) {
		x_max = m.get(0, 3) + m.get(0, 0);
	}
	if (m.get(1, 3) - m.get(1, 1) < y_min) {
		y_min = m.get(1, 3) - m.get(1, 1);
	}
	if (m.get(1, 3) + m.get(1, 1) > y_max) {
		y_max = m.get(1, 3) + m.get(1, 1);
	}
	if (m.get(2, 3) - m.get(2, 2) < z_min) {
		z_min = m.get(2, 3) - m.get(2, 2);
	}
	if (m.get(2, 3) + m.get(2, 2) > z_max) {
		z_max = m.get(2, 3) + m.get(2, 2);
	}

	this->setBoundingBox(x_min, x_max, y_min, y_max, z_min, z_max);

	float o[16],p[16];

	glGetFloatv(GL_PROJECTION_MATRIX,p);
	glGetFloatv(GL_MODELVIEW_MATRIX,o);

	Matrix4 a = Matrix4(o[0], o[1], o[2], o[3], o[4], o[5], o[6], o[7], o[8], o[9], o[10], o[11], o[12], o[13], o[14], o[15]);
	Matrix4 b = Matrix4(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], p[10], p[11], p[12], p[13], p[14], p[15]);
	
	Matrix4 c = a.multiply(b);
	c.transpose();

	Vector4 anear = Vector4(c.get(2,0) + c.get(3,0), c.get(2,1) + c.get(3,1), c.get(2,2) + c.get(3,2), c.get(2,3) + c.get(3,3));
	Vector4 afar = Vector4(-1*c.get(2,0) + c.get(3,0), -1*c.get(2,1) + c.get(3,1), -1*c.get(2,2) + c.get(3,2), -1*c.get(2,3) + c.get(3,3));
	Vector4 bottom = Vector4(c.get(1,0) + c.get(3,0), c.get(1,1) + c.get(3,1), c.get(1,2) + c.get(3,2), c.get(1,3) + c.get(3,3));
	Vector4 top = Vector4(-1*c.get(1,0) + c.get(3,0), -1*c.get(1,1) + c.get(3,1), -1*c.get(1,2) + c.get(3,2), -1*c.get(1,3) + c.get(3,3));
	Vector4 left = Vector4(c.get(0,0) + c.get(3,0), c.get(0,1) + c.get(3,1), c.get(0,2) + c.get(3,2), c.get(0,3) + c.get(3,3));
	Vector4 right = Vector4(-1*c.get(0,0) + c.get(3,0), -1*c.get(0,1) + c.get(3,1), -1*c.get(0,2) + c.get(3,2), -1*c.get(0,3) + c.get(3,3));
	
	Vector3 z = Vector3(anear[0], anear[1], anear[2]);
	Vector3 y = Vector3(afar[0], afar[1], afar[2]);
	Vector3 x = Vector3(bottom[0], bottom[1], bottom[2]);
	Vector3 w = Vector3(top[0], top[1], top[2]);
	Vector3 v = Vector3(left[0], left[1], left[2]);
	Vector3 u = Vector3(right[0], right[1], right[2]);

	anear.normalize();
	afar.normalize();
	bottom.normalize();
	top.normalize();
	left.normalize();
	right.normalize();

	Vector4 plane_arr[6] = {anear, afar, bottom, top, left, right};
	bool cull = false;
	float distance;
	Vector3 n;
	int count = 0;
	Vector4 tmp = Vector4(boxx, boxy, boxz, 1);
	Vector4 transform = c.multiply(tmp);
	for (int i = 0; i < 6; i++) {
		distance = plane_arr[i][0]*transform[0] + plane_arr[i][1]*transform[1] + plane_arr[i][2]*transform[2] + plane_arr[i][3];
		if (distance < -boxr) {
			count++;
		}
	}
	if (count != 6) {
		render(m);
	}

}
