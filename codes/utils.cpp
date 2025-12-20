#include "utils.h" 
void playStateMusic(const char* fileName) {
    char path[100];
    sprintf(path, "../music/%s", fileName);
    PlaySoundA(path, NULL, SND_ASYNC | SND_LOOP | SND_FILENAME);
}
void drawText(float x, float y, float z, const char* string, void* font) {
    glDisable(GL_TEXTURE_2D); 
    glDisable(GL_LIGHTING);  
    glRasterPos3f(x, y, z);
    for (const char* c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    } 
} 
void drawStrokeText(float x, float y, float z, const char* string, float scale, float r, float g, float b, float lineWidth) {
    glDisable(GL_TEXTURE_2D); 
    glDisable(GL_LIGHTING);  
    glPushMatrix(); 
    glTranslatef(x, y, z); 
    glScalef(scale, scale, 1.0f);
    glLineWidth(lineWidth); 
    glColor3f(r, g, b);
    for (const char* c = string; *c != '\0'; c++) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
    }
    glPopMatrix(); 
} 
void drawGlassBox(float x, float y, float w, float h) {
    glDisable(GL_TEXTURE_2D); 
    glDisable(GL_LIGHTING); 
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.05f, 0.0f, 0.12f, 0.45f);
    glBegin(GL_QUADS);
        glVertex2f(x, y); glVertex2f(x + w, y); glVertex2f(x + w, y + h); glVertex2f(x, y + h);
    glEnd(); 
    glLineWidth(3.5f); glColor3f(0.8f, 0.0f, 1.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(x, y); glVertex2f(x + w, y); glVertex2f(x + w, y + h); glVertex2f(x, y + h);
    glEnd();
    glDisable(GL_BLEND);
} 
void drawBackground() {
    glDisable(GL_LIGHTING); 
    glDisable(GL_TEXTURE_2D); 
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPointSize(1.8); 
    glBegin(GL_POINTS);
    float timeVal = glutGet(GLUT_ELAPSED_TIME) * 0.005f;
    for(int i=0; i<STAR_COUNT; i++) {
        float twinkle = 0.4f + 0.6f * sin(timeVal * 2.0 + stars[i].x);
        glColor4f(stars[i].r, stars[i].g, stars[i].b, twinkle);
        glVertex3f(stars[i].x, stars[i].y, stars[i].z);
    }
    glEnd(); 
    glDisable(GL_BLEND);
} 
void drawTexturedPlanet(float radius, int textureIndex) {
    glEnable(GL_TEXTURE_2D); 
    glBindTexture(GL_TEXTURE_2D, planetTextures[textureIndex]); 
    GLfloat mat_ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f }; 
    GLfloat mat_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient); 
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    if(textureIndex == TEX_SUN) {
        GLfloat sun_emit[] = { 1.0f, 0.9f, 0.7f, 1.0f }; 
        glMaterialfv(GL_FRONT, GL_EMISSION, sun_emit);
    } else {
        GLfloat no_emit[] = { 0.0f, 0.0f, 0.0f, 1.0f }; 
        glMaterialfv(GL_FRONT, GL_EMISSION, no_emit);
    }
    GLUquadric* quad = gluNewQuadric(); 
    gluQuadricTexture(quad, GL_TRUE); 
    gluQuadricNormals(quad, GLU_SMOOTH); 
    glPushMatrix(); 
    glRotatef(-90, 1, 0, 0); 
    gluSphere(quad, radius, 40, 40); 
    glPopMatrix(); 
    gluDeleteQuadric(quad); 
    glDisable(GL_TEXTURE_2D);
}  
GLuint LoadTexture(const char* filename) {
    GLuint tex; int w, h; unsigned char* data;
    FILE* f = fopen(filename, "rb"); if(!f) return 0;
    unsigned char head[54]; fread(head, 1, 54, f);
    w = *(int*)&head[18]; h = *(int*)&head[22];
    int size = w * h * 3; data = (unsigned char*)malloc(size); fread(data, 1, size, f); fclose(f);
    for(int i=0; i<size; i+=3) { unsigned char t=data[i]; data[i]=data[i+2]; data[i+2]=t; }
    glGenTextures(1, &tex); glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    free(data); return tex;
} 
void loadAllTextures() {
    planetTextures[TEX_SUN] = LoadTexture("textures/sun.bmp");
    planetTextures[TEX_MERCURY] = LoadTexture("textures/mercury.bmp");
    planetTextures[TEX_VENUS] = LoadTexture("textures/venus.bmp");
    planetTextures[TEX_EARTH] = LoadTexture("textures/earth.bmp");
    planetTextures[TEX_MOON] = LoadTexture("textures/moon.bmp");
    planetTextures[TEX_MARS] = LoadTexture("textures/mars.bmp");
    planetTextures[TEX_JUPITER] = LoadTexture("textures/jupiter.bmp");
    planetTextures[TEX_SATURN] = LoadTexture("textures/saturn.bmp");
    planetTextures[TEX_URANUS] = LoadTexture("textures/uranus.bmp");
    planetTextures[TEX_NEPTUNE] = LoadTexture("textures/neptune.bmp");
    planetTextures[TEX_ASTEROID] = LoadTexture("textures/asteroid.bmp");
} 
void initStars() {
    srand(time(0));
    for(int i=0; i<STAR_COUNT; i++) {
        stars[i].x = (rand() % 400) - 200; 
        stars[i].y = (rand() % 400) - 200; 
        stars[i].z = -(rand() % 150) - 50;
        int colorType = rand() % 5;
        if(colorType == 0) { stars[i].r = 0.2f; stars[i].g = 0.6f; stars[i].b = 1.0f; }
        else if(colorType == 2) { stars[i].r = 1.0f; stars[i].g = 0.7f; stars[i].b = 0.2f; }
        else { stars[i].r = 1.0f; stars[i].g = 1.0f; stars[i].b = 1.0f; }
    }
} 
void initLighting() {
    glEnable(GL_LIGHTING); 
    glEnable(GL_LIGHT0); 
    glEnable(GL_DEPTH_TEST); 
    glEnable(GL_NORMALIZE);
    GLfloat light_pos[] = { 10.0, 10.0, 10.0, 1.0 }; 
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
}
