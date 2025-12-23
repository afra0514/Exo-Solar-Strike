#include "info.h"
#include <string.h>
#include <math.h>
#include <stdio.h> 
#include <GL/glut.h>

extern int WIDTH, HEIGHT;
extern int currentInfoIndex;
extern float infoZoom; 
 
extern void drawTexturedPlanet(float radius, int textureIndex);
extern void drawGlassBox(float x, float y, float w, float h);
extern void drawStrokeText(float x, float y, float z, const char* string, float scale, float r, float g, float b, float lineWidth);
extern void drawBackground();

#define TEX_SUN 0
#define TEX_MERCURY 1
#define TEX_VENUS 2
#define TEX_EARTH 3
#define TEX_MOON 4
#define TEX_MARS 5
#define TEX_JUPITER 6
#define TEX_SATURN 7
#define TEX_URANUS 8
#define TEX_NEPTUNE 9

struct CelestialBodyInfo {
    const char* name; const char* type; const char* realRadius;
    const char* gravity; const char* realTemp; const char* distFromSun;
    const char* orbitPeriod; const char* rotation; const char* moons;
    const char* description;
    float r, g, b; bool hasRing; float simSize;
}; 

CelestialBodyInfo db[] = {
    {"SUN", "Yellow Dwarf (G2V)", "696,340 km", "274 m/s^2", "5,500 C", "0 km", "230 M Years", "25 Days", "8 Planets", "The heart of our solar system. Contains 99.8% of the system's total mass.", 1.0, 0.6, 0.0, false, 3.5},
    {"MERCURY", "Terrestrial Planet", "2,439 km", "3.7 m/s^2", "167 C", "58 million km", "88 Days", "59 Days", "0", "Smallest planet. It shrinks slightly as its iron core cools down.", 0.7, 0.6, 0.5, false, 0.8},
    {"VENUS", "Terrestrial Planet", "6,051 km", "8.87 m/s^2", "464 C", "108 million km", "225 Days", "243 Days", "0", "Spins backwards (retrograde). Hottest planet due to greenhouse effect.", 1.0, 0.95, 0.8, false, 1.1},
    {"EARTH", "Terrestrial Planet", "6,371 km", "9.8 m/s^2", "15 C", "149.6 million km", "365.25 Days", "23h 56m", "1 (The Moon)", "The only planet known to support life. 71% of surface is water.", 0.1, 0.4, 0.9, false, 1.2},
    {"MOON", "Natural Satellite", "1,737 km", "1.62 m/s^2", "-20 to 120 C", "384,400 km (Earth)", "27 Days", "27 Days", "0", "Tidally locked to Earth, so we always see the same face.", 0.8, 0.8, 0.8, false, 0.5},
    {"MARS", "Terrestrial Planet", "3,389 km", "3.71 m/s^2", "-65 C", "227.9 million km", "687 Days", "24h 37m", "2 (Phobos, Deimos)", "Home to Olympus Mons, the largest volcano in the solar system.", 0.9, 0.3, 0.1, false, 0.9},
    {"JUPITER", "Gas Giant", "69,911 km", "24.79 m/s^2", "-110 C", "778.5 million km", "11.8 Years", "9h 56m", "95 (Io, Europa..)", "Massive enough to fit 1,300 Earths inside. Has a faint ring system.", 0.85, 0.65, 0.4, false, 2.5},
    {"SATURN", "Gas Giant", "58,232 km", "10.44 m/s^2", "-140 C", "1.4 billion km", "29.5 Years", "10h 42m", "146 (Titan..)", "Least dense planet; it would float in a giant bathtub of water.", 0.9, 0.8, 0.5, true, 2.1},
    {"URANUS", "Ice Giant", "25,362 km", "8.69 m/s^2", "-195 C", "2.9 billion km", "84 Years", "17h 14m", "27", "Rolls on its side like a ball due to a massive collision eons ago.", 0.5, 0.9, 0.9, false, 1.6},
    {"NEPTUNE", "Ice Giant", "24,622 km", "11.15 m/s^2", "-200 C", "4.5 billion km", "165 Years", "16h 6m", "14", "Has supersonic winds reaching 2,100 km/h. First planet found by math.", 0.1, 0.1, 0.7, false, 1.6},
    {"ASTEROID BELT", "Circumstellar Disc", "1 AU Width", "N/A", "-73 C", "2.2 - 3.2 AU", "3 - 6 Years", "Varies", "Millions", "A torus-shaped region between Mars and Jupiter containing many irregular bodies.", 0.6, 0.6, 0.6, false, 2.0}
}; 

void drawInfoRow(float x, float y, const char* label, const char* value) {
    glColor3f(0.0f, 1.0f, 1.0f);  
    glRasterPos2f(x, y);
    for (const char* c = label; *c != '\0'; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    glColor3f(1.0f, 1.0f, 1.0f); 
    glRasterPos2f(x + 130, y);
    for (const char* c = value; *c != '\0'; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
} 
 
void drawInfoPreview(int index) {
    if(index < 0 || index > 10) return;
 
    glDisable(GL_LIGHTING); 
    glEnable(GL_DEPTH_TEST); 
    glEnable(GL_COLOR_MATERIAL);
    glColor3f(1.0f, 1.0f, 1.0f);  

    static float rot = 0; rot += 0.5f; 
    glPushMatrix();
    glScalef(infoZoom, infoZoom, infoZoom); 
    glRotatef(20, 1, 0, 0); 
    glRotatef(rot, 0, 1, 0);

    if (index == 10) {  
        glDisable(GL_TEXTURE_2D);
        for(int i=0; i<150; i++) {
            glPushMatrix(); 
            float a = (i * 15) * 3.1416 / 180.0; 
            float r = 2.5 + ((rand()%10)/10.0);
            glTranslatef(cos(a)*r, ((rand()%10)/20.0), sin(a)*r); 
            glScalef(0.05, 0.05, 0.05);
            glColor3f(0.6, 0.5, 0.4); 
            glutSolidDodecahedron(); 
            glPopMatrix();
        }
    } else {
        int texID = 0; float s = 1.0;
        switch(index) {
            case 0: texID = TEX_SUN; s = 3.5; break; 
            case 1: texID = TEX_MERCURY; s = 0.8; break;
            case 2: texID = TEX_VENUS; s = 1.1; break; 
            case 3: texID = TEX_EARTH; s = 1.2; break;
            case 4: texID = TEX_MOON; s = 0.5; break; 
            case 5: texID = TEX_MARS; s = 0.9; break;
            case 6: texID = TEX_JUPITER; s = 2.5; break; 
            case 7: texID = TEX_SATURN; s = 2.1; break;
            case 8: texID = TEX_URANUS; s = 1.6; break; 
            case 9: texID = TEX_NEPTUNE; s = 1.6; break;
        }
        drawTexturedPlanet(s, texID);
        
        if (index == 7) { 
            glEnable(GL_BLEND); glDisable(GL_TEXTURE_2D); 
            glColor4f(0.8, 0.7, 0.5, 0.6); 
            glPushMatrix();
            glRotatef(80, 1, 0, 0); 
            glutSolidTorus(0.25, s + 1.2, 30, 40); 
            glPopMatrix();
            glDisable(GL_BLEND);
        }
    }
    glPopMatrix(); 
    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);  
}   

void drawInfoHUD(int w, int h) {
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity(); 
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0, 0.0, 0.0, 0.3); 
    glBegin(GL_QUADS); 
        glVertex2f(0, h - 70); glVertex2f(w, h - 70);
        glVertex2f(w, h); glVertex2f(0, h); 
        glVertex2f(0, 0); glVertex2f(w, 0);
        glVertex2f(w, 60); glVertex2f(0, 60);
    glEnd();
    glDisable(GL_BLEND); 
    glColor3f(1.0, 0.9, 0.0);  
    void* font = GLUT_BITMAP_HELVETICA_18; 
    glRasterPos2f(20, h - 45);
    for (const char* c = "Change (Left/Right Arrows)"; *c != '\0'; c++) glutBitmapCharacter(font, *c);
    glRasterPos2f(w - 260, h - 45);
    for (const char* c = "Zoom (IN-Z/OUT-X)"; *c != '\0'; c++) glutBitmapCharacter(font, *c);
    glRasterPos2f(20, 22); 
    for (const char* c = "BACK (ESC)"; *c != '\0'; c++) glutBitmapCharacter(font, *c);
    glRasterPos2f(w - 120, 22); 
    for (const char* c = "QUIT (Q)"; *c != '\0'; c++) glutBitmapCharacter(font, *c);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
} 

void renderInfoScreen() {
    if(currentInfoIndex > 10) currentInfoIndex = 0; 
    if(currentInfoIndex < 0) currentInfoIndex = 10;
    int w = WIDTH, h = HEIGHT;
    glViewport(0, 0, w, h); 
    glMatrixMode(GL_PROJECTION); glLoadIdentity(); 
    gluPerspective(45, (float)w/h, 0.1, 500);
    glMatrixMode(GL_MODELVIEW); glLoadIdentity(); 
    gluLookAt(0, 0, 30, 0, 0, 0, 0, 1, 0); 
    
    drawBackground();

    glPushMatrix();  
    float planetX = (w > 800) ? 8.5f : 6.0f;
    glTranslatef(planetX, 0.0, 0.0); 
    drawInfoPreview(currentInfoIndex); 
    glPopMatrix(); 
    
    glDisable(GL_LIGHTING); 
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION); glLoadIdentity(); gluOrtho2D(0, w, 0, h);
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();  
    
    float boxX = 20, boxY = 85, boxW = w/2.2, boxH = h - 170;
    drawGlassBox(boxX, boxY, boxW, boxH);
    
    CelestialBodyInfo info = db[currentInfoIndex]; 
    float textX = boxX + 25; 
    float cursorY = boxY + boxH - 45;  
    
    drawStrokeText(textX, cursorY, 0, info.name, 0.35, 1.0, 0.9, 0.0, 3.0); 
    cursorY -= 40;
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(textX, cursorY);
    for (const char* c = info.type; *c != '\0'; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    
    cursorY -= 20;  
    glLineWidth(2.0); glBegin(GL_LINES); glColor3f(0.0, 1.0, 1.0); 
    glVertex2f(textX, cursorY); glVertex2f(boxX + boxW - 30, cursorY); 
    glEnd();
    
    float gap = (h > 600) ? 30 : 25; 
    cursorY -= 40;  
    glColor3f(1.0f, 0.6f, 0.0f); 
    glRasterPos2f(textX, cursorY);
    for (const char* c = "[ PHYSICAL DATA ]"; *c != '\0'; c++) glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *c);
    
    cursorY -= gap;
    drawInfoRow(textX, cursorY, "Radius:", info.realRadius); cursorY -= gap;
    drawInfoRow(textX, cursorY, "Gravity:", info.gravity); cursorY -= gap;
    drawInfoRow(textX, cursorY, "Temp:", info.realTemp); cursorY -= gap + 10; 
    
    glColor3f(1.0f, 0.6f, 0.0f); 
    glRasterPos2f(textX, cursorY);
    for (const char* c = "[ ORBITAL DATA ]"; *c != '\0'; c++) glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *c);
    
    cursorY -= gap;
    drawInfoRow(textX, cursorY, "Dist Sun:", info.distFromSun); cursorY -= gap;
    drawInfoRow(textX, cursorY, "Year:", info.orbitPeriod); cursorY -= gap;
    drawInfoRow(textX, cursorY, "Day:", info.rotation); cursorY -= gap;
    drawInfoRow(textX, cursorY, "Moons:", info.moons); cursorY -= gap + 20; 
    
    glColor3f(0.0f, 1.0f, 0.0f);
    glRasterPos2f(textX, cursorY);
    for (const char* c = "QUICK FACT:"; *c != '\0'; c++) glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *c);
    
    cursorY -= 25; 
    glColor3f(1.0f, 1.0f, 1.0f);
    char desc[300]; strcpy(desc, info.description);
    int maxChars = (int)(boxW / 11); 
    char lines[3][100] = {"", "", ""};
    int len = strlen(desc);
    if (len > maxChars) {
        strncpy(lines[0], desc, maxChars);
        if (len > maxChars * 2) {
            strncpy(lines[1], desc + maxChars, maxChars);
            strcpy(lines[2], desc + maxChars * 2);
        } else {
            strcpy(lines[1], desc + maxChars);
        }
    } else {
        strcpy(lines[0], desc);
    }
    for(int i=0; i<3; i++) {
        if(lines[i][0] != '\0') {
            glRasterPos2f(textX, cursorY);
            for (const char* c = lines[i]; *c != '\0'; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
            cursorY -= 25;
        }
    }
    
    drawInfoHUD(w, h);
    glEnable(GL_DEPTH_TEST); 
    glEnable(GL_LIGHTING);
}