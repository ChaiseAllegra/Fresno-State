#include <string.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include "glut.h"
#endif

#include <stdlib.h>
#include <iostream>
#include <math.h>
//#include "glut32.lib"
#include "SOIL.h"

#define PI 3.14159

using namespace std;

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
    float a=-2.5,b=2.5,c=1;
    float g=a,h=b,j=c;
    float Wwidth,Wheight;

    GLUquadricObj *sphere= NULL;
    GLUquadricObj *sphere2= NULL;
    GLUquadricObj *square=NULL;
    float spin = 0.0;

    GLuint tex=0,mex=1,rex=2;

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

void textureLoad(char * fileName, GLuint handle)
{
    int width, height;
    unsigned char *image;

    glBindTexture(GL_TEXTURE_2D, handle);
    image = SOIL_load_image(fileName, &width, &height,0,SOIL_LOAD_RGB);

    if(!image)cout<<" Image file not found "<<endl;
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,image);
    SOIL_free_image_data(image);
    if(handle==tex)
    cout<<"tex"<<endl;
    if(handle==mex)
    cout<<"mex"<<endl;
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_NEAREST);

}
void square2()
{
    glBegin(GL_POLYGON);
        glVertex3f(20, 12, -5);
        glVertex3f(-20, 15, -5);
        glVertex3f(-22, -12, -5);
        glVertex3f(22, -12, -5);
    glEnd();
}
static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0,0,10,0.0,0.0,0.0,0.0,1.0,100.0);





    if(WireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		//Draw Our Mesh In Wireframe Mesh
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		//Toggle WIRE FRAME

   glPushMatrix();
   glBindTexture(GL_TEXTURE_2D, tex);
   glRotated(86.5,1,0,0);
   glRotated(-spin, 0,0,1);
   gluSphere(sphere,3.0, 40,40);
   glPopMatrix();

   glPushMatrix();
   glBindTexture(GL_TEXTURE_2D, mex);
   glRotated(86.5,1,0,0);
   glRotated(-spin*12, 0,0,1);
   glTranslated(a-0.3,b,c+0.3);
   gluSphere(sphere2,0.8, 20,20);
   glPopMatrix();


   glPushMatrix();
   glBindTexture(GL_TEXTURE_2D, rex);
    square2();
   glPopMatrix();

    //Add positioned light
	GLfloat lightColor0[] = {0.5f, 0.5f, 1.5f, 1.0f}; //Color (0.5, 0.5, 0.5)
	GLfloat lightPos0[] = {g-0.5, h-2.5, j+6, 1.0f}; //Positioned at (4, 0, 8)
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);



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
    break;
   }
  glutPostRedisplay();
}


static void idle(void)
{
    // Use parametric equation with t increment for xpos and y pos
    // Don't need a loop

    spin +=0.005;
    glutPostRedisplay();
}



void mouse(int btn, int state, int x, int y){

    float scale = 100*(Wwidth/Wheight);

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
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

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


    sphere = gluNewQuadric();

    gluQuadricDrawStyle(sphere, GLU_FILL);
    gluQuadricTexture(sphere, GL_TRUE);
    gluQuadricNormals(sphere, GLU_SMOOTH);

    glEnable(GL_TEXTURE_2D);
    textureLoad("image/map.jpg",tex);
    cout<<"init"<<endl;
    sphere2 = gluNewQuadric();

    gluQuadricDrawStyle(sphere2, GLU_FILL);
    gluQuadricTexture(sphere2, GL_TRUE);
    gluQuadricNormals(sphere2, GLU_SMOOTH);

    glEnable(GL_TEXTURE_2D);
    textureLoad("image/moon.jpg",mex);

    /*square = gluNewQuadric();
    gluQuadricDrawStyle(square, GLU_FILL);
    gluQuadricTexture(square, GL_TRUE);
    gluQuadricNormals(square, GLU_SMOOTH);

    glEnable(GL_TEXTURE_2D);
    textureLoad("image/moon.jpg",rex);*/

}

/* Program entry point */
void Initialize() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

	// Lighting set up
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// Set lighting intensity and color
	GLfloat qaAmbientLight[]	= {0.2, 0.2, 0.2, 1.0};
	GLfloat qaDiffuseLight[]	= {0.8, 0.8, 0.8, 1.0};
	GLfloat qaSpecularLight[]	= {1.0, 1.0, 1.0, 1.0};
	glLightfv(GL_LIGHT0, GL_AMBIENT, qaAmbientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, qaDiffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, qaSpecularLight);

	// Set the light position
	GLfloat qaLightPosition[]	= {.5, .5, 0.0, 1.0};
	glLightfv(GL_LIGHT0, GL_POSITION, qaLightPosition);
}
int main(int argc, char *argv[])
{

    glutInit(&argc, argv);

    glutInitWindowSize(800,600);
    glutInitWindowPosition(0,0);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Project Assignment 5");
    init();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(key);
    glutSpecialFunc(Specialkeys);
	//Initialize();
    glutIdleFunc(idle);

    glutMainLoop();

    return EXIT_SUCCESS;
}
