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

#include <SOIL.h>
#include <glut.h>
//chaise allegra
//106006387
//Csci 173

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
    int A1,C1,A2,C2,A3,C3;
}_FVertex;
    _ObjVertex vert[30000];
    _ObjVertex norm[30000];
    _FVertex fline[30000];

int RX = 0, RY = 0, RZ = 0;


float i = 0;
const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

    float xpos = 0.0;
    float ypos = 0.0;
    float Wwidth, Wheight;

    float spin = 0.0;
    GLuint tex[6];



static void resize(int width, int height)
{
    double Ratio;
    Wwidth = (float)width;
    Wheight = (float)height;
    Ratio = (double)width /(double)height;
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

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_NEAREST);

}

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(2.0, 0.0f, 0.0f);
    gluLookAt(0,0,6,0.0,0.0,0.0,0.0,1.0,100.0);

    if(WireFrame)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);      //Draw Our Mesh In Wireframe Mesh
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);      //Toggle WIRE FRAME

    glTranslatef(-2.0f, 0.0f, 3.2f);
    glTranslated(0.0, 0.0, RZscale);
    glRotatef(RX, 1.0, 0.0, 0.0);
    glRotatef(RY, 0.0, 1.0, 0.0);
    glRotatef(RZ, 0.0, 0.0, 1.0);

    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glBindTexture(GL_TEXTURE_2D, tex[0]);
    glBegin(GL_QUADS);
        // Front Face
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);    // Bottom Left Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, 1.0f, 1.0f);    // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, -1.0f, 1.0f);   // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 1.0f);   // Top Left Of The Texture and Quad
    glEnd();

    glBindTexture(GL_TEXTURE_2D, tex[1]);
    glBegin(GL_QUADS);
        // Back Face
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 1.0f, -1.0f);   // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Top Left Of The Texture and Quad
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, 1.0f, -1.0f);   // Bottom Left Of The Texture and Quad
    glEnd();

    glBindTexture(GL_TEXTURE_2D, tex[2]);
    glBegin(GL_QUADS);
        // Top Face
        glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);    // Top Left Of The Texture and Quad
        glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Bottom Left Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f); glVertex3f( -1.0f, 1.0f, 1.0f);   // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f); glVertex3f( -1.0f, 1.0f, -1.0f);  // Top Right Of The Texture and Quad
    glEnd();

    glBindTexture(GL_TEXTURE_2D, tex[3]);
    glBegin(GL_QUADS);
        // Bottom Face
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 1.0f);   // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, 1.0f);   // Top Left Of The Texture and Quad
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Bottom Left Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Bottom Right Of The Texture and Quad
    glEnd();

    glBindTexture(GL_TEXTURE_2D, tex[4]);
    glBegin(GL_QUADS);
        // Right face
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, 1.0f, -1.0f);   // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, 1.0f);   // Top Left Of The Texture and Quad
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, 1.0f, 1.0f);    // Bottom Left Of The Texture and Quad
    glEnd();

    glBindTexture(GL_TEXTURE_2D, tex[5]);
    glBegin(GL_QUADS);
        // Left Face
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, -1.0f);   // Bottom Left Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);    // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 1.0f);   // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Top Left Of The Texture and Quad
    glEnd();

    glScalef(0.2, 0.2, 0.2);
    glTranslatef(0.0, 0.0,0.0);

    glBegin(GL_TRIANGLES);//begin drwing using triangles

    for (int i = 0; i < 22250; i++)
    {

        glColor3f(vert[(fline[i].A1)-1].X,vert[(fline[i].A1)-1].Y,vert[(fline[i].A1)-1].Z);
       glNormal3f(norm[(fline[i].C1)-1].X, norm[(fline[i].C1)-1].Y, norm[(fline[i].C1)-1].Z);
       glVertex3f(vert[(fline[i].A1)-1].X,vert[(fline[i].A1)-1].Y,vert[(fline[i].A1)-1].Z);

       glColor3f(norm[(fline[i].C2)-1].X, norm[(fline[i].C2)-1].Y, norm[(fline[i].C2)-1].Z);
        glNormal3f(norm[(fline[i].C2)-1].X, norm[(fline[i].C2)-1].Y, norm[(fline[i].C2)-1].Z);
        glVertex3f(vert[(fline[i].A2)-1].X,vert[(fline[i].A2)-1].Y,vert[(fline[i].A2)-1].Z);

        glColor3f(vert[(fline[i].A3)-1].X,vert[(fline[i].A3)-1].Y,vert[(fline[i].A3)-1].Z);
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
        case 'a':
            RZ = (RZ + 5)%360;
            break;
        case 'd':
            RZ = (RZ - 5)%360;
            break;
        case 'w':
            WireFrame =! WireFrame;
            break;
    }
}

void Specialkeys(int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_UP:
        RX = (RX +5)%360;
    break;
    case GLUT_KEY_DOWN:
        RX = (RX -5)%360;
    break;
    case GLUT_KEY_LEFT:
        RY = (RY -5)%360;
    break;
    case GLUT_KEY_RIGHT:
        RY = (RY +5)%360;
    break;
    case GLUT_KEY_HOME:
        RZscale = (RZscale -1);
    break;
    case GLUT_KEY_END:
        RZscale = (RZscale +1);
    break;
    }
    glutPostRedisplay();
}

static void init(void)
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glGenTextures(6, tex);
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

    glEnable(GL_TEXTURE_2D);
    textureLoad("images/box.jpg",tex[0]);
    textureLoad("images/box.jpg",tex[1]);
    textureLoad("images/roof.jpg",tex[2]);
    textureLoad("images/floor.jpg",tex[3]);
    textureLoad("images/box.jpg",tex[4]);
    textureLoad("images/box.jpg",tex[5]);
}

int main(int argc, char *argv[])
{
    int t=0;
    glutInit(&argc, argv);

    glutInitWindowSize(800,600);
    glutInitWindowPosition(0,0);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Project Assignment 4");
    init();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutSpecialFunc(Specialkeys);
    ifstream fin;
    fin.open("Cube.obj");
    string prog,placeholder1;

    float Tx,Ty,Tz;
    long x=0,y=0,z=0;
    string prog2;
    while(!fin.eof()){
        string::size_type sz;
        fin >> prog;
        if(prog=="v")
        {
           fin>>Tx;
           vert[x].X=Tx;

           fin>>Ty;
            vert[x].Y=Ty;

           fin>>Tz;
            vert[x].Z=Tz;
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
                       else if(t==1)
                            t+=1;

                       else if(t==2)
                       {
                           fline[d].C1=hi;
                            t+=1;
                       }
                       else if(t==3){
                        fline[d].A2=hi;
                            t+=1;
                       }
                       else if(t==4)
                        t+=1;

                        else if(t==5)
                       {
                            fline[d].C2=hi;
                            t+=1;
                       }
                        else if(t==6){
                        fline[d].A3=hi;
                        t+=1;
                       }
                       else if(t==7)
                            t+=1;

                       else if(t==8)
                       {
                           fline[d].C3=hi;
                           t=0;
                       }

                       val="";
                  }
            v++;
          }
          t=0;
        d++;
        }
    }

    glutMainLoop();
    return EXIT_SUCCESS;
}
