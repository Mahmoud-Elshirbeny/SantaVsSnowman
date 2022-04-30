#include <math.h>
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <iostream>
#include <glut.h>

#define GLUT_KEY_ESCAPE 27
#define DEG2RAD(a) (a * 0.0174532925)


class Vector3f {
public:
	float x, y, z;

	Vector3f(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) {
		x = _x;
		y = _y;
		z = _z;
	}

	Vector3f operator+(Vector3f& v) {
		return Vector3f(x + v.x, y + v.y, z + v.z);
	}

	Vector3f operator-(Vector3f& v) {
		return Vector3f(x - v.x, y - v.y, z - v.z);
	}

	Vector3f operator*(float n) {
		return Vector3f(x * n, y * n, z * n);
	}

	Vector3f operator/(float n) {
		return Vector3f(x / n, y / n, z / n);
	}

	Vector3f unit() {
		return *this / sqrt(x * x + y * y + z * z);
	}

	Vector3f cross(Vector3f v) {
		return Vector3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
};


class Camera {
public:
	Vector3f eye, center, up;

	Camera(float eyeX = 25.0f, float eyeY = 2.0f, float eyeZ = 0.0f, float centerX = 0.0f, float centerY = 0.0f, float centerZ = 0.0f, float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f) {
		eye = Vector3f(eyeX, eyeY, eyeZ);
		center = Vector3f(centerX, centerY, centerZ);
		up = Vector3f(upX, upY, upZ);
	}

	void moveX(float d) {
		Vector3f right = up.cross(center - eye).unit();
		eye = eye + right * d;
		center = center + right * d;
	}

	void moveY(float d) {
		eye = eye + up.unit() * d;
		center = center + up.unit() * d;
	}

	void moveZ(float d) {
		Vector3f view = (center - eye).unit();
		eye = eye + view * d;
		center = center + view * d;
	}

	void rotateX(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + up * sin(DEG2RAD(a));
		up = view.cross(right);
		center = eye + view;
	}

	void rotateY(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + right * sin(DEG2RAD(a));
		right = view.cross(up);
		center = eye + view;
	}

	void look() {
		gluLookAt(
			eye.x, eye.y, eye.z,
			center.x, center.y, center.z,
			up.x, up.y, up.z
		);
	}

	void setView(float x,float y,float z) {
		
			eye = Vector3f(x, y, z);
			center = Vector3f(0, 0, 0);
			up = Vector3f(0, 1, 0);
	}
};


Camera camera;


void setupLights() {
	GLfloat ambient[] = { 0.7f, 0.7f, 0.7, 1.0f };
	GLfloat diffuse[] = { 0.6f, 0.6f, 0.6, 1.0f };
	GLfloat specular[] = { 1.0f, 1.0f, 1.0, 1.0f };
	GLfloat shininess[] = { 50 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

	GLfloat lightIntensity[] = { 0.7f, 0.7f, 1, 1.0f };
	GLfloat lightPosition[] = { -7.0f, 6.0f, 3.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightIntensity);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);
}


void setupCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 640 / 480, 0.001, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera.look();
}


//general
int timer = 20000;
bool won = false;
bool lost = false;
bool gameover = false;

bool animTree = false;
bool animSnowman = false;
bool animLanterns = false;
bool animTori = false;
bool animFence = false;


//ground
void drawGround() {
	glPushMatrix();
	glColor3f(4, 4, 5);
	glTranslated(0, -1, 0);
	glScaled(50, 2, 50);
	glutSolidCube(1);
	glPopMatrix();
}

//fence
float fc1 = 0;
float fc2 = 0;
float fc3 = 0; 

float sc1 = 2;
float sc2 = 2;
float sc3 = 0.5;

double fenceHeight = 0;

void drawFence() {
	//side1
	glColor3f(fc1, fc2, fc3);

	//box1
	glPushMatrix();
	glTranslated(0, fenceHeight, 0);
	glTranslated(23.75, 2.5, 23.75);
	glScaled(2.5, 5, 2.5);
	glutSolidCube(1);
	glPopMatrix();

	//box2
	glPushMatrix();
	glTranslated(0, fenceHeight, 0);
	glTranslated(11.7, 2, 24.5);
	glScaled(22, 1, 1);
	glutSolidCube(1);
	glPopMatrix();

	//box3
	glPushMatrix();
	glTranslated(0, fenceHeight, 0);
	glTranslated(0, 2.5, 23.75);
	glScaled(2.5, 5, 2.5);
	glutSolidCube(1);
	glPopMatrix();

	//box4
	glPushMatrix();
	glTranslated(0, fenceHeight, 0);
	glTranslated(-10.625, 2, 24.5);
	glScaled(25, 1, 1);
	glutSolidCube(1);
	glPopMatrix();

	//box5
	glPushMatrix();
	glTranslated(0, fenceHeight, 0);
	glTranslated(-23.75, 2.5, 23.75);
	glScaled(2.5, 5, 2.5);
	glutSolidCube(1);
	glPopMatrix();


	glColor3f(sc1, sc2, sc3);
	//sphere1
	glPushMatrix();
	glTranslated(0, fenceHeight, 0);

	glTranslated(23.75, 6, 23.75);
	glutSolidSphere(1.5, 10, 10);
	glPopMatrix();

	//sphere2
	glPushMatrix();
	glTranslated(0, fenceHeight, 0);

	glTranslated(0, 6, 23.75);
	glutSolidSphere(1.5, 10, 10);
	glPopMatrix();

	//sphere3
	glPushMatrix();
	glTranslated(0, fenceHeight, 0);

	glTranslated(-23.75, 6, 23.75);
	glutSolidSphere(1.5, 10, 10);
	glPopMatrix();


	//side2
	glColor3f(fc1, fc2, fc3);

	//box1
	glPushMatrix();
	glTranslated(0, fenceHeight, 0);

	glTranslated(-23.75, 2.5, 23.75);
	glScaled(2.5, 5, 2.5);
	glutSolidCube(1);
	glPopMatrix();

	//box2
	glPushMatrix();
	glTranslated(0, fenceHeight, 0);

	glTranslated(-24.5, 2,  11.7);
	glScaled(1, 1, 22);
	glutSolidCube(1);
	glPopMatrix();

	//box3
	glPushMatrix();
	glTranslated(0, fenceHeight, 0);

	glTranslated(-23.75, 2.5,  0);
	glScaled(2.5, 5, 2.5);
	glutSolidCube(1);
	glPopMatrix();

	//box4
	glPushMatrix();
	glTranslated(0, fenceHeight, 0);

	glTranslated(-24.5, 2,-10.625);
	glScaled(1, 1,  25);
	glutSolidCube(1);
	glPopMatrix();

	//box5
	glPushMatrix();
	glTranslated(0, fenceHeight, 0);

	glTranslated(-23.75, 2.5,  - 23.75);
	glScaled(2.5, 5, 2.5);
	glutSolidCube(1);
	glPopMatrix();


	glColor3f(sc1, sc2, sc3);
	//sphere1
	glPushMatrix();
	glTranslated(0, fenceHeight, 0);

	glTranslated(-23.75, 6, 23.75);
	glutSolidSphere(1.5, 10, 10);
	glPopMatrix();

	//sphere2
	glPushMatrix();
	glTranslated(0, fenceHeight, 0);

	glTranslated(-23.75, 6, 0);
	glutSolidSphere(1.5, 10, 10);
	glPopMatrix();

	//sphere3
	glPushMatrix();
	glTranslated(0, fenceHeight, 0);

	glTranslated(-23.75, 6, -23.75);
	glutSolidSphere(1.5, 10, 10);
	glPopMatrix();


	//side3
	glColor3f(fc1, fc2, fc3);

	//box1
	glPushMatrix();
	glTranslated(0, fenceHeight, 0);

	glTranslated(23.75, 2.5, -23.75);
	glScaled(2.5, 5, 2.5);
	glutSolidCube(1);
	glPopMatrix();

	//box2
	glPushMatrix();
	glTranslated(0, fenceHeight, 0);

	glTranslated(11.7, 2, -24.5);
	glScaled(22, 1, 1);
	glutSolidCube(1);
	glPopMatrix();

	//box3
	glPushMatrix();
	glTranslated(0, fenceHeight, 0);

	glTranslated(0, 2.5, -23.75);
	glScaled(2.5, 5, 2.5);
	glutSolidCube(1);
	glPopMatrix();

	//box4
	glPushMatrix();
	glTranslated(0, fenceHeight, 0);

	glTranslated(-10.625, 2, -24.5);
	glScaled(25, 1, 1);
	glutSolidCube(1);
	glPopMatrix();

	//box5
	glPushMatrix();
	glTranslated(0, fenceHeight, 0);

	glTranslated(-23.75, 2.5, -23.75);
	glScaled(2.5, 5, 2.5);
	glutSolidCube(1);
	glPopMatrix();


	glColor3f(sc1, sc2, sc3);
	//sphere1
	glPushMatrix();
	glTranslated(0, fenceHeight, 0);

	glTranslated(23.75, 6, -23.75);
	glutSolidSphere(1.5, 10, 10);
	glPopMatrix();

	//sphere2
	glPushMatrix();
	glTranslated(0, fenceHeight, 0);

	glTranslated(0, 6, -23.75);
	glutSolidSphere(1.5, 10, 10);
	glPopMatrix();

	//sphere3
	glPushMatrix();
	glTranslated(0, fenceHeight, 0);

	glTranslated(-23.75, 6, -23.75);
	glutSolidSphere(1.5, 10, 10);
	glPopMatrix();


}

//christmas trees

double rotationTree = 0;

void drawTrees() {

	//tree1

	//stem
	glPushMatrix();
	glColor3d(0.2, 0.2, 0.2);
	
	glRotated(rotationTree, 0, 1, 0);

	glTranslated(-18, 6, 18); // cylinder at (0,0,1)  
	glRotated(90, 1, 0, 0);
	GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);  //GLU_FILL, GLU_SILHOUETTE ,GLU_POINT
	gluCylinder(qobj, 1, 1, 8, 100, 100);
	glPopMatrix();

	//cone1
	glPushMatrix();
	glColor3d(0.9, 0.2, 0.2);

	glRotated(rotationTree, 0, 1, 0);

	glTranslated(-18, 4, 18); 
	glRotated(-90, 1, 0, 0);
	glutSolidCone(7, 10, 50, 50);
	glPopMatrix();

	//cone2
	glPushMatrix();
	glColor3d(0.8, 0.2, 0.2);
	

	glRotated(rotationTree, 0, 1, 0);

	glTranslated(-18, 8, 18);
	glRotated(-90, 1, 0, 0);
	glutSolidCone(5, 10, 50, 50);
	glPopMatrix();

	//cone3
	glPushMatrix();
	
	glColor3d(0.7, 0.2, 0.2);
	glRotated(rotationTree, 0, 1, 0);

	glTranslated(-18, 12, 18);
	glRotated(-90, 1, 0, 0);
	glutSolidCone(3.5, 10, 50, 50);
	glPopMatrix();



	//tree2

	//stem
	glPushMatrix();
	glColor3d(0.2, 0.2, 0.2);

	glRotated(rotationTree, 0, 1, 0);

	glTranslated(-18, 6, -18); // cylinder at (0,0,1)  
	glRotated(90, 1, 0, 0);
	GLUquadricObj* qobj2;
	qobj2 = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);  //GLU_FILL, GLU_SILHOUETTE ,GLU_POINT
	gluCylinder(qobj, 1, 1, 8, 100, 100);
	glPopMatrix();

	//cone1
	glPushMatrix();
	glColor3d(0.9, 0.2, 0.2);

	glRotated(rotationTree, 0, 1, 0);

	glTranslated(-18, 4, -18);
	glRotated(-90, 1, 0, 0);
	glutSolidCone(7, 10, 50, 50);
	glPopMatrix();

	//cone2
	glPushMatrix();
	glColor3d(0.8, 0.2, 0.2);


	glRotated(rotationTree, 0, 1, 0);

	glTranslated(-18, 8, -18);
	glRotated(-90, 1, 0, 0);
	glutSolidCone(5, 10, 50, 50);
	glPopMatrix();

	//cone3
	glPushMatrix();

	glColor3d(0.7, 0.2, 0.2);
	glRotated(rotationTree, 0, 1, 0);

	glTranslated(-18, 12, -18);
	glRotated(-90, 1, 0, 0);
	glutSolidCone(3.5, 10, 50, 50);
	glPopMatrix();


}

//toriGate
double rotationTori = 0;
void drawTori() {

	
	//blackboot1
	glPushMatrix();
	
	glColor3f(0, 0, 0);

	glRotated(rotationTori, 0, 1, 0);
	
	glTranslated(-12, 1, 6); 
	
	glutSolidCube(2);
	glPopMatrix();


	//blackboot2
	glPushMatrix();

	glColor3f(0, 0, 0);

	glRotated(rotationTori, 0, 1, 0);

	glTranslated(-12, 1, -6);

	glutSolidCube(2);
	glPopMatrix();

	//red column1
	glPushMatrix();

	glColor3f(0.9, 0.3, 0.3);

	glRotated(rotationTori, 0, 1, 0);

	glTranslated(-12, 7.5, -6);
	glScaled(1, 15, 1);
	glutSolidCube(1);
	glPopMatrix();

	//red column2
	glPushMatrix();

	glColor3f(0.9, 0.3, 0.3);

	glRotated(rotationTori, 0, 1, 0);

	glTranslated(-12, 7.5, 6);
	glScaled(1, 15, 1);
	glutSolidCube(1);
	glPopMatrix();

	//red column3
	glPushMatrix();

	glColor3f(0.9, 0.3, 0.3);

	glRotated(rotationTori, 0, 1, 0);

	glTranslated(-12, 15, 0);
	glScaled(1, 3, 1);
	glutSolidCube(1);
	glPopMatrix();

	//red row 1
	glPushMatrix();

	glColor3f(0.9, 0.3, 0.3);

	glRotated(rotationTori, 0, 1, 0);

	glTranslated(-12, 14, 0);
	glScaled(1, 1, 14);
	glutSolidCube(1);
	glPopMatrix();

	//red row 2
	glPushMatrix();

	glColor3f(0.9, 0.3, 0.3);

	glRotated(rotationTori, 0, 1, 0);

	glTranslated(-12, 16, 0);
	glScaled(1, 1, 12);
	glutSolidCube(1);
	glPopMatrix();

	//black row 1
	glPushMatrix();

	glColor3f(0, 0, 0);

	glRotated(rotationTori, 0, 1, 0);

	glTranslated(-12, 17, 0);
	glScaled(1, 1, 16);
	glutSolidCube(1);
	glPopMatrix();
}

//draw Snow man

double scaleSnow = 2;
void drawSnow() {

	//snowman
	
	//blob1
	glPushMatrix();
	glColor3f(4, 4, 5);

	glScaled(scaleSnow, scaleSnow, scaleSnow);

	glTranslated(0, 1.5, 0);

	glutSolidSphere(3, 10, 10);
	glPopMatrix();

	//blob2
	glPushMatrix();
	glColor3f(4, 4, 5);

	glScaled(scaleSnow, scaleSnow, scaleSnow);

	glTranslated(0, 5, 0);

	glutSolidSphere(2.25, 10, 10);
	glPopMatrix();

	//blob3
	glPushMatrix();
	glColor3f(4, 4, 5);

	glScaled(scaleSnow, scaleSnow, scaleSnow);

	glTranslated(0, 8, 0);

	glutSolidSphere(1.5, 10, 10);
	glPopMatrix();

	//eye1
	glPushMatrix();
	glColor3f(0, 0, 0);

	glScaled(scaleSnow, scaleSnow, scaleSnow);

	glTranslated(1.4, 8.5, 0.5);

	glutSolidSphere(0.125, 10, 10);
	glPopMatrix();

	//eye2
	glPushMatrix();
	glColor3f(0, 0, 0);

	glScaled(scaleSnow, scaleSnow, scaleSnow);

	glTranslated(1.4, 8.5, -0.5);

	glutSolidSphere(0.125, 10, 10);
	glPopMatrix();

	//nose
	glPushMatrix();
	glColor3f(2, 1, 0);

	glScaled(scaleSnow, scaleSnow, scaleSnow);

	glTranslated(1.2, 8.2, 0);
	glRotated(90, 0, 1, 0);

	glutSolidCone(0.2, 1.5, 10, 8);
	glPopMatrix();

	//mouth1
	glPushMatrix();
	glColor3f(0, 0, 0);

	glScaled(scaleSnow, scaleSnow, scaleSnow);

	glTranslated(1.3, 8, -0.5);

	glutSolidSphere(0.1, 10, 10);
	glPopMatrix();

	//mouth2
	glPushMatrix();
	glColor3f(0, 0, 0);

	glScaled(scaleSnow, scaleSnow, scaleSnow);

	glTranslated(1.3, 8, 0.5);

	glutSolidSphere(0.1, 10, 10);
	glPopMatrix();

	//mouth3
	glPushMatrix();
	glColor3f(0, 0, 0);

	glScaled(scaleSnow, scaleSnow, scaleSnow);

	glTranslated(1.35, 7.8, 0.35);

	glutSolidSphere(0.1, 10, 10);
	glPopMatrix();

	//mouth4
	glPushMatrix();
	glColor3f(0, 0, 0);

	glScaled(scaleSnow, scaleSnow, scaleSnow);

	glTranslated(1.35, 7.8, -0.35);

	glutSolidSphere(0.1, 10, 10);
	glPopMatrix();

	//mouth5
	glPushMatrix();
	glColor3f(0, 0, 0);

	glScaled(scaleSnow, scaleSnow, scaleSnow);

	glTranslated(1.4, 7.6, -0.1);

	glutSolidSphere(0.1, 10, 10);
	glPopMatrix();

	//mouth6
	glPushMatrix();
	glColor3f(0, 0, 0);

	glScaled(scaleSnow, scaleSnow, scaleSnow);

	glTranslated(1.4, 7.6, 0.1);

	glutSolidSphere(0.1, 10, 10);
	glPopMatrix();
}

//lanterns

double lanternheight1 = 0;
double lanternheight2 = 0;
double lanternheight3 = 0;

void drawLanterns() {
	
	//lantern1
	
	//yellow part
	glPushMatrix();
	glColor3f(4, 2, 0);

	glTranslated(0, lanternheight1, 0);

	glTranslated(12.5, 1, 0);

	glRotated(-90, 1, 0, 0);
	GLUquadricObj* qobj3;
	qobj3 = gluNewQuadric();
	gluQuadricDrawStyle(qobj3, GLU_FILL);  //GLU_FILL, GLU_SILHOUETTE ,GLU_POINT
	gluCylinder(qobj3, 1, 1, 3, 8, 5);

	glPopMatrix();

	//blackbottom
	glPushMatrix();
	glColor3f(0, 0, 0);

	glTranslated(0, lanternheight1, 0);

	glTranslated(12.5, 0.5, 0);

	glRotated(-90, 1, 0, 0);
	GLUquadricObj* qobj4;
	qobj4 = gluNewQuadric();
	gluQuadricDrawStyle(qobj4, GLU_FILL);  //GLU_FILL, GLU_SILHOUETTE ,GLU_POINT
	gluCylinder(qobj4, 0.5, 0.5, 0.5, 8, 5);

	glPopMatrix();

	//blacktop
	glPushMatrix();
	glColor3f(0, 0, 0);

	glTranslated(0, lanternheight1, 0);

	glTranslated(12.5, 4, 0);

	glRotated(-90, 1, 0, 0);
	GLUquadricObj* qobj5;
	qobj5 = gluNewQuadric();
	gluQuadricDrawStyle(qobj5, GLU_FILL);  //GLU_FILL, GLU_SILHOUETTE ,GLU_POINT
	gluCylinder(qobj5, 0.5, 0.5, 0.5, 8, 5);

	glPopMatrix();

	//torus
	glPushMatrix();
	glColor3f(0, 0, 0);

	glTranslated(0, lanternheight1, 0);

	glTranslated(12.5, 4.5, 0);

	glRotated(-90, 1, 0, 0);
	glRotated(90.0, 1, 0, 0);
	glutSolidTorus(0.1, 0.3, 10, 10);

	glPopMatrix();


	//lantern2

	//yellow part
	glPushMatrix();
	glColor3f(4, 2, 0);

	glTranslated(0, lanternheight2, 0);

	glTranslated(0, 1, 12.5);

	glRotated(-90, 1, 0, 0);
	GLUquadricObj* qobj8;
	qobj8 = gluNewQuadric();
	gluQuadricDrawStyle(qobj8, GLU_FILL);  //GLU_FILL, GLU_SILHOUETTE ,GLU_POINT
	gluCylinder(qobj8, 1, 1, 3, 8, 5);

	glPopMatrix();

	//blackbottom
	glPushMatrix();
	glColor3f(0, 0, 0);

	glTranslated(0, lanternheight2, 0);

	glTranslated(0, 0.5, 12.5);

	glRotated(-90, 1, 0, 0);
	GLUquadricObj* qobj7;
	qobj7 = gluNewQuadric();
	gluQuadricDrawStyle(qobj7, GLU_FILL);  //GLU_FILL, GLU_SILHOUETTE ,GLU_POINT
	gluCylinder(qobj7, 0.5, 0.5, 0.5, 8, 5);

	glPopMatrix();

	//blacktop
	glPushMatrix();
	glColor3f(0, 0, 0);

	glTranslated(0, lanternheight2, 0);

	glTranslated(0, 4, 12.5);

	glRotated(-90, 1, 0, 0);
	GLUquadricObj* qobj6;
	qobj6 = gluNewQuadric();
	gluQuadricDrawStyle(qobj6, GLU_FILL);  //GLU_FILL, GLU_SILHOUETTE ,GLU_POINT
	gluCylinder(qobj6, 0.5, 0.5, 0.5, 8, 5);

	glPopMatrix();

	//torus
	glPushMatrix();
	glColor3f(0, 0, 0);

	glTranslated(0, lanternheight2, 0);

	glTranslated(0, 4.5, 12.5);

	glRotated(-90, 1, 0, 0);
	glRotated(90.0, 1, 0, 0);
	glutSolidTorus(0.1, 0.3, 10, 10);

	glPopMatrix();


	//lantern3

	//yellow part
	glPushMatrix();
	glColor3f(4, 2, 0);

	glTranslated(0, lanternheight3, 0);

	glTranslated(0, 1, -12.5);

	glRotated(-90, 1, 0, 0);
	GLUquadricObj* a;
	a = gluNewQuadric();
	gluQuadricDrawStyle(a, GLU_FILL);  //GLU_FILL, GLU_SILHOUETTE ,GLU_POINT
	gluCylinder(a, 1, 1, 3, 8, 5);

	glPopMatrix();

	//blackbottom
	glPushMatrix();
	glColor3f(0, 0, 0);

	glTranslated(0, lanternheight3, 0);

	glTranslated(0, 0.5, -12.5);

	glRotated(-90, 1, 0, 0);
	GLUquadricObj* b;
	b = gluNewQuadric();
	gluQuadricDrawStyle(b, GLU_FILL);  //GLU_FILL, GLU_SILHOUETTE ,GLU_POINT
	gluCylinder(b, 0.5, 0.5, 0.5, 8, 5);

	glPopMatrix();

	//blacktop
	glPushMatrix();
	glColor3f(0, 0, 0);

	glTranslated(0, lanternheight3, 0);

	glTranslated(0, 4, -12.5);

	glRotated(-90, 1, 0, 0);
	GLUquadricObj* c;
	c = gluNewQuadric();
	gluQuadricDrawStyle(c, GLU_FILL);  //GLU_FILL, GLU_SILHOUETTE ,GLU_POINT
	gluCylinder(c, 0.5, 0.5, 0.5, 8, 5);

	glPopMatrix();

	//torus
	glPushMatrix();
	glColor3f(0, 0, 0);

	glTranslated(0, lanternheight3, 0);

	glTranslated(0, 4.5, -12.5);

	glRotated(-90, 1, 0, 0);
	glRotated(90.0, 1, 0, 0);
	glutSolidTorus(0.1, 0.3, 10, 10);

	glPopMatrix();

}

//player
double playerx = 22;
double playerz = 0;
int playerdirection = 1;
double playerRotation = 0;

void drawPlayer() {


	//head
	glPushMatrix();
	glTranslated(playerx, 0, playerz);
	glRotated(playerRotation, 0, 1, 0);
	glTranslated(-playerx, 0, -playerz);


	glColor3f(2.41,2.12, 2.08);

	glTranslated(playerx,2 , playerz);

	glutSolidSphere(0.5, 50, 50);

	glPopMatrix();

	//body
	glPushMatrix();
	glColor3f(2, 0.2, 0.3);
	glTranslated(playerx, 0, playerz);
	glRotated(playerRotation, 0, 1, 0);
	glTranslated(-playerx, 0, -playerz);

	glTranslated(playerx, 1, playerz);

	glScaled(0.3, 1, 1);
	glutSolidCube(1.0);

	glPopMatrix();

	//leg1

	glPushMatrix();
	glColor3f(0.5, 0.1, 0.1);
	glTranslated(playerx, 0, playerz);
	glRotated(playerRotation, 0, 1, 0);
	glTranslated(-playerx, 0, -playerz);

	glTranslated(playerx, 0.25, playerz+0.35);

	glScaled(0.3, 0.5, 0.3);
	glutSolidCube(1.0);

	glPopMatrix();

	//leg1

	glPushMatrix();
	glColor3f(0.5, 0.1, 0.1);
	glTranslated(playerx, 0, playerz);
	glRotated(playerRotation, 0, 1, 0);
	glTranslated(-playerx, 0, -playerz);

	glTranslated(playerx, 0.25, playerz - 0.35);

	glScaled(0.3, 0.5, 0.3);
	glutSolidCube(1.0);

	glPopMatrix();


	//hat
	glPushMatrix();

	glColor3f(2, 0.2, 0.3);
	glTranslated(playerx, 0, playerz);
	glRotated(playerRotation, 0, 1, 0);
	glTranslated(-playerx, 0, -playerz);

	glTranslated(playerx, 2.2, playerz);
	glRotated(-90, 1, 0, 0);
	glutSolidCone(0.5, 1.4, 50, 50);
	glPopMatrix();

	//arm1
	glPushMatrix();
	glColor3f(2, 0.2, 0.3);
	glTranslated(playerx, 0, playerz);
	glRotated(playerRotation, 0, 1, 0);
	glTranslated(-playerx, 0, -playerz);

	glTranslated(playerx, 1.15, playerz + 0.5);

	glScaled(0.3, 0.7, 0.3);
	glutSolidCube(1.0);

	glPopMatrix();

	//arm2
	glPushMatrix();
	glColor3f(2, 0.2, 0.3);
	glTranslated(playerx, 0, playerz);
	glRotated(playerRotation, 0, 1, 0);
	glTranslated(-playerx, 0, -playerz);

	glTranslated(playerx, 1.15, playerz - 0.5);

	glScaled(0.3, 0.7, 0.3);
	glutSolidCube(1.0);

	glPopMatrix();

	//eye1
	glPushMatrix();
	glColor3f(0, 0, 0);

	glTranslated(playerx, 0, playerz);
	glRotated(playerRotation, 0, 1, 0);
	glTranslated(-playerx, 0, -playerz);

	glTranslated(playerx-0.45, 2, playerz+0.2);

	glScaled(0.3, 0.7, 0.3);
	glutSolidSphere(0.1, 10, 10);

	glPopMatrix();

	//eye1
	glPushMatrix();
	glColor3f(0, 0, 0);

	glTranslated(playerx, 0, playerz);
	glRotated(playerRotation, 0, 1, 0);
	glTranslated(-playerx, 0, -playerz);

	glTranslated(playerx - 0.45, 2, playerz - 0.2);

	glScaled(0.3, 0.7, 0.3);
	glutSolidSphere(0.1, 10, 10);

	glPopMatrix();

	//hat ball

	glPushMatrix();
	glColor3f(10, 10, 10);

	glTranslated(playerx, 0, playerz);
	glRotated(playerRotation, 0, 1, 0);
	glTranslated(-playerx, 0, -playerz);

	glTranslated(playerx, 3.6, playerz);

	glutSolidSphere(0.075, 10, 10);

	glPopMatrix();

	//shirt ball1

	glPushMatrix();
	glColor3f(10, 10, 10);

	glTranslated(playerx, 0, playerz);
	glRotated(playerRotation, 0, 1, 0);
	glTranslated(-playerx, 0, -playerz);

	glTranslated(playerx-0.15, 0.8, playerz);

	glScaled(0.3, 0.3, 0.3);
	glutSolidSphere(0.08, 10, 10);

	glPopMatrix();

	//shirt ball2

	glPushMatrix();
	glColor3f(10, 10, 10);

	glTranslated(playerx, 0, playerz);
	glRotated(playerRotation, 0, 1, 0);
	glTranslated(-playerx, 0, -playerz);

	glTranslated(playerx - 0.15, 1, playerz);

	glScaled(0.3, 0.3, 0.3);
	glutSolidSphere(0.08, 10, 10);

	glPopMatrix();

	//shirt ball3

	glPushMatrix();
	glColor3f(10, 10, 10);

	glTranslated(playerx, 0, playerz);
	glRotated(playerRotation, 0, 1, 0);
	glTranslated(-playerx, 0, -playerz);

	glTranslated(playerx - 0.15, 1.2, playerz);

	glScaled(0.3, 0.3, 0.3);
	glutSolidSphere(0.08, 10, 10);

	glPopMatrix();

}


//

void Display() {
	setupCamera();
	setupLights();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawFence();
	drawGround();
	drawTrees();
	drawTori();
	drawSnow();
	drawLanterns();
	drawPlayer();

	glFlush();
}

void Keyboard(unsigned char key, int x, int y) {
	float d = 1;
	if (!gameover) {

	switch (key) {
	case 'z':
		camera.moveY(d);
		break;
	case 'x':
		camera.moveY(-d);
		break;
	case 'c':
		camera.moveX(d);
		break;
	case 'v':
		camera.moveX(-d);
		break;
	case 'b':
		camera.moveZ(d);
		break;
	case 'n':
		camera.moveZ(-d);
		break;

	case 'j':
		camera.setView(5,70,0.1);
		break;
	case 'k':
		camera.setView(70,0.1,0.1);
		break;
	case 'l':
		camera.setView(0.1,0.1,70);
		break;
	case '1':
		if (animTree == true) {
			animTree = false;
		}
		else {
			animTree = true;
		}
		break;

	case '2':
		if (animTori == true) {
			animTori = false;
		}
		else {
			animTori = true;
		}
		break;

	case '3':
		if (animSnowman == true) {
			animSnowman = false;
		}
		else {
			animSnowman = true;
		}
		break;

	case '4':
		if (animLanterns == true) {
			animLanterns = false;
		}
		else {
			animLanterns = true;
		}
		break;

	case '5':
		if (animFence == true) {
			animFence = false;
		}
		else {
			animFence = true;
		}
		break;

	case 'w':
		playerRotation = 0;
		if (playerx > -22) {
			playerx -= 0.2;
			camera.moveZ(0.2);
			camera.moveY(0.02);
		}
		break;

	case 'a':
		playerRotation = 90;
		if (playerz < 22) {
			playerz += 0.2;
			camera.moveX(0.2);
		}
		break;
	case 'd':
		playerRotation = -90;
		if (playerz > -22) {
			playerz -= 0.2;
			camera.moveX(-0.2);
		}
		break;

	case 's':
		playerRotation = 180;
		if (playerx < 22) {
			playerx += 0.2;
			camera.moveZ(-0.2);
			camera.moveY(-0.02);
		}
		break;

	case GLUT_KEY_ESCAPE:
		exit(EXIT_SUCCESS);
	}
		glutPostRedisplay();
	
	}
}
void Special(int key, int x, int y) {
	float a = 2.0;

	if (!gameover) {
		switch (key) {
		case GLUT_KEY_UP:
			camera.rotateX(a);
			break;
		case GLUT_KEY_DOWN:
			camera.rotateX(-a);
			break;
		case GLUT_KEY_LEFT:
			camera.rotateY(a);
			break;
		case GLUT_KEY_RIGHT:
			camera.rotateY(-a);
			break;
		}

		glutPostRedisplay();
	}
}

void Anim() {
	if (!gameover) {

		if (fc1 < 1.5) {
			fc1 += 0.001;
			fc2 += 0.001;
			fc3 += 0.001;
		}
		else {
			fc1 = 0;
			fc2 = 0;
			fc3 = 0;

		}

		if (sc1 > 0.5) {
			sc1 -= 0.001;
			sc2 -= 0.001;
			sc3 -= 0.001;
		}
		else {
			sc1 = 2;
			sc2 = 2;
			sc3 = 0.5;


		}


		if (animTree == true) {
			rotationTree += 0.03;
		}

		if (animTori == true) {
			rotationTori += 0.03;
		}
		if (animSnowman == true) {
			if (scaleSnow < 3) {
				scaleSnow += 0.001;
			}
			else {
				scaleSnow = 1.5;
			}
		}

		if (animLanterns == true) {
			if (lanternheight1 < 4) {
				lanternheight1 += 0.005;
			}
			else {
				lanternheight1 = 0;
			}

			if (lanternheight2 < 4) {
				lanternheight2 += 0.005;
			}
			else {
				lanternheight2 = 0;
			}

			if (lanternheight3 < 4) {
				lanternheight3 += 0.005;
			}
			else {
				lanternheight3 = 0;
			}
		}

		if (animFence == true) {
			if (fenceHeight < 5) {
				fenceHeight += 0.005;
			}
			else {
				fenceHeight = 0;
			}
		}

		timer -= 0.0001;

		if (timer <= 0) {
			lost = true;
		}
		//collison with lant1
		if (playerx <= 15 && playerx >= 11) {
			if (playerz <= 3 && playerz >= -3) {
				lanternheight1 = -10000;

			}
		}


		//collison with lant2
		if (playerz <= 15 && playerz >= 11) {
			if (playerx <= 3 && playerx >= -3) {
				lanternheight2 = -10000;
			}
		}

		//collison with lant3
		if (playerz >= -15 && playerz <= -11) {
			if (playerx <= 3 && playerx >= -3) {
				lanternheight3 = -10000;
			}
		}

		if (lanternheight1 <0 && lanternheight2 <0 && lanternheight3 <0) {
			won = true;
		}

		if (lost || won) {
			gameover = true;
			camera.setView(35, 35, 35);
			if (won) {
				PlaySound(TEXT("won.wav"), NULL, SND_ASYNC);
			}
			else {
				PlaySound(TEXT("lost.wav"), NULL, SND_ASYNC);
			}
		}

		
	}
	else {
		if (won) {
			if (scaleSnow > 0) {
				scaleSnow -= 0.0003;
			}
		}
		else {
			if (scaleSnow < 8) {
				scaleSnow += 0.0014;
			}
		}

	}
	glutPostRedisplay();
}

void main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitWindowSize(1000, 700);
	glutInitWindowPosition(200, 50);
	PlaySound(TEXT("background.wav"), NULL, SND_ASYNC);
	glutCreateWindow("Merry Christmas");
	glutDisplayFunc(Display);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(Special);
	glutIdleFunc(Anim);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	glutMainLoop();
}

