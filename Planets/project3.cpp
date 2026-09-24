/**
Name: Johan Gonzalez
ID: 301301300
Class: CSCI 172
Project Name: Project #3 - Planets
**/


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
bool start = false;
bool rings = false;

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

class planet{
    public:
        double x, y, z;
        double size;
        double rot = 0.0;
        planet(double x, double y, double z, double s){
            this->x = x;
            this->y = y;
            this->z = z;
            this->size = s;
        }

        void change_scale(double new_scale);

};

void planet::change_scale(double scale){
    size *= scale;
    x *= scale;
    y *= scale;
    z *= scale;
}

void draw_rings(double r, double points){
    glDisable(GL_LIGHTING);
    double theta = 360/points;

    glBegin(GL_POINTS);
    for (int i = 0; i < points; i++){
        float x = r * cosf(theta * i);
        float y = r * sinf(theta * i);

        glVertex3f(x, 0, y);
    }
    glEnd();
    glEnable(GL_LIGHTING);

}

//planets
planet sun = planet(0, 0, 0, 1.0);
planet earth = planet(3, 0, 0, .4);
planet moon = planet(.8, 0, 0, .1);
planet planet_B = planet(5, 0, 0, .5);

//vars
double global_rot = 0.0;


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

    // your code here
    if (start){
        planet_B.rot += .02;
        earth.rot += .04;
        moon.rot += .04;
    }

    //rotate all planets
    glRotatef(10, 0, 0, 1);
    glRotatef(global_rot, 0, 1, 0);
    glTranslatef(0, 1, 0);


    if (rings){
        draw_rings(planet_B.x, 480 * planet_B.size);
        draw_rings(earth.x, 360 * earth.size);
    }

    //SUN
    glPushMatrix();
    glColor3d(1, .6, 0);
    glTranslatef(sun.x, sun.y, sun.z);
    glRotatef(sun.rot, 0, 1, 0);
    glutSolidSphere(sun.size, 16, 16);
    glPopMatrix();

    //PLANET B
    glPushMatrix();
    glColor3d(1, 0 , 0);
    glRotatef(planet_B.rot, 0, 1, 0);
    glTranslatef(planet_B.x, planet_B.y, planet_B.z);
    glRotatef(planet_B.rot, 1, .5, 0);
    glutSolidSphere(planet_B.size, 16, 16);
    glPopMatrix();

    glPushMatrix();
    //EARTH
    glColor3d(0, .4 , 1);
    glRotatef(earth.rot, 0, 1, 0);
    glTranslatef(earth.x, earth.y, earth.z);
    glutSolidSphere(earth.size, 16, 16);

    //MOON
    glColor3d(.5, .5, .5);
    glRotatef(moon.rot, 0, 1, 0);
    glTranslatef(moon.x, moon.y, moon.z);
    glutSolidSphere(moon.size, 10, 10);
    glPopMatrix();


    glutSwapBuffers();
}

static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case ' ':
            start = !start;
            break;
        case 'w':
            WireFrame = !WireFrame;
            break;
        case 'r':
            rings = !rings;
            break;
        case 27 :
            break;
        case 'q':
            exit(0);
            break;
    }
}

void Specialkeys(int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_UP:
        sun.change_scale(1.05);
        earth.change_scale(1.05);
        moon.change_scale(1.05);
        planet_B.change_scale(1.05);       //increase current scale
        break;
    case GLUT_KEY_DOWN:
        sun.change_scale(.95);
        earth.change_scale(.95);
        moon.change_scale(.95);
        planet_B.change_scale(.95);      //decrease current scale
        break;
    case GLUT_KEY_LEFT:
        global_rot += 5;
        break;
    case GLUT_KEY_RIGHT:
        global_rot -= 5;
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

    glutCreateWindow("GLUT Planets");
    init();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutSpecialFunc(Specialkeys);

    glutIdleFunc(idle);
    glutMainLoop();

    return EXIT_SUCCESS;
}
