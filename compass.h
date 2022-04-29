#ifndef COMPASS_H
#define COMPASS_H
//socket headers
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <netinet/ip.h>
#include <netinet/in.h>

//opengl headers
#include <GL/glut.h>
#include <GL/freeglut.h>
class Compass
{
public:
    Compass();
    ~Compass();

    static int clientfd;

    static void display();
    static void init();
    static void reshape(int, int);
    static void keypress(int, int, int);
    static void key(unsigned char, int, int);
    static void timer(int);
    static void randData();
    static void idle(void);
    static void tickmarks();
    static void calibrate();
    static void calib();
    static void coordinate();
    static void Airplane();
    static void simulation();
protected:


private:

};
#endif // COMPASS_H
