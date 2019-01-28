#include <string.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>
#include <stdlib.h>
/* GLUT callback Handlers */
float x,y,a,b,c,d,e,f,Tempx,TempY;
    int num, limit=200000;
void resize(int width, int height)
{
    if(width<=height)
        glViewport(0,(GLsizei) (height-width)/2,(GLsizei) width,(GLsizei) width);

    else
        glViewport((GLsizei) (width-height)/2 ,0 ,(GLsizei) height,(GLsizei) height);
}

void init()
{
      x=y=0.5;
    Tempx=x;
    TempY=y;
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     glOrtho(-2.5,2.75,10.5,0, -1.0, 1.0); // adjusted for suitable viewport

}

void display(void)
{
  glClear (GL_COLOR_BUFFER_BIT);        // clear display screen
  // your code here
  int i=0;
  while(i<limit)
  {
        num=rand()%100;
        if(num<85)
        {
            a= 0.85; b =-0.04; c =0.04;d =0.85; e =0; f=1.6;
        }

        else if (num<92)
        {
             a=0.2; b =0.23; c =-0.26; d =0.22; e =0; f=1.6;
        }

        else if(num<99){
            a=-0.15; b =0.26; c =0.28;d =0.24; e =0; f=0.44;
        }
        else
        {
            a=0;b=0;c=0;d=0.16;e=0;f=0;
        }
        x=a*Tempx+c*TempY+e;
        y=b*Tempx+d*TempY+f;
        glColor3f(40.0,10.0,0.0);//RGB Setup
        glBegin(GL_POINTS);
        glVertex3f(x, y,0);
        glEnd();
        Tempx=x;
        TempY=y;
      i++;
  }

  glFlush ();                           // clear buffer
}


void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :                       // esc key to exit
        case 'q':
            exit(0);
            break;
    }

    glutPostRedisplay();
}


/* Program entry point */

int main(int argc, char *argv[])
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize (500, 1500);                //window screen
   glutInitWindowPosition (100, 100);            //window position
   glutCreateWindow ("Program1");                //program title
   init();
   glutDisplayFunc(display);                     //callback function for display
   glutReshapeFunc(resize);                      //callback for reshape
   glutKeyboardFunc(key);                        //callback function for keyboard
   glutMainLoop();                               //loop
    return EXIT_SUCCESS;
}
