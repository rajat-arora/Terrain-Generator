//
//  Terrain.cpp
//  TerrainGenerator
//
//  Copyright (c) 2014  Rajat Arora Jake Harwood. All rights reserved.
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
#include <math.h>
#include <stdio.h>
#include <vector>



//contains the max/min height the terrain will be
float maxHeight = 50;
float minHeight = -10;

using namespace std;

//Setter for wireframe mode
void Terrain::setWireMode(int wMode){
    this->wireMode = wMode;
}

//Setter for turning on light
void Terrain::setLight(bool onoff){
    this->lightOn = onoff;
}
//Setter for turning on terrain color
void Terrain::setColMode(bool cm){
    this->colMode = cm;
}

//constructor, sets terrain size, and the number of cuts of the field

Terrain::Terrain(int const terrainSize, int numberOfCuts){
    
    
    this->terrainSize = terrainSize;
    this->numberOfCuts = numberOfCuts;
    this->heightMap = new float *[terrainSize]; // dynamic array (size 10) of pointers to int
    this->vertexVects = new float *[terrainSize];
    this->surfaceVects = new float *[terrainSize];
    
    for (int i = 0; i < terrainSize; ++i) {
        heightMap[i] = new float[terrainSize];
        vertexVects[i] = new float[terrainSize];
        surfaceVects[i] = new float[terrainSize];
        // each i-th pointer is now pointing to dynamic array (size 10) of actual int values
    }
    
    
    
}


//generate the terrain
void Terrain::generate() {
    //set default values to heightmap
    for (int x = 0; x < this-> terrainSize; x++) {
        for (int z = 0; z < terrainSize; z++) {
            heightMap[x][z] = 10;
            
        }
    }
 	//set the number of iterations to cut
    int iterations = this->terrainSize + 50 + this->numberOfCuts;
    
    
    //generate the heightmap and store it,
    //algorithm reference: http://www.lighthouse3d.com/opengl/terrain/index.php?fault
    
    for (int i = 0; i < iterations; i++) {
        
        float v = rand();
        float a = sinf(v);
        float b = cosf(v);
        float d = sqrtf(2*(terrainSize*terrainSize));
        float c = ((float) rand() / RAND_MAX) * d - d/2.0;
        
        float displacement = 0.9;
        
        // iterate through each point for the terrain and
        //determine the height for each point
        for (int x = 0; x < terrainSize; x++) {
            for (int z = 0; z < terrainSize; z++) {
                
                if (a*x + b*z - c > 0)
                    if ( (heightMap[x][z] + displacement) < maxHeight){
                        heightMap[x][z] += displacement ;
                    }else{
                        heightMap[x][z] = maxHeight;
                    }
                
                    else
                        if((heightMap[x][z] - displacement) > minHeight){
                            heightMap[x][z] -= displacement ;
                        }
            }
            
        }
        displacement += 1;
    }
}



//A strut that will be used as a vector
struct ve {
    float x;
    float y;
    float z;
}v;

//will hold our normals, and vectors
vector<ve> rt;
vector<ve> k;

//normalize each vector
void normalize() {
    float m = sqrt((v.x * v.x) + (v.y * v.y) + (v.z *v.z));
    v.x = v.x /m;
    v.y = v.y/m;
    v.z =v.z/m;
    
}


//cross product each point
//and find the face vector
//store it into rt
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


//This is for color mode.
//Depending on the height we set a color
//further up we go, more red, lower, more green
//returns an array that holds rgb colors
float* c(float x, float y, float z, float **heightMap) {
    // rgb colours
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



//when we do our cuts, the heights, may not
//go to our max height. The more cuts we do, the more likely
//the less, the less likely
//just in case we find the max height in our heightmap
//taken from http://mathbits.com/MathBits/CompSci/Arrays/max.htm
void maximumValue(float ** a, int size) {
    // start with max = first element
    
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


//create our vertex and store in rt
//then do cross product
//from there store our cross product vectors for each vector into k.
void Terrain::vertexNormal(){
    
    for (int x = 0; x < this->terrainSize-1; x++) {
        for (int z = 0; z < this->terrainSize-1; z++){
            
            
            
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
    
    //now iterate through our face vectors and normalize them
    //store into k
    for (int z = 0; z < k.size(); z++){
        v.x =k.at(z).x;
        v.y =k.at(z).y;
        v.z =k.at(z).z;
        
        
        normalize();
        k.at(z) = v;
    }
    
}



//this does the overview drawing of our terrain
void Terrain::overviewDraw(){
   
    
    glPointSize(5);
    
    //iterate through our height map
    //draw a point, and depending on the height of the point, set a color
    for (int x = 0; x < this->terrainSize; x++){
        for (int z = 0; z < this->terrainSize; z++){
               glBegin(GL_POINTS);
            
                glColor3f(heightMap[x][z]/10, heightMap[x][z]/10, heightMap[x][z]/10);
            
                glVertex2f(x*0.02, z*0.02);
            
            
            
            
            
            glEnd();
            
        }
    }
    
    
}

//draw the terrain
void Terrain::draw() {

    //turn on wireframe mode
    if (wireMode == 1){
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
    } else if (wireMode == 2) {
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
    } else if (wireMode == 3){
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        
        //iterate through height map
        for (int x = 0; x < this->terrainSize-1; x++) {
            for (int z = 0; z < this->terrainSize-1; z++) {
                
				//draw a quad for each 4 dots on the heightmap
                glBegin(GL_QUADS);
                
                
                if(heightMap[x][z]>0){
                    
                    glColor3f(heightMap[x][z]/maxHeight, heightMap[x][z]/maxHeight, heightMap[x][z]/maxHeight);
                }else{
                    glColor3f(0,1,0);
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
                    glColor3f(1,0,0);
                    
                }
                glVertex3f(x, heightMap[x][z+1], z+1);
                
                glEnd();
                
                
                
                
            }
            
        }
         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        
         }
    
	 
    //generate 2 texture IDs, store them in array "textures"
    
    //if color mode is on
    if (colMode == true){
        
        //if light mode is on, turn on light as requested
        if (lightOn == true) {

            //set is material settings
            //taken from profs notes
            float amb[4] = { 0.4, 0.6, 0.3, 1 };
            float diff[4] = { 1, 0, 0, 1 };
            float spec[4] = { 0.3, 0, 1, 1 };
            
            float m_amb[] = { 0.6, .6, 0.6, 1.0 };
            float m_dif[] = { 0.78, 0.57, 0.11, 1.0 };
            float m_spec[] = { 0.99, 0.91, 0.81, 1.0 };
            float shiny = 27;
            
            //turn on ambient lighting and set position of light
            GLfloat ambientColor[] = { 0.3f, 0.3f, 0.3f, 1.0f };
            glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
            GLfloat lightPos0[] = { terrainSize / 2.0f, maxHeight, terrainSize
                / 2.0f, terrainSize / 2.0f };
            
            //set materials
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_amb);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_dif);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_spec);
            glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny);
            
            //set light properties
            glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
            glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
            glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
            glLightfv(GL_LIGHT0, GL_SPECULAR, spec);
            
            
            // set all the normals
            for (int x = 0; x < this->terrainSize - 1; x++) {
                glNormal3f(k.at(x).x, k.at(x).y, k.at(x).z);
                
            }
            
        }
        
        for (int x = 0; x < this->terrainSize - 1; x++) {
            
            for (int z = 0; z < this->terrainSize - 1; z++) {
                float* temp;
                maximumValue(heightMap, terrainSize);
                minValue(heightMap, terrainSize);
                glBegin(GL_QUADS);
              
                
                
                
                //get the color for specific terrain height, and
                //then set color and draw vertex
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

        
        
        
    
      // else if color mode is off
    } else if (colMode == false){
    

    
    //some light properties
    
    if (lightOn == true){
        
        //set is material settings
        //taken from profs notes
        float amb[4] = {0.4, 0.6, 0.3, 1};
        float diff[4] = {1,0,0, 1};
        float spec[4] = {0.3,0,1, 1};
    
        float m_amb[] = {0.6, .6, 0.6, 1.0};
        float m_dif[] = {0.78, 0.57, 0.11, 1.0};
        float m_spec[] = {0.99, 0.91, 0.81, 1.0};
        float shiny = 27;
    
        //turn on ambient lighting and set position of light
        GLfloat ambientColor[] = {0.3f, 0.3f, 0.3f, 1.0f};
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
        GLfloat lightPos0[] = {terrainSize/2.0f, maxHeight, terrainSize/2.0f, terrainSize/2.0f};
    
    
        //set materials
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_amb);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_dif);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_spec);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny);
    
        //set light properties
        glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
        glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
        glLightfv(GL_LIGHT0, GL_SPECULAR, spec);
    
    
        // set all the normals
        for (int x = 0; x < this->terrainSize-1; x++) {
            glNormal3f(k.at(x).x,k.at(x).y,k.at(x).z);
        
        }
    
    }

    //iterate through height map
    for (int x = 0; x < this->terrainSize-1; x++) {
        for (int z = 0; z < this->terrainSize-1; z++) {
            
            //draw a quad for each 4 dots on the heightmap
            glBegin(GL_QUADS);
            
            
            if(heightMap[x][z]>0){
                
                glColor3f(heightMap[x][z]/maxHeight, heightMap[x][z]/maxHeight, heightMap[x][z]/maxHeight);
            }else{
                glColor3f(0,1,0);
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
                glColor3f(1,0,0);
                
            }
            glVertex3f(x, heightMap[x][z+1], z+1);
            
            glEnd();
            
            
        
            
        }
      
      }
        
   }
}














