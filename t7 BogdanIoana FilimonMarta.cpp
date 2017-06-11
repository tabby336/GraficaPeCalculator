#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>


// dimensiunea ferestrei in pixeli
#define dim 300

unsigned char prevKey;
GLint k;
// latura cubului
GLdouble lat = 5;

void proiectieParalela(unsigned char);
void DisplayAxe();
void InitObiect();
void DisplayObiect();


double fNear = 1.5;
double fFov = 10;
double ratio = 1;
double headX = -5;
double fFar = 20;
double headY = 5;
double headZ = 5;
double cavalier[16] = {
    1, 0,0, 0,
    0, 1, 0, 0,
    -cos(atan(2.0)) / 2.0, -sin(atan(2.0)) / 2.0, 1, 0,
    0, 0, 0, 1 };


void Init(void) {
    glClearColor(1, 1, 1, 1);
    
    // validare test de adancime
    glEnable(GL_DEPTH_TEST);
    
    // se aloca 1 lista de display numerotata k
    k = glGenLists(1);
    InitObiect();
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
}

void Display()
{
    switch(prevKey)
    {
        case '0':
            // resetarea stivei matricilor de modelare si a celei de proiectie
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            DisplayAxe();
            break;
        case 'A':
            DisplayAxe();
            break;
        case 'C':
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            break;
        case 'X':
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            proiectieParalela('X');
            glMatrixMode(GL_MODELVIEW);
            glRotatef(10, 1, 0, 0);
            DisplayAxe();
            DisplayObiect();
            break;
        case 'Y':
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            proiectieParalela('Y');
            glMatrixMode(GL_MODELVIEW);
            glRotatef(10, 0, 1, 0);
            DisplayAxe();
            DisplayObiect();
            break;
        case 'Z':
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            proiectieParalela('Z');
            glMatrixMode(GL_MODELVIEW);
            glRotatef(10, 0, 0, 1);
            DisplayAxe();
            DisplayObiect();
            break;
        case 'q':
            // proiectie paralela ortografica frontala (fata)
            proiectieParalela('q');
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            DisplayAxe();
            glTranslated(0, 0, -lat);
            DisplayObiect();
            break;
        case 'w':
            // proiectie paralela ortografica frontala (spate)
            proiectieParalela('w');
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glPushMatrix();
            glTranslated(0, 0, -lat);
            glTranslated(lat/2.0, lat/2.0, lat/2.0);
            glRotated(180, 0, 1, 0);
            glTranslated(-lat/2.0, -lat/2.0, -lat/2.0);
            DisplayAxe();
            DisplayObiect();
            glPopMatrix();
            break;
        case '1':
            proiectieParalela('w');
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glPushMatrix();
            glTranslated(0, 0, -lat);
            glTranslated(lat / 2.0, lat / 2.0, lat / 2.0);
            glRotated(90, 1, 0, 0); //angle, x, y, x
            glTranslated(-lat / 2.0, -lat / 2.0, -lat / 2.0);
            DisplayAxe();
            DisplayObiect();
            glPopMatrix();
            break;
        case '2':
            
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glFrustum(-11.0, 11.0, -11.0, 11.0, 1.5, 20.0);
            
            
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glPushMatrix();
//            glTranslated(0, 0, -lat - 3);
//            glTranslated(0, -lat - 3, 0);
//            glTranslated(-lat - 3, 0, 0);

            
            
            glTranslated(-lat -4, 0, 0); //x
            glTranslated(0, -lat -5, 0); //y
            glTranslated(0, 0, -lat -5); //z
            
            DisplayAxe();
            DisplayObiect();
            glPopMatrix();
            
            break;
        case '3':
            
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glFrustum(-5.0, 5.0, -5.0, 5.0, 1.0, 10.0);
            
            
            
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glPushMatrix();
            glTranslated(0, 0, -lat - 3 );
            //glTranslated(0, -lat - 2, 0);
            //glTranslated(-lat - 2, 0, 0);
            glTranslated(0, -lat - 1, 0);
            glTranslated(- lat - 1, 0, 0);
            //glTranslated(lat / 2.0, lat / 2.0, lat / 2.0);
            
            DisplayAxe();
            DisplayObiect();
            glPopMatrix();
            
            break;
            
        case '4':
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glMatrixMode(GL_PROJECTION);
            
            glLoadIdentity();
            //glOrtho(-5, 10, -5, 10, -20, 20);
            glOrtho(-10, 10, -10, 10, 1, 40);
            
            //glPushMatrix();
            //glPushMatrix();
            
            
            
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            
            //glPushMatrix();
            glTranslated(0, 0, -4* lat - 1);
            glTranslated(-lat, 0, 0);
            // glTranslated(lat / 2.0, lat / 2.0, lat / 2.0);
            //glRotated(-30, 0, 1, 0);
//            glRotated(15, 1, 0, 0);
            //glTranslated(-lat / 2.0, -lat / 2.0, -lat / 2.0);
            glMultMatrixd(cavalier);
            
            
            DisplayAxe();
            DisplayObiect();
            //glPopMatrix();
            
            break;
        default:
            break;
    }
    glutSwapBuffers();
}

void Reshape(int w, int h) {
    h = (h == 0) ? 1 : h;
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

void KeyboardFunc(unsigned char key, int x, int y) {
    prevKey = key;
    if (key == 27) // escape
        exit(0);
    glutPostRedisplay();
}

void MouseFunc(int button, int state, int x, int y) {
}

int main(int argc, char** argv) {
    
    glutInit(&argc, argv);
    
    glutInitWindowSize(dim, dim);
    
    glutInitWindowPosition(100, 100);
    
    // GLUT_DEPTH - eliminarea suprafetelor ascunse cu alg. Z-buffer
    glutInitDisplayMode (GL_COLOR_BUFFER_BIT | GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    
    glutCreateWindow (argv[0]);
    
    Init();
    
    glutReshapeFunc(Reshape);
    
    glutKeyboardFunc(KeyboardFunc);
    
    glutMouseFunc(MouseFunc);
    
    glutDisplayFunc(Display);
    //   glutDisplayFunc(DisplayAll);
    
    glutMainLoop();
    
    return 0;
}

void proiectieParalela(unsigned char c) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    switch (c) {
        case 'X':
        case 'Y':
        case 'Z':
            glOrtho(-10, 10, -10, 10, -20, 20);
            break;
        case 'q':
        case 'w':
            glOrtho(-1, 6, -1, 6, -1, 20);
            break;
        default:
            break;
    }
}

void DisplayAxe() {
    int X, Y, Z;
    X = Y = 200;
    Z = 200;
    glLineWidth(2);
    
    // axa Ox - verde
    glColor3f(0.1, 1, 0.1);
    glBegin(GL_LINE_STRIP);
    glVertex3f(0,0,0);
    glVertex3f(X,0,0);
    glEnd();
    
    // axa Oy - albastru
    glColor3f(0.1, 0.1, 1);
    glBegin(GL_LINE_STRIP); 
    glVertex3f(0,0,0);
    glVertex3f(0,Y,0);
    glEnd();
    
    // axa Oz - rosu
    glColor3f(1, 0.1, 0.1);
    glBegin(GL_LINE_STRIP); 
    glVertex3f(0,0,0);
    glVertex3f(0,0,Z);
    glEnd();
    
    glLineWidth(1);
}

void InitObiect() {
    glNewList(k, GL_COMPILE);
    // fata 1
    glColor3f(1, 0, 0); // rosu
    glBegin(GL_QUADS);
    glVertex3d(0, lat, lat);
    glVertex3d(lat, lat, lat);
    glVertex3d(lat, 0, lat);
    glVertex3d(0, 0, lat);
    glEnd();
    // fata 2
    glColor3f(1, 1, 0); // galben
    glBegin(GL_QUADS);
    glVertex3d(lat, 0, 0);
    glVertex3d(lat, 0, lat);
    glVertex3d(lat, lat, lat);
    glVertex3d(lat, lat, 0);
    glEnd();
    // fata 3
    glColor3f(0, 1, 0); // verde
    glBegin(GL_QUADS);
    glVertex3d(0, lat, lat);
    glVertex3d(lat, lat, lat);
    glVertex3d(lat, lat, 0);
    glVertex3d(0, lat, 0);
    glEnd();
    // fata 4
    glColor3f(0, 0, 1); // albastru
    glBegin(GL_QUADS);
    glVertex3d(0, 0, 0);
    glVertex3d(lat, 0, 0);
    glVertex3d(lat, 0, lat);
    glVertex3d(0, 0, lat);
    glEnd();
    // fata 5
    glColor3f(1, 0, 1);  // magenta
    glBegin(GL_QUADS);
    glVertex3d(0, 0, lat);
    glVertex3d(0, 0, 0);
    glVertex3d(0, lat, 0);
    glVertex3d(0, lat, lat);
    glEnd();
    // fata 6
    glColor3f(0, 1, 1); // cyan
    glBegin(GL_QUADS);
    glVertex3d(0, lat, 0);
    glVertex3d(lat, lat, 0);
    glVertex3d(lat, 0, 0);
    glVertex3d(0, 0, 0);
    glEnd();
    glEndList();
}

void DisplayObiect()
{
    glCallList(k);
}
