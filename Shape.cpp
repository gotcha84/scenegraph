#include "Shape.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <GL/glut.h>
#include <math.h>
#include <vector>
#include <time.h>
#include <climits>  

using namespace std;

static bool TURN_LIGHTS_ON = false;

static bool DEBUGGER = false;
static bool DEBUG_LOAD_OBJS = false;
static bool DEBUG_DRAW_LIGHTS = false;

static Shape shape;
static double spin_angle = 0;
static int shape_key = 1;

static bool red = false;
static bool fullscreen = false;

static bool left_clicked = false;
static bool right_clicked = false;
static int x_mouse;
static int y_mouse;

GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat mat_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
GLfloat mat_ambient_color[] = { 0.8, 0.8, 0.2, 1.0 };
GLfloat mat_diffuse[] = { 0.1, 0.5, 0.8, 1.0 };
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat no_shininess[] = { 0.0 };
GLfloat low_shininess[] = { 5.0 };
GLfloat high_shininess[] = { 128 };
GLfloat mat_emission[] = {0.3, 0.2, 0.2, 0.0};

GLfloat d_position[] = {0, 1, 0, 0};
GLfloat p_position[] = {0, -5, 0, 1};
GLfloat s_position[] = {-10, 0, 0, 1};
GLfloat s_direction[] = {1, 0, 0};
static float theta = 0;

static bool shader_toggle = false;
static bool toggle1 = false;
static bool toggle2 = false;
static bool toggle3 = false;
static bool freeze = false;
static bool toggle_frus = false;

int Window::width  = 512;   // set window width in pixels here
int Window::height = 512;   // set window height in pixels here

static int army_size = 10;
vector<vector<MatrixTransform*>> robot(army_size, vector<MatrixTransform*> (army_size));
static float spin_leftarm = 3.14/2;
static float leftarm_dir = -4;
static float spin_rightarm = 0;
static float rightarm_dir = 4;

static float animate = 0.001;
static float spin_leftleg = 0;
static float leftleg_dir = -2;
static float spin_rightleg = 0;
static float rightleg_dir = 2;

clock_t Start = clock();
static int noculltimer = 0;
static int culltimer = 0;

//----------------------------------------------------------------------------
// Callback method called when system is idle.
void Window::idleCallback(void)
{
	//shape.spin(spin_angle);
  displayCallback();
}

//----------------------------------------------------------------------------
// Callback method called when window is resized.
void Window::reshapeCallback(int w, int h)
{
	if (!fullscreen) {
		width = w;
		height = h;
	}
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-10.0, 10.0, -10.0, 10.0, 10, 1000.0); // set perspective projection viewing frustum
  glTranslatef(0, 0, -20);
}

Matrix4& Shape::getModelMatrix() {
	return shape.model;
}

Matrix4& Shape::getModelViewMatrix() {
	return shape.modelview;
}

void Shape::updateModelViewMatrix() {
	Matrix4 cam_inv = Matrix4(shape.getCameraMatrix());
	cam_inv.inverse();
	shape.getModelViewMatrix() = cam_inv.multiply(shape.getModelMatrix());
}

//----------------------------------------------------------------------------
// Callback method called when window readraw is necessary or
// when glutPostRedisplay() was called.
void Window::displayCallback(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear color and depth buffers
  
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(shape.getModelViewMatrix().getGLMatrix());

	Material cube = Material(GL_FRONT_AND_BACK);
	Material dragon = Material(GL_FRONT);
	Material bunny = Material(GL_FRONT_AND_BACK);
	Material sandal = Material(GL_FRONT_AND_BACK);
	
	switch (shape_key) {
		case 1: // cube - robot
			if (TURN_LIGHTS_ON) {
				cube.setAmbient(no_mat);
				cube.setDiffuse(mat_diffuse);
				cube.setSpecular(mat_specular);
				cube.setShininess(high_shininess);
				cube.setEmission(no_mat);
			}

			if (toggle_frus) {
				culltimer+=1;
				unsigned long long Int64 = 0;
				shape.drawRobot2();
				if (clock()-Start > 1000) {
					cout << "culling FPS: " <<  culltimer/((clock() - Start)/1000.0) << '\n';
					Start = clock();
					culltimer = 0;
				}
				
			}
			else {
				noculltimer++;
				unsigned long long Int64 = 0;
				shape.drawRobot();
				if (clock()-Start > 1000) {
					cout << "no culling FPS: " <<  noculltimer/((clock() - Start)/1000.0) << '\n';
					Start = clock();
					noculltimer = 0;
				}
			}
			break;

		case 2: // dragon
			dragon.setAmbient(no_mat);
			dragon.setDiffuse(mat_diffuse);
			dragon.setSpecular(mat_specular);
			dragon.setShininess(high_shininess);
			dragon.setEmission(no_mat);

			drawShape(dragon_nVerts, dragon_vertices, dragon_normals);
			break;
		case 3: // bunny
			bunny.setAmbient(no_mat);
			bunny.setDiffuse(no_mat);
			bunny.setSpecular(mat_specular);
			bunny.setShininess(high_shininess);
			bunny.setEmission(no_mat);

			drawShape(bunny_nVerts, bunny_vertices, bunny_normals);
			break;
		case 4: // sandle
			sandal.setAmbient(no_mat);
			sandal.setDiffuse(no_mat);
			sandal.setSpecular(mat_specular);
			sandal.setShininess(high_shininess);
			sandal.setEmission(no_mat);

			drawShape(sandal_nVerts, sandal_vertices, sandal_normals);
			break;
		case 8: // house scene1
			shape.drawHouse();
			break;
		case 9: // house scene2
			shape.drawHouse();
			break;
	}

	if (toggle1) {
			glPushMatrix();
				glRotatef(theta, 1, 0, 0);
				if (!freeze)	
					shape.directional.setPosition(d_position);
				if (DEBUG_DRAW_LIGHTS) drawDirectionalLight();
			glPopMatrix();
	}

	if (toggle2) {
		glPushMatrix();
			glRotatef(theta, 0, 0, 1);
			if (!freeze)
				shape.point.setPosition(p_position);
			if (DEBUG_DRAW_LIGHTS) drawPointLight();
		glPopMatrix();
	}

	if (toggle3) {
		glPushMatrix();
			glRotatef(theta, 0, 1, 0);
			if (!freeze) {
				shape.spot.setPosition(s_position);
				shape.spot.setSpotDirection(s_direction);
			}
			if (DEBUG_DRAW_LIGHTS) drawSpotLight();
		glPopMatrix();
	}

	if (!freeze)
		theta+=1;
	if (theta > 360 || theta < 0) theta = 0;



	/*
	// TODO: sphere for testing
	glPushMatrix();
		glColor3f(1,1,1);
		Matrix4 i = Matrix4();
		i.identity();
		glLoadMatrixf(i.getGLMatrix());
		glTranslatef(-5,0,0);
		glutSolidSphere(1,10,10);
	glPopMatrix();
	*/

	spin_leftarm+=animate*leftarm_dir;
	spin_rightarm+=animate*rightarm_dir;
	if (spin_leftarm > 3.14*3/4 || spin_leftarm < 3.14/4) leftarm_dir=-1*leftarm_dir;
	if (spin_rightarm > 3.14/4 || spin_rightarm < -3.14/4) rightarm_dir=-1*rightarm_dir;

	spin_leftleg+=(animate*leftleg_dir);
	spin_rightleg+=animate*rightleg_dir;
	if (spin_leftleg > 3.14/16 || spin_leftleg < -3.14/16) leftleg_dir=-1*leftleg_dir;
	if (spin_rightleg > 3.14/16 || spin_rightleg < -3.14/16) rightleg_dir=-1*rightleg_dir;

  glFlush();  
  glutSwapBuffers();
}

void Window::drawDirectionalLight() {
		glBegin(GL_LINES);
			glColor3f(10, 10, 10);
			glVertex3f(0, 10, 0);
			glVertex3f(0 - 5*d_position[0], 10 - 5*d_position[1], 0 - 5*d_position[2]);
		glEnd();
}

void Window::drawPointLight() {
		glColor3f(10, 10, 10);
		glTranslated(p_position[0], p_position[1], p_position[2]);
		glScaled(0.5, 0.5, 0.5);
		glutSolidSphere(1, 10, 10);
}

void Window::drawSpotLight() {		
		glBegin(GL_LINES);
			glColor3f(10, 10, 10);
			glVertex3f(s_position[0], s_position[1], s_position[2]);
			glVertex3f(s_position[0] + 3*s_direction[0], s_position[1] + 3*s_direction[1], s_position[2] + 3*s_direction[2]);
		glEnd();
}

void Window::drawCube() {
		glBegin(GL_QUADS);
			// Draw front face:
			glColor3f(1, 0, 0);
			glNormal3f(0.0, 0.0, 1.0);   
			glVertex3f(-1.0,  1.0,  1.0);
			glVertex3f( 1.0,  1.0,  1.0);
			glVertex3f( 1.0, -1.0,  1.0);
			glVertex3f(-1.0, -1.0,  1.0);
    
			// Draw left side:
			glColor3f(0, 1, 0);
			glNormal3f(-1.0, 0.0, 0.0);
			glVertex3f(-1.0,  1.0,  1.0);
			glVertex3f(-1.0,  1.0, -1.0);
			glVertex3f(-1.0, -1.0, -1.0);
			glVertex3f(-1.0, -1.0,  1.0);
    
			// Draw right side:
			glColor3f(0, 0, 1);
			glNormal3f(1.0, 0.0, 0.0);
			glVertex3f( 1.0,  1.0,  1.0);
			glVertex3f( 1.0,  1.0, -1.0);
			glVertex3f( 1.0, -1.0, -1.0);
			glVertex3f( 1.0, -1.0,  1.0);
  
			// Draw back face:
			glColor3f(1, 0, 1);
			glNormal3f(0.0, 0.0, -1.0);
			glVertex3f(-1.0,  1.0, -1.0);
			glVertex3f( 1.0,  1.0, -1.0);
			glVertex3f( 1.0, -1.0, -1.0);
			glVertex3f(-1.0, -1.0, -1.0);
  
			// Draw top side:
			glColor3f(1, 1, 0);
			glNormal3f(0.0, 1.0, 0.0);
			glVertex3f(-1.0,  1.0,  1.0);
			glVertex3f( 1.0,  1.0,  1.0);
			glVertex3f( 1.0,  1.0, -1.0);
			glVertex3f(-1.0,  1.0, -1.0);
  
			// Draw bottom side:
			glColor3f(0, 1, 1);
			glNormal3f(0.0, -1.0, 0.0);
			glVertex3f(-1.0, -1.0, -1.0);
			glVertex3f( 1.0, -1.0, -1.0);
			glVertex3f( 1.0, -1.0,  1.0);
			glVertex3f(-1.0, -1.0,  1.0);
		glEnd();
}

void Shape::drawRobot() {
	glMatrixMode(GL_MODELVIEW);

	for (int j=0; j<army_size; j++) {
		for (int k=0; k<army_size; k++) {
			
			Matrix4 i;
			i.identity();
			robot[j][k] = &MatrixTransform(i);

			// head
			Matrix4 s = Matrix4(i);
			s.scale(0.5,0.5,0.5);
			s.rotateX(2*spin_leftleg);
			MatrixTransform head = MatrixTransform(s);
			head.addChild(&Sphere());

			// torso
			Matrix4 t = Matrix4(i);
			t.scale(1.5,2,1);
			t.translate(0,-1.5,0);
			t.rotateY(0.5*spin_leftleg);
			MatrixTransform torso = MatrixTransform(t);
			torso.addChild(&Cube());

			// left arm
			Matrix4 la = Matrix4(i);
			la.rotateZ(3.14/4);
			la.translate(1,-0.5,0);
			la.rotateX(spin_leftarm);
			MatrixTransform leftarm = MatrixTransform(la);

			Matrix4 ula = Matrix4(i);
			ula.translate(-1,-0.5,0);
			ula.scale(0.5,1.5,0.5);
			MatrixTransform upper_leftarm = MatrixTransform(ula);
			upper_leftarm.addChild(&Cube());

			Matrix4 lla = Matrix4(i);
			lla.rotateX(3.14/2);
			lla.scale(0.25,0.25,0.25);
			lla.translate(-0.5,-1.5,0);
			MatrixTransform lower_leftarm = MatrixTransform(lla);
			lower_leftarm.addChild(&Sphere());
	
			leftarm.addChild(&upper_leftarm);
			leftarm.addChild(&lower_leftarm);

			// right arm
			Matrix4 ra = Matrix4(i);
			ra.rotateZ(-3.14/4);
			ra.translate(-1,-0.5,0);
			ra.rotateX(spin_rightarm);
			MatrixTransform rightarm = MatrixTransform(ra);
	
			Matrix4 ura = Matrix4(i);
			ura.translate(1,-0.5,0);
			ura.scale(0.5,1.5,0.5);
			MatrixTransform upper_rightarm = MatrixTransform(ura);
			upper_rightarm.addChild(&Cube());

			Matrix4 lra = Matrix4(i);
			lra.rotateX(3.14/2);
			lra.scale(0.25,0.25,0.25);
			lra.translate(0.5,-1.5,0);
			MatrixTransform lower_rightarm = MatrixTransform(lra);
			lower_rightarm.addChild(&Sphere());
	
			rightarm.addChild(&upper_rightarm);
			rightarm.addChild(&lower_rightarm);

			// left leg
			Matrix4 ll = Matrix4(i);
			ll.scale(0.5,1.5,0.5);
			ll.translate(0,-3,0);
			ll.rotateWindowX(spin_leftleg);
			MatrixTransform leftleg = MatrixTransform(ll);

			Matrix4 ull = Matrix4(i);
			ull.translate(1,0,0);
			MatrixTransform upper_leftleg = MatrixTransform(ull);
			upper_leftleg.addChild(&Cube());

			Matrix4 lll = Matrix4(i);
			lll.scale(1,0.1,2);
			lll.translate(1,-0.5,0.5);
			MatrixTransform lower_leftleg = MatrixTransform(lll);
			lower_leftleg.addChild(&Cube());
	
			leftleg.addChild(&upper_leftleg);
			leftleg.addChild(&lower_leftleg);

			// right leg
			Matrix4 rl = Matrix4(i);
			rl.scale(0.5,1.5,0.5);
			rl.translate(0,-3,0);
			rl.rotateWindowX(spin_rightleg);
			MatrixTransform rightleg = MatrixTransform(rl);

			Matrix4 url = Matrix4(i);
			url.translate(-1,0,0);
			MatrixTransform upper_rightleg = MatrixTransform(url);
			upper_rightleg.addChild(&Cube());

			Matrix4 lrl = Matrix4(i);
			lrl.scale(1,0.1,2);
			lrl.translate(-1,-0.5,0.5);
			MatrixTransform lower_rightleg = MatrixTransform(lrl);
			lower_rightleg.addChild(&Cube());
	
			rightleg.addChild(&upper_rightleg);
			rightleg.addChild(&lower_rightleg);

			// add to robot
			robot[j][k]->addChild(&head);
			robot[j][k]->addChild(&torso);
			robot[j][k]->addChild(&leftarm);
			robot[j][k]->addChild(&rightarm);
			robot[j][k]->addChild(&leftleg);
			robot[j][k]->addChild(&rightleg);

			la.rotateX(-3.14/2);
			leftarm.setTransformationMatrix(la);

			i.translate(-5*j,0,-5*k);
			robot[j][k]->setTransformationMatrix(i);
			robot[j][k]->draw(shape.modelview);
		}

	}
}

void Shape::drawRobot2() {
	glMatrixMode(GL_MODELVIEW);
	glColor3f(0,1,0);

	for (int j=0; j<army_size; j++) {
		for (int k=0; k<army_size; k++) {
			
			Matrix4 i;
			i.identity();
			robot[j][k] = &MatrixTransform(i);

			// head
			Matrix4 s = Matrix4(i);
			s.scale(0.5,0.5,0.5);
			s.rotateX(2*spin_leftleg);
			MatrixTransform head = MatrixTransform(s);
			head.addChild(&Sphere());

			// torso
			Matrix4 t = Matrix4(i);
			t.scale(1.5,2,1);
			t.translate(0,-1.5,0);
			t.rotateY(0.5*spin_leftleg);
			MatrixTransform torso = MatrixTransform(t);
			torso.addChild(&Cube());

			// left arm
			Matrix4 la = Matrix4(i);
			la.rotateZ(3.14/4);
			la.translate(1,-0.5,0);
			la.rotateX(spin_leftarm);
			MatrixTransform leftarm = MatrixTransform(la);

			Matrix4 ula = Matrix4(i);
			ula.translate(-1,-0.5,0);
			ula.scale(0.5,1.5,0.5);
			MatrixTransform upper_leftarm = MatrixTransform(ula);
			upper_leftarm.addChild(&Cube());

			Matrix4 lla = Matrix4(i);
			lla.rotateX(3.14/2);
			lla.scale(0.25,0.25,0.25);
			lla.translate(-0.5,-1.5,0);
			MatrixTransform lower_leftarm = MatrixTransform(lla);
			lower_leftarm.addChild(&Sphere());
	
			leftarm.addChild(&upper_leftarm);
			leftarm.addChild(&lower_leftarm);

			// right arm
			Matrix4 ra = Matrix4(i);
			ra.rotateZ(-3.14/4);
			ra.translate(-1,-0.5,0);
			ra.rotateX(spin_rightarm);
			MatrixTransform rightarm = MatrixTransform(ra);
	
			Matrix4 ura = Matrix4(i);
			ura.translate(1,-0.5,0);
			ura.scale(0.5,1.5,0.5);
			MatrixTransform upper_rightarm = MatrixTransform(ura);
			upper_rightarm.addChild(&Cube());

			Matrix4 lra = Matrix4(i);
			lra.rotateX(3.14/2);
			lra.scale(0.25,0.25,0.25);
			lra.translate(0.5,-1.5,0);
			MatrixTransform lower_rightarm = MatrixTransform(lra);
			lower_rightarm.addChild(&Sphere());
	
			rightarm.addChild(&upper_rightarm);
			rightarm.addChild(&lower_rightarm);

			// left leg
			Matrix4 ll = Matrix4(i);
			ll.scale(0.5,1.5,0.5);
			ll.translate(0,-3,0);
			ll.rotateWindowX(spin_leftleg);
			MatrixTransform leftleg = MatrixTransform(ll);

			Matrix4 ull = Matrix4(i);
			ull.translate(1,0,0);
			MatrixTransform upper_leftleg = MatrixTransform(ull);
			upper_leftleg.addChild(&Cube());

			Matrix4 lll = Matrix4(i);
			lll.scale(1,0.1,2);
			lll.translate(1,-0.5,0.5);
			MatrixTransform lower_leftleg = MatrixTransform(lll);
			lower_leftleg.addChild(&Cube());
	
			leftleg.addChild(&upper_leftleg);
			leftleg.addChild(&lower_leftleg);

			// right leg
			Matrix4 rl = Matrix4(i);
			rl.scale(0.5,1.5,0.5);
			rl.translate(0,-3,0);
			rl.rotateWindowX(spin_rightleg);
			MatrixTransform rightleg = MatrixTransform(rl);

			Matrix4 url = Matrix4(i);
			url.translate(-1,0,0);
			MatrixTransform upper_rightleg = MatrixTransform(url);
			upper_rightleg.addChild(&Cube());

			Matrix4 lrl = Matrix4(i);
			lrl.scale(1,0.1,2);
			lrl.translate(-1,-0.5,0.5);
			MatrixTransform lower_rightleg = MatrixTransform(lrl);
			lower_rightleg.addChild(&Cube());
	
			rightleg.addChild(&upper_rightleg);
			rightleg.addChild(&lower_rightleg);

			// add to robot
			robot[j][k]->addChild(&head);
			robot[j][k]->addChild(&torso);
			robot[j][k]->addChild(&leftarm);
			robot[j][k]->addChild(&rightarm);
			robot[j][k]->addChild(&leftleg);
			robot[j][k]->addChild(&rightleg);

			la.rotateX(-3.14/2);
			leftarm.setTransformationMatrix(la);

			i.translate(-5*j,0,-5*k);
			robot[j][k]->setTransformationMatrix(i);
			robot[j][k]->draw2(shape.modelview);
		}

	}
}

void Shape::loadData()
{
  // put code to load data model here
	ObjReader::readObj("dragon_smooth.obj", dragon_nVerts, &dragon_vertices, &dragon_normals, &dragon_texcoords, dragon_nIndices, &dragon_indices);
	ObjReader::readObj("bunny_n.obj", bunny_nVerts, &bunny_vertices, &bunny_normals, &bunny_texcoords, bunny_nIndices, &bunny_indices);
	ObjReader::readObj("sandal.obj", sandal_nVerts, &sandal_vertices, &sandal_normals, &sandal_texcoords, sandal_nIndices, &sandal_indices);
}

void Shape::calculateStuff(int nVerts, float *vertices) {
	float max_arr[3] = {-1000, -1000, -1000};
	float min_arr[3] = {1000, 1000, 1000};

	for (int i = 0; i < nVerts/3; i++) {
		for (int v = 0; v < 3; v++) {
			if (vertices[9*i+3*v] < min_arr[0]) {
				min_arr[0] = vertices[9*i+3*v];
			}
			if (vertices[9*i+3*v] > max_arr[0]) {
				max_arr[0] = vertices[9*i+3*v];
			}
			if (vertices[(9*i)+(3*v)+1] < min_arr[1]) {
				min_arr[1] = vertices[(9*i)+(3*v)+1];
			}
			if (vertices[(9*i)+(3*v)+1] > max_arr[1]) {
				max_arr[1] = vertices[(9*i)+(3*v)+1];
			}
			if (vertices[(9*i)+(3*v)+2] < min_arr[2]) {
				min_arr[2] = vertices[(9*i)+(3*v)+2];
			}
			if (vertices[(9*i)+(3*v)+2] > max_arr[2]) {
				max_arr[2] = vertices[(9*i)+(3*v)+2];
			}

		}
	}

	shape.x = 0.5*(max_arr[0] + min_arr[0]);
	shape.y = 0.5*(max_arr[1] + min_arr[1]);
	shape.z = 0.5*(max_arr[2] + min_arr[2]);

	shape.translation.identity();
	shape.translation.m[0][3] = -shape.x;
	shape.translation.m[1][3] = -shape.y;
	shape.translation.m[2][3] = -shape.z;

	cout << "minimum values: " << min_arr[0] << ", " << min_arr[1] << ", " << min_arr[2] << "\n";
  cout << "maximum values: " << max_arr[0] << ", " << max_arr[1] << ", " << max_arr[2] << "\n\n";

	cout << "center: (" << shape.x << ", " << shape.y << ", " << shape.z << ")\n\n";
	
	float x_diff = max_arr[0]-min_arr[0];
	float y_diff = max_arr[1]-min_arr[1];
	float z_diff = max_arr[2]-min_arr[2];

	float max1 = max(x_diff, y_diff);
	max1 = max(max1, z_diff);

	scaling_x = 28/max1;
	scaling_y = 28/max1;
	scaling_z = 28/max1;

	shape.scale.identity();
	shape.scale.m[0][0] = scaling_x;
	shape.scale.m[1][1] = scaling_y;
	shape.scale.m[2][2] = scaling_z;

	cout << "scaling factor : " << scaling_x << ", " << scaling_y <<  ", " << scaling_z <<"\n\n";

}

void Window::drawShape(int nVerts, float *vertices, float *normals) {
	glBegin(GL_TRIANGLES);
	for (int i=0; i<nVerts/3; i++) {
		if (red == true) {
			// red
			glColor3f(0.5,0.5,0.5);
			red = false;
		}
		else {
			// blue
			glColor3f(0.5,0.5,0.5);
			red = true;
		}
		for (int v=0; v<3; v++) {
			glNormal3f(normals[9*i+3*v], normals[(9*i)+(3*v)+1], normals[(9*i)+(3*v)+2]);
			glVertex3f(vertices[9*i+3*v], vertices[(9*i)+(3*v)+1], vertices[(9*i)+(3*v)+2]);
		}
	}
	glEnd();
}

Shape::Shape() {
	shape.getModelMatrix().identity();

	shape.getModelMatrix() = shape.getModelMatrix().multiply(shape.setScaleMatrix(8.0));

	shape.getCameraMatrix().identity();
	shape.updateModelViewMatrix();

	cout << "initialized model matrix:\n";
	shape.getModelMatrix().print();

	cout << "initialized camera matrix:\n";
	shape.getCameraMatrix().print();

	cout << "initialized modelview matrix:\n";
	shape.getModelViewMatrix().print();
}

Matrix4& Shape::getCameraMatrix() {
	return shape.camera;
}

int main(int argc, char *argv[])
{
  float specular[]  = {1.0, 1.0, 1.0, 1.0};
  float shininess[] = {100.0};

  glutInit(&argc, argv);      	      	      // initialize GLUT
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);   // open an OpenGL context with double buffering, RGB colors, and depth buffering
  glutInitWindowSize(Window::width, Window::height);      // set initial window size
  glutCreateWindow("OpenGL Cube for CSE167");    	      // open window and set window title
  glDisable(GL_LIGHTING);
  
  
  glEnable(GL_DEPTH_TEST);            	      // enable depth buffering
  glClear(GL_DEPTH_BUFFER_BIT);       	      // clear depth buffer
  glClearColor(0.0, 0.0, 0.0, 0.0);   	      // set clear color to black
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  // set polygon drawing mode to fill front and back of each polygon
  glDisable(GL_CULL_FACE);     // disable backface culling to render both sides of polygons
  glDisable(GL_LIGHTING);
  glShadeModel(GL_SMOOTH);             	      // set shading to smooth
  glMatrixMode(GL_PROJECTION); 
  
  // Generate material properties:
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);

	if (TURN_LIGHTS_ON) {
		//Generate light source:
		glEnable(GL_LIGHTING);
  
		GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };

		GLfloat light_diffuse_d[] = { 1.0, 0.0, 0.0, 1.0 };
		GLfloat light_specular_d[] = { 1.0, 0.0, 0.0, 1.0 };

		GLfloat light_diffuse_p[] = { 0.0, 1.0, 0.0, 1.0 };
		GLfloat light_specular_p[] = { 0.0, 1.0, 0.0, 1.0 };

		GLfloat light_diffuse_s[] = { 0.0, 0.0, 1.0, 1.0 };
		GLfloat light_specular_s[] = { 0.0, 0.0, 1.0, 1.0 };

		shape.directional = Light(0);
		shape.directional.setPosition(d_position);
		shape.directional.setAmbient(light_ambient);
		shape.directional.setDiffuse(light_diffuse_d);
		shape.directional.setSpecular(light_specular_d);

		shape.point = Light(1);
		shape.point.setPosition(p_position);
		shape.point.setAmbient(light_ambient);
		shape.point.setDiffuse(light_diffuse_p);
		shape.point.setSpecular(light_specular_p);

		shape.spot = Light(2);
		shape.spot.setPosition(s_position);
		shape.spot.setAmbient(light_ambient);
		shape.spot.setDiffuse(light_diffuse_s);
		shape.spot.setSpecular(light_specular_s);
		shape.spot.setSpotCutoff(5);
		shape.spot.setSpotDirection(s_direction);

		if (!toggle1)
			shape.directional.disable();
		if (!toggle2)
			shape.point.disable();
		if (!toggle3)
			shape.spot.disable();
	}

  // Install callback functions:
  glutDisplayFunc(Window::displayCallback);
  glutReshapeFunc(Window::reshapeCallback);
  glutIdleFunc(Window::idleCallback);

	// to avoid cube turning white on scaling down
  glEnable(GL_NORMALIZE);
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	// Process keyboard input
  glutKeyboardFunc(Window::processNormalKeys);
  glutSpecialFunc(Window::processSpecialKeys);
	glutMouseFunc(Window::processMouseClick);
	glutMotionFunc(Window::processMouseMove);
	
	// load obj files
	if (DEBUG_LOAD_OBJS)
		shape.loadData();

	glutMainLoop();
  return 0;
}

double Shape::getAngle() {
	return angle;
}

void Shape::setAngle(double a) {
	angle = a;
}

Matrix4 Shape::setScaleMatrix(float factor) {
	Matrix4 scale = Matrix4();
	for (int i = 0; i < 3; i++)
	{
		scale.set(i, i, factor);
	}
	return scale;
}

void Shape::drawHouse() {
	int tris = 0;
	glBegin(GL_QUADS);
	for (int i=0; i<7; i++) {
		glColor3f(house_colors[12*i], house_colors[12*i+1], house_colors[12*i+2]);
		glNormal3f(house_indices[i*6], house_indices[i*6+1], house_indices[i*6+2]);
		for (int v=0; v<4; v++) {
			glVertex3f(house_vertices[12*i+3*v], house_vertices[(12*i)+(3*v)+1], house_vertices[(12*i)+(3*v)+2]);
		}
		glNormal3f(house_indices[i*6+3], house_indices[i*6+4], house_indices[i*6+5]);
	}
	glEnd();

	glBegin(GL_TRIANGLES);
	for (int i=7; i<8; i++) {
		glColor3f(house_colors[12*i], house_colors[12*i+1], house_colors[12*i+2]);
		glNormal3f(house_indices[i*6], house_indices[i*6+1], house_indices[i*6+2]);
		for (int v=0; v<3; v++) {
			glVertex3f(house_vertices[12*i+3*v], house_vertices[(12*i)+(3*v)+1], house_vertices[(12*i)+(3*v)+2]);
		}
		tris++;
	}
	glEnd();

	glBegin(GL_QUADS);
	for (int i=8; i<10; i++) {
		glColor3f(house_colors[12*i-3*tris], house_colors[12*i+1-3*tris], house_colors[12*i+2-3*tris]);
		glNormal3f(house_indices[i*6-3*tris], house_indices[i*6+1-3*tris], house_indices[i*6+2-3*tris]);
		for (int v=0; v<4; v++) {
			glVertex3f(house_vertices[12*i+3*v-3*tris], house_vertices[(12*i)+(3*v)+1-3*tris], house_vertices[(12*i)+(3*v)+2-3*tris]);
		}
		glNormal3f(house_indices[i*6+3-3*tris], house_indices[i*6+4-3*tris], house_indices[i*6+5-3*tris]);
	}
	glEnd();
	
	glBegin(GL_TRIANGLES);
	for (int i=10; i<11; i++) {
		glColor3f(house_colors[12*i-3*tris], house_colors[12*i+1-3*tris], house_colors[12*i+2-3*tris]);
		glNormal3f(house_indices[i*6]-3*tris, house_indices[i*6+1]-3*tris, house_indices[i*6+2]-3*tris);
		for (int v=0; v<3; v++) {
			glVertex3f(house_vertices[12*i+3*v-3*tris], house_vertices[(12*i)+(3*v)+1-3*tris], house_vertices[(12*i)+(3*v)+2-3*tris]);
		}
		tris++;
	}
	glEnd();
}


void Shape::spin(double deg)
{
  if (shape.angle > 360.0 || shape.angle < -360.0) shape.angle = 0.0;
	shape.getModelViewMatrix().rotateY(deg);
}


void Window::processNormalKeys(unsigned char key, int x, int y)
{
	switch (key) 
	{
		case 'c':
			// reverse the direction of the spin
			toggle_frus = !toggle_frus;
			break;
		case 'x':
			// move cube left by a small amount
			shape.getModelViewMatrix().translate(-1, 0, 0);
			cout << "move left\n";
			break;
		case 'X':
			// move cube right by a small amount
			shape.getModelViewMatrix().translate(1, 0, 0);
			cout << "move right\n";
			break;
		case 'y':
			// move cube down by a small amount
			shape.getModelViewMatrix().translate(0, -1, 0);
			cout << "move down\n";
			break;
		case 'Y':
			// move cube up by a small amount
			shape.getModelViewMatrix().translate(0, 1, 0);
			cout << "move up\n";
			break;
		case 'z':
			// move cube into of the screen by a small amount
			shape.getModelViewMatrix().translate(0, 0, -1);
			cout << "move in\n";
			break;
		case 'Z':
			// move cube out of the screen by a small amount
			shape.getModelViewMatrix().translate(0, 0, 1);
			cout << "move out\n";
			break;
		case 'r':
			// reset position and size to its initial position
			shape.getModelMatrix().identity();
			if (shape_key == 8)
				shape.getCameraMatrix() = Matrix4::createCameraMatrix(Vector3(0, 10, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
			else if (shape_key == 9)
				shape.getCameraMatrix() = Matrix4::createCameraMatrix(Vector3(-15, 5, 10), Vector3(-5, 0, 0), Vector3(0, 1, 0.5));
			else
				shape.getCameraMatrix().identity();
			shape.setAngle(0);
			shape.updateModelViewMatrix();
			break;
		case 'a':
			// rotate cube about the OpenGL window's z axis by a small number of degrees counterclockwise
			// The z axis crosses the screen in its center.
			if (shape.angle > 360.0 || shape.angle < -360.0) shape.angle = 0.0;
			shape.getModelViewMatrix().rotateWindowZ(-100*spin_angle);
			cout << "rotate CW window z-axis\n";
			break;
		case 'A':
			// rotate cube about the OpenGL window's z axis by a small number of degrees clockwise
			// The z axis crosses the screen in its center.
			if (shape.angle > 360.0 || shape.angle < -360.0) shape.angle = 0.0;
			shape.getModelViewMatrix().rotateWindowZ(100*spin_angle);
			cout << "rotate CCW window z-axis\n";
			break;
		case 's':
			// scale cube down (about its center, not the center of the screen)
			shape.getModelViewMatrix().scale(0.95, 0.95, 0.95);
			cout << "scale down\n";
			break;
		case 'S':
			// scale cube up (about its center, not the center of the screen)
			shape.getModelViewMatrix().scale(1.05, 1.05, 1.05);
			cout << "scale up\n";
			break;
		case 'f': //toggle screenmode
			if(!fullscreen) {
        glutFullScreen();
        fullscreen = true;
			} 
			else {
        glutReshapeWindow(Window::width, Window::height);
        glutPositionWindow(100, 100);
        fullscreen = false;
			}
	    break;
		case '1':
				toggle1 = !toggle1;
				toggle1 ? shape.directional.enable() : shape.directional.disable();
				toggle1 ? cout << "directional ON\n" : cout << "directional OFF\n";
			break;
		case '2':
				toggle2 = !toggle2;
				toggle2 ? shape.point.enable() : shape.point.disable();
				toggle2 ? cout << "point ON\n" : cout << "point OFF\n";
			break;
		case '3':
				toggle3 = !toggle3;
				toggle3 ? shape.spot.enable() : shape.spot.disable();
				toggle3 ? cout << "spot ON\n" : cout << "spot OFF\n";
			break;
		case '5':
			shader_toggle = !shader_toggle;
			if (shader_toggle)
				Shader shader = Shader("diffuse_shading.vert", "diffuse_shading.frag", true);
			else
				Shader shader = Shader("diffuse_shading.vert", "diffuse_shading.frag", false);
			break;
		case 'm':
			freeze = !freeze;
			freeze ? cout << "freeze!\n" : cout << "...unfreeze\n";
			break;
	}
}

void Window::processSpecialKeys(int key, int x, int y)
{
	shape.getModelMatrix().identity();
	shape.getCameraMatrix().identity();
	switch (key) 
	{	
		case GLUT_KEY_F1:
			// cube
			shape_key = 1;
			shape.getModelMatrix().identity();
			shape.getModelMatrix() = shape.getModelMatrix().multiply(shape.setScaleMatrix(8.0));
			shape.getCameraMatrix().identity();
			break;
		case GLUT_KEY_F2:
			// dragon
			shape_key = 2;
			shape.getModelMatrix().identity();
			shape.getModelMatrix().translate(0, -5, 0);
			shape.getModelMatrix() = shape.getModelMatrix().multiply(shape.setScaleMatrix(15.0));
			shape.getCameraMatrix().identity();
			break;
		case GLUT_KEY_F3:
			// bunny
			shape_key = 3;
			shape.getModelMatrix().identity();
			shape.getModelMatrix().translate(0, 4, 0);
			shape.getModelMatrix() = shape.getModelMatrix().multiply(shape.setScaleMatrix(12.0));
			shape.getCameraMatrix().identity();
			break;
		case GLUT_KEY_F4:
			// sandal
			shape_key = 4;
			shape.getModelMatrix().identity();
			shape.getModelMatrix().translate(1, 0, 0);
			shape.getModelMatrix() = shape.getModelMatrix().multiply(shape.setScaleMatrix(4.0));
			shape.getCameraMatrix().identity();
			break;
		case GLUT_KEY_F8:
			// house view1
			shape_key = 8;
			shape.getCameraMatrix() = Matrix4::createCameraMatrix(Vector3(0, 10, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
			break;
		case GLUT_KEY_F9:
			// show house view2
			shape_key = 9;
			shape.getCameraMatrix() = Matrix4::createCameraMatrix(Vector3(-15, 5, 10), Vector3(-5, 0, 0), Vector3(0, 1, 0.5));
			break;
	}
	shape.updateModelViewMatrix();
}

void Window::processMouseClick(int button, int state, int x, int y) {
		switch (button) {
			case GLUT_LEFT_BUTTON:
				if (state == GLUT_DOWN) {
					left_clicked = true;
					x_mouse = x;
					y_mouse = y;
				}
				else
					left_clicked = false;
				break;

			case GLUT_RIGHT_BUTTON:
				if (state == GLUT_DOWN) {
					right_clicked = true;
					x_mouse = x;
					y_mouse = y;
				}
				else
					right_clicked = false;
				break;
		}
}

void Window::processMouseMove(int x, int y) {
	  if (!freeze) {
			if (left_clicked) { // rotate modelview
				if (x != x_mouse || y != y_mouse) {
					shape.getModelViewMatrix().trackballRotation(Window::width, Window::height, x_mouse, y_mouse, x, y);
					x_mouse = x;
					y_mouse = y;
				}
			}
			else if (right_clicked) { // zoom modelview
				if (y < y_mouse) {
					shape.getModelViewMatrix().scale(1.05,1.05,1.05);
					y_mouse = y;
				}
				else if (y > y_mouse) {
					shape.getModelViewMatrix().scale(0.95, 0.95, 0.95);
					y_mouse = y;
				}
			}
		}
		else {
			if (left_clicked) { // rotate light
				if (x != x_mouse || y != y_mouse) {
					if (y < y_mouse)
						theta -= 100*acos((x*x_mouse + y*y_mouse)/(sqrtf(x*x + y*y) * sqrtf(x_mouse*x_mouse + y_mouse*y_mouse)));
					else if (y > y_mouse)
						theta += 100*acos((x*x_mouse + y*y_mouse)/(sqrtf(x*x + y*y) * sqrtf(x_mouse*x_mouse + y_mouse*y_mouse)));

					if (theta > 360) theta = 0;
					if (theta < 0) theta = 360;

					x_mouse = x;
					y_mouse = y;
				}
			}
			else if (right_clicked) { // zoom light
				float y_diff = ((float)(y - y_mouse))/height;

				float y_scale = 1+y_diff;
				cout << y_scale << "\n";
				
				d_position[1]*=y_scale;
				d_position[2]*=y_scale;
				shape.directional.setPosition(d_position);

				p_position[0]*=y_scale;
				p_position[1]*=y_scale;
				shape.point.setPosition(p_position);

				s_position[0]*=y_scale;
				s_position[2]*=y_scale;
				shape.spot.setPosition(s_position);

				y_mouse = y;
			}
		}
}