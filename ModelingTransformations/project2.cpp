/*
Name: Johan Gonzalez Rios
ID: 301301300
Class: CSCI 172
ProjectName: Project02 - Modeling Transformations
*/

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

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };


int cubeRot = 0;        //cube rotation in degrees
float cubeSize = 1;     //cube Size
int teapotRot = 0;        //teapot rotation in degrees
float teapotSize = 1;     //teapot Size
int sphereRot = 0;        //sphere rotation in degrees
float sphereSize = 1;     //sphere Size

//is model able to rotate and scale
bool cube = false;
bool teapot = false;
bool sphere = false;


/* GLUT callback Handlers */

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

    gluLookAt(0,5,10,0.0,0.0,0.0,0.0,1.0,0.0);

    if(WireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		//Draw Our Mesh In Wireframe Mesh
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		//Toggle WIRE FRAME

    // Cube
    glPushMatrix();
    glColor3d(0, 1, 0);
    glTranslatef(4.0, 0.0, -2.0);
    glRotatef(cubeRot, 0, 1, 0);
    glScalef(cubeSize, cubeSize, cubeSize);
    glutSolidCube(2.0);
    glPopMatrix();


    //Teapot
    glPushMatrix();
    glColor3d(1, 0, 0);
    glTranslatef(0.0, 0.0, -3.0);
    glRotatef(teapotRot, 0, 1, 0);
    glScalef(teapotSize, teapotSize, teapotSize);
    glutSolidTeapot(1.5);
    glPopMatrix();


    //Sphere
    glPushMatrix();
    glColor3d(0, 0, 1);
    glTranslatef(-3.5, 0, -1);
    glRotatef(sphereRot, 0, 1, 0);
    glScalef(sphereSize, sphereSize, sphereSize);
    glutSolidSphere(1.3, 15, 15);
    glPopMatrix();



    glutSwapBuffers();
}

static void key(unsigned char key, int x, int y)
{
    //set all mesh scale and rotation unable to be modified
    cube = false;
    teapot = false;
    sphere = false;

    switch (key)
    {
        case 27 :
        case 'c':
            cube = true;
            break;
        case 't':
            teapot = true;
            break;
        case 's':
            sphere = true;
            break;
        case 'w':
            WireFrame = !WireFrame;
            break;
        case 'q':
            exit(0);
            break;
    }
}

void Specialkeys(int key, int x, int y)
{
    //local vars to store scale and rot differences due to key input
    float scale = 0.0;
    int rot = 0;
    switch(key)
    {
    case GLUT_KEY_UP:
        scale = .5;
        break;
    case GLUT_KEY_DOWN:
        scale = -.5;
        break;
    case GLUT_KEY_RIGHT:
        rot = 5;
        break;
    case GLUT_KEY_LEFT:
        rot = -5;
        break;
   }

   //add differences to selected mesh scale/rotation
   if (cube){
        cubeSize += scale;
        cubeRot += rot;
   }
   else if (teapot){
        teapotSize += scale;
        teapotRot += rot;
   }
   else if (sphere){
        sphereSize += scale;
        sphereRot += rot;
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

    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);

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
