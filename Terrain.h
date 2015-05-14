//
//  Terrain.h
//  TerrainGenerator
//
//  Copyright (c) 2014  Rajat Arora Jake Harwood. All rights reserved.
//

#ifndef __TerrainGenerator__Terrain__
#define __TerrainGenerator__Terrain__

#include <stdio.h>
#include <vector>

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

using namespace std;

class Terrain{
    
public:
    
    //Identify all of our public functions
    Terrain(int terrainSize,int numberOfCuts  );
    void generate();
    float getHeight();
    void setNumberOfCuts();
    void GetNumberOfCuts();
    void generateTerrain();
    void draw();
    void vertexNormal();
    void setWireMode(int wMode);
    void setLight(bool onoff);
    void overviewDraw();
    void setColMode(bool cm);
    
    
    
    
private:
    
    // Identify our private function
    float * crossProduct(void);
    
    // Identify our private data structures
    float ** heightMap ;
    float ** vertexVects;
    float ** surfaceVects;
    
    // Identify our private variables
    int terrainSize;
    int numberOfCuts ;
    int wireMode = 1;
    bool lightOn = false;
    bool colMode;
    
    
    
};



#endif /* defined(__TerrainGenerator__Terrain__) */