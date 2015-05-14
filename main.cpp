//
//  main.cpp
//  TerrainGenerator
//  Copyright (c) 2014 Rajat Arora.  Jake Harwood All rights reserved.
//


#ifndef _OPENGL_
#define _OPENGL_
#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else

#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif
#endif

#include "Terrain.h"
#include <stdlib.h>
#include <iostream>



// Defines all of our variables

float camPos[] = { 50, 60, 50 };
Terrain t = Terrain(300,300); // Create new instance of Terrain class
float L1Pos[4] = { 10, 50, 10, 0 }; //store position of light
int wMode = 1;
int tSize = 50;
bool lightOn = false;
int shadeMode = 0;
int height, width;
bool colMode = false;
bool refresh = false;



// Keyboard function
void keyboard(unsigned char key, int x, int y) {
    
    
    switch (key) {
        case 'q': // Exit window
        case 'Q':
            exit(0);
            break;
        case'[':
            camPos[1] += 1; // Go upwards in scene
            break;
        case ']':
            camPos[1] -= 1; // Go downwards in scene
            break;
        case 'r':
            case 'R': // Randomize terrain
            t = Terrain(tSize,80);
            t.setWireMode(wMode);
            t.setColMode(colMode);
            t.generate();
            t.vertexNormal();
            refresh = true;
            
            break;
        case 'w':
            case 'W': // Turn on/off Wiremode
            if (wMode == 1){
                wMode++;
            } else if (wMode == 2){
                wMode++;
            }else if (wMode == 3){
                wMode = 1;
            }
            
            t.setWireMode(wMode);
            break;
        case 'a':
            case 'A': // Increase size and randomize terrain
            if (tSize < 300){
                tSize+=50;
                t = Terrain(tSize,80);
                t.generate();
                t.vertexNormal();
                t.setWireMode(wMode);
            }

            break;
        case 's':
            case 'S': // Decrease and randomize terrain
            if (tSize > 50){
                tSize-=50;
                t = Terrain(tSize,80);
                t.generate();
                t.vertexNormal();
                t.setWireMode(wMode);
            }
            break;
        case 'l':
            case 'L': // Turn on lighting
            if (lightOn == false){
                lightOn = true;
                glEnable(GL_LIGHTING);
            } else if (lightOn == true){
                lightOn = false;
                glDisable(GL_LIGHTING);
            }
            
            t.setLight(lightOn);
            break;
          
            
        case 'd':
            case 'D': // Switch between each type of shading (Flat / Gourad)
            
            // If light mode is requested, turn the light on
            if (lightOn == true) {
            glEnable(GL_COLOR_MATERIAL);
            glEnable(GL_LIGHTING);
            glEnable(GL_LIGHT0);
            glEnable(GL_NORMALIZE);
            
            // Dictates what type of shading to use
            if (shadeMode == 0){
                shadeMode = 1;
            glShadeModel(GL_SMOOTH);
            } else if (shadeMode == 1){
                shadeMode = 0;
                glShadeModel(GL_FLAT);
            }
            }
            break;
        
        case 'c': // Enables/Disables colour mode
            case 'C':
            if (colMode == true){
                colMode = false;
            } else if (colMode == false){
                colMode = true;
            }
            t.setColMode(colMode);
            
            break;
            
    
    }
    glutPostRedisplay();
}


void special(int key, int x, int y) {
  
    /* arrow key presses move the camera */
    switch (key) {
        case GLUT_KEY_LEFT:
            camPos[0] -= 1;
            break;
            
        case GLUT_KEY_RIGHT:
            camPos[0] += 1;
            break;
            
        case GLUT_KEY_UP:
            camPos[2] -= 1;
            break;
            
        case GLUT_KEY_DOWN:
            camPos[2] += 1;
            break;
            
        case GLUT_KEY_HOME:
            camPos[1] += 1;
            break;
            
        case GLUT_KEY_END:
            camPos[1] -= 1;
            break;
            
    }
    
}

// Reshape function
void reshape(int w, int h){
    width = w; //sets variable to width of window
    height = h; // sets height to height of window
}


/* display function */
void display(void) {
    
    glClearColor(0,0,0,0);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clears buffers
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
        gluLookAt(camPos[0], camPos[1], camPos[2], 0, 0, 0, 0, 1, 0);
        t.draw(); //Calls our draw function from the Terrain class
    glPopMatrix();
    
    glutSwapBuffers();
    glutPostRedisplay();
}

/* display function for our 2-d overview window */
void overviewDisplay(void){
    
   // glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(-120,-75, 300, 300);
    
    

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    t.overviewDraw(); // Calls the overviewDraw function from the Terrain class

    glutSwapBuffers();
    glutPostRedisplay();
   
    
   
}


// Initialization function
void init(void) {
    glClearColor(0, 0, 0, 0);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-2, 2, -2, 2, -2, 2);
    gluPerspective(45, 1, 1, 100);
    
    // Sets up all of our lighting
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_FLAT); // Flat shading enabled by default
    
    // Sets up cull facing
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    
}







/* main function - program entry point */
int main(int argc, char** argv) {
    
    cout << "--LEGEND-- \n";
    cout << "Key 'q': Exits program \n";
    cout << "Key 'r': Random terrain generation \n";
    cout << "Key 'w': Enables/Disables full polygon/wireframe/full poly-wireframe mode \n";
    cout << "Key 'a': Generate new enlarged terrain (up to 300x300) \n";
    cout << "Key 's': Generate new smaller terrain (down to 50x50) \n";
    cout << "Key 'l': Enables/Disables lighting \n";
    cout << "Key 'd': Switches between flat/gourad shading \n";
    cout << "Key 'c': Switches on colour mode \n";
    cout << "\n";
    
    cout << "Please input the terrain size and number of iterations \n";
    cout << "Size: \n";
    int size;
    cin >> size;
    cout << "Iterations: \n";
    int cuts;
    cin >> cuts;
    
    
   
    
    
    
    // Generates a terrain on program startup
    t = Terrain(size,cuts);
    t.generate();
    t.vertexNormal();
    
    
    glutInit(&argc, argv);		//starts up GLUT
    
    //** 2-D OVERVIEW WINDOW **/
    
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(300, 300);
    glutInitWindowPosition(910, 100);
    
    glutCreateWindow("2-D Overview");	//creates the window
    glutDisplayFunc(overviewDisplay); //registers "overviewDisplay" as the display callback function
  
  
    
    //** MAIN TERRAIN WINDOW **
    
    
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    
    glutCreateWindow("Terrain Generator");	//creates the window
    glutDisplayFunc(display);   //registers "display" as the display callback function
    glutKeyboardFunc(keyboard); // registers "keyboard" callback
    glutSpecialFunc(special); // registers "special key" callback
    glutReshapeFunc(reshape); // registers "reshape" callback
    init();
    

    
    glutMainLoop();				//starts the event loop
    
    
    return (0);					//return may not be necessary on all compilers
}

