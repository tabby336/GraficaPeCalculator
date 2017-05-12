#include <GL/glut.h>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>

using namespace std;

// dimensiunea ferestrei in pixeli
#define dim 300

unsigned char prevKey;
double rotation = 0;

enum EObiect {cubw, cubs, sferaw, sferas} ob = cubw;

void DisplayAxe() {
  int X, Y, Z;
  X = Y = 200;
  Z = 200;

  glLineWidth(2);

  // axa Ox - verde
  glColor3f(0, 1, 0);
  glBegin(GL_LINE_STRIP); 
    glVertex3f(0,0,0);
    glVertex3f(X,0,0);
  glEnd();

  // axa Oy - albastru
  glColor3f(0, 0, 1);
  glBegin(GL_LINE_STRIP); 
    glVertex3f(0,0,0);
    glVertex3f(0,Y,0);
  glEnd();

  // axa Oz - rosu
  glColor3f(1, 0, 0);
  glBegin(GL_LINE_STRIP); 
    glVertex3f(0,0,0);
    glVertex3f(0,0,Z);
  glEnd();

  glLineWidth(1);
}

// cub wireframe
void Display1() {
   glColor3f(1,0,0);
   glutWireCube(2);
}

// cub solid
void Display2() {
   glColor3f(1,0,0);
   glutSolidCube(2);
}

// sfera wireframe
void Display3() {
   glColor3f(0,0,1);
   glutWireSphere(1, 10, 10);
}

// sfera solida
void Display4() {
   glColor3f(0,0,1);
   glutSolidSphere(1, 10, 10);
}

void DisplayObiect()
{
  switch (ob)
  {
  case cubw:
    Display1();
    break;
  case cubs:
    Display2();
    break;
  case sferaw:
    Display3();
    break;
  case sferas:
    Display4();
    break;
  default:
    break;
  }
}

// rotatia cu un unghi de 10 grade in raport cu axa x
void DisplayX() {
  glMatrixMode(GL_MODELVIEW);
  glRotated(10,1,0,0);
}

// rotatia cu un unghi de 10 grade in raport cu axa y
void DisplayY() {
  glMatrixMode(GL_MODELVIEW);
  glRotated(10,0,1,0);
}

// rotatia cu un unghi de 10 grade in raport cu axa z
void DisplayZ() {
  glMatrixMode(GL_MODELVIEW);
  glRotated(10,0,0,1);
}

// Translatia cu 0.2, 0.2, 0.2
void DisplayT() {
  glMatrixMode(GL_MODELVIEW);
  glTranslatef(0.2, 0.2, 0.2);
}

// Scalarea cu 1.2, 1.2, 1.2
void DisplayS() {
  glMatrixMode(GL_MODELVIEW);
  glScalef(2, 2, 2);
}

void Init(void) {
  glClearColor(1, 1, 1, 1);
  glLineWidth(2);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-10, 10, -10, 10, 30, -30);

  glClear(GL_COLOR_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glRotated(20, 1, 0, 0);
  glRotated(-20, 0, 1, 0);
}

#define PI 3.14159265

struct Punct
{
  Punct(double X, double Y, double Z) {
    x = X;
    y = Y;
    z = Z;
  }
  void rotate_z(double unghi) {
    Punct P(x, y, z);
    double unghi_radiani = PI * unghi / 180.0;
    x = P.x * cos(unghi_radiani) - P.y * sin(unghi_radiani);
    y = P.x * sin(unghi_radiani) + P.y * cos (unghi_radiani);
    z = P.z;
    // cout <<fixed<<unghi<<"   "<< cos(unghi_radiani) <<"\n";
  }

  void rotate_y(double unghi) {
    Punct P(x, y, z);
    double unghi_radiani = PI * unghi / 180.0;
    z = P.z * cos (unghi_radiani) - P.x * sin(unghi_radiani);
    x = P.z * sin(unghi_radiani) + P.x * cos(unghi_radiani);
    y = P.y;
    // cout <<fixed<<unghi<<"   "<< cos(unghi_radiani) <<"\n";
  }

  void rotate_x(double unghi) {
    Punct P(x, y, z);
    double unghi_radiani = PI * unghi / 180.0;
    y = P.y * cos(unghi_radiani) - P.z * sin(unghi_radiani);
    z = P.y * sin(unghi_radiani) + P.z * cos(unghi_radiani);
    x = P.x;
    // cout <<fixed<<unghi<<"   "<< cos(unghi_radiani) <<"\n";
  }
  double x, y, z;
};

double dist(Punct A, Punct B) {
  return sqrt((B.x - A.x) * (B.x - A.x) + (B.z - A.z) * (B.z - A.z) + (B.y - A.y) * (B.y - A.y));
}

double transforma_x(Punct A, Punct B, Punct P) {
  double D1 = sqrt((B.x - A.x) * (B.x - A.x) + (B.z - A.z) * (B.z - A.z));
  double D2 = sqrt((B.x - A.x) * (B.x - A.x) + (B.z - A.z) * (B.z - A.z) + (B.y - A.y) * (B.y - A.y));
  return (B.z - A.z) / D1 * P.x -
         (B.x - A.x) * (B.y - A.y) / (D1 * D2) * P.y+ 
         (B.x - A.x) / D2 * P.z;
}

double transforma_y(Punct A, Punct B, Punct P) {
  double D1 = sqrt((B.x - A.x) * (B.x - A.x) + (B.z - A.z) * (B.z - A.z));
  double D2 = sqrt((B.x - A.x) * (B.x - A.x) + (B.z - A.z) * (B.z - A.z) + (B.y - A.y) * (B.y - A.y));
  return D1 / D2 * P.y+ 
         (B.y - A.y) / D2 * P.z;
}

double transforma_z(Punct A, Punct B, Punct P) {
  double D1 = sqrt((B.x - A.x) * (B.x - A.x) + (B.z - A.z) * (B.z - A.z));
  double D2 = sqrt((B.x - A.x) * (B.x - A.x) + (B.z - A.z) * (B.z - A.z) + (B.y - A.y) * (B.y - A.y));
  return (-1) * (B.x - A.x) / D1 * P.x -
         (B.y - A.y) * (B.z - A.z) / (D1 * D2) * P.y+ 
         (B.z - A.z) / D2 * P.z;
}

Punct transforma(Punct A, Punct B, Punct P) {
  Punct p(transforma_x(A, B, P), transforma_y(A, B, P), transforma_z(A, B, P));
  return p;
}

void Displayb() {
  DisplayAxe();
  Punct A(10, 1, 1);
  Punct B(2, 2, 2);
  Punct C(3, 3, 3);
  glColor3f(0, 1, 0);
  glPolygonMode(GL_FRONT, GL_LINE);
  glBegin(GL_TRIANGLES); 
    glVertex3f(A.x,A.y,A.z);
    glVertex3f(B.x,B.y,B.z);
    glVertex3f(C.x,C.y,C.z); 
  glEnd();

  Punct at = transforma(A, B, A);
  Punct bt = transforma(A, B, B);
  Punct ct = transforma(A, B, C);

  glColor3f(0, 0, 1);
  glPolygonMode(GL_FRONT, GL_LINE);
  glBegin(GL_TRIANGLES); 
    glVertex3f(at.x,at.y,at.z);
    cout << at.x << " " << at.y << " " << at.z << "\n";
    cout << dist(at, bt) << " " << dist(A, B) << "\n";
    glVertex3f(bt.x,bt.y,bt.z);
    cout << bt.x << " " << bt.y << " " << bt.z << "\n";
    glVertex3f(ct.x,ct.y,ct.z); 
    cout << ct.x << " " << ct.y << " " << ct.z << "\n";
  glEnd();
}

void deseneaza_patrat(Punct cub[], int i0, int i1, int i2, int i3) {
  glPolygonMode(GL_FRONT, GL_LINE);
  glBegin(GL_LINE_LOOP);
    glVertex3f(cub[i0].x,cub[i0].y,cub[i0].z);
    glVertex3f(cub[i1].x,cub[i1].y,cub[i1].z);
    glVertex3f(cub[i2].x,cub[i2].y,cub[i2].z); 
    glVertex3f(cub[i3].x,cub[i3].y,cub[i3].z);
  glEnd();
}

void deseneaza_segment(Punct a, Punct b) {
  glPolygonMode(GL_FRONT, GL_LINE);
  glBegin(GL_LINE_LOOP);
    glColor3f(0, 0, 0);
    glVertex3f(a.x, a.y, a.z);
    glVertex3f(b.x, b.y, b.z);
  glEnd();
}
void Displayc() {
  // glMatrixMode(GL_MODELVIEW);
  DisplayAxe();
  int x = 4;
  Punct cub[] = {
    Punct(0, 0, x),
    Punct(x, 0, x),
    Punct(x, 0, 0),
    Punct(0, 0, 0),
    Punct(0, x, x),
    Punct(x, x, x),
    Punct(x, x, 0),
    Punct(0, x, 0),
  };

  for (int i = 0; i < 8; ++i) {
    cub[i].rotate_z(-45);
    cub[i].rotate_y(-54.735610317);
    cub[i].rotate_z(rotation);
    cub[i].rotate_y(54.735610317);
    cub[i].rotate_z(45);
    cout <<i<<"  "<<fixed<< cub[i].x << " " << cub[i].y << " " << cub[i].z << "\n";
  }
  deseneaza_patrat(cub, 0, 1, 2, 3);
  deseneaza_patrat(cub, 4, 5, 6, 7);
  deseneaza_patrat(cub, 0, 1, 5, 4);
  deseneaza_patrat(cub, 1, 2, 6, 5);
  deseneaza_patrat(cub, 2, 3, 7, 6);
  deseneaza_patrat(cub, 3, 0, 4, 7);
  deseneaza_segment(cub[3], cub[5]);
  int i = 5;
  cout <<fixed<< cub[i].x << " " << cub[i].y << " " << cub[i].z << "\n";
  i=3;
  cout <<fixed<< cub[i].x << " " << cub[i].y << " " << cub[i].z << "\n";
}

void Display(void) {
  switch(prevKey) 
  {
  case 'a':
    DisplayAxe();
    break;
  case '0':
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotated(20, 1, 0, 0);
    glRotated(-20, 0, 1, 0);
    break;
  case '1':
    Display1();
    ob = cubw;
    break;
  case '2':
    Display2();
    ob = cubs;
    break;
  case '3':
    Display3();
    ob = sferaw;
    break;
  case '4':
    Display4();
    ob = sferas;
    break;
  case 'b':
    Displayb();
    break;
  case 'c':
    glClear(GL_COLOR_BUFFER_BIT);
    Displayc();
    rotation += 10;
    // DisplayAxe();
    // DisplayObiect();
    break;
  case 'x':
    glClear(GL_COLOR_BUFFER_BIT);
    DisplayX();
    DisplayAxe();
    DisplayObiect();
    break;
  case 'y':
    glClear(GL_COLOR_BUFFER_BIT);
    DisplayY();
    DisplayAxe();
    DisplayObiect();
    break;
  case 'z':
    glClear(GL_COLOR_BUFFER_BIT);
    DisplayZ();
    DisplayAxe();
    DisplayObiect();
    break;
  case 't':
    glClear(GL_COLOR_BUFFER_BIT);
    DisplayT();
    DisplayAxe();
    DisplayObiect();
    break;
  case 's':
    glClear(GL_COLOR_BUFFER_BIT);
    DisplayS();
    DisplayAxe();
    DisplayObiect();
    break;
  default:
    break;
  }
  glutSwapBuffers();
}

void Reshape(int w, int h) {
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

   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);

   glutCreateWindow (argv[0]);

   Init();

   glutReshapeFunc(Reshape);
   
   glutKeyboardFunc(KeyboardFunc);
   
   glutMouseFunc(MouseFunc);

   glutDisplayFunc(Display);
   
   glutMainLoop();

   return 0;
}
