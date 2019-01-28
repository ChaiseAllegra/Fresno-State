#include <string.h>
#include <GL/gl.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <SOIL.h>
#define PI 3.14159
#include <time.h>       /* time */
using namespace std;
bool WireFrame= false;
float i = 0;
const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };
const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };
int collision(float xpos, float ypos, float zpos);//function declaration
static void withTimer(int);
float a = 0;
float b = 5;
float c = 5;
float bound = 0.8;
int RX = 0, RY = 0, RZ = 0;
float CurXpos,CurYpos,CurZpos;//current x position of the ball, current y position of the ball,
float r = 0.3f, t = 0.07;//size of the ball, speed of the ball
float xdir=1,ydir=1,zdir=1;//the direction the ball travels to in the x and y directions, set these to the maximum of the play area
bool RwHit=false,TwHit=false,ZwHit=false;//Right wall hit: if set to false the ball just hit the left wall if set true it just hit the right wall, TwHit the same but with top and bottom
float Bxvelocity,Byvelocity,Bzvelocity;//The x and y velocity of the ball
float q=bound-0.1;
float zval = static_cast <float> (rand()) / static_cast <float> (RAND_MAX/q);


float RZscale = 1;

    float xpos =0;
    float ypos =0;
    float zpos=0;
    float Wwidth,Wheight;

    GLUquadricObj *sphere = NULL;

    float spin = 0.0;

    float xdes =0.0;
    float ydes =0.0;
    float zdes =0.0;

    float xcurr =0.0;
    float ycurr =0.0;
    float zcurr =0.0;
    float  mlocX =0;
    float  mlocY =0;
    float  mlocZ =0;

//    GLUquadricObj *sphere = NULL;
//    float spin = 0.0;
    GLuint tex[6];
    GLuint tex2;
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
    gluPerspective (45.0f,Ratio,0.1f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
 }
 int collision(float xpos, float ypos, float zpos)
{
   // cout<<"zpos: "<<zpos<<","<<"Curzpos: "<<CurZpos<<", "<<"Bound: "<<bound<<endl;

    if(xpos>=bound)
    {return 1;}//Right wall collision
    if(xpos<=-bound)//left wall collision
    { return 2; }
    if(ypos>=bound)//top wall collision
    {return 3;}
    if(ypos<=-bound)//bottom wall collision
    { return 4; }
    if(zpos>=bound)//wall closest to camera hit
        {return 5;}
    if(zpos<=-bound)//wall across form the camera hit
        {return 6;}
    return 0;
}
static void withTimer(int val)
{

   float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX/q);
   float p = static_cast <float> (rand()) / static_cast <float> (RAND_MAX/q);
   float ye = static_cast <float> (rand()) / static_cast <float> (RAND_MAX/q);

     spin +=0.5;
     Bxvelocity=t * (xdir);//XVelocity: Multiply the speed by the x direction you want to travel in
     CurXpos = CurXpos +  Bxvelocity;//Add the velocity calcualtion to he current x position
     Byvelocity = t * (ydir);////YVelocity: Multiply the speed by the y direction you want to travel in
     CurYpos = CurYpos + Byvelocity;//Add the velocity calcualtion to he current Y position
     Bzvelocity = t * (zdir);////YVelocity: Multiply the speed by the y direction you want to travel in
     CurZpos = CurZpos + Bzvelocity;//Add the velocity calcualtion to he current Y position

        if(collision(CurXpos,CurYpos,CurZpos)==1)
            xdir=-r;
        if(collision(CurXpos,CurYpos,CurZpos)==2)
            xdir=r;
        if(collision(CurXpos,CurYpos,CurZpos)==3)
            ydir=-p;
        if(collision(CurXpos,CurYpos,CurZpos)==4)
            ydir=p;
        if(collision(CurXpos,CurYpos,CurZpos)==5)
            zdir=-ye;
        if(collision(CurXpos,CurYpos,CurZpos)==6)
            zdir=ye;
    glutPostRedisplay();
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
    gluLookAt(0,0,10,0.0,0.0,0.0,0.0,1.0,100.0);
    if(WireFrame)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);      //Draw Our Mesh In Wireframe Mesh
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);      //Toggle WIRE FRAME
    glTranslated(0, 0, RZscale-130);
    glRotatef(RX,1,0,0);
    glRotatef(RY,0,1,0);
    glRotatef(RZ,0,0,1);
    glScalef(50.0, 50.0, 50.0);
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, tex[0]);
    glBegin(GL_QUADS);
    // Front Face
    /*
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);    // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, 1.0f, 1.0f);    // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, -1.0f, 1.0f);   // Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 1.0f);   // Top Left Of The Texture and Quad
    glEnd();
    */
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
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, 1.0f);   // Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -1.0f, -1.0f, -1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -1.0f, -1.0f, 1.0f);  // Top Right Of The Texture and Quad
    glEnd();
    glBindTexture(GL_TEXTURE_2D, tex[3]);
    glBegin(GL_QUADS);
    // Bottom Face
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);   // Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, 1.0f, -1.0f);   // Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, 1.0f, 1.0f);    // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);    // Bottom Right Of The Texture and Quad
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
    glPushMatrix();
        glTranslated(CurXpos, CurYpos, CurZpos);//move the ball to these x and y and z coordinates
        glScalef(0.5, 0.5, 0.5);
        glBindTexture(GL_TEXTURE_2D, tex2);
        glRotated(86.5, 1, 0, 0);
        glRotated(-spin, 0, 0, 1);
        gluSphere(sphere, 0.3, 40, 40);
    glPopMatrix();
    glutTimerFunc(20,withTimer,0);
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
         RZscale = (RZscale -5);
         break;
     case GLUT_KEY_END:
         RZscale = (RZscale +5);
         break;
   }
  glutPostRedisplay();
}

void mouse(int btn, int state, int x, int y){
    float z = rand() % 3 + (-1);
    float scale = 70*(Wwidth/Wheight);
    float mx = (float) (x-Wwidth/2)/scale;
    float my = (float) (Wheight/2-y)/scale;
  //  float mz = (float) (Wheight/2-z)/scale;
     //   cout << Wwidth << endl;
    //cout << Wheight << endl;
    switch(btn){
        case GLUT_LEFT_BUTTON:
        if(state==GLUT_DOWN){
                mlocX = mx;
                mlocY = my;
               cout<<mx<<", "<<my<<endl;
                if(mx>-bound&&mx<bound&&my>-bound&&my<bound)
                {
                    cout<<"within bounds"<<endl;
                CurXpos=mx;
                CurYpos=my;
                if(zdir>0)
                zdir=-zval;
                if(zdir<0)
                zdir=zval;
                }
                /*if(mx>bound)
                {
                    CurXpos=bound-0.1;
                }
                if(mx<bound)
                {
                    CurXpos=-bound+0.1;
                }
                if(my>bound)
                {
                    CurYpos=bound-0.1;
                }
                if(my<bound)
                {
                    CurYpos=-bound+0.1;
                }*/
                cout<<"current x and y pos: "<<CurXpos<<", "<<CurYpos<<endl;
                cout<<""<<endl;
                 // t=0;
            }
            break;
    }
     glutPostRedisplay();
};
static void init(void)
{
    zdes = (((double) rand() / (RAND_MAX)) + 1);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    glGenTextures(6, tex);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
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
    //glEnable(GL_LIGHT0);
    //glEnable(GL_NORMALIZE);
    //glEnable(GL_LIGHTING);
    sphere = gluNewQuadric();
    gluQuadricDrawStyle(sphere, GLU_FILL);
    gluQuadricTexture(sphere, GL_TRUE);
    gluQuadricNormals(sphere, GLU_SMOOTH);
    glEnable(GL_TEXTURE_2D);
    textureLoad("image/map.jpg",tex2);
/*
    glEnable(GL_TEXTURE_2D);
    textureLoad("image/front.jpg",tex[0]);
    textureLoad("image/back.jpg",tex[1]);
    textureLoad("image/bo1ttom.jpg",tex[2]);
    textureLoad("image/top.jpg",tex[3]);
    textureLoad("image/left.jpg",tex[4]);
    textureLoad("image/right.jpg",tex[5]);
*/
    textureLoad("image/d.jpg",tex[0]);
    textureLoad("image/d.jpg",tex[1]);
    textureLoad("image/d.jpg",tex[2]);
    textureLoad("image/d.jpg",tex[3]);
    textureLoad("image/d.jpg",tex[4]);
    textureLoad("image/d.jpg",tex[5]);
}
/* Program entry point */
int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(800,600);
    glutInitWindowPosition(0,0);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Project Assignment 3");
    init();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(key);
    glutSpecialFunc(Specialkeys);
    //glutIdleFunc(idle);
    glutMainLoop();
    return EXIT_SUCCESS;
}
