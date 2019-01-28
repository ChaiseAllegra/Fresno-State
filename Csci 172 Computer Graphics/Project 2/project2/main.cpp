//Chaise Allegra 106006387 Csci 172 Project 2, modeling transformations
#include <string.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <iostream>

#include <math.h>
using namespace std;

bool WireFrame= false,flagC=false,flagT=false,flagS=false;//flags to see what button has been pressed
GLfloat scaleCz=-3.0,scaleSz=-1.0,scaleTz=2.0;//variables used to zoom on the z axis
GLfloat viewangleTz=0,viewangleCz=0,viewangleSz=0;//variables used to rotate the objects
const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

float viewangle =0;
/* GLUT callback Handlers */
void glutSolidCube ( GLdouble size ); // size = 3.0
void glutSolidTeapot ( GLdouble size ); // size = 1.5
void glutSolidSphere ( GLdouble radius, GLint slices, GLint stacks); // (1.3,24,24)

static void resize(int width, int height)
{
     double Ratio;

   if(width<=height)
            glViewport(0,(GLsizei) (height-width)/2,(GLsizei) width,(GLsizei) width);
    else
          glViewport((GLsizei) (width-height)/2 ,0 ,(GLsizei) height,(GLsizei) height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective (50.0f,1,0.1f, 100.0f);
 }

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0, 0, -3);
    gluLookAt(0,5,10,0.0,0.0,0.0,0.0,1.0,0.0);

    if(WireFrame)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);      //Draw Our Mesh In Wire-frame Mesh
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);      //Toggle WIRE FRAME

    // your code here
    glPushMatrix ();//place us on the position of the stack we want to be at
    glRotatef(viewangleSz, 0,1,0);//rotates the object
    glTranslated(-3.5,0.0,scaleSz);//moves the object along the z axis
    glutSolidSphere ( 1.3, 24, 24);//place the object
    glPopMatrix ();

    glPushMatrix ();//place us on the position of the stack we want to be at
    glRotatef(viewangleCz, 0,1,0);//rotates the object
    glTranslated(4.0,0.0,scaleCz);//moves the object along the z axis
    glutSolidCube ( 3.0 );//place the object
    glPopMatrix ();

    glPushMatrix ();//place us on the position of the stack we want to be at
    glRotatef(viewangleTz, 0,1,0);//rotates the object
    glTranslated(0.0,0.0,scaleTz);//moves the object along the z axis
    glutSolidTeapot ( 1.5 );//place the object
    glPopMatrix ();

    glutSwapBuffers();
}

static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;
        case 'c': //if c is pressed set the other flags false and set cflag to true
            flagC=true;
            flagT=false;
            flagS=false;
            break;
        case 't':
            flagC=false;
            flagT=true;
            flagS=false;
            break;
        case 's':
            flagC=false;
            flagT=false;
            flagS=true;
            break;
            case 'r': //resets the flags so now we can zoom or rotate all objects at once
            flagC=false;
            flagT=false;
            flagS=false;
            break;
    }
}

void Specialkeys(int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_LEFT:
    if(flagC) {//if c has been pressed
        viewangleCz-=5;//change the variable that is used to rotate the object
        }
                if(flagT) {//if T has been pressed
       viewangleTz-=5;//change the variable that is used to rotate the object
        }
                if(flagS) {//if S has been pressed
        viewangleSz-=5;//change the variable that is used to rotate the object
        }
                if(flagT==false&&flagC==false&&flagS==false){//if no buttons have been pressed rotate all objects at once
                    viewangleTz-=5;
                    viewangleSz-=5;
                    viewangleCz-=5;
                }
    break;
    case GLUT_KEY_RIGHT:
            if(flagC) {
        viewangleCz+=5;
        }
                if(flagT) {
       viewangleTz+=5;
        }
                if(flagS) {
        viewangleSz+=5;
        }
                if(flagT==false&&flagC==false&&flagS==false){
                    viewangleTz+=5;
                    viewangleSz+=5;
                    viewangleCz+=5;
                }
    break;
    case GLUT_KEY_UP: if(flagC) {//if c has been pressed
        scaleCz+=0.5;//change the increment the variable that is used to transform the object along the z axis
        }
                if(flagT) {
        scaleTz+=0.5;
        }
                if(flagS) {
        scaleSz+=0.5;
        }
                if(flagT==false&&flagC==false&&flagS==false){//if no buttons have been pressed transform all objects at once
                    scaleSz+=0.5;
                    scaleTz+=0.5;
                    scaleCz+=0.5;
                }

    break;
    case GLUT_KEY_DOWN: if(flagC) {
        scaleCz-=0.5;
    }
                if(flagT) {
        scaleTz-=0.5;
                }
                if(flagS){
        scaleSz-=0.5;
        }
                if(flagT==false&&flagC==false&&flagS==false)
                {
                    scaleSz-=0.5;
                    scaleTz-=0.5;
                    scaleCz-=0.5;
                }


    break;
   }
  glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}

static void init(void)
{
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

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
}

/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(800,600);
    glutInitWindowPosition(300,300);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("GLUT Shapes");
    init();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutSpecialFunc(Specialkeys);

    glutIdleFunc(idle);
    glutMainLoop();

    return EXIT_SUCCESS;
}
