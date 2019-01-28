#include <string.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include "glut.h"
#endif
#define SPACEBAR 32
#include <stdlib.h>
#include <iostream>
#include <math.h>


using namespace std;

bool WireFrame= false;
float i =0;

GLint m_viewport[4];
const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

    float xpos =2;
    float ypos =2;
    float xb1=0;
    float yb1=0;
    float xdes =4.0;
    float ydes =4.0;

    float oldx=0,oldy=0;
    //float t =0.0;
    float t1=0.0;

    bool first=true;

    int tmp=0;

    float xcurr =0.0;
    float ycurr =0.0;

    float  mlocX =0;
    float  mlocY =0;

    int I = 0;
    int mouseCounter=0;

    float Wwidth,Wheight;

    struct cord {
        float x;
        float y;
    };

    cord click[50];

static void withTimer(int);
/* GLUT callback Handlers */
long long factorial(int x)
{
    if(x == 0)
    return 1;
  long long facto = 1;
  for(; x > 0; x = x - 1)
  {
    facto = facto * (x);
  }
  return facto;
}
float doMath(int I)
{
    return (factorial(mouseCounter)/(factorial(mouseCounter-I)*factorial(I)))*(pow((1-t1),(mouseCounter-I)))*pow(t1,I);
}

void square(){
  glBegin(GL_QUADS);
      glVertex3f(4.3,4.3,0.0);
      glVertex3f(-4.3,4.3,0.0);
      glVertex3f(-4.3,-4.3,0.0);
      glVertex3f(4.3,-4.3,0.0);
  glEnd();

}


void bezier(int n){
    float x1math,y1math,x1pos,y1pos;
for(float t=0;t<=1;t+=0.0005)
{
  for(int i=0; i<mouseCounter;i++)
  {
        y1math=doMath(i);
         x1math=doMath(i);

         x1pos += x1math*click[i].x;
         y1pos += y1math*click[i].y;
  }
  glBegin(GL_POINTS);
        glVertex3f(x1pos, y1pos, 2);
        glEnd();
        x1pos=0;
        y1pos=0;
}
}
static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0,0,10,0.0,0.0,0.0,0.0,1.0,100.0);

    if(WireFrame)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);    //Draw Our Mesh In Wireframe Mesh
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    //Toggle WIRE FRAME

    glPushMatrix();
        glColor3f(3.0,3.0,10.0);
        square();
    glPopMatrix();



    glPushMatrix();//DRAW THE LINE
        glColor3f(1.0,0.0,0.0);
        glPointSize(3);
         glBegin(GL_POINTS);
        glVertex3f(xpos, ypos, 2);
        glEnd();
    glPopMatrix();

    glPushMatrix();
        glColor3f(1.0,0.0,0.0);
        glPointSize(5);
    glPopMatrix();

    if(tmp%2==0)
    {
             for(int i = 0; i <=mouseCounter; i++)
            {
                glBegin(GL_POINTS);
                    glVertex3f(click[i].x, click[i].y, 2);
                glEnd();

            }
    }

    if(mouseCounter>=3)
    {

        glutTimerFunc(20,withTimer,0);
    }
glPushMatrix();
        glColor3f(1.0,0.0,0.0);
        glTranslated(xb1, yb1, 2);
        glutSolidSphere(0.2,24,24);
    glPopMatrix();
    glutSwapBuffers();
    glutPostRedisplay();
}

void withTimer(int val){

    if(t1 <= 1)
        t1+=0.0009;
    else t1=1;

    xpos = 0;
    ypos = 0;

    float ymath,xmath;


    for(int I = 1; I <= mouseCounter; I++)
    {
         ymath=doMath(I);
         xmath=doMath(I);

         xpos += xmath*click[I].x;
         ypos += ymath*click[I].y;
    }

        xb1=xpos;
        yb1=ypos;

    glutPostRedisplay();
}

static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;

        case 'w':
            WireFrame =!WireFrame;
            break;
        case 32:
            {
                for(int d=0;d<50;d++)
                {
                   click[d].x=0;
                   click[d].y=0;
                   t1=0;
                   mouseCounter=0;
                }
            }
            break;
    }
}

void mouse(int btn, int state, int x, int y){
    float scale = 70*(Wwidth/Wheight);

    float mx = (float) (x-Wwidth/2)/scale;
    float my = (float) (Wheight/2-y)/scale;

    switch(btn){
        case GLUT_LEFT_BUTTON:
        if(state==GLUT_DOWN){
                  mlocX = mx;
                  mlocY = my;

                  xcurr = xpos;
                  ycurr = ypos;

                   mouseCounter++;
                  click[mouseCounter].x=mx;
                  click[mouseCounter].y=my;

                    bezier(5);

                    t1=0;

                  if(mouseCounter > 50)
                    mouseCounter = 0;
                    if(first==true)
                    {
                                click[0].x=click[mouseCounter].x;
                                click[0].y=click[mouseCounter].y;
                                    first=false;
                    }
            }
            break;

            case GLUT_RIGHT_BUTTON:
                if(state==GLUT_DOWN)
                {
                    tmp++;
                }
                break;
    }
    glutPostRedisplay();
};



static void init(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);                 // assign a color you like

    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);

    for(int d=0;d<50;d++)
    {
        click[d].x=0;
        click[d].y=0;
    }
}

/* Program entry point */
static void resize(int width, int height)
{
     double Ratio;

     Wwidth = (float)width;
     Wheight = (float)height;

     Ratio= (double)width /(double)height;

    glViewport(0,0,(GLsizei) width,(GLsizei) height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

  gluPerspective (45.0f,Ratio,0.1f, 100.0f);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glGetIntegerv( GL_VIEWPORT, m_viewport );

 }
int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    glutInitWindowSize(800,600);
    glutInitWindowPosition(80,80);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Assignment 1");
    init();
    glutReshapeFunc(resize);
    if(mouseCounter<=50)
        glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(key);

    glutMainLoop();

    return EXIT_SUCCESS;
}

