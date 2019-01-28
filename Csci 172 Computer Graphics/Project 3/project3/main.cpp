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

bool WireFrame= false;
int toggle=1;
const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

// --- variables for rotation of objects ---
float t_viewangle = 0;
// -----------------------------------------
// --- variables for translation of objects ---
float tx = 0.0, ty = 0.0, tz = 2.0;
// --------------------------------------------
// --- variables for key combination presses ---
bool tFlag = false;
bool xflag=false,yflag=false,zflag=false;
// ---------------------------------------------
#define PI 3.14159
GLfloat matrixX[16];
GLfloat matrixY[16];
GLfloat matrixZ[16];
GLfloat x, y, z, w;
static GLint RotateY=0; /* model rotation Y index*/
static GLint RotateX=0; /* model rotation X index*/
static GLint RotateZ=0; /* model rotation X index*/


/* GLUT callback Handlers */
void glutSolidTeapot ( GLdouble size ); // size = 1.5
void CreateMatrix(GLfloat *pMatrix);
void CreateFromAxisAngle(GLfloat X, GLfloat Y, GLfloat Z, GLfloat degree);
void glMultMatrixf(GLfloat matrixX);

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
    glTranslatef(0, 0, -3); // initial location of objects
    gluLookAt(0,5,10,0.0,0.0,0.0,0.0,1.0,0.0);



    if(WireFrame)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);      //Draw Our Mesh In Wire-frame Mesh
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);      //Toggle WIRE FRAME

    // your code here -----------------
    CreateMatrix(matrixX); /* initial quatonion */
    if(zflag==true)
    {
        CreateFromAxisAngle(0, 0, 1, RotateZ); /* quatonion for z rotation */
    }
    if(xflag==true){
        CreateFromAxisAngle(1, 0, 0, RotateX); /* quatonion for x rotation */
    }
    if(yflag==true){
        CreateFromAxisAngle(0, 1, 0, RotateY); /* quatonion for y rotation */
    }
    glMultMatrixf(matrixX); /* multiply original matrix */
    // -------code for teapot ---------
    glPushMatrix ();
        glColor3f(1.0,0.0,0.0);
        glTranslated(0.0,0.0,0.0);
        glutSolidTeapot(1.5);
        glPopMatrix();
        glutSwapBuffers();
    glPopMatrix ();
}

void CreateFromAxisAngle(GLfloat X, GLfloat Y, GLfloat Z, GLfloat degree)
{
    /* First we want to convert the degrees to radians since the angle is assumed to be in radians*/
        GLfloat angle = (GLfloat)((degree / 180.0f) * PI);
    /* Here we calculate the sin( theta / 2) once for optimization */
        GLfloat result = (GLfloat)sin( angle / 2.0f );
    /* Calculate the w value by cos( theta / 2 ) */
        w = (GLfloat)cos( angle / 2.0f );
    /* Calculate the x, y and z of the quaternion */
        x = (GLfloat)(X * result);
        y = (GLfloat)(Y * result);
        z = (GLfloat)(Z * result);
}

void CreateMatrix(GLfloat *pMatrix)
{
    // First row
    pMatrix[0] = 1.0f - 2.0f * ( y * y + z * z );
    pMatrix[1] = 2.0f * (x * y + z * w);
    pMatrix[2] = 2.0f * (x * z - y * w);
    pMatrix[3] = 0.0f;
    // Second row
    pMatrix[4] = 2.0f*(x * y - w * z);
    pMatrix[5] = 1.0f - 2.0f * ( x * x + z * z );
    pMatrix[6] = 2.0f * (z * y + x * w );
    pMatrix[7] = 0.0f;

    // Third row
    pMatrix[8] = 2.0f * (x * z + w * y);
    pMatrix[9] = 2.0f * (y * z - w * x);
    pMatrix[10] =  1.0f - 2.0f * (x * x + y * y);
    pMatrix[11] = 0.0f;

    // Fourth row
    pMatrix[12] = 0.0f;
    pMatrix[13] = 0.0f;
    pMatrix[14] = 0.0f;
    pMatrix[15] = 1.0f;
}

static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'a':
            RotateY = (RotateY +5)%360;
            yflag=true;
            xflag=false;
            zflag=false;
            break;
        case 'd':
            RotateY = (RotateY -5)%360;
            yflag=true;
            xflag=false;
            zflag=false;
            break;
        case 'w':
            RotateX = (RotateX +5)%360;
            yflag=false;
            xflag=true;
            zflag=false;
            break;
        case 's':
            RotateX = (RotateX -5)%360;
            yflag=false;
            xflag=true;
            zflag=false;
            break;
        case 'z':
            RotateZ = (RotateZ +5)%360;
            yflag=false;
            xflag=false;
            zflag=true;
            break;
        case 'x':
            RotateZ = (RotateZ -5)%360;
            yflag=false;
            xflag=false;
            zflag=true;
            break;
        case 't':
            if(toggle%2==1){
            WireFrame=true;
            toggle++;
            }
            else{
                WireFrame=false;
                toggle++;
            }
            break;
    }
}

void Specialkeys(int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_UP:
        RotateY = (RotateY +5)%360;
    break;

    case GLUT_KEY_DOWN:
        RotateZ = (RotateZ -5)%360;
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
    glutInitWindowPosition(10,10);
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
