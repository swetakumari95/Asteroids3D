#include <iostream>
#include <cmath>
#include <chrono>
#include <ctime>
#include <thread>
#include <string>
#include <algorithm>
#include <GL/glut.h>

using namespace std;

float scale = 1, scalefactor = 0.2;    //scalefactor to generate different speeds for different asteroids
float cx = 0, cy = 0, r = 1, z = -10;
int flag = 0;

int numberOfHighScores = 0;

string name;
int namelength = 0;

typedef struct {
	int val;
	string name;
}scoretype;

scoretype highscore[10];

int selectedButton = 0, screen = 0;

FILE* fp = NULL;

/*void idle() {
this_thread::sleep_for(chrono::milliseconds(50));
scale += scalefactor;   //0.2;
z--;
glutPostRedisplay();
}*/

int delay = 100;

unsigned long long int score = 0;

time_t clk;


void idle() {
	time_t now = time(NULL);  //finds out the current time
	if (difftime(now, clk) > 5) {  //increments score based on the time elapsed
		score += 25;
		clk = now;
		//cout << score << endl;
	}
}


void drawBitmapText(string str, float x, float y, float z, float r, float g, float b) { 
	//function to draw the text on the screen 
	glColor3ub((int)r, (int)g, (int)b);
	glRasterPos3f(x, y, z);
	for (int i = 0; i<str.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
	}
	glColor3f(1, 1, 1);
}


void drawBitmapTextLarge(string str, float x, float y, float z, float r, float g, float b) {
	//function to draw slightly larger text on screen
	glColor3ub((int)r, (int)g, (int)b);
	glRasterPos3f(x, y, z);
	for (int i = 0; i<str.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
	}
	glColor3f(1, 1, 1);
}


void drawButton(float x, float y, float width, float height, bool selected) {
	//function to draw the menu buttons on the screen
	if (selected)
		glColor3f(1, 0, 0);
	glBegin(GL_LINES);  //draws the box around the text for options
	glVertex3f(x, y, 0);
	glVertex3f(x + width, y, 0);
	glVertex3f(x, y - height, 0);
	glVertex3f(x + width, y - height, 0);
	glEnd();

	float r = (height / 2);
	for (float i = 270; i < 450; i += 0.01) { //draws the semi-circle on both the ends of the box for options
		float x1 = x - r*cos(i*3.14 / 180);
		float y1 = y - r + r*sin(i*3.14 / 180);
		float x2 = x + width + r*cos((360 - i)*3.14 / 180);
		float y2 = y - r + r*sin((360 - i)*3.14 / 180);
		glBegin(GL_POINTS);
		//glColor3f(1, 0, 0);
		glVertex3f(x1, y1, 0);
		glVertex3f(x2, y2, 0);
		glEnd();
	}
	glColor3f(1, 1, 1);
}


void timer(int val) {
	if (scale < 4)
		scale += scalefactor;
	else
		scale += 0.25;
	z--;

	if (z > -10)
		glutTimerFunc(delay, timer, 100);

	glutPostRedisplay();
}


void drawBackground() {
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
}


void drawSpaceShip() {
	//spacecraft interiors
	glColor3f(0.1, 0.1, 0.1);
	glBegin(GL_POLYGON);
	glVertex3f(-10, -10, 10);
	glVertex3f(-10, -8, 10);
	glVertex3f(10, -8, 10);
	glVertex3f(10, -10, 10);
	glEnd();
	//box for displaying the score
	glColor3f(0, 0, 0);
	glBegin(GL_POLYGON);
	glVertex3f(-2, -9.5, 10);
	glVertex3f(-2, -8.5, 10);
	glVertex3f(2, -8.5, 10);
	glVertex3f(2, -9.5, 10);
	glEnd();
	//outline of the box
	for (float i = -9.5; i <= -8.5; i += 0.2) {
		glColor3ub(rand() % 255, rand() % 255, rand() % 255);
		glBegin(GL_POINTS);
		glVertex3f(-2, i, 10);
		glEnd();
	}
	for (float i = -2; i <= 2; i += 0.2) {
		glColor3ub(rand() % 255, rand() % 255, rand() % 255);
		glBegin(GL_POINTS);
		glVertex3f(i, -8.5, 10);
		glEnd();
	}
	for (float i = -9.5; i <= -8.5; i += 0.2) {
		glColor3ub(rand() % 255, rand() % 255, rand() % 255);
		glBegin(GL_POINTS);
		glVertex3f(2, i, 10);
		glEnd();
	}
	for (float i = -2; i <= 2; i += 0.2) {
		glColor3ub(rand() % 255, rand() % 255, rand() % 255);
		glBegin(GL_POINTS);
		glVertex3f(i, -9.5, 10);
		glEnd();
	}
	//displaying the score
	string str = "SCORE : " + to_string(score);
	drawBitmapText(str, -1.9, -9.25, 10, rand() % 255, rand() % 255, rand() % 255);
	drawBitmapText(" ", -1.9, -9.25, 10, rand() % 255, rand() % 255, rand() % 255);
	//random panels on the display screen
	for (float i = -9.5; i<-2.4; i += 0.8) {
		glColor3ub(rand() % 255, rand() % 255, rand() % 255);
		glBegin(GL_QUADS);
		glVertex3f(i, -9.8, 10);
		glVertex3f(i, -9.4, 10);
		glVertex3f(i + 0.4, -9.4, 10);
		glVertex3f(i + 0.4, -9.8, 10);
		glEnd();
		glColor3ub(rand() % 255, rand() % 255, rand() % 255);
		glBegin(GL_QUADS);
		glVertex3f(i, -9, 10);
		glVertex3f(i, -8.6, 10);
		glVertex3f(i + 0.4, -8.6, 10);
		glVertex3f(i + 0.4, -9, 10);
		glEnd();
	}
	for (float i = 2.7; i<9.5; i += 0.8) {
		glColor3ub(rand() % 255, rand() % 255, rand() % 255);
		glBegin(GL_QUADS);
		glVertex3f(i, -9.8, 10);
		glVertex3f(i, -9.4, 10);
		glVertex3f(i + 0.4, -9.4, 10);
		glVertex3f(i + 0.4, -9.8, 10);
		glEnd();
		glColor3ub(rand() % 255, rand() % 255, rand() % 255);
		glBegin(GL_QUADS);
		glVertex3f(i, -9, 10);
		glVertex3f(i, -8.6, 10);
		glVertex3f(i + 0.4, -8.6, 10);
		glVertex3f(i + 0.4, -9, 10);
		glEnd();
	}
	//corners for the spacecraft interiors
	glColor3f(0.1, 0.1, 0.1);
	glBegin(GL_TRIANGLES);
	glVertex3f(-10, -8, 10);
	glVertex3f(-10, -5, 10);
	glVertex3f(-5, -8, 10);
	glEnd();
	glBegin(GL_TRIANGLES);
	glVertex3f(10, -8, 10);
	glVertex3f(10, -5, 10);
	glVertex3f(5, -8, 10);
	glEnd();
	//panels in the corner portion of spacecraft

	float dec = 0;
	for (float y = -8; y<-5.8; y += 0.6) {
		for (float x = -9.5; x<(-5.7 - dec); x += 0.6) {
			glColor3ub(rand() % 255, rand() % 255, rand() % 255);
			glBegin(GL_QUADS);
			glVertex3f(x, y, 10);
			glVertex3f(x, y + 0.3, 10);
			glVertex3f(x + 0.3, y + 0.3, 10);
			glVertex3f(x + 0.3, y, 10);
			glEnd();
		}
		dec += 1.2;
	}
	dec = 0;
	for (float y = -8; y<-5.8; y += 0.6) {
		for (float x = 9.5; x>(5.7 + dec); x -= 0.6) {
			glColor3ub(rand() % 255, rand() % 255, rand() % 255);
			glBegin(GL_QUADS);
			glVertex3f(x, y, 10);
			glVertex3f(x, y + 0.3, 10);
			glVertex3f(x - 0.3, y + 0.3, 10);
			glVertex3f(x - 0.3, y, 10);
			glEnd();
		}
		dec += 1.2;
	}
}


void timer2(int n) {
	glutPostRedisplay();
}


void saveHighScoreScreen() {
	//save high score screen displayed only when user makes a high score
	//it allows the user to enter their name
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	drawBackground();

	drawBitmapText("CONGRATULATIONS!", -2.8, 8, 10, 255, 255, 0);
	drawBitmapText("You made a new high score!", -3.5, 7, 10, 255, 255, 0);
	drawBitmapText("Enter your name : ", -4, 3, 10, 255, 255, 0);
	drawBitmapText(name, 0.6, 3, 10, 255, 255, 0);
	drawBitmapText("......................", 0.4, 2.7, 10, 255, 255, 0);

	drawBitmapText("Use letter keys to enter your name.", -4, -6, 10, 255, 255, 255);
	drawBitmapText("Hit enter when you are done.", -3.4, -7, 10, 255, 255, 255);
	drawSpaceShip();
	glutTimerFunc(delay, timer2, 100);
	glFlush();
}


void highScoreScreen() {
	//displays the top ten high scores obtained by the player
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	drawBackground();

	drawBitmapText("HIGH SCORE", -2, 8, 10, 0, 0, 255);

	float ypos = 6;
	for (int i = 0; i < numberOfHighScores; i++) {
		drawBitmapText(highscore[i].name, -4, ypos, 10, 0, 255, 255);
		drawBitmapText(to_string(highscore[i].val), 2, ypos--, 10, 0, 255, 255);
	}

	for (int i = numberOfHighScores; i < 10; i++) {
		drawBitmapText("-----", -4, ypos, 10, 0, 255, 255);
		drawBitmapText("-----", 2, ypos--, 10, 0, 255, 255);
	}

	drawBitmapText("Press the ESC button to return to main menu.", -5.5, -7, 10, 255, 255, 255);

	drawSpaceShip();
	glutTimerFunc(delay, timer2, 100);
	glFlush();
}


void instructionScreen() {
	//displays the instructions for the game
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	drawBackground();

	string str[] = { "You are in space, and you command a spacecraft.", "Accidentally you were pushed into the asteroid belt.", "Your aim is to shoot or dodge the asteroids.", "Use up(w), down(s), left(a) or right(d) buttons to move your spacecraft.", "Use the spacebar to shoot the asteroids.", "You are awarded extra points for shooting the asteroids.", "The game is over when an asteroid strikes the spacecraft.", "Have fun!" };

	drawBitmapText("INSTRUCTIONS", -2.5, 8, 10, 0, 0, 255);
	float ypos = 6;
	for (int i = 0; i<(sizeof(str) / sizeof(*str)); i++) {
		float xpos;
		int l = str[i].length();
		if (l>60) xpos = -8;
		else if (l>55) xpos = -7.5;
		else if (l>50) xpos = -7;
		else if (l>45) xpos = -6.5;
		else if (l>40) xpos = -6;
		else if (l>35) xpos = -5.5;
		else if (l>30) xpos = -5;
		else if (l>25) xpos = -4.5;
		else if (l>20) xpos = -4;
		else if (l>15) xpos = -3.5;
		else if (l>10) xpos = -3;
		else if (l>5)  xpos = -2.5;
		drawBitmapText(str[i], xpos, ypos, 10, 0, 255, 255);
		ypos -= 1.5;
	}

	drawBitmapText("Press the ESC button to return to main menu.", -5.5, -7, 10, 255, 255, 255);

	drawSpaceShip();
	glutTimerFunc(delay, timer2, 100);
	glFlush();
}


void startScreen() {
	//displays the start screen of the game with options to start game, view instructions, high score and quit
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1, 1, 1);
	drawBackground();

	drawBitmapTextLarge("ASTEROIDS 3D", -2.8, 8, 10, 255, 255, 0);

	float x = -2, y = 6, delta = 2;
	drawButton(x, y, 3, 1, selectedButton == 0); // Take additional parameter of selectedButton == 0
	drawBitmapText("New Game", x + 0.25, y - 0.7, 0, 255, selectedButton == 0 ? 0 : 255, selectedButton == 0 ? 0 : 255);
	y -= delta;
	drawButton(x, y, 3, 1, selectedButton == 1);
	drawBitmapText("Instructions", x + 0.18, y - 0.7, 0, 255, selectedButton == 1 ? 0 : 255, selectedButton == 1 ? 0 : 255);
	y -= delta;
	drawButton(x, y, 3, 1, selectedButton == 2);
	drawBitmapText("High Score", x + 0.2, y - 0.7, 0, 255, selectedButton == 2 ? 0 : 255, selectedButton == 2 ? 0 : 255);
	y -= delta;
	drawButton(x, y, 3, 1, selectedButton == 3);
	drawBitmapText("Quit", x + 1, y - 0.7, 0, 255, selectedButton == 3 ? 0 : 255, selectedButton == 3 ? 0 : 255);

	drawBitmapText("Use up and down arrow keys to select an option.", -6, -6, 10, 0, 255, 255);
	drawBitmapText("Use enter key to select the option.", -4, -7, 10, 0, 255, 255);

	drawSpaceShip();
	glutTimerFunc(delay, timer2, 100);
	glFlush();
}


void gameOverScreen() {
	//game over screen with options to replay, go to main menu and exit
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1, 1, 1);
	drawBackground();

	drawBitmapText("GAME OVER", -2, 8, 10, 255, 255, 0);

	float x = -2, y = 6, delta = 2;
	drawButton(x, y, 3, 1, selectedButton == 0); // 5th parameter is for highlighting.
	drawBitmapText("Replay", x + 0.8, y - 0.7, 0, 255, selectedButton == 0 ? 0 : 255, selectedButton == 0 ? 0 : 255);
	y -= delta;
	drawButton(x, y, 3, 1, selectedButton == 1);
	drawBitmapText("Main Menu", x + 0.2, y - 0.7, 0, 255, selectedButton == 1 ? 0 : 255, selectedButton == 1 ? 0 : 255);
	y -= delta;
	drawButton(x, y, 3, 1, selectedButton == 2);
	drawBitmapText("Quit", x + 1, y - 0.7, 0, 255, selectedButton == 2 ? 0 : 255, selectedButton == 2 ? 0 : 255);

	drawBitmapText("Use up and down arrow keys to select an option.", -6, -6, 10, 0, 255, 255);
	drawBitmapText("Use enter key to select the option.", -4, -7, 10, 0, 255, 255);

	drawSpaceShip();
	glutTimerFunc(delay, timer2, 100);
	glFlush();
}


void display() {
	// the main game screen is displayed 
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	drawBackground();

	//Displaying the circle
	//glPointSize(5);
	for (float i = 0; i < 180; i += 0.01) {
		float x1 = cx + scale*r*cos(i*3.14);
		float y1 = cy + scale*r*sin(i*3.14);
		float x2 = cx + scale*r*cos((360 - i)*3.14);
		float y2 = cy + scale*r*sin((360 - i)*3.14);
		glBegin(GL_LINES);
		glColor3f(0.4, 0.1, 0.1);
		glVertex3f(x1, y1, z);
		glVertex3f(x2, y2, z);
		glEnd();
	}

	if (z <= -10) {
		// Game over condition - Asteroid hits spaceship
		if (cx < 10 && cx > -10 && cy < 10 && cy > -10 && flag) {
			glutIdleFunc(NULL);
			if (numberOfHighScores < 10 || score > highscore[numberOfHighScores - 1].val) {
				namelength = 0;
				name = "";
				screen = 5;
				glutDisplayFunc(saveHighScoreScreen);
				glutTimerFunc(delay, timer2, 100);
			}
			else {
				screen = 4;
				glutDisplayFunc(gameOverScreen);
				glutTimerFunc(delay, timer2, 100);
			}
			//glutPostRedisplay();
		}
		else {
			flag = 1;
			r = 1;
			z = 10;
			scale = 1;
			scalefactor = (rand() % 5 + 1) / 10.0;    //scalefactor to generate different speeds for different asteroids
			if (scalefactor < 0.2)
				scalefactor *= 1.5;
			cx = (rand() % 20) - 10;
			cy = (rand() % 20) - 10;
			glutTimerFunc(delay, timer, 100);
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

	drawSpaceShip();

	glFlush();
}


bool compare(scoretype a, scoretype b) {
	//for comparison of high scores
	return a.val > b.val;
}


void keyboard(unsigned char ch, int x, int y) {
	switch (screen) {
	case 0: // Start Screen
		// to identify selected button and display appropriate screen
		if (ch == 13) {
			switch (selectedButton) {
			case 0:
				glutIdleFunc(idle);
				glutDisplayFunc(display);
				screen = 1;
				break;
			case 1:
				glutDisplayFunc(instructionScreen);
				screen = 2;
				break;
			case 2:
				glutDisplayFunc(highScoreScreen);
				screen = 3;
				break;
			case 3:
				exit(0);
			}
		}
		break;
	case 1: // Game screen
		//Controls: functionality for normal keys like a,w,s,d
		switch (ch) {
		case 'a': cx += 1; break;
		case 'd': cx -= 1; break;
		case 'w': cy -= 1; break;
		case 's': cy += 1; break;
		case 32:
			//laser shoots to the center of the screen
			glLineWidth(2);
			glBegin(GL_LINES);
			glColor3f(0, 1, 0);
			glVertex3f(-10, -5, 0);
			glVertex3f(-8, -4, 0);
			glVertex3f(-7, -3.5, 0);
			glVertex3f(-5, -2.5, 0);
			glVertex3f(-4, -2, 0);
			glVertex3f(-2, -1, 0);
			glVertex3f(-1, -0.5, 0);
			glVertex3f(0, 0, 0);

			glVertex3f(10, -5, 0);
			glVertex3f(8, -4, 0);
			glVertex3f(7, -3.5, 0);
			glVertex3f(5, -2.5, 0);
			glVertex3f(4, -2, 0);
			glVertex3f(2, -1, 0);
			glVertex3f(1, -0.5, 0);
			glVertex3f(0, 0, 0);
			glEnd();
			glFlush();
			//glPointSize(1);
			glLineWidth(1);

			//when the laser hits the asteroid
			//double theta = atan(cy/cx);
			//if(0 < (cx + (scale *r)) && 0 > (cx - (scale*r)) && 0 < (cy + (scale *r)) && 0 > (cy - (scale*r)) ){
			if (((cx*cx) + (cy*cy)) < (scale*scale*r*r)) {
				cx = 1000;
				cy = 1000;
				//giving special score if you hit the asteroid
				score += 50;
			}
		}
		//glutPostRedisplay();
		break;
	case 2: //instructions screen
		if (ch == 27) { //escape key to return to main menu
			glutDisplayFunc(startScreen);
			screen = 0;
		}
		break;
	case 3: //High score screen
		if (ch == 27) { //escape key to return to main menu
			glutDisplayFunc(startScreen);
			screen = 0;
		}
		break;
	case 4: //Game over screen
		//finding the selected button and displaying the appropriate screen
		if (ch == 13) {
			switch (selectedButton) {
			case 0: //resume button
				glutIdleFunc(idle);
				scale = 1, scalefactor = 0.2;
				cx = 0, cy = 0, r = 1, z = -10;
				flag = 0;
				selectedButton = 0, screen = 1;
				score = 0;
				glutDisplayFunc(display);
				//glutPostRedisplay();
				break;
			case 1: //main menu button
				glutDisplayFunc(startScreen);
				scale = 1, scalefactor = 0.2;
				cx = 0, cy = 0, r = 1, z = -10;
				flag = 0;
				selectedButton = 0, screen = 0;
				score = 0;
				//glutPostRedisplay();
				break;
			case 2: //exit button
				//write highscores back to file
				exit(0);
			}
		}
		break;
	case 5: //Enter name for high score screen
		if (isalpha(ch)) { //to save the user's name in high score 
			if (namelength < 10) {
				name += ch;
				namelength++;
			}
		}
		else if (ch == 13) { //on pressing enter, takes the user to game over screen, and updates high score
			if (namelength != 0) {
				if (numberOfHighScores < 10) {
					highscore[numberOfHighScores].name = name;
					highscore[numberOfHighScores++].val = score;
				}
				else {
					highscore[numberOfHighScores - 1].name = name;
					highscore[numberOfHighScores - 1].val = score;
				}
				sort(highscore, highscore + numberOfHighScores, compare);
				fp = fopen("highscore.txt", "w");
				fprintf(fp, "%d ", numberOfHighScores);
				for (int i = 0; i < numberOfHighScores; i++) {
					char *cname = &(highscore[i].name[0]);
					fprintf(fp, "%s %d ", cname, highscore[i].val);
				}
				glutDisplayFunc(gameOverScreen);
				screen = 4;
				name = "";
				namelength = 0;
			}
		}
	}
}


//Controls: functionality for special function keys(direction keys) like up, down, left, right arrows
void arrowKeyPress(int key, int x, int y) {
	switch (screen) {
	case 0: //screen 0 is the start screen
		//helps select the options 
		switch (key) {
		case GLUT_KEY_UP: selectedButton--;
			if (selectedButton < 0)
				selectedButton = 3;
			break;
		case GLUT_KEY_DOWN: selectedButton = (selectedButton + 1) % 4; break;
		}
		//glutPostRedisplay();
		break;
	case 1: //screen 1 is the game screen
		//used to maove the screen up, down, left and right
		switch (key) {
		case GLUT_KEY_LEFT: cx += 1; break;
		case GLUT_KEY_RIGHT: cx -= 1; break;
		case GLUT_KEY_UP: cy -= 1; break;
		case GLUT_KEY_DOWN: cy += 1; break;
		}
		//glutPostRedisplay();
		break;
	case 4: //screen 4 is the game over screen
		//helps select the options
		switch (key) {
		case GLUT_KEY_UP: selectedButton--;
			if (selectedButton < 0)
				selectedButton = 2;
			break;
		case GLUT_KEY_DOWN: selectedButton = (selectedButton + 1) % 3; break;
		}
		//glutPostRedisplay();
		break;
	}
}


void init() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-10, 10, -10, 10, -10, 10); 
	glMatrixMode(GL_MODELVIEW);
}


int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowSize(700, 600); 
	glutCreateWindow("Asteroids 3D");
	init();
	
	fp = fopen("highscore.txt", "r");  //file used to store the high scores
	if (!fp) {
		fp = fopen("highscore.txt", "w+");
		fprintf(fp, "%d ", 0);
	}
	fscanf(fp, "%d", &numberOfHighScores);
	for (int i = 0; i < 10; i++)
		highscore[i].val = -1;
	for (int i = 0; i < numberOfHighScores; i++) {
		char cname[11];
		fscanf(fp, "%s%d ", cname, &highscore[i].val);
		highscore[i].name = cname;
		//printf("\n%s%d", cname, highscore[i].val);
	}
	fclose(fp);
	
	clk = time(NULL); //to get the time, used for score calculation
	
	glutDisplayFunc(startScreen);
	glutKeyboardFunc(keyboard);
	//glutIdleFunc(idle);
	glutSpecialFunc(arrowKeyPress);
	glutMainLoop();
}
