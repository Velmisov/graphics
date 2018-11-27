#include <GL/freeglut.h>
#include <math.h>
#include <iostream>
using namespace std;

const double PI = acos(-1);

static int w = 0, h = 0;

float camera_rotate_x = 0.f;
float camera_rotate_y = 0.f;
float camera_dist = 0.f;

float car_x = 0.f;
float car_z = 0.f;
float car_rotate_y = 0.f;

// Функция вызывается перед вхождением в главный цикл
void init() {
    glClearColor(0, 0, 0, 1);

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    const GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    const GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

    // camera light
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.05);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.001);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 30.f);

    // lamps
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT3, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT4, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT4, GL_SPECULAR, light_specular);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void drawFloor() {
    glColor3f(1, 1, 1);
    glBegin(GL_QUADS);
    float step = 0.05f;
    for (float x = -10.f; x < 10.f; x += step) {
        for (float z = -10.f; z < 10.f; z += step) {
            glNormal3f(0, 1, 0);
            glVertex3f(x, 0, z);
            glNormal3f(0, 1, 0);
            glVertex3f(x, 0, z + step);
            glNormal3f(0, 1, 0);
            glVertex3f(x + step, 0, z + step);
            glNormal3f(0, 1, 0);
            glVertex3f(x + step, 0, z);
        }
    }
    glEnd();
}

void drawLamps() {
    const GLfloat light_pos[] = {0.f, 2.1f, 0.f, 1.f};

    glColor3f(0.5f, 0.5f, 0.5f);
    glPushMatrix();
    glTranslatef(-4, 0, -4);
    glRotatef(-90.f, 1, 0, 0);
    glutSolidCylinder(0.1, 2, 10, 10);
    glRotatef(90.f, 1, 0, 0);
    glLightfv(GL_LIGHT1, GL_POSITION, light_pos);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-4, 0, 4);
    glRotatef(-90.f, 1, 0, 0);
    glutSolidCylinder(0.1, 2, 10, 10);
    glRotatef(90.f, 1, 0, 0);
    glLightfv(GL_LIGHT2, GL_POSITION, light_pos);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(4, 0, 4);
    glRotatef(-90.f, 1, 0, 0);
    glutSolidCylinder(0.1, 2, 10, 10);
    glRotatef(90.f, 1, 0, 0);
    glLightfv(GL_LIGHT3, GL_POSITION, light_pos);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(4, 0, -4);
    glRotatef(-90.f, 1, 0, 0);
    glutSolidCylinder(0.1, 2, 10, 10);
    glRotatef(90.f, 1, 0, 0);
    glLightfv(GL_LIGHT4, GL_POSITION, light_pos);
    glPopMatrix();
}

void drawCar() {
    glTranslatef(car_x, 0.5f, car_z);
    glRotatef(car_rotate_y, 0, 1, 0);
    glColor3f(0, 0, 1);
    glutSolidCube(1.f);
}

// Функция вызывается каждый кадр для его отрисовки
void update() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    drawFloor();
    drawLamps();
    drawCar();

    glFlush();
    glutSwapBuffers();
}

void setCameraLight() {
    GLint viewport[4];
    GLdouble modelview_mat[16];
    GLdouble projection_mat[16];
    GLdouble camera_posd[3];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview_mat);
    glGetDoublev(GL_PROJECTION_MATRIX, projection_mat);

    gluUnProject(double(viewport[2] - viewport[0]) / 2, double(viewport[3] - viewport[1]) / 2, 0.,
            modelview_mat, projection_mat, viewport,
            &camera_posd[0], &camera_posd[1], &camera_posd[2]);

    GLfloat camera_pos[4] {0.f, 0.f, 0.f, 1.f};
    for (int i = 0; i < 3; ++i)
        camera_pos[i] = (GLfloat)camera_posd[i];

    GLfloat spot_direction[3];
    for (int i = 0; i < 3; ++i)
        spot_direction[i] = -camera_pos[i];

    double len = sqrt(spot_direction[0] * spot_direction[0] +
            spot_direction[1] * spot_direction[1] +
            spot_direction[2] * spot_direction[2]);
    for (int i = 0; i < 3; ++i)
        spot_direction[i] /= len;

    glLightfv(GL_LIGHT0, GL_POSITION, camera_pos);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
}

// Обновление камеры
void updateCamera() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65.f, (float)w / h, 0.1f, 1000.f);
    gluLookAt(0., 5., -20, 0., 0., 0., 0., 1., 0.);
    glTranslatef(0.f, -5 * camera_dist, 20 * camera_dist);
    glRotatef(camera_rotate_x, 1, 0, 0);
    glRotatef(camera_rotate_y, 0, 1, 0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    setCameraLight();
}

// Управление мышкой
void mouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        switch (button) {
            case GLUT_LEFT_BUTTON:
                if (glIsEnabled(GL_LIGHT0))
                    glDisable(GL_LIGHT0);
                else
                    glEnable(GL_LIGHT0);
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
            car_z += sin(-car_rotate_y / 180 * PI);
            car_x += cos(-car_rotate_y / 180 * PI);
            break;
        case GLUT_KEY_DOWN:
            car_z -= sin(-car_rotate_y / 180 * PI);
            car_x -= cos(-car_rotate_y / 180 * PI);
            break;

        case GLUT_KEY_RIGHT:
            car_rotate_y -= 5.f;
            break;
        case GLUT_KEY_LEFT:
            car_rotate_y += 5.f;
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
            camera_rotate_y += 5;
            if (camera_rotate_y > 360)
                car_rotate_y -= 360;
            updateCamera();
            break;
        case 'd':
            camera_rotate_y -= 5;
            if (camera_rotate_y < -360)
                camera_rotate_y += 360;
            updateCamera();
            break;
        case 'w':
            camera_rotate_x -= 2;
            updateCamera();
            break;
        case 's':
            camera_rotate_x += 2;
            updateCamera();
            break;
        case '1':
            if (glIsEnabled(GL_LIGHT1))
                glDisable(GL_LIGHT1);
            else
                glEnable(GL_LIGHT1);
            break;
        case '2':
            if (glIsEnabled(GL_LIGHT2))
                glDisable(GL_LIGHT2);
            else
                glEnable(GL_LIGHT2);
            break;
        case '3':
            if (glIsEnabled(GL_LIGHT3))
                glDisable(GL_LIGHT3);
            else
                glEnable(GL_LIGHT3);
            break;
        case '4':
            if (glIsEnabled(GL_LIGHT4))
                glDisable(GL_LIGHT4);
            else
                glEnable(GL_LIGHT4);
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
