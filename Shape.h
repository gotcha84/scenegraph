#ifndef _SHAPE_H_
#define _SHAPE_H_


#include "shader.h"
#include "Vector3.h"
#include "Matrix4.h"
#include "Light.h"
#include "Material.h"
#include "objreader.h"
#include "MatrixTransform.h"
#include "Sphere.h"
#include "Cube.h"
#include "Cone.h"
#include "Torus.h"
#include "Tetrahedron.h"

class Shape
{
	public:
		Matrix4 model;
		Matrix4 camera;
		Matrix4 modelview;
		
		Light directional;
		Light point;
		Light spot;

		Matrix4 translation;
		Matrix4 scale;

		double angle;
		double x, y, z; // center
		float scaling_x, scaling_y, scaling_z;
		
		Shape();
		Shape(Matrix4&);
		Matrix4& getCameraMatrix();
		Matrix4& getModelMatrix();
		Matrix4& getModelViewMatrix();
		void updateModelViewMatrix();
		void viewFrustumCulling(MatrixTransform*);
		void drawArmy();
		double getAngle();
		void setAngle(double);
		Matrix4 setScaleMatrix(float);
    void spin(double);
		void drawHouse();
		void drawRobot();
		void drawRobot2();
		void calculateStuff(int, float*);
		void loadData();
		void computeBoundingBox(vector<vector<MatrixTransform*>>, int);
};

class Window	  // output window related routines
{
  public:
    static int width, height; 	            // window size

    static void idleCallback(void);
    static void reshapeCallback(int, int);
    static void displayCallback(void);
	//static void viewFrustumCulling(void);
		static void drawShape(int nVerts, float* vertices, float* normals);
		static void processNormalKeys(unsigned char, int, int);
		static void processSpecialKeys(int, int, int);
		static void processMouseClick(int, int, int, int);
		static void processMouseMove(int, int);
		static void drawCube();
		static void drawRobot();
		static void drawDirectionalLight();
		static void drawPointLight();
		static void drawSpotLight();
};

// house
static float house_vertices[] = {  
						-4,-4,4, 4,-4,4, 4,4,4, -4,4,4,     // front face
						-4,-4,-4, -4,-4,4, -4,4,4, -4,4,-4, // left face
						4,-4,-4,-4,-4,-4, -4,4,-4, 4,4,-4,  // back face
						4,-4,4, 4,-4,-4, 4,4,-4, 4,4,4,     // right face
						4,4,4, 4,4,-4, -4,4,-4, -4,4,4,     // top face
						-4,-4,4, -4,-4,-4, 4,-4,-4, 4,-4,4, // bottom face

						-20,-4,20, 20,-4,20, 20,-4,-20, -20,-4,-20, // grass
						-4,4,4, 4,4,4, 0,8,4,                       // front attic wall
						4,4,4, 4,4,-4, 0,8,-4, 0,8,4,               // left slope
						-4,4,4, 0,8,4, 0,8,-4, -4,4,-4,             // right slope
						4,4,-4, -4,4,-4, 0,8,-4											// rear attic wall
};
static float house_colors[] = { 
            1,0,0, 1,0,0, 1,0,0, 1,0,0,  // front is red
            0,1,0, 0,1,0, 0,1,0, 0,1,0,  // left is green
            1,0,0, 1,0,0, 1,0,0, 1,0,0,  // back is red
            0,1,0, 0,1,0, 0,1,0, 0,1,0,  // right is green
            0,0,1, 0,0,1, 0,0,1, 0,0,1,  // top is blue
            0,0,1, 0,0,1, 0,0,1, 0,0,1,  // bottom is blue
  
            0,0.5,0, 0,0.5,0, 0,0.5,0, 0,0.5,0, // grass is dark green
            0,0,1, 0,0,1, 0,0,1,                // front attic wall is blue
            1,0,0, 1,0,0, 1,0,0, 1,0,0,         // left slope is green
            0,1,0, 0,1,0, 0,1,0, 0,1,0,         // right slope is red
            0,0,1, 0,0,1, 0,0,1,	              // rear attic wall is red
};
static int house_indices[] = {
            0,2,3,    0,1,2,      // front face
            4,6,7,    4,5,6,      // left face
            8,10,11,  8,9,10,     // back face
            12,14,15, 12,13,14,   // right face
            16,18,19, 16,17,18,   // top face
            20,22,23, 20,21,22,   // bottom face
                   
            24,26,27, 24,25,26,   // grass
            28,29,30,             // front attic wall
            31,33,34, 31,32,33,   // left slope
            35,37,38, 35,36,37,   // right slope
            39,40,41	            // rear attic wall
};
static int house_nVerts = sizeof(house_vertices)/sizeof(house_vertices[0]);
static int house_nIndices = sizeof(house_indices)/sizeof(house_indices[0]);

// dragon
static int dragon_nVerts;
static float *dragon_vertices;
static float *dragon_normals;
static float *dragon_texcoords;
static int dragon_nIndices;
static int *dragon_indices;

// bunny
static int bunny_nVerts;
static float *bunny_vertices;
static float *bunny_normals;
static float *bunny_texcoords;
static int bunny_nIndices;
static int *bunny_indices;

// sandal
static int sandal_nVerts;
static float *sandal_vertices;
static float *sandal_normals;
static float *sandal_texcoords;
static int sandal_nIndices;
static int *sandal_indices;

#endif