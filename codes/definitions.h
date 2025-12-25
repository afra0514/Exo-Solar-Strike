 #ifndef DEFINITIONS_H
#define DEFINITIONS_H
#include <GL/glut.h>
#include <windows.h>
#include <mmsystem.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h> 
extern int WIDTH;
extern int HEIGHT;
const float PI = 3.1415926535;
const int STAR_COUNT = 800; 
enum State { WELCOME, MENU, INFO, SOLAR_SIM, SUN_EARTH_MOON, GAME_DIFFICULTY, GAME, GAME_OVER }; 
enum PlanetID {
    TEX_SUN = 0, TEX_MERCURY, TEX_VENUS, TEX_EARTH, TEX_MOON,
    TEX_MARS, TEX_JUPITER, TEX_SATURN, TEX_URANUS, TEX_NEPTUNE, TEX_ASTEROID
}; 
struct Star { float x, y, z, brightness, speed, r, g, b; };
#endif