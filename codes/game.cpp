#include "game.h"
#include <math.h>
#include <stdio.h>
float baseSpeed = 0.15f; 
float astScaleX[5], astScaleY[5], astScaleZ[5];
int astSlices[5], astStacks[5];  
void initGameVars() {
    shipX = 0.0f; shipY = -8.0f; score = 0;
    if (difficultyLevel == 1) baseSpeed = 0.15f;
    else if (difficultyLevel == 2) baseSpeed = 0.25f;
    else baseSpeed = 0.40f;
    for(int i=0; i<5; i++) {
        asteroidX[i] = (rand() % 18) - 9; 
        asteroidY[i] = 10.0f + i * 4;
        asteroidRot[i] = rand() % 360; 
        asteroidScale[i] = 0.4f + ((rand() % 30) / 100.0f); 
        astScaleX[i] = 0.75f + (rand() % 50) / 100.0f; 
        astScaleY[i] = 0.75f + (rand() % 50) / 100.0f; 
        astScaleZ[i] = 0.75f + (rand() % 50) / 100.0f;
        astSlices[i] = 6 + rand() % 4; 
        astStacks[i] = 5 + rand() % 3; 
    }
} 
void updateGameLogic() {
    if (!gameRunning) return;
    float currentSpeed = baseSpeed + (score * 0.002f);
    for(int i=0; i<5; i++) {
        asteroidY[i] -= currentSpeed;
        asteroidRot[i] += 2.0f + (difficultyLevel * 0.5f);
        float hitBoxX = (difficultyLevel == 3) ? 0.7f : 0.9f;
        float hitBoxY = 0.8f;
        if(abs(shipX - asteroidX[i]) < hitBoxX && abs(shipY - asteroidY[i]) < hitBoxY) {
            gameRunning = false; 
            currentState = GAME_OVER;
        }
        if(asteroidY[i] < -11.0f) {
            asteroidY[i] = 11.0f + (rand() % 5); 
            asteroidX[i] = (rand() % 18) - 9;
            asteroidScale[i] = 0.35f + ((rand() % 30) / 100.0f);
            astScaleX[i] = 0.75f + (rand() % 50) / 100.0f;
            astScaleY[i] = 0.75f + (rand() % 50) / 100.0f;
            astScaleZ[i] = 0.75f + (rand() % 50) / 100.0f;
            astSlices[i] = 6 + rand() % 4;
            astStacks[i] = 5 + rand() % 3;
            score += 10 * difficultyLevel;
        }
    }
} 
void drawRealAsteroid(int index) {
    glPushMatrix(); 
    glTranslatef(asteroidX[index], asteroidY[index], 0.0);
    glRotatef(asteroidRot[index], (index % 2 == 0 ? 1.0 : 0.5), 1.0, 0.3);
    float s = asteroidScale[index];
    glScalef(s * astScaleX[index], s * astScaleY[index], s * astScaleZ[index]); 
    glEnable(GL_TEXTURE_2D); 
    glBindTexture(GL_TEXTURE_2D, planetTextures[TEX_ASTEROID]);
    glShadeModel(GL_FLAT); 
    GLfloat mat_ambient[] = { 0.8f, 0.8f, 0.8f, 1.0f };    
    GLfloat mat_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };    
    GLfloat mat_emission[] = { 0.2f, 0.2f, 0.2f, 1.0f };   
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient); 
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
    glColor3f(1.0f, 1.0f, 1.0f); 
    GLUquadric* quad = gluNewQuadric(); 
    gluQuadricTexture(quad, GL_TRUE); 
    gluSphere(quad, 1.0, astSlices[index], astStacks[index]); 
    GLfloat no_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);
    gluDeleteQuadric(quad);
    glShadeModel(GL_SMOOTH); 
    glDisable(GL_TEXTURE_2D); 
    glPopMatrix();
}  
void renderDifficultyMenu() {
    glDisable(GL_LIGHTING); glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION); glLoadIdentity(); 
    gluPerspective(45.0, (float)WIDTH/HEIGHT, 0.1, 200.0);
    glMatrixMode(GL_MODELVIEW); glLoadIdentity(); 
    gluLookAt(0.0, 0.0, 40.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    drawBackground(); 
    glMatrixMode(GL_PROJECTION); glLoadIdentity(); 
    gluOrtho2D(0, WIDTH, 0, HEIGHT);
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    float bx = (WIDTH - 500) / 2; 
    float by = (HEIGHT - 400) / 2;
    drawGlassBox(bx, by, 500, 400); 
    drawStrokeText(WIDTH/2 - 210, by + 330, 0, "SELECT DIFFICULTY", 0.35f, 0.0, 1.0, 1.0, 3.0);
    drawStrokeText(WIDTH/2 - 100, HEIGHT/2 + 50, 0, "[1] EASY", 0.25f, 0.2, 1.0, 0.2, 2.0);
    drawStrokeText(WIDTH/2 - 100, HEIGHT/2, 0, "[2] MEDIUM", 0.25f, 1.0, 1.0, 0.0, 2.0);
    drawStrokeText(WIDTH/2 - 100, HEIGHT/2 - 50, 0, "[3] HARD", 0.25f, 1.0, 0.4, 0.4, 2.0);
    glColor3f(0.8, 0.8, 0.8); 
    drawText(bx + 190, by + 40, 0, "Press 'Q' to Exit", GLUT_BITMAP_HELVETICA_12);
}  
void renderGame() {
    glDisable(GL_LIGHTING); glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION); glLoadIdentity(); 
    gluPerspective(45.0, (float)WIDTH/HEIGHT, 0.1, 200.0);
    glMatrixMode(GL_MODELVIEW); glLoadIdentity(); 
    gluLookAt(0.0, 0.0, 40.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    drawBackground(); 
    glMatrixMode(GL_PROJECTION); glLoadIdentity(); 
    gluOrtho2D(-10, 10, -10, 10);
    glMatrixMode(GL_MODELVIEW); glLoadIdentity(); 
    glDisable(GL_LIGHTING); glDisable(GL_TEXTURE_2D);
    glPushMatrix(); 
    glTranslatef(shipX, shipY, 0.0);
    float blink = (rand() % 10) / 10.0f; 
    glColor3f(1.0f, 0.6f, 0.0f);
    glBegin(GL_TRIANGLES); 
        glVertex2f(-0.25, -0.7); 
        glVertex2f(0.25, -0.7); 
        glVertex2f(0.0, -1.4 - (blink*0.4)); 
    glEnd();
    glColor3f(0.9f, 0.9f, 1.0f); 
    glBegin(GL_POLYGON); 
        glVertex2f(0.0, 1.3); 
        glVertex2f(-0.4, 0.2); 
        glVertex2f(-0.45, -0.8); 
        glVertex2f(0.45, -0.8); 
        glVertex2f(0.4, 0.2); 
    glEnd();
    glColor3f(0.0f, 0.6f, 1.0f);
    glBegin(GL_TRIANGLES); glVertex2f(-0.4, 0.0); glVertex2f(-1.2, -0.9); glVertex2f(-0.4, -0.7); glEnd();
    glBegin(GL_TRIANGLES); glVertex2f(0.4, 0.0);  glVertex2f(1.2, -0.9);  glVertex2f(0.4, -0.7); glEnd();
    glColor3f(0.0f, 1.0f, 1.0f); 
    glBegin(GL_POLYGON); glVertex2f(0.0, 0.7); glVertex2f(-0.2, 0.15); glVertex2f(0.0, -0.1); glVertex2f(0.2, 0.15); glEnd();
    glPopMatrix();
    glEnable(GL_LIGHTING); glEnable(GL_DEPTH_TEST);
    GLfloat lightPos[] = { 0.0, 5.0, 25.0, 1.0 };  
    GLfloat lightAmb[] = { 0.5f, 0.5f, 0.5f, 1.0f }; 
    GLfloat lightDiff[] = { 1.3f, 1.3f, 1.3f, 1.0f }; 
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiff);
    for(int i=0; i<5; i++) drawRealAsteroid(i);
    glDisable(GL_LIGHTING); glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION); glLoadIdentity(); 
    gluOrtho2D(0, WIDTH, 0, HEIGHT);
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    drawGlassBox(0, HEIGHT - 60, WIDTH, 60);
    char buf[50]; 
    sprintf(buf, "SCORE: %d", score); 
    drawStrokeText(20, HEIGHT - 40, 0, buf, 0.25, 0.0, 1.0, 0.0, 2.0);
    const char* dStr = (difficultyLevel==1?"EASY":(difficultyLevel==2?"MEDIUM":"HARD"));
    sprintf(buf, "MODE: %s", dStr); 
    drawStrokeText(WIDTH - 250, HEIGHT - 40, 0, buf, 0.25, 1.0, 1.0, 0.0, 2.0);
}