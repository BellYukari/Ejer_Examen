#define STB_IMAGE_IMPLEMENTATION
#include <GL/glut.h>
#include <stb_image.h>
#include <cmath>
#include <iostream>

GLfloat angle = 0.0f;
GLfloat zoom = -10.0f;
GLuint cubeTextureID, sphereTextureID;


GLfloat esferaX = 0.0f, esferaY = 1.5f, esferaZ = 2.5f;
int currentVertex = 0;
GLfloat vertices[][3] = { {2.0f, 1.5f, 2.5f}, {-2.0f, 1.5f, 2.5f}, {0.0f, 1.5f, -2.5f} };

void loadTexture(const char* filename, GLuint& textureID) {
    int width, height, numChannels;
    unsigned char* data = stbi_load(filename, &width, &height, &numChannels, 0);
    if (!data) {
        std::cerr << "Error al cargar la textura: " << filename << std::endl;
        return;
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (numChannels == 3) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }
    else if (numChannels == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }

    stbi_image_free(data);
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_TEXTURE_2D);
    loadTexture("img/icon.png", cubeTextureID);   
    loadTexture("img/icon.png", sphereTextureID); 
}

void drawCube() {
    glBindTexture(GL_TEXTURE_2D, cubeTextureID);
    glBegin(GL_QUADS);

    // Cara frontal
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);

    // Cara trasera
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);

    // Cara izquierda
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);

    // Cara derecha
    glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);

    // Cara superior
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);

    // Cara inferior
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, -1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 1.0f);

    glEnd();
}

void drawTexturedSphere(float radius, int slices, int stacks) {
    glBindTexture(GL_TEXTURE_2D, sphereTextureID);
    GLUquadric* quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);
    glPushMatrix();
    glTranslatef(esferaX, esferaY, esferaZ);
    glColor3f(1.0f, 1.0f, 1.0f);
    gluSphere(quad, radius, slices, stacks);
    glPopMatrix();
    gluDeleteQuadric(quad);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, zoom);
    glRotatef(angle, 1.0f, 1.0f, 0.0f);

    drawCube();
    drawTexturedSphere(0.5f, 20, 20);  

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w / h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) exit(0);
}

void update(int value) {
    
    angle += 2.0f;
    if (angle > 360) angle -= 360;

    // Movimiento de la esfera
    static float t = 0.01f;
    esferaX = (1 - t) * vertices[currentVertex][0] + t * vertices[(currentVertex + 1) % 3][0];
    esferaY = (1 - t) * vertices[currentVertex][1] + t * vertices[(currentVertex + 1) % 3][1];
    esferaZ = (1 - t) * vertices[currentVertex][2] + t * vertices[(currentVertex + 1) % 3][2];

    t += 0.01f;
    if (t >= 1.0f) {
        t = 0.0f;
        currentVertex = (currentVertex + 1) % 3;
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Un Cubo jirando con una espera haciendole sobra");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16, update, 0);

    glutMainLoop();
    return 0;
}