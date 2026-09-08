//Barnsley's Fractal Fern

 #define _WCHAR_T_DEFINED

#include <string.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>

/* global vars */
int n = 200000;
int num = 0;
float x, y, oldx, oldy, a, b, c, d, e, f;



/* GLUT callback Handlers */
void resize(int width, int height)
{
    if(width<=height)
        glViewport(0,(GLsizei) (height-width)/2,(GLsizei) width,(GLsizei) width);

    else
        glViewport((GLsizei) (width-height)/2 ,0 ,(GLsizei) height,(GLsizei) height);
}

void init()
{
 glClearColor(0,0,0,1);
 glColor3d(0,1,0);
 oldx = 0.5;
 oldy = 0.5;
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 glOrtho(-2.5,2.75,10.5,0, -1.0, 1.0); // adjusted for suitable viewport

}

void display(void)
{
    glClear (GL_COLOR_BUFFER_BIT);        // clear display screen

    for (int i = 0; i < n; i++){
        num = rand()% 100;
        if (num < 85){      //function 2 85%: 0-84
            a = 0.85;
            b = -0.04;
            c = 0.04;
            d = 0.85;
            e = 0;
            f = 1.6;
        }
        else if (num < 92){ //function 3 7%: 85-91
            a = 0.2;
            b = 0.23;
            c = -0.26;
            d = 0.22;
            e = 0;
            f = 1.6;
        }
        else if (num < 99){ //function 4 7%: 92-98
            a = -0.15;
            b = 0.26;
            c = 0.28;
            d = 0.24;
            e = 0;
            f = 0.44;
        }
        else{               //function 1 1%: 99
            a = 0.85;
            b = -0.04;
            c = 0.04;
            d = 0.85;
            e = 0;
            f = 1.6;
        }

        x = a*oldx + c*oldy + e;
        y = b*oldx + d*oldy + f;

        oldx = x;
        oldy = y;

        glBegin(GL_POINTS);
        glVertex2d(x, y);
        glEnd();

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
   glutInitWindowSize (600, 800);                //window screen
   glutInitWindowPosition (100, 100);            //window position
   glutCreateWindow ("Program1");                //program title
   init();
   glutDisplayFunc(display);                     //callback function for display
   glutReshapeFunc(resize);                      //callback for reshape
   glutKeyboardFunc(key);                        //callback function for keyboard
   glutMainLoop();                               //loop

    return EXIT_SUCCESS;
}
