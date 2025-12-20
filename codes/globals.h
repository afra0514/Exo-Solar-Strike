#ifndef GLOBALS_H
#define GLOBALS_H
#include "definitions.h"  
extern int WIDTH;
extern int HEIGHT; 
extern State currentState; 
extern float solarAngle, moonAngle, cameraZoom, infoZoom;
extern float camX, camY; 
extern float shipX, shipY;
extern float asteroidX[5], asteroidY[5], asteroidRot[5], asteroidScale[5];
extern int score, difficultyLevel, currentInfoIndex;
extern bool gameRunning;
extern Star stars[STAR_COUNT];
extern GLuint planetTextures[15];
#endif
