#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
 
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
// Needed on MsWindows
#include <windows.h>
#endif // Win32 platform
 
#include <GLUT/glut.h>
#include <stdio.h>
#include <iostream>
#include <time.h> 

int gemarray[10][10]; //gem type at each location

// generate a 'random' array
void makeArray() 
{
	for(int i=0; i<10; i++) 
	{
		for(int j=0; j<10; j++)
			gemarray[i][j] = rand()%3;
	}		
}

// Based on array of gem values, draws gems on screen
void drawShapes()
{
	double x,y; //coordinates to start drawing from
	int type;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	for(int i=0; i<10; i++) 
	{
		for(int j=0; j<10; j++)
		{
			//Lower left corner
			x = -1+(j)*0.2;
			y = 0.8-(i)*0.2;
			type = gemarray[i][j];

			switch(type)
			{
				case 0:
					glBegin(GL_TRIANGLE_FAN); //triangle fan - pentagon
					glColor3d(1.0, 0.0, 1.0); //purple
					glVertex2d(x+0.1, y+0.1);
					glColor3d(0.0, 0.0, 0.2); 
					glVertex2d(x+0.1, y+0.19);
					glVertex2d(x+0.02,y+0.13);
					glVertex2d(x+0.05,y+0.01);
					glVertex2d(x+0.15,y+0.01);
					glVertex2d(x+0.18, y+0.13);
					glVertex2d(x+0.1, y+0.19);
					glEnd();
					break;
				case 1:
					glBegin(GL_TRIANGLE_FAN); //triangle fan - triangle
					glColor3d(0.0, 1.0, 0.0); //green
					glVertex2d(x+0.1, y+0.1);
					glColor3d(0.0, 0.0, 0.2); 
					glVertex2d(x+0.1, y+0.19);
					glVertex2d(x+0.01,y+0.01);
					glVertex2d(x+0.19,y+0.01);
					glVertex2d(x+0.1, y+0.19);
					glEnd();
					break;
				case 2:
					glBegin(GL_TRIANGLE_FAN); //triangle fan - diamond
					glColor3d(1.0, 0.0, 0.0); //red
					glVertex2d(x+0.1, y+0.1);
					glColor3d(0.0, 0.0, 0.2); 
					glVertex2d(x+0.1, y+0.19);
					glVertex2d(x+0.01,y+0.1);
					glVertex2d(x+0.1,y+0.01);
					glVertex2d(x+0.19, y+0.1);
					glVertex2d(x+0.1, y+0.19);
					glEnd();
					break;
			}

		}
	}
	glutSwapBuffers();

}

// recursively finds all connected gems of 'type'
// changes all the gems in the group to type 'changeto'
int changeNeighbors(int type, int i, int j, int changeto)
{
	if (i > 9 || i < 0 || j > 9 || j < 0 || type != gemarray[i][j])
		return 0;
	else
	{
		gemarray[i][j] = changeto;
		return 1 + changeNeighbors(type, i, j+1, changeto) + 
			changeNeighbors(type, i+1, j, changeto) + changeNeighbors(type, i-1, j, changeto)
			+ changeNeighbors(type, i, j-1, changeto);
	}
		
}

// moves all the gems 'down' to fill empty spaces marked with -1
// replaces all spaces above gems to -2
void moveDown()
{
	int *dropto, *dropfrom, *top, *bottom = NULL;

	for (int j = 0; j <=9; j++) //left to right
	{
		top = &gemarray[0][j];
		bottom = &gemarray[9][j];
		dropto = dropfrom = top;

		for (dropto = top; dropto < bottom+1; dropto+=10) 
		{ // move downwards until find empty space
			if (*dropto == -1)
			{
				for(dropfrom = dropto; dropfrom > top-1; dropfrom-= 10)
				{ // look upwards from dropto until find a gem
					if(*dropfrom != -1)
						break;
				}
				int *to = dropto;
				int *from = dropfrom;
				while (from > top-1) // move everything down from dropfrom to dropto
				{
					*to = *from;
					to-=10; 
					from-=10;
				} // to is now at the topmost gem
				while (to > top-1) // changes empty spaces above gem to -2
				{
					*to = -2;
					to-= 10;
				}
			}
		}
	}
}

// On click detects connecting gems, removes them, shifts everything else down
void onClick(int button, int state, int x, int y) 
{
	if(state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) 
	{
		// find location in array
		int w = glutGet(GLUT_WINDOW_WIDTH);
		int h = glutGet(GLUT_WINDOW_HEIGHT);
		int j = floor((double)x/w*10);
		int i = floor((double)y/h*10);

		int type = gemarray[i][j];

		if (type == -1) // do nothing if clicked on empty space
			return;

		// change all connecting gems of same type to -1
		int count = changeNeighbors(type, i, j, -1);
		
		if (count < 4) // if less than 4 were changed, change everything back
			changeNeighbors(-1, i, j, type);	
		else
			moveDown();

		drawShapes();
	}
}

// recursively finds all connected gems of 'type'
// changes all the gems in the group to type 'changeto'
void onDisplay()
{
	makeArray();
	drawShapes();
}

// resets gems upon pressing 'r' key
void onKeyboard(unsigned char key, int x, int y) //parameter
{
	if (key == 'r')
	{
		makeArray();
		glutPostRedisplay(); //refresh the screen
	}	
}

int main(int argc, char *argv[]) 
{
 glutInit(&argc, argv);
 glutInitWindowSize(640, 480); 
 glutInitWindowPosition(100, 100);
 glutInitDisplayMode(
GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); //RGBA if you want an alpha channel for transparency
 glutCreateWindow("GEM GAME");
 
 // Register event handlers here
 glutKeyboardFunc(onKeyboard);
 glutDisplayFunc(onDisplay);
 glutMouseFunc(onClick);
 glutMainLoop();
 return 0;
}