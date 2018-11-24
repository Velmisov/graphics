#include <GL/freeglut.h>
#include <time.h>
#include <stdlib.h>

static int w = 0, h = 0;

double rotate_x = 0.;
double rotate_y = 0.;
double rotate_z = 0.;

int number_of_figures = 7;
int figure = 0;

// Функция вызывается перед вхождением в главный цикл
void Init() {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glutSwapBuffers();
}

void nextRandomColor() {
    glColor3f((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
}

// Функция вызывается каждый кадр для его отрисовки
void Update() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glRotated(rotate_x, 1, 0, 0);
    glRotated(rotate_y, 0, 1, 0);
    glRotated(rotate_z, 0, 0, 1);

    switch (figure) {
        case 0:
            glutWireSphere(0.5f, 20, 20);
            break;
        case 1:
            glutWireCube(0.5f);
            break;
        case 2:
            glutWireTorus(0.2, 0.5, 20, 50);
            break;
        case 3:
            glutWireTeapot(0.5);
            break;
        case 4:
            glBegin(GL_TRIANGLES);
            glVertex2f(-0.5f, -0.5f);
            glVertex2f(0.f, 0.5f);
            glVertex2f(0.5f, -0.5f);
            glEnd();
            break;
        case 5:
            glBegin(GL_QUADS);
            glVertex2f(-0.5f, -0.5f);
            glVertex2f(-0.5f, 0.5f);
            glVertex2f(0.5f, 0.5f);
            glVertex2f(0.5f, -0.5f);
            glEnd();
            break;
        case 6:
            glBegin(GL_TRIANGLES);
            glColor3f(1.f, 0.f, 0.f);
            glVertex2f(-0.5f, -0.5f);
            glColor3f(0.f, 1.f, 0.f);
            glVertex2f(0.f, 0.5f);
            glColor3f(0.f, 0.f, 1.f);
            glVertex2f(0.5f, -0.5f);
            glEnd();
            break;
    }

    glFlush();
    glutSwapBuffers();
}

// Управление мышью
void Mouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        switch (button) {
            case GLUT_LEFT_BUTTON:
                figure = (figure + 1) % number_of_figures;
                break;
            case GLUT_RIGHT_BUTTON:
                figure = (figure - 1 + number_of_figures) % number_of_figures;
                break;
        }
        nextRandomColor();
        rotate_x = rotate_y = rotate_z = 0.;
    }
    glutPostRedisplay();
}

// Управление клавиатурой
void Keyboard(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            rotate_x += 5;
            break;
        case GLUT_KEY_DOWN:
            rotate_x -= 5;
            break;

        case GLUT_KEY_RIGHT:
            rotate_y += 5;
            break;
        case GLUT_KEY_LEFT:
            rotate_y -= 5;
            break;

        case GLUT_KEY_PAGE_UP:
            rotate_z += 5;
            break;
        case GLUT_KEY_PAGE_DOWN:
            rotate_z -= 5;
            break;
    }
    glutPostRedisplay();
}

// Функция вызывается при изменении размеров окна
void Reshape(int width, int height) {
    w = width;
    h = height;
    glViewport(0, 0, w, h);
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
    glutInit(&argc, argv);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("2d objects");

    glutDisplayFunc(Update);

    glutReshapeFunc(Reshape);

    Init();

    glutSpecialFunc(Keyboard);
    glutMouseFunc(Mouse);
    nextRandomColor();

    glutMainLoop();

    return 0;
}
