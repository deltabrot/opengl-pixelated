#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <unistd.h>
#include "loader.h"
#include "framebuffer.h"

const int WIDTH = 800, HEIGHT = 600;
bool* keyStates = new bool[256];
bool* keySpecialStates = new bool[256];

unsigned int fbo;
unsigned int fbo_depth;
unsigned int fbo_texture; // The texture object to write our frame buffer object to

float xPos = 0.0f, yPos = 0.0f, zPos = -5.0f, rotation = 0.0f, rotationVert = 0.0f;
float playerSpeed = 0.2f, playerHeight = 10.0f, maxPlayerHeight = 8.0f, minPlayerHeight = 3.0f;
int currentWidth = 800, currentHeight = 600;
float resScale = 4.0f;
float x = 0.0f;
float tailX[100], tailY[100], tailZ[100];

Model knuckles("Models/Knuckles.obj"), deer("Models/lowpolydeer.obj", 200), male("Models/lowpolymale.obj", 2.0f);

void keyOperations();
void keySpecialOperations();
void keyDown(unsigned char key, int x, int y);
void keyUp(unsigned char key, int x, int y);
void keySpecialDown(int key, int x, int y);
void keySpecialUp(int key, int x, int y);
void reshape(int x, int y);
void display();
void initGL();

void display(){
	keyOperations();
	keySpecialOperations();
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glViewport(0, 0, WIDTH/resScale, HEIGHT/resScale);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glRotatef(rotationVert, 1, 0, 0);
	glRotatef(rotation, 0, 1, 0);
	glTranslatef(xPos, yPos, zPos);
	glTranslatef(0,-playerHeight,0);
	
	// Get positions for light source and tail
	GLfloat lightPos[] = {10*sinf(x++/50.0f),4.0f+(sinf(x/10.0f)*3),10*cosf(x/50.0f),1};
	
	for(int i=99;i>0;i--){
		tailX[i] = tailX[i-1];
		tailY[i] = tailY[i-1];
		tailZ[i] = tailZ[i-1];
	}
	tailX[0] = lightPos[0];
	tailY[0] = lightPos[1];
	tailZ[0] = lightPos[2];

	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	// Draw the ground
	glNormal3f(0,1,0);
	glColor3f(0.0f,0.8f,0.0f);
	float scale = 1.0f;
	int size = 30;
	for(int i=-size;i<=size;i++){
		for(int j=-size;j<size;j++){
			glBegin(GL_QUADS);
			glVertex3f(i/scale,0,j/scale);
			glVertex3f((i+1)/scale,0,j/scale);
			glVertex3f((i+1)/scale,0,(j+1)/scale);
			glVertex3f(i/scale,0,(j+1)/scale);
			glEnd();
		}
	}
	// Draw the ceiling
	glNormal3f(0,-1,0);
	glColor3f(0.0f, 0.0f,0.8f);
	for(int i=-size;i<=size;i++){
		for(int j=-size;j<size;j++){
			glBegin(GL_QUADS);
			glVertex3f(i/scale,11,j/scale);
			glVertex3f((i+1)/scale,11,j/scale);
			glVertex3f((i+1)/scale,11,(j+1)/scale);
			glVertex3f(i/scale,11,(j+1)/scale);
			glEnd();
		}
	}

	// Draw imported models
	// Ugandan Knuckles
	glColor3f(1.0f,0.0f,0.0f);
	glRotatef(x, 0, 1.0f, 0);
	knuckles.draw();
	glRotatef(-x, 0, 1.0f, 0);

	// Low poly deer
	glColor3f(1.0f,0.0f,1.0f);
	glTranslatef(10,0,0);
	glRotatef(180, 0, 1.0f, 0);
	deer.draw();
	glRotatef(-180, 0, 1.0f, 0);
	glTranslatef(-10,0,0);

	// Low poly male	
	glColor3f(0.0f,1.0f,1.0f);
	glTranslatef(-10,0,0);
	male.draw();
	glTranslatef(10,0,0);

	// Draw light source and tail
	glDisable(GL_LIGHTING);
	glPointSize(1.5f);
	glColor3f(1.0f, 1.0f,1.0f);
	glBegin(GL_POINTS);
	glVertex3f(lightPos[0], lightPos[1], lightPos[2]);
	glEnd();
	
	for(int i=0;i<99;i++){
		glColor4f(1.0f,1.0f,0.0f,pow(2,(99-i)/100.0f)-1.0f);
		glBegin(GL_LINES);
		glVertex3f(tailX[i], tailY[i], tailZ[i]);
		glVertex3f(tailX[i+1], tailY[i+1], tailZ[i+1]);
		glEnd();
	}
	glEnable(GL_LIGHTING);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0,0,WIDTH/resScale,HEIGHT/resScale,0,0,currentWidth,currentHeight,GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glutSwapBuffers();
}

int main (int argc, char **argv){
	for(int i=0;i<100;i++){
		tailX[i] = 0;
		tailY[i] = 4;
		tailZ[i] = 10;
	}
	glutInit(&argc, argv);
	initGL();
	if (GLEW_OK != glewInit()) {
		std::cout << "Couldn't initialize GLEW" << std::endl;
		exit(0);
	}
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	initFrameBuffer(&fbo, &fbo_depth, &fbo_texture);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glutMainLoop();
}	

void reshape(int width, int height){
	currentWidth = width;
	currentHeight = height;
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(75, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0);
	glMatrixMode(GL_MODELVIEW);
}

void initGL(){
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition((1400-WIDTH)/2.0f, (1050-HEIGHT)/2.0f);
	glutCreateWindow("Cel Shading - OpenGL");

	glutDisplayFunc(display);
	glutIdleFunc(glutPostRedisplay);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);
	glutSpecialFunc(keySpecialDown);
	glutSpecialUpFunc(keySpecialUp);
}

void keyOperations(){
	if(keyStates['w']){
		xPos -= playerSpeed*sinf((M_PI/180)*rotation);
		zPos += playerSpeed*cosf((M_PI/180)*rotation);
	}
	if(keyStates['s']){
		xPos += playerSpeed*sinf((M_PI/180)*rotation);
		zPos -= playerSpeed*cosf((M_PI/180)*rotation);
	}
	if(keyStates['a']){
		xPos += playerSpeed*sinf((M_PI/180)*(rotation+90));
		zPos -= playerSpeed*cosf((M_PI/180)*(rotation+90));
	}
	if(keyStates['d']){
		xPos -= playerSpeed*sinf((M_PI/180)*(rotation+90));
		zPos += playerSpeed*cosf((M_PI/180)*(rotation+90));	
	}
	if(keyStates['c']){
		playerHeight -= (playerHeight - minPlayerHeight)/10.0f;
	}
	else{
		playerHeight += (maxPlayerHeight - playerHeight)/10.0f;
	}
}

void keySpecialOperations(){
	if(keySpecialStates[GLUT_KEY_LEFT]){
		rotation -= 2.5f;
	}
	if(keySpecialStates[GLUT_KEY_RIGHT]){
		rotation += 2.5f;
	}
	if(keySpecialStates[GLUT_KEY_UP]){
		rotationVert -= 2.5f;
	}
	if(keySpecialStates[GLUT_KEY_DOWN]){
		rotationVert += 2.5f;
	}
	if(rotationVert > 90){
		rotationVert = 90;
	}
	if(rotationVert < -90){
		rotationVert = -90;
	}
}

void keyDown(unsigned char key, int x, int y){
	keyStates[key] = true;
}

void keyUp(unsigned char key, int x, int y){
	keyStates[key] = false;
}
void keySpecialDown(int key, int x, int y){
	keySpecialStates[key] = true;
}

void keySpecialUp(int key, int x, int y){
	keySpecialStates[key] = false;
}
