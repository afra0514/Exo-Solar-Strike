#ifndef UTILS_H
#define UTILS_H
#include "globals.h"
void drawText(float x, float y, float z, const char* string, void* font = GLUT_BITMAP_HELVETICA_12);
void drawStrokeText(float x, float y, float z, const char* string, float scale, float r, float g, float b, float lineWidth);
void drawBackground();
void drawGlassBox(float x, float y, float w, float h);
void drawTexturedPlanet(float radius, int textureID);
void loadAllTextures();
void initStars();
void initLighting();
void playStateMusic(const char* fileName);
GLuint LoadTexture(const char* filename);
#endif
