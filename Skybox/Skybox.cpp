#include <SOIL.h>

#include <string.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <GL/gl.h>
#include <GL/glext.h>
#include <stdlib.h>
#include <iostream>

#include <math.h>

using namespace std;

enum images{
    ROCKWALL,
    CUBEMAP,
    IMAGE_LAST
};

bool WireFrame= false;

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

//Amount of Images loaded with IDs
GLuint* texID = new GLuint[IMAGE_LAST];


struct vector{
    float x = 0.0;
    float y = 0.0;
    float z = 0.0;
};

//MODEL
float model_rot_speed = 5.0;
vector model_rot;

//SKYBOX
float skybox_rot_speed = 1.0;
vector skybox_rot;

//SCENE
float scene_zoom = 1.0;
float zoom_amount = 1.1;

//CAMERA
float cam_speed = .5;
vector camera_pos;


/* GLUT callback Handlers */

void textureLoader(char* filename, unsigned int id){
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &texID[id]);
    glBindTexture(GL_TEXTURE_2D, texID[id]);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


    int width, height;
    unsigned char* image;

    image = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGB);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

    if (!image){
        printf("file did not load");
        return;
    }

    SOIL_free_image_data(image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


void cubemapLoader(int id){
    glEnable(GL_TEXTURE_CUBE_MAP);
    glGenTextures(1, &texID[id]);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texID[id]);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    texID[id] = SOIL_load_OGL_cubemap("images/right.png", "images/left.png", "images/top.png", "images/bottom.png", "images/front.png", "images/back.png", 0, 0, SOIL_LOAD_RGB);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

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

 /**DRAW A SKYBOX WITH CUBEMAP DIRECTION COORDS**/
 static void skybox(float size){
     glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, -1.0f); // Normal Back Face
        glTexCoord3f(-1.0f, 1.0f, -1.0f); glVertex3f(-size, size, -size); // Top Right Of The Texture and Quad
        glTexCoord3f(-1.0f, -1.0f, -1.0f); glVertex3f(-size, -size, -size); // Bottom Right Of The Texture and Quad
        glTexCoord3f(1.0f, -1.0f, -1.0f); glVertex3f( size, -size, -size); // Bottom Left Of The Texture and Quad
        glTexCoord3f(1.0f, 1.0f, -1.0f); glVertex3f( size, size, -size); // Top Left Of The Texture and Quad
    glNormal3f(-1.0f, 0.0f, 0.0f); // Left Face
        glTexCoord3f(-1.0f, -1.0f, 1.0f); glVertex3f(-size, -size, size); // Bottom Right Of The Texture and Quad
        glTexCoord3f(-1.0f, -1.0f, -1.0f); glVertex3f(-size, -size, -size); // Bottom Left Of The Texture and Quad
        glTexCoord3f(-1.0f, 1.0f, -1.0f); glVertex3f(-size, size, -size); // Top Left Of The Texture and Quad
        glTexCoord3f(-1.0f, 1.0f, 1.0f); glVertex3f(-size, size, size); // Top Right Of The Texture and Quad
    glNormal3f(1.0f, 0.0f, 0.0f); // Right face
        glTexCoord3f(1.0f, -1.0f, -1.0f); glVertex3f( size, -size, -size); // Bottom Right Of The Texture and Quad
        glTexCoord3f(1.0f, -1.0f, 1.0f); glVertex3f( size, -size, size); // Bottom Left Of The Texture and Quad
        glTexCoord3f(1.0f, 1.0f, 1.0f); glVertex3f( size, size, size); // Top Left Of The Texture and Quad
        glTexCoord3f(1.0f, 1.0f, -1.0f); glVertex3f( size, size, -size); // Top Right Of The Texture and Quad
     glNormal3f(0.0f, 0.0f, 1.0f); // Normal Front Face
        glTexCoord3f(-1.0f, -1.0f, 1.0f); glVertex3f(-size, -size, size); // Bottom Left Of The Texture and Quad
        glTexCoord3f(-1.0f, 1.0f, 1.0f); glVertex3f(-size, size, size); // Top Left Of The Texture and Quad
        glTexCoord3f(1.0f, 1.0f, 1.0f); glVertex3f( size, size, size); // Top Right Of The Texture and Quad
        glTexCoord3f(1.0f, -1.0f, 1.0f); glVertex3f( size, -size, size); // Bottom Right Of The Texture and Quad
    glNormal3f(0.0f, 1.0f, 0.0f); // Normals Top Face
        glTexCoord3f(-1.0f, 1.0f, -1.0f); glVertex3f(-size, size, -size); // Top Left Of The Texture and Quad
        glTexCoord3f(1.0f, 1.0f, -1.0f); glVertex3f( size, size, -size); // Top Right Of The Texture and Quad
        glTexCoord3f(1.0f, 1.0f, 1.0f); glVertex3f( size, size, size); // Bottom Right Of The Texture and Quad
        glTexCoord3f(-1.0f, 1.0f, 1.0f); glVertex3f(-size, size, size); // Bottom Left Of The Texture and Quad
    glNormal3f(0.0f, -1.0f, 0.0f); // Bottom Face
        glTexCoord3f(-1.0f, -1.0f, -1.0f); glVertex3f(-size, -size, -size); // Top Right Of The Texture and Quad
        glTexCoord3f(-1.0f,-1.0f, 1.0f); glVertex3f(-size, -size, size); // Bottom Right Of The Texture and Quad
        glTexCoord3f(1.0f, -1.0f, 1.0f); glVertex3f( size, -size, size); // Bottom Left Of The Texture and Quad
        glTexCoord3f(1.0f, -1.0f, -1.0f); glVertex3f( size, -size, -size); // Top Left Of The Texture and Quad
     glEnd();
 }

 /**DRAW A CUBE WITH 2D TEXTURE COORDINATES**/
  static void cube(float size){
     glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, -1.0f); // Normal Back Face
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-size, size, -size); // Top Right Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, -size, -size); // Bottom Right Of The Texture and Quad
        glTexCoord2f(0.0f, 0.0f); glVertex3f( size, -size, -size); // Bottom Left Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f); glVertex3f( size, size, -size); // Top Left Of The Texture and Quad
    glNormal3f(-1.0f, 0.0f, 0.0f); // Left Face
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, -size, size); // Bottom Right Of The Texture and Quad
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, -size, -size); // Bottom Left Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size, -size); // Top Left Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-size, size, size); // Top Right Of The Texture and Quad
    glNormal3f(1.0f, 0.0f, 0.0f); // Right face
        glTexCoord2f(1.0f, 0.0f); glVertex3f( size, -size, -size); // Bottom Right Of The Texture and Quad
        glTexCoord2f(0.0f, 0.0f); glVertex3f( size, -size, size); // Bottom Left Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f); glVertex3f( size, size, size); // Top Left Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f); glVertex3f( size, size, -size); // Top Right Of The Texture and Quad
     glNormal3f(0.0f, 0.0f, 1.0f); // Normal Front Face
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, -size, size); // Bottom Left Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size, size); // Top Left Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f); glVertex3f( size, size, size); // Top Right Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f); glVertex3f( size, -size, size); // Bottom Right Of The Texture and Quad
    glNormal3f(0.0f, 1.0f, 0.0f); // Normals Top Face
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size, -size); // Top Left Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f); glVertex3f( size, size, -size); // Top Right Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f); glVertex3f( size, size, size); // Bottom Right Of The Texture and Quad
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, size, size); // Bottom Left Of The Texture and Quad
    glNormal3f(0.0f, -1.0f, 0.0f); // Bottom Face
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-size, -size, -size); // Top Right Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, -size, size); // Bottom Right Of The Texture and Quad
        glTexCoord2f(0.0f, 0.0f); glVertex3f( size, -size, size); // Bottom Left Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f); glVertex3f( size, -size, -size); // Top Left Of The Texture and Quad
     glEnd();
 }

/**DISPLAY**/
static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //Skybox
    glRotatef(skybox_rot.x, 1, 0, 0);
    glRotatef(skybox_rot.y, 0, 1, 0);

    glPushMatrix();
        glEnable(GL_TEXTURE_CUBE_MAP);
        glDepthMask(GL_FALSE);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texID[CUBEMAP]);
        skybox(1);
        glDepthMask(GL_TRUE);
        glDisable(GL_TEXTURE_CUBE_MAP);
    glPopMatrix();

    gluLookAt(0,0,10,0.0,0.0,0.0,0.0,1.0,0.0);

    if(WireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		//Draw Our Mesh In Wireframe Mesh
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		//Toggle WIRE FRAME


    // Move Camera
    glTranslatef(camera_pos.x, camera_pos.y, camera_pos.z);

    // Zoom Scene
    glScalef(scene_zoom, scene_zoom, scene_zoom);

    //Cube
    glPushMatrix();
        glEnable(GL_TEXTURE_2D);
        glRotatef(model_rot.x, 1, 0, 0);
        glRotatef(model_rot.y, 0, 1, 0);
        glBindTexture(GL_TEXTURE_2D, texID[ROCKWALL]);
        cube(1);
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();


    glutSwapBuffers();
}

static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'w':
            model_rot.x = int(model_rot.x - model_rot_speed) % 360;
            break;
        case 'a':
            model_rot.y = int(model_rot.y - model_rot_speed) % 360;
            break;
        case 's':
            model_rot.x = int(model_rot.x + model_rot_speed) % 360;
            break;
        case 'd':
            model_rot.y = int(model_rot.y + model_rot_speed) % 360;
            break;
        case 'i':
            camera_pos.y -= cam_speed;
            break;
        case 'j':
            camera_pos.x += cam_speed;
            break;
        case 'k':
            camera_pos.y += cam_speed;
            break;
        case 'l':
            camera_pos.x -= cam_speed;
            break;
        case 'o':
            camera_pos.x = 0.0f;
            camera_pos.y = 0.0f;
            camera_pos.z = 0.0f;
        case 'z':
            scene_zoom *= zoom_amount;
            break;
        case 'x':
            scene_zoom /= zoom_amount;
            break;
        case ' ':
            //RESET ALL
            model_rot.x = 0.0f;
            model_rot.y = 0.0f;
            skybox_rot.x = 0.0f;
            skybox_rot.y = 0.0f;

            camera_pos.x  = 0.0f;
            camera_pos.y = 0.0f;

            scene_zoom = 1.0;
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
        skybox_rot.x = int(skybox_rot.x - skybox_rot_speed) % 360;
        break;
    case GLUT_KEY_LEFT:
        skybox_rot.y = int(skybox_rot.y - skybox_rot_speed) % 360;
        break;
    case GLUT_KEY_DOWN:
        skybox_rot.x = int(skybox_rot.x + skybox_rot_speed) % 360;
        break;
    case GLUT_KEY_RIGHT:
        skybox_rot.y = int(skybox_rot.y + skybox_rot_speed) % 360;
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

    //glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    //glEnable(GL_LIGHTING);

    textureLoader("images/rockwall.jpg", ROCKWALL);
    cubemapLoader(CUBEMAP);

    //printf("%i", CUBEMAP);

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
