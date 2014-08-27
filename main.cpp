#pragma once

#define WIN32_LEAN_AND_MEAN		
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <glut.h>
#include <windows.h>
#include <math.h>
#include <iostream>
//#include "RandomColor.h"

#define SCREEN_W 1024
#define SCREEN_H 768
#define X .525731112119133606 
#define Z .850650808352039932

#define Ia 1.0
#define Id 1.0

int windowH, windowW;

static GLfloat vdata[12][3] = {    
    {-X, 0.0, Z}, {X, 0.0, Z}, {-X, 0.0, -Z}, {X, 0.0, -Z},    
    {0.0, Z, X}, {0.0, Z, -X}, {0.0, -Z, X}, {0.0, -Z, -X},    
    {Z, X, 0.0}, {-Z, X, 0.0}, {Z, -X, 0.0}, {-Z, -X, 0.0} 
};
static GLuint tindices[20][3] = { 
    {0,4,1}, {0,9,4}, {9,5,4}, {4,5,8}, {4,8,1},    
    {8,10,1}, {8,3,10}, {5,3,8}, {5,2,3}, {2,7,3},    
    {7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6}, 
    {6,1,10}, {9,0,11}, {9,11,2}, {9,2,5}, {7,2,11} };

static GLfloat light_pos[3] = {0.25f,1.0f,0.0f};
static GLfloat view_pos[3]  = {0.0f,0.0f,-4.5f};
static GLfloat amb_color[3] = {0.8f,0.8f,0.8f};
static GLfloat dif_color[3] = {1.0f,1.0f,1.0f};
static GLfloat obj_color[3] = {0.0f,0.0f,1.0f};
static int vertexRender = 0;
//static RandomColor Cor = RandomColor();
static float Ka = .5f;
static float Kd = .5f;
static float Ks = .5f;
static int nShiny = 1;

void ProcessMenu(int value){
	// Save menu identifier as method flag
	vertexRender = value;
	    
	// Trigger a redraw
	glutPostRedisplay();
}

void normalize(GLfloat *a) {
    GLfloat d=sqrt(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]);
    a[0]/=d; a[1]/=d; a[2]/=d;
}

float calcMagnitude(GLfloat *vetor) {
	float magnitude = sqrt(pow(vetor[0],2) + pow(vetor[1],2) + pow(vetor[2],2));
	return magnitude;
}

float dotProduct(GLfloat *vetor1, GLfloat *vetor2) {
	float mag_vetor1 = calcMagnitude(vetor1);
    float mag_vetor2 = calcMagnitude(vetor2);

	float angulo = (vetor1[0]*vetor2[0] + vetor1[1]*vetor2[1] + vetor1[2]*vetor2[2])/(mag_vetor1*mag_vetor2);

	return angulo;
}

void calcColor(GLfloat *vetn, GLfloat x, GLfloat y, GLfloat z){
	//Passos para o cálculo de iluminação:
	//1) cor do objeto
	//2) ka = [0, 1]
	//3) Se vertices está iluminado (menos de 90 graus)
	//4) kd = [0, 1]
	//5) atenuação atmosférica (cálculo de dist.)
	//6) Se vertice está iluminado (menos de 90 graus). 
	//7) shininess, quanto maior, menor a região e mais intensa
	//8) soma tudo!

	// aqui vai a resolução da equação conforme visto em aula.

	float r = 0.2F;
	float g = 0.8F;
	float b = 0.9F;
	
	r = max(0, min(1,r));
	g = max(0, min(1,g));
	b = max(0, min(1,b));
	
	GLfloat lp[3] = {light_pos[0]-x, light_pos[1]-y, light_pos[2]-z};
	
	r = r * dotProduct(lp, vetn) * Ka * Kd * Ks;
	g = g * dotProduct(lp, vetn) * Ka * Kd * Ks;
	b = b * dotProduct(lp, vetn) * Ka * Kd * Ks;

	/*
	*****Forma certa de implementar preciso melhorar esse codigo um pouco(MUITO)
	
	GLfloat s[3] = {light_pos[0]-x, light_pos[1]-y, light_pos[2]-z};
	
	GLfloat d = calcMagnitude(s);
	GLfloat ad = dotProduct(s, vetn);
	GLfloat fatt = 1 / (d*d);
		

	r = r * d * ad * fatt;
	g = g * d * ad * fatt;
	b = b * d * ad * fatt;
	*/

	glColor3f(r,g,b);
}

void drawtri(GLfloat *a, GLfloat *b, GLfloat *c, int div, float r, GLfloat *trans_vet) {
	if (div<=0) {
		calcColor(a, a[0]*r+trans_vet[0], a[1]*r+trans_vet[1], a[2]*r+trans_vet[2]);
        	glNormal3fv(a); 
		glVertex3f( a[0]*r+trans_vet[0], a[1]*r+trans_vet[1], a[2]*r+trans_vet[2]);
		if (vertexRender) 
			calcColor(b, b[0]*r+trans_vet[0], b[1]*r+trans_vet[1], b[2]*r+trans_vet[2]);
	      glNormal3fv(b); glVertex3f( b[0]*r+trans_vet[0], b[1]*r+trans_vet[1], b[2]*r+trans_vet[2]);

		if (vertexRender) 
			calcColor(c, c[0]*r+trans_vet[0], c[1]*r+trans_vet[1], c[2]*r+trans_vet[2]);
	
		glNormal3fv(c); 
		glVertex3f( c[0]*r+trans_vet[0], c[1]*r+trans_vet[1], c[2]*r+trans_vet[2]);
      } else {
        GLfloat ab[3], ac[3], bc[3];
        for (int i=0;i<3;i++) {
            ab[i]=(a[i]+b[i])/2;
            ac[i]=(a[i]+c[i])/2;
            bc[i]=(b[i]+c[i])/2;
        }
        normalize(ab); normalize(ac); normalize(bc);
        drawtri(a, ab, ac, div-1, r,trans_vet);
        drawtri(b, bc, ab, div-1, r,trans_vet);
        drawtri(c, ac, bc, div-1, r,trans_vet);
        drawtri(ab, bc, ac, div-1, r,trans_vet); 
    }  
}

void drawsphere(int ndiv, float radius, GLfloat *trans_vet) {
    glBegin(GL_TRIANGLES);
    for (int i=0;i<20;i++)
        drawtri(vdata[tindices[i][0]], vdata[tindices[i][1]], vdata[tindices[i][2]], ndiv, radius, trans_vet);
    glEnd();
}

void Reshape(int w, int h){
}

void Display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();

	glLoadIdentity();
	gluPerspective(45.0f, (float)windowW/windowH, 1.0f, 100.0f);
	gluLookAt(view_pos[0], view_pos[1], view_pos[2], 
		    0.0f       , 0.0f       , 0.0f, 
     		    0.0f       , 1.0f       , 0.0f);

	glPopMatrix();

	glLoadIdentity();

	/*
	 * Desenha Ponto de Luz
	 */
	glColor3f(1.0f,1.0f,1.0f);
	glPushMatrix();
	glTranslatef(light_pos[0],light_pos[1], light_pos[2]);
	glutSolidSphere(0.1-(1.1+light_pos[2])/20,15,15);
	glPopMatrix();

	/*
	 * Desenha Esferas
	 */
	GLfloat translate[3] = {-0.5f,0.0f,1.0f};
	drawsphere(3, 0.3, translate);

	GLfloat translate2[3] = {0.5f,0.0f,0.0f};
	drawsphere(3, 0.4, translate2);

	glFinish();
	glutSwapBuffers();
}

void Key(unsigned char key, int x, int y) {
	switch(key) {
		case 'd': case 'D':
			light_pos[0] += .1f;
			break;
		case 'a': case 'A':
			light_pos[0] -= .1f;
			break;
		case 'w': case 'W':
			light_pos[1] += .1f;
			break;
		case 's': case 'S':
			light_pos[1] -= .1f;
			break;
		case 'e': case 'E':
			light_pos[2] += .1f;
			break;
		case 'q': case 'Q':
			light_pos[2] -= .1f;
			break;
		case 'v': case 'V':
			vertexRender = true;
			break;
		case 'f': case 'F':
			vertexRender = false;
			break;
		case ',':
			if (Ka > 0.0f) Ka -= .1f;
			break;
		case '.':
			if (Ka < 1.0f) Ka += .1f;
			break;
		case '-':
			if (Kd > 0.0f) Kd -= .1f;
			break;
		case '+':
			if (Kd < 1.0f) Kd += .1f;
			break;
		case '/':
			if (Ks > 0.0f) Ks -= .1f;
			break;
		case '*':
			if (Ks < 1.0f) Ks += .1f;
			break;
		case ']':
			if (nShiny > 1) nShiny -= 1;
			break;
		case '[':
			if (nShiny < 200) nShiny += 1;
			break;
		// presets
		case '1':
			Kd = 0.0;
			Ks = 0.0;
			nShiny = 0;
			break;
		case '2':
			Kd = 1.0;
			Ks = 1.0;
			nShiny = 10;
			break;
		case 27:
			exit(0);
	}
}

bool InitGL(){
	/*
	 * Carrega matrix identidade
	 */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	/*
	 * Parametros de shade
	 */
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glColor4f(1.0f,1.0f,1.0f,1.0f);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	/*
	 * Parêmetros de profundidade
	 */
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glEnable(GL_NORMALIZE); // para normalizar os vetores normais de objetos

	return true;
}


int main(int argc, char** argv){
	windowW = 640;
	windowH = 480;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(windowW, windowH);
	glutInitWindowPosition((int)(SCREEN_H/2 - windowH/2), (int)(SCREEN_W/2 - windowW/2));
	glutCreateWindow("Iluminação na mão grande");

	// Cria menu
	glutCreateMenu(ProcessMenu);
	glutAddMenuEntry("Face Normals (F)",0);
	glutAddMenuEntry("Vertex Normals (V)",1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	if (!InitGL()) {return 1;}

	glutDisplayFunc(Display);
	glutIdleFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Key);

	glutMainLoop();
	return 0;
}
