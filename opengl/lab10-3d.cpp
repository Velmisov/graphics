#include <GL/freeglut.h>
#include <time.h>
#include <stdlib.h>

static int w = 0, h = 0;

double rotate_x = 0.;
double rotate_y = 0.;
double rotate_z = 0.;

int mode = 0;

// Функция вызывается перед вхождением в главный цикл
void Init() {
    glClearColor(0, 0, 0, 1);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    GLfloat pos[4] = {0, 10, 2, 1};
//    glLightfv(GL_LIGHT0, GL_POSITION, pos);
//    glEnable(GL_LIGHTING);
//    glEnable(GL_LIGHT0);
//    glEnable(GL_COLOR_MATERIAL);

    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    glutSwapBuffers();
}

// Функция вызывается каждый кадр для его отрисовки
void Update() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glColor3f(1.f, 1.f, 1.f);
    glutWireSphere(0.05, 10, 10);

    glPushMatrix();
    switch (mode) {
        case 0:
            glRotated(rotate_x, 1, 0, 0);
            glRotated(rotate_y, 0, 1, 0);
            glRotated(rotate_z, 0, 0, 1);

            glPushMatrix();
            glTranslatef(-1.f, -0.3f, 0.f);
            glColor3f(0.5f, 0.5f, 0.5f);
            glutSolidCube(0.2f);
            glPopMatrix();

            glPushMatrix();
            glTranslatef(-0.7f, -0.3f, 0.f);
            glColor3f(1.f, 1.f, 0.f);
            glutSolidCube(0.2f);
            glPopMatrix();

            glPushMatrix();
            glTranslatef(-0.4f, -0.3f, 0.f);
            glColor3f(1.f, 0.5f, 0.f);
            glutSolidCube(0.2f);
            glPopMatrix();
            break;
        case 1:
            glTranslatef(0.f, -0.3f, 0.f);
            glRotated(rotate_x, 1, 0, 0);
            glRotated(rotate_y, 0, 1, 0);
            glRotated(rotate_z, 0, 0, 1);

            glColor3f(1.f, 1.f, 0.f);
            glutSolidCube(0.2f);

            glTranslatef(-0.5f, 0.f, 0.f);
            glColor3f(0.5f, 0.5f, 0.5f);
            glutSolidCube(0.2f);

            glTranslatef(1.f, 0.f, 0.f);
            glColor3f(1.f, 0.5f, 0.f);
            glutSolidCube(0.2f);

            break;
        case 2:
            glTranslatef(-0.5f, -0.3f, 0.f);

            glRotated(rotate_x, 1, 0, 0);
            glRotated(rotate_y, 0, 1, 0);
            glRotated(rotate_z, 0, 0, 1);

            glColor3f(0.5f, 0.5f, 0.5f);
            glutSolidCube(0.2f);

            glLoadIdentity();
            glTranslatef(0.f, -0.3f, 0.f);

            glRotated(rotate_x, 1, 0, 0);
            glRotated(rotate_y, 0, 1, 0);
            glRotated(rotate_z, 0, 0, 1);

            glColor3f(1.f, 1.f, 0.f);
            glutSolidCube(0.2f);

            glLoadIdentity();
            glTranslatef(0.5f, -0.3f, 0.f);

            glRotated(rotate_x, 1, 0, 0);
            glRotated(rotate_y, 0, 1, 0);
            glRotated(rotate_z, 0, 0, 1);

            glColor3f(1.f, 0.5f, 0.f);
            glutSolidCube(0.2f);
            break;
    }
    glPopMatrix();

    glFlush();
    glutSwapBuffers();
}

// Управление мышью
void Mouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        rotate_x = rotate_y = rotate_z = 0;
        switch (button) {
            case GLUT_LEFT_BUTTON:
                mode = (mode + 1) % 3;
                break;
            case GLUT_RIGHT_BUTTON:
                mode = (mode - 1 + 3) % 3;
                break;
        }
    }
    glutPostRedisplay();
}

// Управление вращениями
void Rotate(int key, int x, int y) {
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


void Keyboard(unsigned char key, int x, int y) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    switch (key) {
        case 'o':
            glOrtho(-2.f, 2.f, -2.f, 2.f, -2.f, 2.f);
            break;
        default:
            gluPerspective(65.f, (float)w / h, 1.f, 1000.f);
            gluLookAt(0., 2., 2., 0., 0., 0., 0., 1., 0.);
            break;
    }
    glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay();
}

// Функция вызывается при изменении размеров окна
void Reshape(int width, int height) {
    w = width;
    h = height;
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65.f, (float)w / h, 1.f, 1000.f);
    gluLookAt(0., 2., 2., 0., 0., 0., 0., 1., 0.);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
    glutInit(&argc, argv);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("3d objects");
    Init();

    glutDisplayFunc(Update);

    glutReshapeFunc(Reshape);

    glutSpecialFunc(Rotate);
    glutKeyboardFunc(Keyboard);
    glutMouseFunc(Mouse);

    glutMainLoop();

    return 0;
}
