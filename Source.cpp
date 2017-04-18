#include <iostream>
#include <cmath>
#include <chrono>
#include <ctime>
#include <thread>
#include <GL/glut.h>

using namespace std;

float scale = 1, scalefactor = 0.2;    //scalefactor to generate different speeds for different asteroids
float cx = 0, cy = 0, r = 1, z = -10;
int flag = 0;

//TODO: Random number of asteroids at the same time, random timing,  etc. 
//TODO: gameplay should be smooth
//TODO: game over, opening screen, menus, asteroid texture, panel at the bottom of the screen, healthbar?, enter to pause the game
//TODO: work on laser, prevent continuous key press

//done: Try different speeds for different asteroids, shooting controls, aim
//aim still takes a square region for hit(make it circle), add special scores for hitting the asteroids


/*void idle() {
	this_thread::sleep_for(chrono::milliseconds(50));
	scale += scalefactor;   //0.2;
	z--;
	glutPostRedisplay();
}*/

int delay = 100;

unsigned long long int score = 0;

time_t clk;

void idle(){
	time_t now = time(NULL);
	if(difftime(now,clk) > 5){
		score += 10;
		clk = now;
		cout<<score<<endl;
	}
}

void timer(int val){
	if(scale < 4)
		scale += scalefactor;
	else
		scale += 0.25;
	z--;
	glutPostRedisplay();
	if(z > -10)
		glutTimerFunc(delay,timer,100);
}

void display() {
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	//Space background
	float xpos, ypos, numStars = 50;
	while (numStars--) {
		xpos = (rand() % 20) - 10;
		ypos = (rand() % 20) - 10;
		glColor3f(1, 1, 1);
		glBegin(GL_POINTS);
		glVertex3f(xpos, ypos, -10);
		glEnd();
	}

	//Displaying the circle
	glColor3f(1, 1, 1);
	glPointSize(5);
	for (float i = 0; i < 180; i += 0.01) {
		float x1 = cx + scale*r*cos(i*3.14);
		float y1 = cy + scale*r*sin(i*3.14);
		float x2 = cx + scale*r*cos((360 - i)*3.14);
		float y2 = cy + scale*r*sin((360 - i)*3.14);
		glBegin(GL_LINES);
		glColor3f(1, 0, 0);
		glVertex3f(x1, y1, z);
		glVertex3f(x2, y2, z);
		glEnd();
	}
	
	if (z <= -10) {
		if(cx < 10 && cx > -10 && cy < 10 && cy > -10 && flag){
			glClearColor(1,1,1,1);
			glClear(GL_COLOR_BUFFER_BIT);
			
		}
		else{
		flag = 1;
		r = 1;
		z = 10;
		scale = 1;
		scalefactor = (rand() % 5 + 1) / 10.0;    //scalefactor to generate different speeds for different asteroids
		//this_thread::sleep_for(chrono::milliseconds(rand()%5000)); // Figure out how to add delay without causing lag in controls
		if(scalefactor < 0.2)
			scalefactor *= 1.5;
		cx = (rand() % 20) - 10;
		cy = (rand() % 20) - 10;
		glutTimerFunc(delay,timer,100);
		}
	}
	
	//Drawing the pointer for aim
	glBegin(GL_LINES);
		glColor3f(1, 1, 1);
		glVertex3f(-1, 0, 0);
		glVertex3f(1, 0, 0);
	glEnd();
	glBegin(GL_LINES);
		glColor3f(1, 1, 1);
		glVertex3f(0, 1, 0);
		glVertex3f(0, -1, 0);
	glEnd();
	
	glFlush();
}

//Controls: functionality for normal keys like a,w,s,d
void keyboard(unsigned char ch, int x, int y) {
	//Figure out how to avoid the lag with key press and hold
	switch (ch) {
	case 'a': cx += 1; break;
	case 'd': cx -= 1; break;
	case 'w': cy -= 1; break;
	case 's': cy += 1; break;
	case 32:
		//laser shoots to the center of the screen
		glPointSize(5);   
		glBegin(GL_LINES);
			glColor3f(0,1,0);
			glVertex3f(-10,-10,0);
			glVertex3f(-8,-8,0);
			glVertex3f(-7,-7,0);
			glVertex3f(-5,-5,0);
			glVertex3f(-4,-4,0);
			glVertex3f(-2,-2,0);
			glVertex3f(-1,-1,0);
			glVertex3f(0,0,0);
			
			glVertex3f(10,-10,0);
			glVertex3f(8,-8,0);
			glVertex3f(7,-7,0);
			glVertex3f(5,-5,0);
			glVertex3f(4,-4,0);
			glVertex3f(2,-2,0);
			glVertex3f(1,-1,0);
			glVertex3f(0,0,0);
		glEnd();
		glFlush();
		//glLineWidth(1);
	
		//when the laser hits the asteroid
		//double theta = atan(cy/cx);
		//if(0 < (cx + (scale *r)) && 0 > (cx - (scale*r)) && 0 < (cy + (scale *r)) && 0 > (cy - (scale*r)) ){
		if(((cx*cx) + (cy*cy)) < (scale*scale*r*r)){
			cx = 1000;
			cy = 1000;
			//giving special score if you hit the asteroid
			score += 50;
		}
	}
	glutPostRedisplay();
}

//Controls: functionality for special function keys(direction keys) like up, down, left, right arrows
void arrowKeyPress(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT: cx += 1; break;
	case GLUT_KEY_RIGHT: cx -= 1; break;
	case GLUT_KEY_UP: cy -= 1; break;
	case GLUT_KEY_DOWN: cy += 1; break;
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
	clk = time(NULL);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
	glutSpecialFunc(arrowKeyPress);
	glutMainLoop();
}
