#include <GL/freeglut.h>
#include <time.h>
#include <stdlib.h>

static int w = 0, h = 0;

double rotate_x = 0.;
double rotate_y = 0.;
double rotate_z = 0.;

// Функция вызывается перед вхождением в главный цикл
void Init() {
    glClearColor(0, 0, 0, 1);

    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);
}

// Функция вызывается каждый кадр для его отрисовки
void Update() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    glColor3f(1.f, 1.f, 1.f);
    glBegin(GL_QUADS);
    glVertex3f(-1.f, -1.f, 0.f);
    glVertex3f(-1.f, 1.f, 0.f);
    glVertex3f(1.f, 1.f, 0.f);
    glVertex3f(1.f, -1.f, 0.f);
    glEnd();

    glFlush();
    glutSwapBuffers();
}

// Переключение вида вращения
void Mouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        rotate_x = rotate_y = rotate_z = 0;
        switch (button) {
            case GLUT_LEFT_BUTTON:
                break;
            case GLUT_RIGHT_BUTTON:
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

// Переключение вида камеры
void Keyboard(unsigned char key, int x, int y) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    switch (key) {
        case 'o':
            glOrtho(-2.f, 2.f, -2.f, 2.f, -2.f, 2.f);
            break;
        default:
            gluPerspective(65.f, (float)w / h, 1.f, 1000.f);
            gluLookAt(0., -2., 2., 0., 0., 0., 0., 1., 0.);
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
    gluLookAt(0., -2., 2., 0., 0., 0., 0., 1., 0.);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char* argv[]) {
    srand(time(NULL));

    glutInit(&argc, argv);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("texture and lightning");

    Init();

    glutReshapeFunc(Reshape);

    glutDisplayFunc(Update);

    glutSpecialFunc(Rotate);
    glutKeyboardFunc(Keyboard);
    glutMouseFunc(Mouse);

    glutMainLoop();

    return 0;
}
