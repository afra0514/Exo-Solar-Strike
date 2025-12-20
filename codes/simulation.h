#ifndef SIMULATION_H
#define SIMULATION_H
#include "utils.h"  
void renderSolarSystem(); 
void renderSunEarthMoon();  
void drawOrbit(float radius); 
void drawPlanetName(float yOffset, const char* name);
#endif