#include <GL/glut.h>
#include <GL/gl.h>
#include <string>
#include <fstream>
#include "vector3.h"
#include <vector>
#include <iostream>
#include <cfloat>
#include <cmath>
#include "BMPLoader.h"
#include <algorithm>

#define PI 3.14159265
//derlerken g++ hw2.cpp -lGLU -lGL -lglut

using namespace std;
vector<Vector3> envir; //pdfteki sıra ile
vector<Vector3> road;
vector<Vector3> lights;
//textures
GLuint trear, troad, tgrass, tsky, tlight;
float posx = 0, posy, posz = 0, ptx = 1, pty, ptz = 0, ux = 0, uy = 1, uz = 0;

Vector3 rght;
bool isDay = true;
bool* keyStates = new bool[256];
bool* keySpecialStates = new bool[256];

float floorP[4], leftP[4], rightP[4], nearP[4], farP[4]; // A B C D respectively
float height; // value for posy

bool isCr = false;

float xrot = 0, yrot = 0, angle = 0;

int jCount = -1, cCount = -1;

float lasty;
float aR = 1, aG = 1, aB = 1;
float nR = 1, nG = 1, nB = 1;

//GLfloat position0[] = {1000.0, 1000.0 , 1000.0, 0.0};
//GLfloat blackColor0[] = {0.1 , 0.1 , 0.1 , 1.0};
//~ GLfloat whiteColor0[] = {1.0 , 1.0 , 1.0 , 1.0};


//walking axis
float walkZ, walkX;

Vector3 Rot(Vector3 u, Vector3 tor, float deg){
	Vector3 ret;
	float cs = cos(deg * PI / 180);
	float sn = sin(deg * PI / 180);
	u.normalize();
	ret._x = tor._x*(cs + (u._x * u._x)*(1 - cs)) + tor._y*((u._x * u._y)*(1 - cs) - (u._z*sn)) + tor._z*((u._x * u._z*(1-cs)) + u._y * sn);
	ret._y = tor._x*((u._x * u._y)*(1 - cs)+(u._z*sn)) + tor._y*(cs + (u._y * u._y)*(1 - cs)) + tor._z*((u._y * u._z*(1-cs)) - u._x * sn);
	ret._z = tor._x*((u._x * u._z*(1-cs)) - u._y * sn) + tor._y*((u._y * u._z*(1-cs)) + u._x * sn) + tor._z*(cs + (u._z * u._z)*(1 - cs));
	return ret;
}
GLuint LoadTexture( const char * filename, int width, int height ) {
    GLuint texture;
   

    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

 
	BMPClass bmp;
	BMPLoad(filename,bmp);
	glTexImage2D(GL_TEXTURE_2D,0,3,bmp.width,bmp.height,0,GL_RGB,GL_UNSIGNED_BYTE,bmp.bytes);
    return texture;
}
void planeCreator(){
	floorP[0] = envir[1]._y * (envir[2]._z - envir[3]._z) + envir[2]._y * (envir[3]._z - envir[1]._z) + envir[3]._y * (envir[1]._z - envir[2]._z);
	floorP[1] = envir[1]._z * (envir[2]._x - envir[3]._x) + envir[2]._z * (envir[3]._x - envir[1]._x) + envir[3]._z * (envir[1]._x - envir[2]._x);
	floorP[2] = envir[1]._x * (envir[2]._y - envir[3]._y) + envir[2]._x * (envir[3]._y - envir[1]._y) + envir[3]._x * (envir[1]._y - envir[2]._y);
	floorP[3] = -1 * ( envir[1]._x * (envir[2]._y * envir[3]._z - envir[3]._y * envir[2]._z) + envir[2]._x * (envir[3]._y * envir[1]._z - envir[1]._y * envir[3]._z) + envir[3]._x * (envir[1]._y * envir[2]._z - envir[2]._y * envir[1]._z));
	
	leftP[0] = envir[0]._y * (envir[3]._z - envir[7]._z) + envir[3]._y * (envir[7]._z - envir[0]._z) + envir[7]._y * (envir[0]._z - envir[3]._z);
	leftP[1] = envir[0]._z * (envir[3]._x - envir[7]._x) + envir[3]._z * (envir[7]._x - envir[0]._x) + envir[7]._z * (envir[0]._x - envir[3]._x);
	leftP[2] = envir[0]._x * (envir[3]._y - envir[7]._y) + envir[3]._x * (envir[7]._y - envir[0]._y) + envir[7]._x * (envir[0]._y - envir[3]._y);
	leftP[3] = -1 * ( envir[0]._x * (envir[3]._y * envir[7]._z - envir[7]._y * envir[3]._z) + envir[3]._x * (envir[7]._y * envir[0]._z - envir[0]._y * envir[7]._z) + envir[7]._x * (envir[0]._y * envir[3]._z - envir[3]._y * envir[0]._z));
	
	rightP[0] = envir[1]._y * (envir[2]._z - envir[6]._z) + envir[2]._y * (envir[6]._z - envir[1]._z) + envir[6]._y * (envir[1]._z - envir[2]._z);
	rightP[1] = envir[1]._z * (envir[2]._x - envir[6]._x) + envir[2]._z * (envir[6]._x - envir[1]._x) + envir[6]._z * (envir[1]._x - envir[2]._x);
	rightP[2] = envir[1]._x * (envir[2]._y - envir[6]._y) + envir[2]._x * (envir[6]._y - envir[1]._y) + envir[6]._x * (envir[1]._y - envir[2]._y);
	rightP[3] = -1 * ( envir[1]._x * (envir[2]._y * envir[6]._z - envir[6]._y * envir[2]._z) + envir[2]._x * (envir[6]._y * envir[1]._z - envir[1]._y * envir[6]._z) + envir[6]._x * (envir[1]._y * envir[2]._z - envir[2]._y * envir[1]._z));
	
	nearP[0] = envir[1]._y * (envir[5]._z - envir[4]._z) + envir[5]._y * (envir[4]._z - envir[1]._z) + envir[4]._y * (envir[1]._z - envir[5]._z);
	nearP[1] = envir[1]._z * (envir[5]._x - envir[4]._x) + envir[5]._z * (envir[4]._x - envir[1]._x) + envir[4]._z * (envir[1]._x - envir[5]._x);
	nearP[2] = envir[1]._x * (envir[5]._y - envir[4]._y) + envir[5]._x * (envir[4]._y - envir[1]._y) + envir[4]._x * (envir[1]._y - envir[5]._y);
	nearP[3] = -1 * ( envir[1]._x * (envir[5]._y * envir[4]._z - envir[4]._y * envir[5]._z) + envir[5]._x * (envir[4]._y * envir[1]._z - envir[1]._y * envir[4]._z) + envir[4]._x * (envir[1]._y * envir[5]._z - envir[5]._y * envir[1]._z));
	
	farP[0] = envir[6]._y * (envir[2]._z - envir[3]._z) + envir[2]._y * (envir[3]._z - envir[6]._z) + envir[3]._y * (envir[6]._z - envir[2]._z);
	farP[1] = envir[6]._z * (envir[2]._x - envir[3]._x) + envir[2]._z * (envir[3]._x - envir[6]._x) + envir[3]._z * (envir[6]._x - envir[2]._x);
	farP[2] = envir[6]._x * (envir[2]._y - envir[3]._y) + envir[2]._x * (envir[3]._y - envir[6]._y) + envir[3]._x * (envir[6]._y - envir[2]._y);
	farP[3] = -1 * ( envir[6]._x * (envir[2]._y * envir[3]._z - envir[3]._y * envir[2]._z) + envir[2]._x * (envir[3]._y * envir[6]._z - envir[6]._y * envir[3]._z) + envir[3]._x * (envir[6]._y * envir[2]._z - envir[2]._y * envir[6]._z));
		
	//~ cout << leftP[0] << " " << leftP[1] << " " << leftP[2] << " " << leftP[3] << " " <<  endl;
	//~ cout<< "right " << rightP[0] << " " << rightP[1] << " " << rightP[2] << " " << rightP[3] << " " <<  endl;
	//~ cout<< "near  " << nearP[0] << " " << nearP[1] << " " << nearP[2] << " " << nearP[3] << " " <<  endl;
	//~ cout<< "farP  " << farP[0] << " " << farP[1] << " " << farP[2] << " " << farP[3] << " " <<  endl;
}

float findY(float x, float z, float plane[]){
	float y = (plane[3] + plane[2] * z + plane[0] * x) / plane[1];
	return -y;
}
float findX(float y, float z, float plane[]){
	float x = (plane[3] + plane[2] * z + plane[1] * y) / plane[0];
	return -x;
}
float findZ(float x, float y, float plane[]){
	float z = (plane[3] + plane[1] * y + plane[0] * x) / plane[2];
	return -z;
}
bool out(){
	float xR = findX(posy,posz, rightP);
	float xL = findX(posy, posz, leftP);

	if (posx > xR -10 || posx < xL + 10)
		return true;
		
	float zN = findZ(posx,posy, nearP);
	float zF = findZ(posx, posy, farP);

	if (posz > zN - 10|| posz < zF + 10)
		return true;
		
	return false;
}
bool srt(Vector3 a, Vector3 b){
	float i = (a._x - posx) * (a._x - posx) + (a._z - posz) * (a._z - posz);
	float j = (b._x - posx) * (b._x - posx) + (b._z - posz) * (b._z - posz);
	
	return (i < j);
}

void camMoveFwd(){	
	Vector3 pos(posx, posy, posz);
	Vector3 pt(ptx, pty, ptz);
	Vector3 viewing = pt - pos;
	
	Vector3 Walking(viewing._x, (walkX * viewing._x) + (walkZ * viewing._z), viewing._z);
	Walking.normalize();
	Vector3 temp(posx, posy, posz);
	
	posx += Walking._x * 2;
	posy += Walking._y * 2;
	posz += Walking._z * 2;
	
	if(out())
	{
		posx = temp._x;
		posy = temp._y;
		posz = temp._z;
	}
	else
	{
		ptx = ptx + Walking._x * 2;
		pty = pty + Walking._y * 2;
		ptz = ptz  + Walking._z * 2;
	}
}
void camMoveBck(){	
	Vector3 pos(posx, posy, posz);
	Vector3 pt(ptx, pty, ptz);
	Vector3 viewing = pt - pos;
	
	Vector3 Walking(viewing._x, (walkX * viewing._x) + (walkZ * viewing._z), viewing._z);
	Walking.normalize();
	Vector3 temp(posx, posy, posz);
	
	posx -= Walking._x * 2;
	posy -= Walking._y * 2;
	posz -= Walking._z * 2;
	
	if(out())
	{
		posx = temp._x;
		posy = temp._y;
		posz = temp._z;
	}
	else
	{
		ptx = ptx + Walking._x * 2;
		pty = pty + Walking._y * 2;
		ptz = ptz  + Walking._z * 2;
	}
}
void camLeft(){
	Vector3 pos(posx, posy, posz);
	Vector3 pt(ptx, pty, ptz);
	Vector3 up(ux, uy, uz);
	Vector3 viewing = pt - pos;
	rght = (viewing).cross(up);
	
	Vector3 Walking(rght._x, (walkX * rght._x) + (walkZ * rght._z), rght._z);
	Walking.normalize();
	Vector3 temp(posx, posy, posz);
	
	posx -= Walking._x * 2;
	posy -= Walking._y * 2;
	posz -= Walking._z * 2;
	
	if (out()) 
	{
		posx = temp._x;
		posy = temp._y;
		posz = temp._z;
	}
	
	ptx = posx + viewing._x;
	pty = posy + viewing._y;
	ptz = posz + viewing._z;
	
}
void camRight(){	
	Vector3 up(ux, uy, uz);
	Vector3 pos(posx, posy, posz);
	Vector3 pt(ptx, pty, ptz);
	Vector3 viewing = pt - pos;
	rght = (viewing).cross(up);
	
	
	Vector3 Walking(rght._x, (walkX * rght._x) + (walkZ * rght._z), rght._z);
	Walking.normalize();
	Vector3 temp(posx, posy, posz);
	
	posx += Walking._x * 2;
	posy += Walking._y * 2;
	posz += Walking._z * 2;
	
	if (out()) 
	{
		posx = temp._x;
		posy = temp._y;
		posz = temp._z;
	}
	
	ptx = posx + viewing._x;
	pty = posy + viewing._y;
	ptz = posz + viewing._z;
}

void jmp(){
	if(jCount == 0)
	{
		jCount = -1;
		return;
	}
	if (jCount >= 25)
	 {
		posy += 3;
		pty += 3;
		
	 }
	 if (jCount < 25)
	 {
		posy -= 3;
		pty -= 3;
	 }
	 jCount--;
	 
}
void crouch(){
	height = findY(posx, posz, floorP);
	if(posy > (height + 5))
	{
		posy -= 0.3;
		pty -= 0.3;
	}	
}

void incLight(){
	if(aR < 1 && aG < 1 && aB < 1)
	{
		aR += 0.01;
		aG += 0.01;
		aB += 0.01;
	}
	else
		isDay = true;
}
void decLight(){
	if(aR > 0.2 && aG > 0.2 && aB > 0.2)
	{
		aR -= 0.01;
		aG -= 0.01;
		aB -= 0.01;
	}
	else
		isDay = false;
}
void keyPressed(unsigned char key, int x, int y){
	keyStates[key] = true;
}
void keyUp(unsigned char key, int x, int y){
	keyStates[key] = false;
}
void keyOperations(){	
	if(keyStates['q'])
		exit(0);
		
	if(keyStates['w'])
	{
		camMoveFwd();
	}
	if(keyStates['s'])
	{
		camMoveBck();
	}
	if(keyStates['a'])
	{
		camLeft();
	}
	if(keyStates['d'])
	{
		camRight();
	}
	if(keyStates[32]) //Jump will be space
		if(jCount == -1)
			jCount = 50;
	if(keyStates['c']) //Crouch will be ctrl
	{
		isCr = true;
		crouch();
	}
	if(!keyStates['c'] && isCr)
	{
		posy = findY(posx, posz, floorP) + 30;
		pty = posy;
		isCr = false;
	}
	if (keyStates['m'])
	{
		isDay = true;
		aR = 1;
		aG = 1;
		aB = 1;
	}
	if (keyStates['n'])
	{
		isDay = false;
		aR = 0.2;
		aG = 0.2;
		aB = 0.2;
	}
	if (keyStates['x'])
		incLight();
	if(keyStates['z'])
		decLight();	
}


void camRotate(){
	
	Vector3 pt(ptx, pty, ptz);
	Vector3 pos(posx, posy, posz);
	Vector3 up(ux, uy, uz);
	Vector3 viewing(ptx-posx, pty-posy, ptz-posz);
	rght = (viewing).cross(up);
	Vector3 nR = Rot(up, rght, -yrot);
	rght = nR;
	Vector3 nV = Rot(up, viewing, -yrot);

	ptx = nV._x + posx;
	pty = nV._y + posy;
	ptz = nV._z + posz;	
}
void mouseMovement(int x, int y){
	if(x == 450 && y == 350)
		return;
	float diffx = x - 450;
	float diffy = y - 350;
	
	xrot += (float) diffy; //set the xrot to xrot with the addition of the difference in the y position
	if (xrot > 90 || xrot < -90)
		xrot -= diffy;
		
	yrot = (float) diffx * 0.8;// set the xrot to yrot with the addition of the difference in the x position
	camRotate();
}
void timeFunc(int b){
	glutWarpPointer(450, 350);
	keyOperations();
	if(jCount != (-1))
		jmp();
   glutPostRedisplay();
   glutTimerFunc(10, timeFunc, 1);
}
void setLight(){		
	glEnable(GL_LIGHTING);
	GLfloat whiteColor0[] = {aR , aG , aB , 1.0};
	//~ GLfloat whiteColor0[] = {1, 1, 1, 1.0};
	
	
	GLfloat global_ambient[] = { aR , aG , aB , 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
	
	if (lights.size())
	{
		//~ GLfloat position0[] = {lights[0]._x, lights[0]._y + 210, lights[0]._z};
		GLfloat position0[] = {envir[7]._x, envir[7]._y, envir[7]._z};
		glEnable(GL_LIGHT0);
		glLightfv(GL_LIGHT0, GL_POSITION , position0);
		glLightfv(GL_LIGHT0 , GL_AMBIENT , whiteColor0);
	}
	else
	{		
		glEnable(GL_LIGHTING);
		GLfloat white[] = {1, 1, 1, 1};
		
		GLfloat pos[] = {envir[7]._x, envir[7]._y, envir[7]._z};
		glEnable(GL_LIGHT1);
		glLightfv(GL_LIGHT1, GL_POSITION , pos);
		glLightfv(GL_LIGHT1 , GL_DIFFUSE , white);
	}
	
}
void sun(){
	glEnable(GL_LIGHTING);
	GLfloat white[] = {1, 1, 1, 1};
	
	GLfloat pos[] = {envir[7]._x, envir[7]._y, envir[7]._z};
	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_POSITION , pos);
	glLightfv(GL_LIGHT1 , GL_DIFFUSE , white);
	
	GLfloat pos2[] = {envir[6]._x, envir[6]._y, envir[6]._z};
	glEnable(GL_LIGHT2);
	glLightfv(GL_LIGHT2, GL_POSITION , pos2);
	glLightfv(GL_LIGHT2 , GL_DIFFUSE , white);
}
void setPtLights(){
	GLfloat ptWhiteColor[] = {0.8 ,0.8 ,0.5 , 1.0}; 
	GLfloat whiteColor0[] = {aR , aG , aB , 1.0};
	
	glEnable(GL_LIGHTING);
	
	if (lights.size() >= 8)
	{
		glEnable(GL_LIGHT0);	
		GLfloat position0[] = {lights[0]._x, lights[0]._y + 210, lights[0]._z};
		glLightfv(GL_LIGHT0, GL_POSITION , position0);
		glLightfv(GL_LIGHT0, GL_AMBIENT, whiteColor0);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, ptWhiteColor);
		
		GLfloat position1[] = {lights[1]._x, lights[1]._y + 210, lights[1]._z};
		glEnable(GL_LIGHT1);
		glLightfv(GL_LIGHT1, GL_POSITION , position1);
		glLightfv(GL_LIGHT1 , GL_DIFFUSE , ptWhiteColor);
		
		
		GLfloat position2[] = {lights[2]._x, lights[2]._y + 210, lights[2]._z};
		glEnable(GL_LIGHT2);
		glLightfv(GL_LIGHT2, GL_POSITION, position2);
		glLightfv(GL_LIGHT2, GL_DIFFUSE, ptWhiteColor);
		
		
		GLfloat position3[] = {lights[3]._x, lights[3]._y + 210, lights[3]._z};
		glEnable(GL_LIGHT3);
		glLightfv(GL_LIGHT3, GL_POSITION, position3);
		glLightfv(GL_LIGHT3, GL_DIFFUSE, ptWhiteColor);
		
		GLfloat position4[] = {lights[4]._x, lights[4]._y + 210, lights[4]._z};
		glEnable(GL_LIGHT4);
		glLightfv(GL_LIGHT4, GL_POSITION, position4);
		glLightfv(GL_LIGHT4, GL_DIFFUSE, ptWhiteColor);
		
		
		GLfloat position5[] = {lights[5]._x, lights[5]._y + 210, lights[5]._z};
		glEnable(GL_LIGHT5);
		glLightfv(GL_LIGHT5, GL_POSITION, position5);
		glLightfv(GL_LIGHT5, GL_DIFFUSE, ptWhiteColor);
		
		GLfloat position6[] = {lights[6]._x, lights[6]._y + 210, lights[6]._z};
		glEnable(GL_LIGHT6);
		glLightfv(GL_LIGHT6, GL_POSITION, position6);
		glLightfv(GL_LIGHT6, GL_DIFFUSE, ptWhiteColor);
		
		GLfloat position7[] = {lights[7]._x, lights[7]._y + 210, lights[7]._z};
		glEnable(GL_LIGHT7);
		glLightfv(GL_LIGHT7, GL_POSITION, position7);
		glLightfv(GL_LIGHT7, GL_DIFFUSE, ptWhiteColor);
	}
	else
	{
		for (int i = 0; i < lights.size(); i++)
		{
			glEnable(GL_LIGHT0 + i);
			GLfloat pos[] = {lights[i]._x, lights[i]._y + 210, lights[i]._z};
			glLightfv(GL_LIGHT0 + i, GL_POSITION, pos);
			glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, ptWhiteColor);
		}
		
	}
	
}
void Parse(string filepath){
	fstream read;
	read.open(filepath.c_str());
	Vector3 temp1;
	string type;
	read >> type;
	for (int i = 0; i < 8; i++)
	{
		read >> temp1._x >> temp1._y >> temp1._z;
		envir.push_back(temp1);
	}
	read >> type;
	for (int i = 0; i < 4; i++)
	{
		read >> temp1._x >> temp1._y >> temp1._z;
		road.push_back(temp1);
	}
	
	read >> type;
	while(read >> temp1._x >> temp1._y >> temp1._z)
	{
		lights.push_back(temp1);
	}	
}
void reshape(int width, int height){
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);	 // Switch to the projection matrix so that we can manipulate how our scene is viewed  
	glLoadIdentity(); // Reset the projection matrix to the identity matrix so that we don't get any artifacts (cleaning up)  
	gluPerspective(60, (GLfloat)width / (GLfloat)height, 0.0001, 10000.0);       // Set the Field of view angle (in degrees), the aspect ratio of our window, and the new and far planes 
	glMatrixMode(GL_MODELVIEW);
	
}
void setCamera () {
	glRotatef(xrot,1,0,0);  //rotate our camera on the x-axis yukarı asagı
    gluLookAt(posx, posy, posz, ptx, pty, ptz, ux, uy, uz);
}
void skybox(){
	//glDisable(GL_LIGHTING);
	//glColor3f(nR,nG,nB);
	glColor3f(1, 1, 1);
	
	glBindTexture( GL_TEXTURE_2D, tsky );
    glBegin (GL_QUADS);
    glTexCoord2d(0,0); 
    glVertex3f(envir[6]._x, envir[6]._y, envir[6]._z); 
    glTexCoord2d(1,0); 
    glVertex3f(envir[7]._x, envir[7]._y, envir[7]._z); 
    glTexCoord2d(1,1); 
    glVertex3f(envir[4]._x, envir[4]._y, envir[4]._z);
    glTexCoord2d(0,1); 
    glVertex3f(envir[5]._x, envir[5]._y, envir[5]._z);
    glEnd();
    
    //arka yüz	
    glBindTexture( GL_TEXTURE_2D, trear );
    glBegin(GL_QUADS);
    glTexCoord2d(0.25,1); 
    glVertex3f(envir[6]._x, envir[6]._y, envir[6]._z);
    glTexCoord2d(0.5,1); 
    glVertex3f(envir[7]._x, envir[7]._y, envir[7]._z); 
    glTexCoord2d(0.5,0); 
    glVertex3f(envir[3]._x, envir[3]._y, envir[3]._z); 
    glTexCoord2d(0.25,0); 
    glVertex3f(envir[2]._x, envir[2]._y, envir[2]._z); 
    glEnd();
    
    //sol yüz    
    glBegin(GL_QUADS);
    glTexCoord2d(0.5,1); 
    glVertex3f(envir[7]._x, envir[7]._y, envir[7]._z); 
    glTexCoord2d(0.5,0); 
    glVertex3f(envir[3]._x, envir[3]._y, envir[3]._z); 
    glTexCoord2d(0.75,0); 
    glVertex3f(envir[0]._x, envir[0]._y, envir[0]._z);
    glTexCoord2d(0.75,1); 
    glVertex3f(envir[4]._x, envir[4]._y, envir[4]._z);
    glEnd();
    
    //ön yüz
    glBegin(GL_QUADS);    
    glTexCoord2d(1,1); 
    glVertex3f(envir[5]._x, envir[5]._y, envir[5]._z);
    glTexCoord2d(0.75,1); 
    glVertex3f(envir[4]._x, envir[4]._y, envir[4]._z);
    glTexCoord2d(0.75,0); 
    glVertex3f(envir[0]._x, envir[0]._y, envir[0]._z);
    glTexCoord2d(1,0); 
    glVertex3f(envir[1]._x, envir[1]._y, envir[1]._z);
    glEnd();
    
    //sağ yüz
    glBegin(GL_QUADS);
    glTexCoord2d(0.25,1); 
    glVertex3f(envir[6]._x, envir[6]._y, envir[6]._z);
    glTexCoord2d(0,1); 
    glVertex3f(envir[5]._x, envir[5]._y, envir[5]._z);
    glTexCoord2d(0,0); 
    glVertex3f(envir[1]._x, envir[1]._y, envir[1]._z);
    glTexCoord2d(0.25,0); 
    glVertex3f(envir[2]._x, envir[2]._y, envir[2]._z); 
    glEnd();
    
    //glColor3f(0.3, 0.3, 0.3);
    glBindTexture( GL_TEXTURE_2D, tgrass );
    glBegin (GL_QUADS);
    glTexCoord2d(0,1); 
    glVertex3f(envir[2]._x, envir[2]._y, envir[2]._z); 
    glTexCoord2d(1,1); 
    glVertex3f(envir[3]._x, envir[3]._y, envir[3]._z); 
    glTexCoord2d(1,0); 
    glVertex3f(envir[0]._x, envir[0]._y, envir[0]._z);
    glTexCoord2d(0,0); 
    glVertex3f(envir[1]._x, envir[1]._y, envir[1]._z);
    glEnd();
    
    //road    
    glColor3f(1, 1, 1);
    glBindTexture( GL_TEXTURE_2D, troad );
    glBegin (GL_QUADS);
    glTexCoord2d(0, 10);
    glVertex3f(road[0]._x, road[0]._y, road[0]._z);
    glTexCoord2d(0, 0);    
    glVertex3f(road[1]._x, road[1]._y, road[1]._z);
    glTexCoord2d(1, 0);
    glVertex3f(road[2]._x, road[2]._y, road[2]._z);
    glTexCoord2d(1, 10);
    glVertex3f(road[3]._x, road[3]._y, road[3]._z);
    glEnd();
    
    //sokak lambaları çizmek
	for (int i = 0; i < lights.size(); i++)
	{		
		glTranslatef(lights[i]._x, lights[i]._y + 210, lights[i]._z);
		glColor3f(0.75, 0.7, 0.6);
		glutSolidSphere(20, 50, 50);
		glTranslatef(-lights[i]._x, -lights[i]._y - 210, -lights[i]._z);
	}
	glColor3f(1, 1, 1);
	for (int i = 0; i < lights.size(); i++)
	{
		GLUquadric* cyl = gluNewQuadric();
		glTranslatef(lights[i]._x, lights[i]._y, lights[i]._z);
		glRotatef(-90, 1, 0, 0);
		gluQuadricTexture(cyl, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D, tlight);
		gluCylinder(cyl, 10, 3, 200, 60, 60);
		glRotatef(90, 1, 0, 0);
		glTranslatef(-lights[i]._x, -lights[i]._y, -lights[i]._z);
	}   
}
void display(void){
	//keyOperations();
	glClearColor(0.f, 0.f, 0.0f, 1.f);//clear the background window to black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();	
	setCamera();
	
	glEnable( GL_TEXTURE_2D );
	skybox();
	glDisable(GL_TEXTURE_2D);
	
	sort(lights.begin(), lights.end(), srt);
	
	if (!isDay)
	{
		nR = 1;
		nG = 1;
		nB = 1;
		setPtLights();
	}
	else
	{
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHT2);
		glDisable(GL_LIGHT3);
		glDisable(GL_LIGHT4);
		glDisable(GL_LIGHT5);
		glDisable(GL_LIGHT6);
		glDisable(GL_LIGHT7);
		nR = 0.1;
		nG = 0.1;
		nB = 0.1;
		setLight();	
	}
	setLight();		
	glutSwapBuffers();	
}
int main(int argc, char **argv){
	for (int i = 0; i < 256; i++)
		keyStates[i] = false;
	Parse(argv[1]);
	
	walkZ = (envir[0]._y - envir[3]._y) / (envir[0]._z - envir[3]._z);
	walkX = (envir[1]._y - envir[0]._y) / (envir[1]._x - envir[0]._x);
	
	planeCreator();
	height = findY(0, 0, floorP);
	posy = height + 30;
	pty = posy;
	
	glutInit (&argc, argv);	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
	
	glutDisplayFunc (display);
	glutIdleFunc (display);
	glutReshapeFunc (reshape);
	
	glutInitWindowSize(900, 700);
	glutInitWindowPosition (100, 100);
	glutCreateWindow("CENG477_HW2");
	
	
	glViewport(0, 0, (GLsizei)900, (GLsizei)700);
	glMatrixMode(GL_PROJECTION);	 // Switch to the projection matrix so that we can manipulate how our scene is viewed  
	glLoadIdentity(); // Reset the projection matrix to the identity matrix so that we don't get any artifacts (cleaning up)  
	gluPerspective(60, (GLfloat)900 / (GLfloat)700, 0.1, 10000.0);       // Set the Field of view angle (in degrees), the aspect ratio of our window, and the new and far planes 
	glMatrixMode(GL_MODELVIEW);
	//~ glViewport(0, 0, (GLsizei)900, (GLsizei)700);
	//~ glMatrixMode(GL_PROJECTION);	 // Switch to the projection matrix so that we can manipulate how our scene is viewed  
	//~ glLoadIdentity(); // Reset the projection matrix to the identity matrix so that we don't get any artifacts (cleaning up)  
	//~ gluPerspective(90, (GLfloat)900 / (GLfloat)700, 0.0001, 10000.0);       // Set the Field of view angle (in degrees), the aspect ratio of our window, and the new and far planes 
	//~ glMatrixMode(GL_MODELVIEW);
	//~ glLoadIdentity();
	
	tsky = LoadTexture("okyanustepe.bmp", 1024, 1024);
	tgrass = LoadTexture("oc.bmp", 540, 359);
	troad = LoadTexture("tile.bmp", 2000, 2000);
	tlight = LoadTexture("pbrk.bmp", 32, 128);
	trear = LoadTexture("okyanusside.bmp",1024, 256);

	
	//glutWarpPointer(450, 350);
	glutKeyboardFunc(keyPressed);
	glutKeyboardUpFunc(keyUp);
	glutPassiveMotionFunc(mouseMovement);
	
	glutTimerFunc(10, timeFunc, 1);
	glutMainLoop ();
	
	//FreeTexture(tsky);
	//FreeTexture(tgrass);
	
	return 0;
}
