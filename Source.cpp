#include <cmath>
#include <chrono>
#include <thread>
#include <GL/glut.h>

using namespace std;

float scale = 1;

void idle() {
	this_thread::sleep_for(chrono::milliseconds(50));
	scale += 0.3;
	if (scale > 5)
		scale = 0.1;
	glutPostRedisplay();
}

void display() {
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1, 1, 1);
	glPointSize(5);
	float cx = 0, xy = 0, r = 1;
	for (float i = 0; i < 180; i += 0.01) {
		float x1 = scale*r*cos(i*3.14);
		float y1 = scale*r*sin(i*3.14);
		float x2 = scale*r*cos((360-i)*3.14);
		float y2 = scale*r*sin((360-i)*3.14);
		glBegin(GL_LINES);
		glColor3f(0, 0, 1);
		glVertex3f(x1, y1, 5);
		glVertex3f(x2, y2, 5);
		glColor3f(1, 0, 0);
		glVertex3f(x1, y1, -5);
		glVertex3f(x2, y2, -5);
		glEnd();
	}
	glutIdleFunc(idle);
	glFlush();
}

void init() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-10, 10, -10, 10, -10, 10);
	//gluOrtho2D(0, 500, 0, 500);
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Asteroids!");
	init();
	glutDisplayFunc(display);
	glutMainLoop();
}