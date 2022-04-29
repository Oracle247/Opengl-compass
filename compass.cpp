#include "compass.h"
#include <math.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;
#define GLUT_DISABLE_ATEXT_HACK

int Compass::clientfd =  (socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));

Compass::Compass()

{
//    clientfd(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));
    //create a socket
//    int clientfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientfd == -1){
        cerr << "cant create a socket!";
//        return -1;
    }
    //bind the socket to a IP / port
    sockaddr_in server;

    server.sin_family = AF_INET;
    server.sin_port = htons(5400);
    server.sin_addr.s_addr = INADDR_ANY;
    //inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);
    if(connect(clientfd, (sockaddr*)&server , sizeof(server)) == -1){
        cerr << "cant connect";
//        return -2;
    }

    cout<<"connected to server"<<endl;

}

Compass::~Compass()
{
}
static double a = 0;
static double t = 0;
static double i = 0;
static double theta = 0;
int refreshmill = 100000;
void Compass::display()
{



    float theta;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glLoadIdentity();
    glPointSize(5.0);

    glBegin(GL_POLYGON);
    glColor3f(0.4,0.4,0.4);
    for(int i = 0; i < 720; i++){
        theta = i*3.142/180;
        glVertex2f(0.88*cos(theta), 0.88*sin(theta));

    }
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0,0,0);
    for(int i = 0; i < 720; i++){
        theta = i*3.142/180;

        glVertex2f(0.82*cos(theta), 0.82*sin(theta));

    }
    glEnd();



    glBegin(GL_POLYGON);
    glColor3f(0.05,0.05,0.1);
    for(int i = 0; i < 720; i++){
        theta = i*3.142/180;
        glVertex2f(0.7*cos(theta), 0.7*sin(theta));

    }
    glEnd();


    tickmarks();
    coordinate();

    double compass_angle= 0;
    compass_angle = a;
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
        glRotated(-compass_angle,0,0,1);

        glBegin(GL_POLYGON);
        glColor3f(1,0,0);
        glVertex2d(0,-0.06);
        glVertex2d(-0.05,0);
        glColor3f(0.1,1,0.1);
        glVertex2d(0,0.6);
        glColor3f(1,0,0);
        glVertex2d(0.05,0);
        glEnd();
    glPopMatrix();
    //compass_angle += 100;

    glBegin(GL_POLYGON);
    glColor3f(0,0,0);
    for(int i = 0; i < 360; i++){
        theta = i*3.142/180;
        glColor3f(0,0,0);
        glVertex2f(0.01*cos(theta), 0.01*sin(theta));

    }
    glEnd();

    calibrate();

    simulation();



    calib();

//    double compass_angle= 0;
    compass_angle = a;

    glPushMatrix();
    glTranslated(0,i,0);
    glRotated(-compass_angle,0,0,1);
    Airplane();
    glPopMatrix();


    glutSwapBuffers();
}

void Compass::init()
{
    glClearColor(0.2f, 0.2, 0.2f, 1);
}

void Compass::reshape(int w, int h)
{
//    if(h==0){
//        h = 1;
//    }
    const float ar = (float) w / (float) h;
    glViewport(0,0, w,h);
    glMatrixMode(GL_PROJECTION_MATRIX);
    glLoadIdentity();

    gluOrtho2D(-ar,ar,-1.0,1.0);
    glMatrixMode(GL_MODELVIEW);
}

/*void Compass::timer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(refreshmill, timer,0);
}*/

void Compass::idle()
{
    double rand;
    recv(clientfd, &rand, sizeof(double), 0);
    cout << "data recieved"<<endl;
    cout << rand<<endl;
    i = rand;

    glutPostRedisplay();
}

void Compass::keypress(int key, int x, int y)
{
    switch(key){
        case GLUT_KEY_RIGHT:
                a++;
                glutPostRedisplay();
            break;
        case GLUT_KEY_LEFT:
                a--;
                glutPostRedisplay();
            break;
    }

}

void Compass::key(unsigned char key, int x, int y)
{
    switch(key){
        case 'w':
            a = 0;
            glutPostRedisplay();
            break;

        case 'a':
            a = -90;
            glutPostRedisplay();
            break;

        case 's':
            a = 180;
            glutPostRedisplay();
            break;

        case 'd':
            a = 90;
            glutPostRedisplay();
            break;
    }
}

//void Compass::randData()
//{
//    //create a socket
//    int clientfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//    if (clientfd == -1){
//        cerr << "cant create a socket!";
////        return -1;
//    }
//    //bind the socket to a IP / port
//    sockaddr_in server;

//    server.sin_family = AF_INET;
//    server.sin_port = htons(5400);
//    server.sin_addr.s_addr = INADDR_ANY;
//    //inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);
//    if(connect(clientfd, (sockaddr*)&server , sizeof(server)) == -1){
//        cerr << "cant connect";
////        return -2;
//    }

//    cout<<"connected to server"<<endl;

//    for(int i = 0; i < 10; i++){
//        double rand;
//        int k = recv(clientfd, &rand, sizeof(double), 0);
//        cout << "data recieved"<<endl;
//        cout << rand<<endl;
//        t = rand;
//    }

//}

void Compass::tickmarks()
{
    double angle = 0;
    for(int i = 0; i <= 8; i++){
        glPushMatrix();
        glRotated(angle, 0, 0, 1);
        glTranslated(0,0,0);

        glBegin(GL_LINES);
        glColor3f(1,0,0);
        glVertex2f(0,0.8);
        glColor3f(1,0,0);
        glVertex2f(0,0.7);
        glEnd();
        glPopMatrix();
        angle += 45;
    }

    for(int i = 0; i <72; i++){
        if((i % 9) != 0){
            glPushMatrix();
            glRotated(angle, 0, 0, 1);
            glTranslated(0,0,0);
            if((i % 3) == 0){
                glBegin(GL_LINES);
                glColor3f(1,0.2,0);
                glVertex2f(0,0.615);
                glVertex2f(0,0.7);
                glEnd();
            }
            else{
                glBegin(GL_LINES);
                glColor3f(0,1,0);
                glVertex2f(0,0.63);
                glVertex2f(0,0.7);
                glEnd();
            }
            glPopMatrix();
        }
        angle += 5;
    }

    for(int i = 0; i < 360; i++){
        if((i % 5) != 0){
            glPushMatrix();
            glRotated(angle, 0, 0, 1);
            glTranslated(0,0,0);

            glBegin(GL_LINES);
            glColor3f(0,0,1);
            glVertex2f(0,0.67);
            glVertex2f(0,0.7);
            glEnd();
            glPopMatrix();
        }
        angle += 1;
    }
}

void Compass::calibrate()
{
    {
        glPushMatrix();
            glRotated(-13.5, 0, 0, 1);
            glTranslatef(0,0,0);
            glColor3f(0,0,1);
            glRasterPos2f(0,0.73);
            const unsigned char* t = reinterpret_cast<const unsigned char *>("15");
            glutBitmapString(GLUT_BITMAP_9_BY_15, t);
        glPopMatrix();
    }

    {
        glPushMatrix();
            glRotated(-29.5, 0, 0, 1);
            glTranslatef(0,0,0);
            glColor3f(0,0,1);
            glRasterPos2f(0,0.73);
            const unsigned char* t = reinterpret_cast<const unsigned char *>("30");
            glutBitmapString(GLUT_BITMAP_9_BY_15, t);
        glPopMatrix();
    }

    {
        glPushMatrix();
            glRotated(-61.0, 0, 0, 1);
            glTranslatef(0,0,0);
            glColor3f(0,0,1);
            glRasterPos2f(0,0.72);
            const unsigned char* t = reinterpret_cast<const unsigned char *>("60");
            glutBitmapString(GLUT_BITMAP_9_BY_15, t);
        glPopMatrix();
    }
    {
        glPushMatrix();
            glRotated(-77.0, 0, 0, 1);
            glTranslatef(0,0,0);
            glColor3f(0,0,1);
            glRasterPos2f(0,0.72);
            const unsigned char* t = reinterpret_cast<const unsigned char *>("75");
            glutBitmapString(GLUT_BITMAP_9_BY_15, t);
        glPopMatrix();
    }

    {
        glPushMatrix();
            glRotated(-107.0, 0, 0, 1);
            glTranslatef(0,0,0);
            glColor3f(0,0,1);
            glRasterPos2f(0,0.71);
            const unsigned char* t = reinterpret_cast<const unsigned char *>("105");
            glutBitmapString(GLUT_BITMAP_9_BY_15, t);
        glPopMatrix();
    }

    {
        glPushMatrix();
            glRotated(-125.0, 0, 0, 1);
            glTranslatef(0,0,0);
            glColor3f(0,0,1);
            glRasterPos2f(0,0.725);
            const unsigned char* t = reinterpret_cast<const unsigned char *>("120");
            glutBitmapString(GLUT_BITMAP_9_BY_15, t);
        glPopMatrix();
    }

    {
        glPushMatrix();
            glRotated(-155.0, 0, 0, 1);
            glTranslatef(0,0,0);
            glColor3f(0,0,1);
            glRasterPos2f(0,0.75);
            const unsigned char* t = reinterpret_cast<const unsigned char *>("150");
            glutBitmapString(GLUT_BITMAP_9_BY_15, t);
        glPopMatrix();
    }

    {
        glPushMatrix();
            glRotated(-169.0, 0, 0, 1);
            glTranslatef(0,0,0);
            glColor3f(0,0,1);
            glRasterPos2f(0,0.77);
            const unsigned char* t = reinterpret_cast<const unsigned char *>("165");
            glutBitmapString(GLUT_BITMAP_9_BY_15, t);
        glPopMatrix();
    }

    {
        glPushMatrix();
            glRotated(-199.0, 0, 0, 1);
            glTranslatef(0,0,0);
            glColor3f(0,0,1);
            glRasterPos2f(0,0.805);
            const unsigned char* t = reinterpret_cast<const unsigned char *>("195");
            glutBitmapString(GLUT_BITMAP_9_BY_15, t);
        glPopMatrix();
    }

    {
        glPushMatrix();
            glRotated(-213.0, 0, 0, 1);
            glTranslatef(0,0,0);
            glColor3f(0,0,1);
            glRasterPos2f(0,0.815);
            const unsigned char* t = reinterpret_cast<const unsigned char *>("210");
            glutBitmapString(GLUT_BITMAP_9_BY_15, t);
        glPopMatrix();
    }

    {
        glPushMatrix();
            glRotated(-240.0, 0, 0, 1);
            glTranslatef(0,0,0);
            glColor3f(0,0,1);
            glRasterPos2f(0,0.82);
            const unsigned char* t = reinterpret_cast<const unsigned char *>("240");
            glutBitmapString(GLUT_BITMAP_9_BY_15, t);
        glPopMatrix();
    }

    {
        glPushMatrix();
            glRotated(-255.0, 0, 0, 1);
            glTranslatef(0,0,0);
            glColor3f(0,0,1);
            glRasterPos2f(0,0.82);
            const unsigned char* t = reinterpret_cast<const unsigned char *>("255");
            glutBitmapString(GLUT_BITMAP_9_BY_15, t);
        glPopMatrix();
    }

    {
        glPushMatrix();
            glRotated(-282.0, 0, 0, 1);
            glTranslatef(0,0,0);
            glColor3f(0,0,1);
            glRasterPos2f(0,0.81);
            const unsigned char* t = reinterpret_cast<const unsigned char *>("285");
            glutBitmapString(GLUT_BITMAP_9_BY_15, t);
        glPopMatrix();
    }

    {
        glPushMatrix();
            glRotated(-297.0, 0, 0, 1);
            glTranslatef(0,0,0);
            glColor3f(0,0,1);
            glRasterPos2f(0,0.80);
            const unsigned char* t = reinterpret_cast<const unsigned char *>("300");
            glutBitmapString(GLUT_BITMAP_9_BY_15, t);
        glPopMatrix();
    }

    {
        glPushMatrix();
            glRotated(-325.0, 0, 0, 1);
            glTranslatef(0,0,0);
            glColor3f(0,0,1);
            glRasterPos2f(0,0.77);
            const unsigned char* t = reinterpret_cast<const unsigned char *>("330");
            glutBitmapString(GLUT_BITMAP_9_BY_15, t);
        glPopMatrix();
    }

    {
        glPushMatrix();
            glRotated(-340.0, 0, 0, 1);
            glTranslatef(0,0,0);
            glColor3f(0,0,1);
            glRasterPos2f(0,0.75);
            const unsigned char* t = reinterpret_cast<const unsigned char *>("345");
            glutBitmapString(GLUT_BITMAP_9_BY_15, t);
        glPopMatrix();
    }
    /*double angle = 15;
    for(int i = 1; i < 17; i++){
        //string str = to_string(i);
        const unsigned char* t = reinterpret_cast<const unsigned char *>("");
        glPushMatrix();
            glRotated(-angle, 0, 0, 1);
            glTranslatef(0,0,0);
            glColor3f(0,0,1);
            glRasterPos2f(0,0.71);
            glutBitmapString(GLUT_BITMAP_HELVETICA_18, t);
        glPopMatrix();
        angle += 15;
    }*/

}

void Compass::calib()
{
    glBegin(GL_LINES);
    glColor3f(1,0,0);
    glVertex2f(-0.6f,0.9f);
    glVertex2f(-0.6f,-0.9f);
    glEnd();

    double cali = 0;
    for(int i = 0; i <91; i++){
        glPushMatrix();
        glTranslated(0,cali,0);
        if((i % 5) == 0){
            glBegin(GL_LINES);
            glColor3f(1,0.2,0);
            glVertex2f(-0.57,-0.9);
            glVertex2f(-0.63,-0.9);
            glEnd();
        }
        else{
            glBegin(GL_LINES);
            glColor3f(0,1,0);
            glVertex2f(-0.59,-0.9);
            glVertex2f(-0.61,-0.9);
            glEnd();
        }
        glPopMatrix();
        cali += 0.02;
    }

    {
        glPushMatrix();
            glTranslatef(-0.78,-0.9,0);
            glColor3f(1,1,1);
            glRasterPos2f(0,0);
            const unsigned char* t = reinterpret_cast<const unsigned char *>("1200");
            glutBitmapString(GLUT_BITMAP_9_BY_15, t);
        glPopMatrix();
    }

    {
        glPushMatrix();
            glTranslatef(-0.78,-0.7,0);
            glColor3f(1,1,1);
            glRasterPos2f(0,0);
            const unsigned char* t = reinterpret_cast<const unsigned char *>("1400");
            glutBitmapString(GLUT_BITMAP_9_BY_15, t);
        glPopMatrix();
    }

    {
        glPushMatrix();
            glTranslatef(-0.78,-0.5,0);
            glColor3f(1,1,1);
            glRasterPos2f(0,0);
            const unsigned char* t = reinterpret_cast<const unsigned char *>("1600");
            glutBitmapString(GLUT_BITMAP_9_BY_15, t);
        glPopMatrix();
    }

    {
        glPushMatrix();
            glTranslatef(-0.78,-0.3,0);
            glColor3f(1,1,1);
            glRasterPos2f(0,0);
            const unsigned char* t = reinterpret_cast<const unsigned char *>("1800");
            glutBitmapString(GLUT_BITMAP_9_BY_15, t);
        glPopMatrix();
    }

    {
        glPushMatrix();
            glTranslatef(-0.78,-0.1,0);
            glColor3f(1,1,1);
            glRasterPos2f(0,0);
            const unsigned char* t = reinterpret_cast<const unsigned char *>("2000");
            glutBitmapString(GLUT_BITMAP_9_BY_15, t);
        glPopMatrix();
    }

    {
        glPushMatrix();
            glTranslatef(-0.78,0.1,0);
            glColor3f(1,1,1);
            glRasterPos2f(0,0);
            const unsigned char* t = reinterpret_cast<const unsigned char *>("2200");
            glutBitmapString(GLUT_BITMAP_9_BY_15, t);
        glPopMatrix();
    }

    {
        glPushMatrix();
            glTranslatef(-0.78,0.3,0);
            glColor3f(1,1,1);
            glRasterPos2f(0,0);
            const unsigned char* t = reinterpret_cast<const unsigned char *>("2400");
            glutBitmapString(GLUT_BITMAP_9_BY_15, t);
        glPopMatrix();
    }

    {
        glPushMatrix();
            glTranslatef(-0.78,0.5,0);
            glColor3f(1,1,1);
            glRasterPos2f(0,0);
            const unsigned char* t = reinterpret_cast<const unsigned char *>("2600");
            glutBitmapString(GLUT_BITMAP_9_BY_15, t);
        glPopMatrix();
    }

    {
        glPushMatrix();
            glTranslatef(-0.78,0.7,0);
            glColor3f(1,1,1);
            glRasterPos2f(0,0);
            const unsigned char* t = reinterpret_cast<const unsigned char *>("2800");
            glutBitmapString(GLUT_BITMAP_9_BY_15, t);
        glPopMatrix();
    }

    {
        glPushMatrix();
            glTranslatef(-0.78,0.9,0);
            glColor3f(1,1,1);
            glRasterPos2f(0,0);
            const unsigned char* t = reinterpret_cast<const unsigned char *>("3000");
            glutBitmapString(GLUT_BITMAP_9_BY_15, t);
        glPopMatrix();
    }
}

void Compass::coordinate()
{
    glPushMatrix();
        glTranslatef(0,0,0);
        glColor3f(1,0,0);
        glRasterPos2f(-0.03,0.6);
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'N');
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0,0,0);
        glColor3f(1,0,0);
        glRasterPos2f(-0.03,-0.67);
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'S');
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0,0,0);
        glColor3f(1,0,0);
        glRasterPos2f(0.62,-0.03);
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'E');
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0,0,0);
        glColor3f(1,0,0);
        glRasterPos2f(-0.66,-0.03);
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'W');
    glPopMatrix();

    {
        glPushMatrix();
            glRotated(-43.0, 0, 0, 1);
            glTranslatef(0,0,0);
            glColor3f(1,0,0);
            glRasterPos2f(0,0.60);
            const unsigned char* t = reinterpret_cast<const unsigned char *>("NE");
            glutBitmapString(GLUT_BITMAP_9_BY_15, t);
        glPopMatrix();
    }

    {
        glPushMatrix();
            glRotated(-139.0, 0, 0, 1);
            glTranslatef(0,0,0);
            glColor3f(1,0,0);
            glRasterPos2f(0,0.63);
            const unsigned char* t = reinterpret_cast<const unsigned char *>("SE");
            glutBitmapString(GLUT_BITMAP_9_BY_15, t);
        glPopMatrix();
    }

    {
        glPushMatrix();
            glRotated(-225.0, 0, 0, 1);
            glTranslatef(0,0,0);
            glColor3f(1,0,0);
            glRasterPos2f(0,0.67);
            const unsigned char* t = reinterpret_cast<const unsigned char *>("SW");
            glutBitmapString(GLUT_BITMAP_9_BY_15, t);
        glPopMatrix();
    }

    {
        glPushMatrix();
            glRotated(-313.0, 0, 0, 1);
            glTranslatef(0,0,0);
            glColor3f(1,0,0);
            glRasterPos2f(0,0.64);
            const unsigned char* t = reinterpret_cast<const unsigned char *>("NW");
            glutBitmapString(GLUT_BITMAP_9_BY_15, t);
        glPopMatrix();
    }

}

void Compass::Airplane()
{
    glBegin(GL_POLYGON);
    glColor3f(1,1,1);
    glVertex2f(0.5f, 0.0f);
    glVertex2f(0.1f,-0.05f);
    glVertex2f(0.1f, 0.05f);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(1,1,1);
    glVertex2f(-0.5f, 0.0f);
    glVertex2f(-0.1f,-0.05f);
    glVertex2f(-0.1f, 0.05f);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(1,1,1);
    for(int i = 0; i < 720; i++){
        theta = i*3.142/180;

        glVertex2f(0.11*cos(theta), 0.1*sin(theta));

    }
    glEnd();

    glBegin(GL_LINES);
    glColor3f(1,1,1);
    glVertex2f(-0.1f,-0.05f);
    glVertex2f(-0.1f,-0.15f);
    glEnd();

    glBegin(GL_LINES);
    glColor3f(1,1,1);
    glVertex2f(0.1f,-0.05f);
    glVertex2f(0.1f,-0.15f);
    glEnd();

    glBegin(GL_POINTS);
    glColor3f(1,1,1);
    glVertex2f(0.1f,-0.15f);
    glEnd();

    glBegin(GL_POINTS);
    glColor3f(1,1,1);
    glVertex2f(-0.1f,-0.15f);
    glEnd();

    glBegin(GL_POINTS);
    glColor3f(0.7,0.7,0.7);
    glVertex2f(0.0f,0.0f);
    glEnd();
}

void Compass::simulation()
{
    glPushMatrix();

    glTranslatef(1.4f,0.45f, 0);
    glRotated(-a,0,0,1);

    glBegin(GL_LINES);
    glColor3f(0,0,1);
    glVertex2f(0, -0.3);
    glVertex2f(0, 0.2);
    glEnd();

    glBegin(GL_LINES);
    glColor3f(0,0,1);
    glVertex2f(-0.2, 0);
    glVertex2f(0.2, 0);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(1,0,0);
    glVertex2f(-0.05, 0.15);
    glVertex2f(0.05, 0.15);
    glVertex2f(0, 0.2);
    glEnd();

    glPopMatrix();
}

