#include "compass.h"

int main(int argc, char *argv[])
{
    Compass compass;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);

    glutInitWindowPosition(200,100);
    glutInitWindowSize(850,500);

    glutCreateWindow("Window 1");

    glutDisplayFunc(compass.display);
    glutReshapeFunc(compass.reshape);
    glutSpecialFunc(compass.keypress);
    glutKeyboardFunc(compass.key);
//    glutTimerFunc(0, compass.timer, 0);
    glutIdleFunc(compass.idle);
    compass.init();
    glutMainLoop();
}

