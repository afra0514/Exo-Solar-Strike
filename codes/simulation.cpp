#include "simulation.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>  
void drawHUDText(float x, float y, const char* text) {
    void* font = GLUT_BITMAP_HELVETICA_18;  
    glRasterPos2f(x, y);
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
} 
void drawSimulationHUD() {
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, WIDTH, 0, HEIGHT);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity(); 
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0, 0.0, 0.0, 0.3); 
    glBegin(GL_QUADS); 
        glVertex2f(0, HEIGHT - 70); glVertex2f(WIDTH, HEIGHT - 70);
        glVertex2f(WIDTH, HEIGHT); glVertex2f(0, HEIGHT); 
        glVertex2f(0, 0); glVertex2f(WIDTH, 0);
        glVertex2f(WIDTH, 60); glVertex2f(0, 60);
    glEnd();
    glDisable(GL_BLEND); 
    glColor3f(1.0, 0.9, 0.0);  
    drawHUDText(20, HEIGHT - 45, "Scroll (Arrow Keys)"); 
    drawHUDText(WIDTH - 260, HEIGHT - 45, "Zoom(IN-Z/OUT-X)"); 
    drawHUDText(20, 22, "BACK (ESC)"); 
    drawHUDText(WIDTH - 120, 22, "QUIT (Q)");
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
} 
void drawOrbit(float radius) {
    glDisable(GL_LIGHTING); glDisable(GL_TEXTURE_2D); glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); glColor4f(0.3, 0.5, 0.7, 0.25);
    glBegin(GL_LINE_LOOP);
    for(int i=0; i<360; i+=2) { 
        float angle = i * PI / 180.0; 
        glVertex3f(cos(angle)*radius, 0, sin(angle)*radius); 
    }
    glEnd(); glDisable(GL_BLEND); glEnable(GL_LIGHTING);
}  
void drawPlanetName(float yOffset, const char* name) {
    glDisable(GL_LIGHTING); glDisable(GL_TEXTURE_2D); glColor3f(1.0, 1.0, 1.0);
    glRasterPos3f(0.0, yOffset, 0.0);
    for (const char* c = name; *c != '\0'; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    glEnable(GL_LIGHTING);
} 
void renderSolarSystem() {
    glMatrixMode(GL_PROJECTION); glLoadIdentity(); gluPerspective(45.0, (float)WIDTH/HEIGHT, 0.1, 500.0);
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();  
    gluLookAt(camX, cameraZoom + camY, cameraZoom * 1.5, camX, camY, 0.0, 0.0, 1.0, 0.0);
    drawBackground(); 
    GLfloat sun_emission[] = { 1.0f, 0.9f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_EMISSION, sun_emission);
    glPushMatrix(); glRotatef(solarAngle * 0.2, 0.0, 1.0, 0.0); drawTexturedPlanet(4.5, TEX_SUN); glPopMatrix();
    GLfloat no_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);
    glDisable(GL_LIGHTING); drawText(-0.5, 6.5, 0.0, "SUN"); glEnable(GL_LIGHTING);
    drawOrbit(8.0); glPushMatrix(); glRotatef(solarAngle * 4.1, 0.0, 1.0, 0.0); glTranslatef(8.0, 0.0, 0.0); drawTexturedPlanet(0.6, TEX_MERCURY); drawPlanetName(1.0, "Mercury"); glPopMatrix();
    drawOrbit(12.0); glPushMatrix(); glRotatef(solarAngle * 1.6, 0.0, 1.0, 0.0); glTranslatef(12.0, 0.0, 0.0); drawTexturedPlanet(1.1, TEX_VENUS); drawPlanetName(1.5, "Venus"); glPopMatrix();
    drawOrbit(18.0); glPushMatrix(); glRotatef(solarAngle, 0.0, 1.0, 0.0); glTranslatef(18.0, 0.0, 0.0); drawTexturedPlanet(1.3, TEX_EARTH); drawPlanetName(1.8, "Earth");
        glPushMatrix(); glRotatef(solarAngle * 12.0, 0.0, 1.0, 0.0); glTranslatef(2.8, 0.0, 0.0); drawTexturedPlanet(0.4, TEX_MOON); glPopMatrix(); glPopMatrix();
    drawOrbit(25.0); glPushMatrix(); glRotatef(solarAngle * 0.5, 0.0, 1.0, 0.0); glTranslatef(25.0, 0.0, 0.0); drawTexturedPlanet(1.0, TEX_MARS); drawPlanetName(1.4, "Mars"); glPopMatrix();
    glDisable(GL_LIGHTING); glBegin(GL_POINTS); glColor3f(0.7, 0.65, 0.6);
    for(int i=0; i<2500; i++) { 
        float angle = (rand()%3600)/10.0f * (PI/180.0f) + (solarAngle*0.8f*PI/180.0f); 
        float dist = 30.0f + (rand()%40)/10.0f; 
        glVertex3f(cos(angle)*dist, ((rand()%20)-10)/20.0f, sin(angle)*dist); 
    }
    glEnd(); glEnable(GL_LIGHTING);
    drawOrbit(40.0); glPushMatrix(); glRotatef(solarAngle * 0.4, 0.0, 1.0, 0.0); glTranslatef(40.0, 0.0, 0.0); drawTexturedPlanet(2.8, TEX_JUPITER); drawPlanetName(3.5, "Jupiter"); glPopMatrix();
    drawOrbit(55.0); glPushMatrix(); glRotatef(solarAngle * 0.3, 0.0, 1.0, 0.0); glTranslatef(55.0, 0.0, 0.0); drawTexturedPlanet(2.4, TEX_SATURN); drawPlanetName(3.0, "Saturn");
        glDisable(GL_LIGHTING); glEnable(GL_BLEND); glColor4f(0.8, 0.7, 0.5, 0.7); glPushMatrix(); glRotatef(85, 1, 0, 0); glutSolidTorus(0.3, 4.0, 30, 50); glPopMatrix(); glDisable(GL_BLEND); glEnable(GL_LIGHTING); glPopMatrix();
    drawOrbit(68.0); glPushMatrix(); glRotatef(solarAngle * 0.2, 0.0, 1.0, 0.0); glTranslatef(68.0, 0.0, 0.0); drawTexturedPlanet(1.8, TEX_URANUS); drawPlanetName(2.4, "Uranus"); glPopMatrix();
    drawOrbit(78.0); glPushMatrix(); glRotatef(solarAngle * 0.1, 0.0, 1.0, 0.0); glTranslatef(78.0, 0.0, 0.0); drawTexturedPlanet(1.8, TEX_NEPTUNE); drawPlanetName(2.4, "Neptune"); glPopMatrix();
    drawSimulationHUD();
} 
void renderSunEarthMoon() {
    glMatrixMode(GL_PROJECTION); glLoadIdentity(); gluPerspective(45.0, (float)WIDTH/HEIGHT, 0.1, 300.0);
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();  
    gluLookAt(camX, (cameraZoom * 0.6) + camY, cameraZoom, camX, camY, 0.0, 0.0, 1.0, 0.0);
    drawBackground();
    GLfloat sun_emission[] = { 1.0f, 0.8f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_EMISSION, sun_emission);
    glPushMatrix(); glRotatef(solarAngle * 0.2, 0.0, 1.0, 0.0); drawTexturedPlanet(4.0, TEX_SUN); glPopMatrix();
    GLfloat no_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);
    glDisable(GL_LIGHTING); glDisable(GL_DEPTH_TEST); glColor3f(1.0, 0.9, 0.0); drawText(0.0, 5.5, 0.0, "SUN"); glEnable(GL_DEPTH_TEST); glEnable(GL_LIGHTING);
    drawOrbit(16.0);
    glPushMatrix();
        glRotatef(solarAngle, 0.0, 1.0, 0.0); glTranslatef(16.0, 0.0, 0.0);
    
        glPushMatrix(); 
            glRotatef(23.5, 0.0, 0.0, 1.0); 
            glRotatef(solarAngle * 4.0, 0.0, 1.0, 0.0); 
            drawTexturedPlanet(2.0, TEX_EARTH); 
        glPopMatrix(); 
        drawPlanetName(2.8, "EARTH");
        
        drawOrbit(4.5); 
        glPushMatrix(); 
            glRotatef(moonAngle, 0.0, 1.0, 0.0); 
            glTranslatef(4.5, 0.0, 0.0); 
            drawTexturedPlanet(0.6, TEX_MOON);
            drawPlanetName(1.0, "Moon");
        glPopMatrix();
    glPopMatrix();
    drawSimulationHUD();
}