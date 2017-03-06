/*
 *	Terrain.cpp by Rajat Arora and Jake Harwood 
 *	This is an interatice terrain mesh ranging from 50x50 to 300x300 vertices.
 *  Terrain using quad polygons that make up 3D surface.
 *	Initial height values according to Fault Algorithm.
 */

/* OpenGL cross platform inclues */
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

/* includes header files and libraries */
#include "Terrain.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <vector>


/* contains the max/min height the terrain will be */
float maxHeight = 50;
float minHeight = -10;

/* using standard library */
using namespace std;

/* Setter for wireframe mode */
void Terrain::setWireMode(int wMode){
    this->wireMode = wMode;
}

/* Setter for turning on light */
void Terrain::setLight(bool onoff){
    this->lightOn = onoff;
}

/*Setter for turning on terrain color */
void Terrain::setColMode(bool cm){
    this->colMode = cm;
}

/* Constructor, sets terrain size, and the number of cuts of the iteration */
Terrain::Terrain(int const terrainSize, int numberOfCuts){
    this->terrainSize = terrainSize; //50*50 ~ 300*300
    this->numberOfCuts = numberOfCuts; //
    this->heightMap = new float *[terrainSize]; // dynamic array (size 10) of pointers to int
    this->vertexVects = new float *[terrainSize];
    this->surfaceVects = new float *[terrainSize];
    
/* each i-th pointer pointing to dynamic array (size 10) of actual int values */
    for (int i = 0; i < terrainSize; ++i) {
        heightMap[i] = new float[terrainSize];
        vertexVects[i] = new float[terrainSize];
        surfaceVects[i] = new float[terrainSize];  
    }   
}

/* generate the terrain by fault algorithm */
void Terrain::generate() {
    /* set default values to heightmap */
    for (int x = 0; x < this-> terrainSize; x++) {
        for (int z = 0; z < terrainSize; z++) {
            heightMap[x][z] = 10;      
        }
    }
 	/* set the number of iterations to cut */
    int iterations = this->terrainSize + 50 + this->numberOfCuts;
    
    
   /* 
    *   Generate the heightmap and store it in 2D array
    *   algorithm reference: http://www.lighthouse3d.com/opengl/terrain/index.php?fault
    *	The idea of this algorithm is to have zero height then random line 
	*	which dives terrain in two parts, one side of the line have displaced upwards, 
	*	wheras the points on the other side displaced downwards.
    */    
    for (int i = 0; i < iterations; i++) {
        
        float v = rand(); 
        float a = sinf(v);
        float b = cosf(v);
        float d = sqrtf(2*(terrainSize*terrainSize));
        float c = ((float) rand() / RAND_MAX) * d - d/2.0;
        
        float displacement = 0.9;
        
        /* iterate through each point for the terrain and determine the height for each point */
        for (int x = 0; x < terrainSize; x++) {
            for (int z = 0; z < terrainSize; z++) {
                if (a*x + b*z - c > 0)
                	/* based on the line it cuts, points displaced upwards */
                    if ( (heightMap[x][z] + displacement) < maxHeight){
                        heightMap[x][z] += displacement ;
                    }else{
                        heightMap[x][z] = maxHeight;
                    }
                    /* points displaced downwards on the other side of the line */
                    else
                        if((heightMap[x][z] - displacement) > minHeight){
                            heightMap[x][z] -= displacement ;
                        }
            }        
        }
        displacement += 1;
    }
}



/* A strut that will be used as a vector */
struct ve {
    float x;
    float y;
    float z;
};

/* normals and vectors for shading and lighting */
vector<ve> rt;
vector<ve> k;

/* 
 * calculates the unit vector in the same direction as the original vector 
 * If you want to know why We need to calculate normal please read the following
 * http://www.opengl-tutorial.org/beginners-tutorials/tutorial-8-basic-shading/
 */
void normalize() {
    float m = sqrt((v.x * v.x) + (v.y * v.y) + (v.z *v.z)); 
    v.x = v.x /m;
    v.y = v.y/m;
    v.z =v.z/m;
}
/* Basic shading calculation, cross product and normals */
/* Recall that  a X b = ||a|| * ||b|| * sin(angle) n */
/* calculates cross product for each point, find the face vector and store it into rt */
void cross() {
   
    v.x = (rt.at(0).y * rt.at(1).z - rt.at(0).z * rt.at(1).y);
    v.y = ( rt.at(0).z * rt.at(1).x - rt.at(0).x * rt.at(1).z);
    v.z= (rt.at(0).x * rt.at(1).y - rt.at(0).y * rt.at(1).x);
    rt.push_back(v);
    
    
    v.x = (rt.at(1).y * rt.at(2).z - rt.at(1).z * rt.at(2).y);
    v.y = ( rt.at(1).z * rt.at(2).x - rt.at(1).x * rt.at(2).z);
    v.z= (rt.at(1).x * rt.at(2).y - rt.at(1).y * rt.at(2).x);
    rt.push_back(v);
    
    
    v.x = (rt.at(3).y * rt.at(4).z - rt.at(3).z * rt.at(4).y);
    v.y = ( rt.at(3).z * rt.at(4).x - rt.at(3).x * rt.at(4).z);
    v.z= (rt.at(3).x * rt.at(4).y - rt.at(3).y * rt.at(4).x);
    rt.push_back(v);
}

/* 
 * The Color Mode function
 * returns an array holds RGB colors
 * the lowest height values (green) to hightest height values (red)
 */
float* c(float x, float y, float z, float **heightMap) {

    /* different rgb colours values depending on the minimum height and maximum height value */
    float t = 0, t1=1, t2=0;
    if (y <  minHeight * 0.8) {
        
        t = 173 / 255.0f;
        t1 = 255 / 255.0f;
        t2 = 047 / 255.0f;
    }
    if (y< minHeight * 0.6
        && y > minHeight * 0.8) {
        
        t = 50 / 255.0f;
        t1 = 205 / 255.0f;
        t2 = 50 / 255.0f;
    }
    if (y < minHeight * 0.4
        && y > minHeight * 0.6) {
        
        t = 154 / 255.0f;
        t1 = 205 / 255.0f;
        t2 = 127 / 255.0f;
    }
    if (y < minHeight * 0.2
        && y> minHeight * 0.4) {
        t = 34 / 255.0f;
        t1 = 139 / 255.0f;
        t2 = 3 / 255.0f;
    }
    
    if (y < minHeight * 0.0
        &&y > minHeight * 0.2) {
        t = 107 / 255.0f;
        t1 = 142 / 255.0f;
        t2 = 35 / 255.0f;
    }
    
    if (y < maxHeight * 0.4
        && y > maxHeight * 0.2) {
        t = 255 / 255.0f;
        t1 = 106 / 255.0f;
        t2 = 106/ 255.0f;
    }
    
    if (y < maxHeight * 0.6
        && y> maxHeight * 0.4) {
        t = 238 / 255.0f;
        t1 = 99 / 255.0f;
        t2 = 99/ 255.0f;
    }
    
    if (y< maxHeight * 0.8
        && y > maxHeight * 0.6) {
        
        t = 205 / 255.0f;
        t1 = 85 / 255.0f;
        t2 = 85 / 255.0f;
    }
    if (y > maxHeight * 0.8) {
        
        t = 139 / 255.0f;
        t1 = 58 / 255.0f;
        t2 = 58/ 255.0f;
    }
    
    float *tn = new float[3];
    tn[0] = t;
    tn[1] = t1;
    tn[2] = t2;
    return tn;
}

/* 
 * Function fo find the maximum value in array 
 * When doing fault algorithm cuts, the height value may not go to maximum height
 * The more cutes more likely height value go to maximum height
 * Refference : http://mathbits.com/MathBits/CompSci/Arrays/max.htm
 */
void maximumValue(float ** a, int size) {
    /* start with max = first element */
    
    float max = a[0][0];
    for (int x = 0; x < size - 1; x++) {
        
        for (int z = 0; z < size - 1; z++) {
            if (a[x][z] > max) {
                max = a[x][z];
            }
        }
    }
    maxHeight = max;
    
}

/* 
 * Function fo find the minimum value in array 
 * Less cuts given less likely go to minimum height
 */
void minValue(float ** a, int size) {
    // start with max = first element
    
    float min = a[0][0];
    for (int x = 0; x < size - 1; x++) {
        
        for (int z = 0; z < size - 1; z++) {
            if (a[x][z] < min) {
                min = a[x][z];
            }
        }
    }
    minHeight = min;
    
}

/*
 * OpenGL only does correct shading and lighting if and only if
 * Normals are calculates correctly.
 * We need to Normalize vector and face vector using different math 
 * hence using cross product here
 * Refference :http://www.opengl-tutorial.org/beginners-tutorials/tutorial-8-basic-shading/
 */
void Terrain::vertexNormal(){
    /* iterate starts*/
    for (int x = 0; x < this->terrainSize-1; x++) {
        for (int z = 0; z < this->terrainSize-1; z++){
            
            /* create vertex and store in rt for cross product */            
            v.x =x;
            v.y =heightMap[x][z];
            v.z =z;
            rt.push_back(v);
            
            
            v.x =x+1;
            v.y =heightMap[x+1][z];
            v.z =z;
            rt.push_back(v);
            
            
            v.x =x;
            v.y =heightMap[x][z+1];
            v.z =z+1;
            rt.push_back(v);
            cross();
            
            k.push_back(v);
            rt.clear();
            
        }
    }
    
    /* normalize face vectors */
    for (int z = 0; z < k.size(); z++){
        v.x =k.at(z).x;
        v.y =k.at(z).y;
        v.z =k.at(z).z;
        
        /* call normalize to do the cross product vectors */
        normalize();
        k.at(z) = v;
    }
    
}

/* 
 * 2D terrain overview
 * Display a second GLUUT window and draw a 2D overhead representation of the
 * terrain in. Each pixel in the view coloured according to the grayscale colours
 * depending on the different height values
 */

void Terrain::overviewDraw(){
	/* using PointSize 5 */
    glPointSize(5);
    
    /*iterate through our height map */
    for (int x = 0; x < this->terrainSize; x++){
        for (int z = 0; z < this->terrainSize; z++){
               glBegin(GL_POINTS);
                /* draw a point, and depending on the height of the point, set a color */
                glColor3f(heightMap[x][z]/10, heightMap[x][z]/10, heightMap[x][z]/10);
                glVertex2f(x*0.02, z*0.02);           
            
            glEnd();
            
        }
    }
    
}

/*
 * Draw terrain as heightmap ( 2D array of height values)
 * Once the heightmap is loaded, use openGL quad strips primititives 
 */

void Terrain::draw() {

	/* 
	 * Toggle wirefram mode between three option
	 * 1. Solid polygons 2. Wirefram Version of the terrain 3. Both solid and wirefram
	 */
    if (wireMode == 1){
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL); //basic GLUT function
    } else if (wireMode == 2) {
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
    } else if (wireMode == 3){
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        
		/* 
		 * using index of the heightmap for every xz vertex, 
		 * and use the corresponding y value. 
		 * Colour your terrain according to topographic maps. 
		 * The lowest parts should be shades of green,
		 * and higher parts should become progressively more orange/red.
		 */ 
        for (int x = 0; x < this->terrainSize-1; x++) {
            for (int z = 0; z < this->terrainSize-1; z++) {
                
				/* draw a quad for each 4 dots on the heightmap */
                glBegin(GL_QUADS);
                
                if(heightMap[x][z]>0){                    
                    glColor3f(heightMap[x][z]/maxHeight, heightMap[x][z]/maxHeight, heightMap[x][z]/maxHeight);
                }else{
                    glColor3f(0,1,0); /* green */
                }
                
                glVertex3f(x, heightMap[x][z], z);
                
                if(heightMap[x+1][z]>0){
                    glColor3f(heightMap[x+1][z]/maxHeight, heightMap[x+1][z]/maxHeight, heightMap[x+1][z]/maxHeight);
                }else{
                    glColor3f(0,1,0);
                    
                }
                glVertex3f(x+1, heightMap[x+1][z], z);
                
                if(heightMap[x+1][z+1]>0){
                    glColor3f(heightMap[x+1][z+1]/maxHeight, heightMap[x+1][z+1]/maxHeight, heightMap[x+1][z+1]/maxHeight);
                }else{
                    glColor3f(0,1,0);
                    
                }
                
                glVertex3f(x+1, heightMap[x+1][z+1], z+1);
                
                if(heightMap[x+1][z+1]>0){
                    glColor3f(heightMap[x+1][z+1]/maxHeight, heightMap[x+1][z+1]/maxHeight, heightMap[x+1][z+1]/maxHeight);
                }else{
                    glColor3f(1,0,0); /* red */
                    
                }
                glVertex3f(x, heightMap[x][z+1], z+1);
                
                glEnd();         
                
            }
            
        }
         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        
         }
       
    /* if color mode is on, generate 2 textures */ 

    if (colMode == true){
        
        /* if light mode is on, turn on light as requested */
        if (lightOn == true) {

            /* material 1 settings */
            /* Ambient, Specular, Attenuation and Gamma*/
            float amb[4] = { 0.4, 0.6, 0.3, 1 };
            float diff[4] = { 1, 0, 0, 1 };
            float spec[4] = { 0.3, 0, 1, 1 };
            /* material 2 settings */
            float m_amb[] = { 0.6, .6, 0.6, 1.0 };
            float m_dif[] = { 0.78, 0.57, 0.11, 1.0 };
            float m_spec[] = { 0.99, 0.91, 0.81, 1.0 };
            float shiny = 27;
            
            /* turn on ambient lighting and set position of light */
            GLfloat ambientColor[] = { 0.3f, 0.3f, 0.3f, 1.0f };
            glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
            GLfloat lightPos0[] = { terrainSize / 2.0f, maxHeight, terrainSize
                / 2.0f, terrainSize / 2.0f };
            
            /* set materials according to material 2*/
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_amb);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_dif);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_spec);
            glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny);
            
            /*set light properties according to material 1*/
            glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
            glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
            glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
            glLightfv(GL_LIGHT0, GL_SPECULAR, spec);
            
            
            /* set all the normals for shading and lighting purpose*/
            for (int x = 0; x < this->terrainSize - 1; x++) {
                glNormal3f(k.at(x).x, k.at(x).y, k.at(x).z);
                
            }
            
        }
        
        for (int x = 0; x < this->terrainSize - 1; x++) {
            
            for (int z = 0; z < this->terrainSize - 1; z++) {
                float* temp;
                /* call maximumValue to get max height */
                maximumValue(heightMap, terrainSize);
                /* call minValue to get min height */
                minValue(heightMap, terrainSize);
                /* Begin to draw using QUAD */
                glBegin(GL_QUADS);
                
				/* 
				 * using index of the heightmap for every xz vertex, 
				 * and use the corresponding y value. 
				 * Colour your terrain according to topographic maps. 
				 * The lowest parts should be shades of green,
				 * and higher parts should become progressively more orange/red.
				 */ 
                temp = c(x, heightMap[x][z], z, heightMap);
                glColor3f(temp[0], temp[1], temp[2]);
                glVertex3f(x, heightMap[x][z], z);
                
                temp = c(x + 1, heightMap[x + 1][z], z, heightMap);
                glColor3f(temp[0], temp[1], temp[2]);
                glVertex3f(x + 1, heightMap[x + 1][z], z);
                
                temp = c(x + 1, heightMap[x + 1][z + 1], z + 1, heightMap);
                glColor3f(temp[0], temp[1], temp[2]);
                glVertex3f(x + 1, heightMap[x + 1][z + 1], z + 1);
                
                temp = c(x, heightMap[x][z + 1], z+1 , heightMap);
                glColor3f(temp[0], temp[1], temp[2]);
                glVertex3f(x, heightMap[x][z + 1], z + 1);
                
                glEnd();
                
            }
            
        }

      /* no textures for materials and lighting while color mode is off */
    } else if (colMode == false){
    

    
    /* light setting */
    
    if (lightOn == true){
        
        /* Ambient, Specular, Attenuation and Gamma*/
       
    	/* Material 1 */
        float amb[4] = {0.4, 0.6, 0.3, 1};
        float diff[4] = {1,0,0, 1};
        float spec[4] = {0.3,0,1, 1};
    
    	/* Material 2 */
        float m_amb[] = {0.6, .6, 0.6, 1.0};
        float m_dif[] = {0.78, 0.57, 0.11, 1.0};
        float m_spec[] = {0.99, 0.91, 0.81, 1.0};
        float shiny = 27;
    
        /* turn on ambient lighting and set position of light */
        GLfloat ambientColor[] = {0.3f, 0.3f, 0.3f, 1.0f};
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
        /* 
         * Set light position according to terrain size and height values 
         * Make sure camera is looking at the right position
         */
        GLfloat lightPos0[] = {terrainSize/2.0f, maxHeight, terrainSize/2.0f, terrainSize/2.0f};
    
    
        /* set materials using Material 2 */
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_amb);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_dif);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_spec);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny);
    
        /*s et light properties using Material 1 */
        glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
        glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
        glLightfv(GL_LIGHT0, GL_SPECULAR, spec);
    
    
        /*set all the normals */
        for (int x = 0; x < this->terrainSize-1; x++) {
            glNormal3f(k.at(x).x,k.at(x).y,k.at(x).z);
        
        }
    
    }
		/* 
		 * REPEAT
		 * using index of the heightmap for every xz vertex, 
		 * and use the corresponding y value. 
		 * Colour your terrain according to topographic maps. 
		 * The lowest parts should be shades of green,
		 * and higher parts should become progressively more orange/red.
		 */ 
    for (int x = 0; x < this->terrainSize-1; x++) {
        for (int z = 0; z < this->terrainSize-1; z++) {
            
            //draw a quad for each 4 dots on the heightmap
            glBegin(GL_QUADS);
            
            if(heightMap[x][z]>0){
                
                glColor3f(heightMap[x][z]/maxHeight, heightMap[x][z]/maxHeight, heightMap[x][z]/maxHeight);
            }else{
                glColor3f(0,1,0); /* green color*/
            }
            
            glVertex3f(x, heightMap[x][z], z);
            
            if(heightMap[x+1][z]>0){
                glColor3f(heightMap[x+1][z]/maxHeight, heightMap[x+1][z]/maxHeight, heightMap[x+1][z]/maxHeight);
            }else{
                glColor3f(0,1,0);
                
            }
            glVertex3f(x+1, heightMap[x+1][z], z);
            
            if(heightMap[x+1][z+1]>0){
                glColor3f(heightMap[x+1][z+1]/maxHeight, heightMap[x+1][z+1]/maxHeight, heightMap[x+1][z+1]/maxHeight);
            }else{
                glColor3f(0,1,0);
                
            }
            
            glVertex3f(x+1, heightMap[x+1][z+1], z+1);
            
            if(heightMap[x+1][z+1]>0){
                glColor3f(heightMap[x+1][z+1]/maxHeight, heightMap[x+1][z+1]/maxHeight, heightMap[x+1][z+1]/maxHeight);
            }else{
                glColor3f(1,0,0); /* red color*/
                
            }
            glVertex3f(x, heightMap[x][z+1], z+1);
            
            glEnd(); /* End of drawing */
                       
        }
      
      }
        
   }
}

