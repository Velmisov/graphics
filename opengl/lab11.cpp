#include <GL/freeglut.h>
#include <math.h>

const double PI = acos(-1);

static int w = 0, h = 0;

float camera_rotate_x = 0.f;
float camera_rotate_z = 0.f;
float camera_dist = 0.f;

float car_x = 0.f;
float car_y = 0.f;
float car_rotate_z = 0.f;

// Функция вызывается перед вхождением в главный цикл
void init() {
    glClearColor(0, 0, 0, 1);

    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);
}

// Функция вызывается каждый кадр для его отрисовки
void update() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    glColor3f(1.f, 1.f, 1.f);
    glRectf(-10.f, -10.f, 10.f, 10.f);

    glTranslatef(car_x, car_y, 0.5f);
    glRotatef(car_rotate_z, 0.f, 0.f, 1.f);
    glColor3f(0.f, 0.f, 1.f);
    glutSolidCube(1.f);

    glFlush();
    glutSwapBuffers();
}

// Обновление камеры
void updateCamera() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65.f, (float)w / h, 0.1f, 1000.f);
    gluLookAt(0., -20., 5, 0., 0., 0., 0., 1., 0.);
    glTranslatef(0.f, 20 * camera_dist, -5 * camera_dist);
    glRotatef(camera_rotate_x, 1.f, 0.f, 0.f);
    glRotatef(camera_rotate_z, 0.f, 0.f, 1.f);
    glMatrixMode(GL_MODELVIEW);
}

// Управление мышкой
void mouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        switch (button) {
            case GLUT_LEFT_BUTTON:
                break;
            case GLUT_RIGHT_BUTTON:
                break;
            default:
                if (button == 3)
                    camera_dist -= 0.1f;
                else if (button == 4)
                    camera_dist += 0.1f;
                else return;

                updateCamera();
        }
    }
    glutPostRedisplay();
}

// Управление машинкой
void driving(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            car_y += sin(car_rotate_z / 180 * PI);
            car_x += cos(car_rotate_z / 180 * PI);
            break;
        case GLUT_KEY_DOWN:
            car_y -= sin(car_rotate_z / 180 * PI);
            car_x -= cos(car_rotate_z / 180 * PI);
            break;

        case GLUT_KEY_RIGHT:
            car_rotate_z -= 2.f;
            break;
        case GLUT_KEY_LEFT:
            car_rotate_z += 2.f;
            break;

        default:
            return;
    }
    glutPostRedisplay();
}

// Управление клавиатурой
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'a':
            camera_rotate_z += 5;
            updateCamera();
            break;
        case 'd':
            camera_rotate_z -= 5;
            updateCamera();
            break;
        case 'w':
            camera_rotate_x += 0.5f;
            updateCamera();
            break;
        case 's':
            camera_rotate_x -= 0.5f;
            updateCamera();
            break;
        case 'o':
            glOrtho(-2.f, 2.f, -2.f, 2.f, -2.f, 2.f);
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

// Вызывается при изменении размеров окна
void reshape(int width, int height) {
    w = width;
    h = height;
    
    glViewport(0, 0, w, h);

    updateCamera();
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("texture and lightning");

    init();

    glutReshapeFunc(reshape);

    glutDisplayFunc(update);

    glutSpecialFunc(driving);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);

    glutMainLoop();

    return 0;
}
