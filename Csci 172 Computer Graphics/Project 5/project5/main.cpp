#include <string.h>
#include <sstream>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <iostream>
#include <math.h>
#include<vector>
#define PI 3.14159
#include <fstream>
#include <string>

using namespace std;
int RXamount=0,RYamount=0,RZamount=0;
float RZscale=1,RXscale=0,RYscale=0;
bool WireFrame= false,flagC=false,flagT=false,flagS=false;//flags to see what button has been pressed
GLfloat viewangleTz=0,viewangleCz=0,viewangleSz=0;//variables used to rotate the objects
int num=0;
GLfloat scaleCz=-3.0,scaleSz=-1.0,scaleTz=2.0;//variables used to zoom on the z axis

typedef struct _ObjVertex {
     float X, Y, Z;
} ObjVertex;
typedef struct _FVertex{
    int A1,B1,C1,A2,B2,C2,A3,B3,C3;
}_FVertex;
    _ObjVertex vert[30000];
    _ObjVertex norm[30000];
    _FVertex fline[30000];

//bool WireFrame= false;
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
    glTranslatef(0, 0, -50);
    gluLookAt(0,5,10,0.0,0.0,0.0,0.0,1.0,0.0);

        if(WireFrame)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);      //Draw Our Mesh In Wire-frame Mesh
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);      //Toggle WIRE FRAME
         glTranslated(RXscale,RYscale,RZscale);//allows us to translate it further
    glRotatef(RXamount,1,0,0);//rotate around x axis
    glRotatef(RYamount,0,1,0);//rotate around y axis
    glRotatef(RZamount,0,0,1);//rotate around z axis
    glRotatef(viewangleCz, 0,1,0);//rotates the object
    glPushMatrix ();//place us on the position of the stack we want to be at
    glRotatef(viewangleCz, 0,1,0);//rotates the object
    glTranslated(0.0,0.0,scaleCz);//moves the object along the z axis
   // glutSolidTeapot ( 1.5 );//place the object
    glPopMatrix ();

    glBegin(GL_TRIANGLES);//begin drwing using triangles

    for (int i = 0; i < 22250; i++)
    {

       glNormal3f(norm[(fline[i].C1)-1].X, norm[(fline[i].C1)-1].Y, norm[(fline[i].C1)-1].Z);
       glVertex3f(vert[(fline[i].A1)-1].X,vert[(fline[i].A1)-1].Y,vert[(fline[i].A1)-1].Z);

        glNormal3f(norm[(fline[i].C2)-1].X, norm[(fline[i].C2)-1].Y, norm[(fline[i].C2)-1].Z);
        glVertex3f(vert[(fline[i].A2)-1].X,vert[(fline[i].A2)-1].Y,vert[(fline[i].A2)-1].Z);

         glNormal3f(norm[(fline[i].C3)-1].X, norm[(fline[i].C3)-1].Y, norm[(fline[i].C3)-1].Z);
        glVertex3f(vert[(fline[i].A3)-1].X,vert[(fline[i].A3)-1].Y,vert[(fline[i].A3)-1].Z);


    }
    glEnd();

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
            if(num%2==0)
            {


            WireFrame=true;
            num+=1;
            }
            else{
                WireFrame=false;
                num+=1;
            }
            break;

    }
}

void Specialkeys(int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_LEFT:
        viewangleCz-=5;//change the variable that is used to rotate the object

    break;
    case GLUT_KEY_RIGHT:

        viewangleCz+=5;


    break;
    case GLUT_KEY_UP:
        RXamount = (RXamount +5)%360;
        break;
    case GLUT_KEY_DOWN:
     RXamount=(RXamount -5)%360;
     break;
     case GLUT_KEY_HOME:
     RZscale=(RZscale -1);
     break;
    case GLUT_KEY_END:
     RZscale=(RZscale +1);
     break;

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
}

/* Program entry point */

int main(int argc, char *argv[])
{
    int t=0;
    glutInit(&argc, argv);
    int stop=0;
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
    ifstream fin;
    fin.open("ateneam.obj");
    string prog,placeholder1;

    float Tx,Ty,Tz;
    long x=0,y=0,z=0;
    string prog2;
    while(prog!="faces"){
        string::size_type sz;
        fin >> prog;
        if(prog=="v")
        {
           fin>>Tx;
           vert[x].X=Tx/200;

           fin>>Ty;
            vert[x].Y=Ty/200;

           fin>>Tz;
            vert[x].Z=Tz/200;
            x++;


        }
        if(prog=="vn")
        {
            fin>>Tx;
            norm[y].X=Tx;

            fin>>Ty;
            norm[y].Y=Ty;

            fin>>Tz;
            norm[y].Z=Tz;

            y++;
        }
        static int d=0;
        int temp1=0;
        if(prog=="f")
        {
          string tmp,val;
          int v=1;

          int hi=0;

          getline(fin,tmp);

          string::iterator it=(tmp.begin());
          while(it!=tmp.end()-1)
          {
                  ++it;
                  val+=(tmp.at(v));

                  if((tmp.at(v)=='/') || (tmp.at(v)==' '))
                  {
                   stringstream conv(val);
                   conv>>hi;

                   if(t==0){
                    fline[d].A1=hi;

                        t+=1;
                   }
                   else if(t==1){
                    fline[d].B1=hi;

                        t+=1;
                   }
                   else if(t==2)
                   {
                       fline[d].C1=hi;

                        t+=1;
                   }
                   else if(t==3){
                    fline[d].A2=hi;

                        t+=1;
                   }
                   else if(t==4){
                    fline[d].B2=hi;

                    t+=1;
                   }
                    else if(t==5)
                   {
                        fline[d].C2=hi;

                        t+=1;
                   }
                    else if(t==6){
                    fline[d].A3=hi;

                    t+=1;
                   }
                   else if(t==7){
                        fline[d].B3=hi;

                        t+=1;
                   }
                   else if(t==8)
                   {
                       fline[d].C3=hi;

                       t=0;
                   }

                   val="";
                  }
            v++;
          }
        d++;
        }
    }

    glutMainLoop();
    return EXIT_SUCCESS;
}

