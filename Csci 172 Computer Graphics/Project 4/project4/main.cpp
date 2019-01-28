//chaise allegra
//106006387
//3d modeling
#include <string.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <iostream>
#include <math.h>

#define PI 3.14159

using namespace std;
int RXamount=0,RYamount=0,RZamount=0;
float RZscale=1,RXscale=0,RYscale=0;
GLfloat Vertices[8][3]= {
    0.0, 0.0, 0.0,
    0.0, 0.0, 1.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 1.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 1.0,
    1.0, 1.0, 0.0,
    1.0, 1.0, 1.0
};

GLfloat Normals[6][3] = {
    0.0, 0.0, 1.0,
    0.0, 0.0, -1.0,
    0.0, 1.0, 0.0,
    0.0, -1.0, 0.0,
    1.0, 0.0, 0.0,
    -1.0, 0.0, 0.0
};

GLint ind[12][6] ={ // vertices/normals
    1,2, 7,2, 5,2,
    1,2, 3,2, 7,2,
    1,6, 4,6, 3,6,
    1,6, 2,6, 4,6,
    3,3, 8,3, 7,3,
    3,3, 4,3, 8,3,
    5,5, 7,5, 8,5,
    5,5, 8,5, 6,5,
    1,4, 5,4, 6,4,
    1,4, 6,4, 2,4,
    2,1, 6,1, 8,1,
    2,1, 8,1, 4,1
};

bool WireFrame= false;
float i =0;
const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

    float xpos =0;
    float ypos =0;
    float Wwidth,Wheight;

/* GLUT callback Handlers */

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

 }

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(1.5f, 0.0f, -7.0f);
    gluLookAt(0,0,6,0.0,0.0,0.0,0.0,1.0,100.0);

    if(WireFrame)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);      //Draw Our Mesh In Wireframe Mesh
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);      //Toggle WIRE FRAME

    // your code here
    // --------code for cube ----------
    glTranslatef(-2,-0.3,8);//translates to center
    glTranslated(RXscale,RYscale,RZscale);//allows us to translate it further
    glRotatef(RXamount,1,0,0);//rotate around x axis
    glRotatef(RYamount,0,1,0);//rotate around y axis
    glRotatef(RZamount,0,0,1);//rotate around z axis

    glBegin(GL_TRIANGLES);//begin drwing using triangles

    for (int i = 0; i < 12; i++)//this will draw 12 triangles to make the cube
    {

         glColor3f(Vertices[ind[i][0]-1][0], Vertices[ind[i][0]-1][1], Vertices[ind[i][0]-1][2]);//generate the color for this sectino of the triangle
        glNormal3f(Normals[0][0], Normals[0][1], Normals[0][2]);//generates the normals for the point
        glVertex3f(Vertices[ind[i][0]-1][0], Vertices[ind[i][0]-1][1], Vertices[ind[i][0]-1][2]);//grabs the vertex that the point is created on

        glColor3f(Vertices[ind[i][2]-1][0], Vertices[ind[i][2]-1][1], Vertices[ind[i][2]-1][2]);//generate the color for this sectino of the triangle
        glNormal3f(Normals[2][0], Normals[2][1], Normals[2][2]);//generates the normals for the point
        glVertex3f(Vertices[ind[i][2]-1][0], Vertices[ind[i][2]-1][1], Vertices[ind[i][2]-1][2]);//grabs the vertex that the point is created on

        glColor3f(Vertices[ind[i][4]-1][0], Vertices[ind[i][4]-1][1], Vertices[ind[i][4]-1][2]);//generate the color for this sectino of the triangle
        glNormal3f(Normals[4][0], Normals[4][1], Normals[4][2]);//generates the normals for the point
        glVertex3f(Vertices[ind[i][4]-1][0], Vertices[ind[i][4]-1][1], Vertices[ind[i][4]-1][2]);//grabs the vertex that the point is created on
    }
    glEnd();//end

    // --------------------------------
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
        case 'a':
            RZamount = (RZamount +5)%360;
            break;
         case 'd':
            RZamount = (RZamount -5)%360;
            break;
      case 'w':
        WireFrame =!WireFrame;
           break;
    }
}

void Specialkeys(int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_UP:
        RXamount = (RXamount +5)%360;
        break;
    case GLUT_KEY_DOWN:
     RXamount=(RXamount -5)%360;
     break;
    case GLUT_KEY_LEFT:
     RYamount=(RYamount -5)%360;
     break;
    case GLUT_KEY_RIGHT:
     RYamount=(RYamount +5)%360;
     break;
    case GLUT_KEY_HOME:
     RZscale=(RZscale -1);
     break;
    case GLUT_KEY_END:
     RZscale=(RZscale +1);
     break;
   }
  glutPostRedisplay();
}


static void idle(void)
{
    // Use parametric equation with t increment for xpos and y pos
    // Don't need a loop
    glutPostRedisplay();
}



void mouse(int btn, int state, int x, int y){

    float scale = 54*(Wwidth/Wheight);

    switch(btn){
        case GLUT_LEFT_BUTTON:

        if(state==GLUT_DOWN){

               // get new mouse coordinates for x,y
               // use scale to match right
            }
            break;
    }
     glutPostRedisplay();
};



static void init(void)
{
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);

    glClearColor(1.0f, 0.5f, 1.0f, 0.0f);                 // assign a color you like

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
}

/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    glutInitWindowSize(800,600);
    glutInitWindowPosition(0,0);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Project Assignment 4");
    init();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(key);
    glutSpecialFunc(Specialkeys);

    glutIdleFunc(idle);

    glutMainLoop();

    return EXIT_SUCCESS;
}
