#include "simulation.h"
#include "game.h"
#include "info.h"
#include <stdio.h>
#include <stdlib.h>  
void reshape(int w, int h) {
    if (h == 0) h = 1;
    WIDTH = w;  
    HEIGHT = h; 
    glViewport(0, 0, w, h);
}  
void renderMenus() {
    glDisable(GL_LIGHTING); 
    glDisable(GL_DEPTH_TEST); 
    glMatrixMode(GL_PROJECTION); glLoadIdentity(); 
    gluPerspective(45, (float)WIDTH/HEIGHT, 0.1, 200);
    glMatrixMode(GL_MODELVIEW); glLoadIdentity(); 
    gluLookAt(0,0,40, 0,0,0, 0,1,0); 
    drawBackground(); 
    glMatrixMode(GL_PROJECTION); glLoadIdentity(); 
    gluOrtho2D(0, WIDTH, 0, HEIGHT);
    glMatrixMode(GL_MODELVIEW); glLoadIdentity(); 
    float bx = (WIDTH - 600) / 2;
    float by = (HEIGHT / 2) - 150; 
    if (currentState == WELCOME) { 
        drawGlassBox(bx, by, 600, 350); 
        drawStrokeText(bx + 85, by + 240, 0, "EXO SOLAR STRIKE", 0.33f, 0.0, 1.0, 1.0, 4.0); 
        glLineWidth(2.0); glColor3f(1.0, 0.8, 0.0); 
        glBegin(GL_LINES); 
            glVertex2f(bx + 80, by + 225); 
            glVertex2f(bx + 520, by + 225); 
        glEnd(); 
        drawStrokeText(bx + 140, by + 130, 0, "Press ENTER to Start", 0.22f, 1.0, 0.6, 0.1, 2.0); 
        glColor3f(0.8, 0.8, 0.8); 
        drawText(bx + 245, by + 60, 0, "Press 'Q' to Exit", GLUT_BITMAP_HELVETICA_12); 
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(0.0, 0.0, 0.0, 0.7); 
        glBegin(GL_QUADS);
            glVertex2f(0, 0); glVertex2f(WIDTH, 0); glVertex2f(WIDTH, 40); glVertex2f(0, 40);
        glEnd();
        glDisable(GL_BLEND); 
        float footerX = (WIDTH / 2) - 180; 
        glColor3f(0.0, 0.9, 1.0);  
int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
const char* line1 = "Developed By Afra & Sumaiya";
int line1Width = glutBitmapLength(GLUT_BITMAP_8_BY_13, (const unsigned char*)line1);
float x1 = (windowWidth / 2.0f) - (line1Width / 2.0f);
glColor3f(1.0f, 0.7f, 0.0f);  
drawText(x1, 18, 0, line1, GLUT_BITMAP_8_BY_13); 
const char* line2 = "2025 | All Rights Reserved";
int line2Width = glutBitmapLength(GLUT_BITMAP_HELVETICA_10, (const unsigned char*)line2);
float x2 = (windowWidth / 2.0f) - (line2Width / 2.0f);
glColor3f(0.4f, 0.6f, 1.0f);  
drawText(x2, 5, 0, line2, GLUT_BITMAP_HELVETICA_10);
    } else if (currentState == MENU) {
        drawGlassBox(bx, (HEIGHT/2) - 225, 600, 450);
        float menuBy = (HEIGHT/2) - 225;
        drawStrokeText(bx + 180, menuBy + 370, 0, "MAIN MENU", 0.35f, 1.0, 1.0, 1.0, 4.0);
        drawStrokeText(bx + 110, menuBy + 280, 0, "[1] Solar System Simulation", 0.20f, 0.0, 0.9, 1.0, 2.0);
        drawStrokeText(bx + 110, menuBy + 220, 0, "[2] Sun-Earth-Moon Simulation", 0.20f, 0.8, 0.8, 1.0, 2.0);
        drawStrokeText(bx + 110, menuBy + 160, 0, "[3] Galaxy Info Data", 0.20f, 1.0, 0.8, 0.2, 2.0);
        drawStrokeText(bx + 110, menuBy + 100, 0, "[4] Asteroid Game", 0.20f, 1.0, 0.3, 0.3, 2.0);
        glColor3f(0.8, 0.8, 0.8); 
        drawText(bx + 235, menuBy + 40, 0, "Press 'Q' to Exit", GLUT_BITMAP_HELVETICA_12);
    } else if (currentState == GAME_OVER) {
        drawGlassBox(bx, (HEIGHT/2) - 225, 600, 450);
        float goBy = (HEIGHT/2) - 225;
        drawStrokeText(bx + 160, goBy + 280, 0, "GAME OVER", 0.4f, 1.0, 0.1, 0.1, 5.0);
        char buf[30]; sprintf(buf, "Score: %d", score); 
        drawStrokeText(bx + 210, goBy + 200, 0, buf, 0.3f, 1.0, 1.0, 1.0, 3.0);
        drawStrokeText(bx + 130, goBy + 100, 0, "ENTER : Retry", 0.2f, 0.0, 1.0, 0.0, 2.0);
        drawStrokeText(bx + 130, goBy + 60, 0, "ESC : Main Menu", 0.2f, 0.8, 0.8, 0.8, 2.0);
    }
}  
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   
    if(currentState == WELCOME || currentState == MENU || currentState == GAME_OVER) {
        renderMenus();
    } 
    else if (currentState == GAME_DIFFICULTY) {
        renderDifficultyMenu();
    } 
    else if (currentState == INFO) {
        renderInfoScreen();
    } 
    else {
        glEnable(GL_DEPTH_TEST); 
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glBindTexture(GL_TEXTURE_2D, 0); 
        glColor3f(1.0f, 1.0f, 1.0f);    
        GLfloat lightPos[] = { 0.0f, 0.0f, 0.0f, 1.0f }; 
        glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
        if(currentState == SOLAR_SIM) renderSolarSystem();
        else if(currentState == SUN_EARTH_MOON) renderSunEarthMoon();
        else if(currentState == GAME) renderGame();
    }
    glutSwapBuffers();
}  
void update(int value) {
    if (currentState != MENU && currentState != WELCOME && currentState != INFO) { 
        solarAngle += 0.3f; 
        moonAngle += 1.5f; 
    }
    if (currentState == GAME && gameRunning) {
        updateGameLogic();
    }
    glutPostRedisplay(); 
    glutTimerFunc(16, update, 0);
}  
void keyboard(unsigned char key, int x, int y) {
    if (key == 'q' || key == 'Q') exit(0);
    if(currentState == WELCOME && key == 13) { 
        currentState = MENU; 
        playStateMusic("simulation_music.wav"); 
    }
    else if(currentState == MENU) {
        if(key == '1') { currentState = SOLAR_SIM; cameraZoom = 45.0; camX = 0; camY = 0; }
        if(key == '2') { currentState = SUN_EARTH_MOON; cameraZoom = 30.0; camX = 0; camY = 0; }
        if(key == '3') { currentState = INFO; currentInfoIndex = 0; infoZoom = 1.0f; }
        if(key == '4') { currentState = GAME_DIFFICULTY; }
    }
    else if(currentState == GAME_DIFFICULTY) {
        if(key == '1' || key == '2' || key == '3') { 
            difficultyLevel = (key - '0'); 
            currentState = GAME; 
            score = 0; 
            gameRunning = true; 
            initGameVars(); 
            playStateMusic("game_music.wav"); 
        }
        if(key == 27) { currentState = MENU; playStateMusic("simulation_music.wav"); }
    }
    else if(currentState == INFO) {
        if(key == 27) { currentState = MENU; playStateMusic("simulation_music.wav"); }
        if(key == 'z' || key == 'Z') infoZoom += 0.1f; 
        if(key == 'x' || key == 'X') infoZoom -= 0.1f;
        if(infoZoom < 0.5f) infoZoom = 0.5f; 
        if(infoZoom > 3.0f) infoZoom = 3.0f;
    }
    else if(currentState == SOLAR_SIM || currentState == SUN_EARTH_MOON) {
        if(key == 27) { currentState = MENU; playStateMusic("simulation_music.wav"); }
        if(key == 'z' || key == 'Z') cameraZoom -= 1.0f; 
        if(key == 'x' || key == 'X') cameraZoom += 1.0f;
        if(cameraZoom < 5.0) cameraZoom = 5.0; 
        if(cameraZoom > 150.0) cameraZoom = 150.0;
    }
    else if(currentState == GAME_OVER) {
        if(key == 27) { currentState = MENU; playStateMusic("simulation_music.wav"); }
        if(key == 13) currentState = GAME_DIFFICULTY;
    }
    else if(key == 27) { currentState = MENU; playStateMusic("simulation_music.wav"); }
}  
void specialKeys(int key, int x, int y) {
    if(currentState == GAME && gameRunning) {
        float s = 0.6f;
        if(key == GLUT_KEY_LEFT && shipX > -9.0f) shipX -= s;
        if(key == GLUT_KEY_RIGHT && shipX < 9.0f) shipX += s;
        if(key == GLUT_KEY_UP && shipY < 6.0f) shipY += s;
        if(key == GLUT_KEY_DOWN && shipY > -9.0f) shipY -= s;
    } 
    else if(currentState == SOLAR_SIM || currentState == SUN_EARTH_MOON) {
        float scrollSpeed = 1.0f;
        if(key == GLUT_KEY_LEFT) camX -= scrollSpeed;
        if(key == GLUT_KEY_RIGHT) camX += scrollSpeed;
        if(key == GLUT_KEY_UP) camY += scrollSpeed;
        if(key == GLUT_KEY_DOWN) camY -= scrollSpeed;
    }
    if(currentState == INFO) {
        if(key == GLUT_KEY_RIGHT) { currentInfoIndex++; if(currentInfoIndex > 10) currentInfoIndex = 0; }
        if(key == GLUT_KEY_LEFT) { currentInfoIndex--; if(currentInfoIndex < 0) currentInfoIndex = 10; }
    }
} 
int main(int argc, char** argv) {
    glutInit(&argc, argv); 
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT); 
    glutCreateWindow("Exo Solar Strike");
    initStars(); 
    initLighting(); 
    loadAllTextures();
    playStateMusic("simulation_music.wav");
    glutDisplayFunc(display); 
    glutReshapeFunc(reshape); 
    glutTimerFunc(16, update, 0);
    glutKeyboardFunc(keyboard); 
    glutSpecialFunc(specialKeys);
    glutMainLoop(); 
    return 0;
}