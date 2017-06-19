#pragma comment(lib, "glaux")
#pragma comment(lib, "legacy_stdio_definitions.lib")
#include <Windows.h>
#include <iostream>
#include <stdio.h>
#include <GL/glaux.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glut.h>
#pragma warning(disable:4996)

/*
	BAEKSEUNGJIN, CHOISUNGWOO
*/

void init(void);
void display(void);
void reshape(int w, int h);

GLfloat color[4];

int static_X, static_Y;
void mouseMotion(int x, int y);
void OnMouseClick(int button, int state, int xx, int yy);
void keyboard(unsigned char key, int x, int y);
void TurnOnLight(void);
int loadGLTextures(void);
AUX_RGBImageRec *loadBMP(char *filename);

float click_rotation_X;
float click_rotation_Y;
float click_rotation_Z;
bool left_click;
float zoom;
GLuint texture[2];
GLUquadricObj *head;
GLUquadricObj *body;
GLubyte *pBytes;
BITMAPINFO *info;

void TurnOnLight(void)
{
	glShadeModel(GL_SMOOTH);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	float ambientLight[] = { 1.0f, 1.0f, 1.0f, 0.2f };
	float diffuseLight[] = { 1.0f, 1.0f, 1.0f, 0.2f };
	float specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float lightPosition[] = { 10.0f, 0.0f, 0.0f, 1.0f };
	float spotlightPosition[] = { 6.0f, 0.5f, 0.0f, 1.0f };
	float spotlightDirection[] = { -1.0f, 0.0f, -1.0f };

	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight); 
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	glEnable(GL_LIGHT0);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 40.0f);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 30.0f);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotlightDirection);

}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(600, 600);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Santa Claus");
	init();

	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(OnMouseClick);
	glutMotionFunc(mouseMotion);
	glutMainLoop();

	return 0;
}

void init(void)
{
	zoom = 0;
	left_click = false;

	click_rotation_X = 0.0;
	click_rotation_Y = 0.0;

	glClearColor(0.8, 0.8, 0.8, 0.0);
	TurnOnLight();
	loadGLTextures();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}


void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_LIGHT0);
	glScalef(1 + zoom, 1 + zoom, 1 + zoom);
	glDisable(GL_LIGHTING);

	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glEnable(GL_LIGHTING);

	GLUquadric *qobj = gluNewQuadric();
	gluQuadricNormals(qobj, GLU_SMOOTH);
	gluQuadricDrawStyle(qobj, GLU_FILL);

	glRotatef(click_rotation_X, 1, 0, 0);
	glRotatef(click_rotation_Y, 0, 1, 1);
	color[0] = 1.0;
	color[1] = 1.0;
	color[2] = 1.0;
	color[3] = 0.0;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
	glLightfv(GL_LIGHT0, GL_AMBIENT, color);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, color);

	GLUquadricObj *obj;
	obj = gluNewQuadric();
	gluQuadricDrawStyle(obj, GLU_FILL);
	gluQuadricNormals(obj, GL_SMOOTH);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glRotatef(270.0, 1.0, 0.0, 0.0);
	gluSphere(head, 0.3, 50, 30);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glDisable(GL_TEXTURE_2D);

	glPushMatrix();
	glTranslatef(0.0, -0.5, 0.0);
	glRotatef(270.0, 1.0, 0.0, 0.0);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	gluCylinder(body, 0.23, 0.17, 0.5, 30, 30);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);

	float radius = 0.23;
	float angle1 = 3.141592 / 180;
	int max_angle = 360;
	float angle2 = 0;

	glBegin(GL_POLYGON);

	for (int i = 0; i < max_angle; i++)
	{
		angle2 = i * angle1;
		glVertex2f((cos(angle2) * radius), (sin(angle2) * radius));
	}

	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glTranslatef(0.0, -0.4, 0.0);

	color[0] = 1.0; color[1] = 0.0; color[2] = 0.0; color[3] = 1.0;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
	glLightfv(GL_LIGHT0, GL_AMBIENT, color);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, color);

	glTranslatef(0.0, 0.6, 0.0);
	glRotatef(-90, 1.0, 0.0, 0.0);
	glutSolidCone(0.23, 0.3, 40, 40);

	glRotatef(90, 1.0, 0.0, 0.0);
	glTranslatef(0.0, -0.15, 0.0);
	glTranslatef(0.0, -0.1, 0.0);
	glRotatef(90, 1.0, 0.0, 0.0);

	glTranslatef(0.0, -0.007, -0.59);
	glutSolidSphere(0.05, 20, 20);

	glPushMatrix();
	{
		color[0] = 1.0;
		color[1] = 1.0;
		color[2] = 1.0;
		color[3] = 0.0;
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
		glLightfv(GL_LIGHT0, GL_AMBIENT, color);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, color);
		glTranslatef(-0.3, 0.0, 0.95);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glRotatef(-45, 0, 1, 0);
		glRotatef(180, 1, 0, 0);
		gluCylinder(obj, 0.05, 0.05, 0.2, 100, 20);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(0.3, 0.0, 0.95);

		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glRotatef(45, 0, 1, 0);
		glRotatef(180, 1, 0, 0);
		gluCylinder(obj, 0.05, 0.05, 0.2, 100, 20);
	}
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	glPushMatrix();
	{
		glTranslatef(-0.3, 0.0, 0.95);
		color[0] = 1.0; 
		color[1] = 0.0; 
		color[2] = 0.0;
		color[3] = 1.0;
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
		glLightfv(GL_LIGHT0, GL_AMBIENT, color);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, color);
		glutSolidSphere(0.05, 150, 150);

		glTranslatef(-0.045, 0.03, -0.01);
		glutSolidSphere(0.02, 150, 150);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(0.3, 0.0, 0.95);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
		glLightfv(GL_LIGHT0, GL_AMBIENT, color);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, color);
		glutSolidSphere(0.05, 150, 150);

		glTranslatef(0.045, 0.03, -0.01);
		glutSolidSphere(0.02, 150, 150);
	}
	glPopMatrix();

	glPushMatrix();
	{
		color[0] = 0.0;
		color[1] = 0.0;
		color[2] = 0.0;
		color[3] = 1.0;
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
		glLightfv(GL_LIGHT0, GL_AMBIENT, color);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, color);
		glTranslatef(-0.1, 0, 1.06);
		glRotatef(180, 1, 0, 0);
		glutSolidTorus(0.04, 0.04, 30, 30);

		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
		glLightfv(GL_LIGHT0, GL_AMBIENT, color);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, color);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
		glLightfv(GL_LIGHT0, GL_AMBIENT, color);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, color);
		glTranslatef(0.1, 0, 1.06);
		glRotatef(180, 1, 0, 0);
		glutSolidTorus(0.04, 0.04, 30, 30);

		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
		glLightfv(GL_LIGHT0, GL_AMBIENT, color);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, color);
	}
	glPopMatrix();

	glutSwapBuffers();
	gluDeleteQuadric(obj);
}

void keyboard(unsigned char key, int x, int y)
{ 
	if (key == 'a' || key == 'A')
	{
		zoom += 0.1;
		glutPostRedisplay();
	}
	else if (key == 'z' || key == 'Z')
	{
		zoom -= 0.1;
		glutPostRedisplay();
	}
}

void mouseMotion(int x, int y)
{
	if (left_click == true)
	{
		click_rotation_Y += y - static_Y;
		click_rotation_X += x - static_X;
		glutPostRedisplay();
	}
	static_X = x;
	static_Y = y;
}

void OnMouseClick(int button, int state, int x, int y)
{ 
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		static_X = x;
		static_Y = y;
		left_click = true;
	}
	else
	{
		left_click = false;
	}
}

AUX_RGBImageRec *loadBMP(char *filename)
{
	FILE *fp = fopen(filename, "r");
	if (!filename)
		return NULL;

	if (fp)
	{
		fclose(fp);
		return auxDIBImageLoad(filename);
	}
	return NULL;
}

int loadGLTextures()
{
	int Status = FALSE;
	head = gluNewQuadric();
	gluQuadricDrawStyle(head, GLU_FILL);
	gluQuadricTexture(head, GL_TRUE);
	body = gluNewQuadric();
	gluQuadricDrawStyle(body, GLU_FILL);
	gluQuadricTexture(body, GL_TRUE);
	AUX_RGBImageRec *TextureImage[2];

	memset(TextureImage, 0, sizeof(void *) * 1);

	if (TextureImage[0] = loadBMP("head.bmp"))
	{
		Status = TRUE;
		glGenTextures(1, &texture[0]);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
	}

	if (TextureImage[1] = loadBMP("body.bmp"))
	{
		Status = TRUE;
		glGenTextures(1, &texture[1]);
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[1]->sizeX, TextureImage[1]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[1]->data);
	}

	if (TextureImage[0])
	{
		if (TextureImage[0]->data)
			free(TextureImage[0]->data);
		free(TextureImage[0]);
	}
	if (TextureImage[1])
	{
		if (TextureImage[1]->data)
			free(TextureImage[1]->data);
		free(TextureImage[1]);
	}

	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	return Status;
}