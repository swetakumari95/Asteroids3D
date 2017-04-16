#include <cmath>
#include <chrono>
#include <thread>
#include <GL/glut.h>

using namespace std;

float scale = 1;
float cx = 0, cy = 0, r = 1, z = 10;


//TODO : Random number of asteroids, random timing, shooting controls, aim etc. Try different speeds for different asteroids
// Still remain smooth

void idle() {
	this_thread::sleep_for(chrono::milliseconds(50));
	scale += 0.2;
	z--;
	glutPostRedisplay();
}

void display() {
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1, 1, 1);
	glPointSize(5);
	for (float i = 0; i < 180; i += 0.01) {
		float x1 = cx + scale*r*cos(i*3.14);
		float y1 = cy + scale*r*sin(i*3.14);
		float x2 = cx + scale*r*cos((360-i)*3.14);
		float y2 = cy + scale*r*sin((360-i)*3.14);
		glBegin(GL_LINES);
		glColor3f(1, 1, 1);
		glVertex3f(x1, y1, z );
		glVertex3f(x2, y2, z );
		glColor3f(1, 0, 0);
		glVertex3f(x1, y1, z );
		glVertex3f(x2, y2, z );
		glEnd();
	}
	if (scale > 5) {
		r = 1;
		z = 10;
		scale = 1;
		//this_thread::sleep_for(chrono::milliseconds(rand()%5000)); // Figure out how to add delay without causing lag in controls
		cx = (rand() % 20) - 10;
		cy = (rand() % 20) - 10;
	}
	else
		glutIdleFunc(idle);
	glFlush();
}

void keyboard(unsigned char ch, int x, int y) {
	//Controls - Find out for direction keys
	//Figure out how to avoid the lag with key press and hold
	 switch(ch) {
	 case'a':cx += 1; break;
	 case 'd': cx -= 1; break;
	 case 'w': cy += 1; break;
	 case 's': cy -= 1; break;
	}
	 glutPostRedisplay();
}

void init() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-10, 10, -10, 10, -10, 10); //-10 and 10 are changeable...
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowSize(800, 800); //Choose a good window size. Maybe use screen size - Full screen stuff?
	glutCreateWindow("Asteroids!");
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
}