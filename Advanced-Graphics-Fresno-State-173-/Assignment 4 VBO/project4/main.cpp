#include <string.h>
#include <windows.h>
#define GLEW_STATIC
#include <sstream>
#include <GL/glew.h>
#include <glew.h>
#include <GL/glu.h>
#ifndef GL_H
#define GL_H
#endif
#include <GL/glut.h>
#include <iostream>
#include <math.h>
#include<vector>
#define PI 3.14159
#include <fstream>
#include <cstdlib>
#include <SOIL.h>
#include <glut.h>
/*
Chaise Allegra
106006387
5/7/18
*/

GLuint vertexbuffer;
using namespace std;
typedef struct {
     float X, Y, Z;
}Vertex;

struct Index{
    int X1,Z1, X2,Z2, X3,Z3;
};
    Vertex V[8];
    Vertex N[6];
    Index F[12];

    int ind[36];    // debug
    float T[108];   // for debug

void  arrayExpand();
int RX = 0, RY = 0, RZ = 0;
float RZscale = 1;
bool WireFrame = false;
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
GLuint vbo;

PFNGLGENBUFFERSARBPROC glewGenBuffersARB = NULL; // VBO Name Generation
PFNGLBINDBUFFERARBPROC glewBindBufferARB = NULL; // VBO Bind
PFNGLBUFFERDATAARBPROC glewBufferDataARB = NULL; // VBO Data Loading
PFNGLDELETEBUFFERSARBPROC glewDeleteBuffersARB = NULL; // VBO Deletion Procedure

void loadFile()
{
    int n = 0;
    ifstream fin;
    fin.open("Cube.obj");
    string temp;
    float a, b, c;
    int x = 0, y = 0;
    while(!fin.eof())
    {
        fin >> temp;
        if(temp == "v") // for vertices
        {
            fin>>a;
            V[x].X = a;
            fin>>b;
            V[x].Y = b;
            fin>>c;
            V[x].Z = c;
            x++;
        }
        if(temp == "vn") // for normals
        {
            fin>>a;
            N[y].X = a;
            fin>>b;
            N[y].Y = b;
            fin>>c;
            N[y].Z = c;
            y++;
        }
        static int i = 0;
        if(temp == "f") // for faces
        {
            string temp2, hold;
            int ind = 1;
            int m = 0;
            getline(fin,temp2);
            string::iterator it = (temp2.begin());
            while(it != temp2.end()-1) // until the end of the line
            {
                ++it;
                hold += (temp2.at(ind));
                if((temp2.at(ind) == '/') || (temp2.at(ind) == ' ')) // skip the '/' or spaces
                {
                    stringstream conv(hold); // convert the hold to integer for the array
                    conv>>m;
                    if(n == 0){
                        F[i].X1 = m;
                        n++;
                    }
                    else if(n == 1){
                        n++;
                    }
                    else if(n == 2){
                        F[i].Z1 = m;
                        n++;
                    }
                    else if(n == 3){
                        F[i].X2 = m;
                        n++;
                    }
                    else if(n == 4){
                        n++;
                    }
                    else if(n == 5){
                        F[i].Z2 = m;
                        n++;
                    }
                    else if(n == 6){
                        F[i].X3 = m;
                        n++;
                    }
                    else if(n == 7){
                        n++;
                    }
                    else if(n == 8){
                        F[i].Z3 = m;
                        n = 0;
                    }
                    hold = "";
                }
                ind++;
            }
            n = 0;
            i++;
        }
    }
    arrayExpand();
}

void makeRainbowCube() // draws the cube from the obj file
{
    int i =0;
    for (int i = 0; i < 12; i++)
    {
        glBegin(GL_TRIANGLES);
            glArrayElement(F[i].X1-1);
            glArrayElement(F[i].X2-1);
            glArrayElement(F[i].X3-1);
        glEnd();
    }
}

void makeSkyBox()
{
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
}

/*----------------------------------------------------------------------*/
Vertex eVertArray[12];
Vertex normalArray[12];
void arrayExpand()
{
    for (int i = 0; i < 12; i++)
    {
        eVertArray[i].X=V[(F[i].X1-1)].X;
        eVertArray[i].Y=V[(F[i].X1-1)].Y;
        eVertArray[i].Z=V[(F[i].X1-1)].Z;
        eVertArray[i+1].X=V[(F[i].X2-1)].X;
        eVertArray[i+1].Y=V[(F[i].X2-1)].Y;
        eVertArray[i+1].Z=V[(F[i].X2-1)].Z;
        eVertArray[i+2].X=V[(F[i].X3-1)].X;
        eVertArray[i+2].Y=V[(F[i].X3-1)].Y;
        eVertArray[i+2].Z=V[(F[i].X3-1)].Z;
        i+=3;
    }
    int a = 0;
    int b = 0;
    for (int i = 0; i < 12; i++)
    {
        normalArray[i].X=N[(F[i].Z1-1)].X;
        normalArray[i].Y=N[(F[i].Z1-1)].Y;
        normalArray[i].Z=N[(F[i].Z1-1)].Z;
        normalArray[i+1].X=N[(F[i].Z2-1)].X;
        normalArray[i+1].Y=N[(F[i].Z2-1)].Y;
        normalArray[i+1].Z=N[(F[i].Z2-1)].Z;
        normalArray[i+2].X=N[(F[i].Z3-1)].X;
        normalArray[i+2].Y=N[(F[i].Z3-1)].Y;
        normalArray[i+2].Z=N[(F[i].Z3-1)].Z;
        i+=3;
    }
    return;
}
/*----------------------------------------------------------------------*/

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
    gluLookAt(0,0,10,0.0,0.0,0.0,0.0,1.0,100.0);

    if(WireFrame)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);      //Draw Our Mesh In Wireframe Mesh
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);      //Toggle WIRE FRAME

    glTranslatef(-2.0f, 0.0f, 6.7f);
    glTranslated(0.0, 0.0, RZscale);
    glRotatef(RX, 1.0, 0.0, 0.0);
    glRotatef(RY, 0.0, 1.0, 0.0);
    glRotatef(RZ, 0.0, 0.0, 1.0);
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    makeSkyBox();

    glScalef(0.2, 0.2, 0.2);
    glTranslatef(0.0, 0.0,0.0);

    glDisable(GL_TEXTURE_2D);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glVertexPointer(3,GL_FLOAT,0,V);
    glNormalPointer(GL_FLOAT,3*sizeof(float),normalArray);
    glColorPointer(3,GL_FLOAT,0,eVertArray);

    makeRainbowCube();

    //--------------------new display------------------------
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER_ARB,0);
    //--------------------------------------------------------

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
    /*----------------------------------------------------------------------*/
    glewInit();
    /*----------------------------------------------------------------------*/

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glGenTextures(6, tex);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // assign a color you like
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

    loadFile();
    /*----------------------------------------------------------------------*/
    glGenBuffersARB(1, &vbo);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo);

    glBufferDataARB(GL_ARRAY_BUFFER_ARB, 8*sizeof(Vertex)+12*sizeof(Vertex)+24*sizeof(Vertex), 0, GL_DYNAMIC_DRAW);  //+12*sizeof(Vertex)
    glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, 8*sizeof(Vertex), V);
    glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 8*sizeof(Vertex), 12*sizeof(Vertex), normalArray);
    glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 12*sizeof(Vertex), 24*sizeof(Vertex), eVertArray); // expanded V array

    int buff;
    glGetBufferParameterivARB(GL_ARRAY_BUFFER_ARB,GL_BUFFER_SIZE_ARB,&buff);
    cout<<" buffer size "<<buff<<endl;
    glBindBuffer(GL_ARRAY_BUFFER_ARB,0);
    /*----------------------------------------------------------------------*/

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
    int n =0;
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

    glutMainLoop();
    return EXIT_SUCCESS;
}
