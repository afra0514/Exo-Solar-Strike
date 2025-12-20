#include "globals.h"  
int WIDTH = 1000; 
int HEIGHT = 700;
State currentState = WELCOME;
float solarAngle = 0.0f;
float moonAngle = 0.0f;
float cameraZoom = 45.0f;
float infoZoom = 1.0f;
float camX = 0.0f; 
float camY = 0.0f; 
float shipX = 0.0f;
float shipY = -8.0f;
float asteroidX[5], asteroidY[5], asteroidRot[5], asteroidScale[5];
int score = 0;
int difficultyLevel = 1;
int currentInfoIndex = 0;
bool gameRunning = false;
Star stars[STAR_COUNT];
GLuint planetTextures[15];
