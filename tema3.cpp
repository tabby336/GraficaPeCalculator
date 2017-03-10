#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <float.h>
#include  <iostream>

#include <GL/freeglut.h>

// dimensiunea ferestrei in pixeli
#define dim 300
#define NRITER_JF 5000
#define MODMAX_JF 10000000

#define MX_JF 0.005
#define MY_JF 0.005

unsigned char prevKey;
int nivel = 0;


class C2coord
{
public:
  C2coord() 
  {
    m.x = m.y = 0;
  }

  C2coord(double x, double y) 
  {
    m.x = x;
    m.y = y;
  }

  C2coord(const C2coord &p) 
  {
    m.x = p.m.x;
    m.y = p.m.y;
  }

  C2coord &operator=(C2coord &p)
  {
    m.x = p.m.x;
    m.y = p.m.y;
    return *this;
  }

  int operator==(C2coord &p)
  {
    return ((m.x == p.m.x) && (m.y == p.m.y));
  }

protected:
  struct SDate
  {
    double x,y;
  } m;
};

class CPunct : public C2coord
{
public:
  CPunct() : C2coord(0.0, 0.0)
  {}

  CPunct(double x, double y) : C2coord(x, y)
  {}

  CPunct &operator=(const CPunct &p)
  {
    m.x = p.m.x;
    m.y = p.m.y;
    return *this;
  }

  void getxy(double &x, double &y)
  {
    x = m.x;
    y = m.y;
  }

  int operator==(CPunct &p)
  {
    return ((m.x == p.m.x) && (m.y == p.m.y));
  }

  void marcheaza()
  {
    glBegin(GL_POINTS);
      glVertex2d(m.x, m.y);
    glEnd();
  }

  void print(FILE *fis)
  {
    fprintf(fis, "(%+f,%+f)\n", m.x, m.y);
  }

};

class CVector : public C2coord
{
public:
  CVector() : C2coord(0.0, 0.0)
  {
    normalizare();
  }

  CVector(double x, double y) : C2coord(x, y)
  {
    normalizare();
  }

  CVector &operator=(CVector &p)
  {
    m.x = p.m.x;
    m.y = p.m.y;
    return *this;
  }

  int operator==(CVector &p)
  {
    return ((m.x == p.m.x) && (m.y == p.m.y));
  }

  CPunct getDest(CPunct &orig, double lungime)
  {
    double x, y;
    orig.getxy(x, y);
    fflush(stdout); 
    CPunct p(x + m.x * lungime, y + m.y * lungime);
    return p;
  }

  void rotatie(double grade)
  {
    double x = m.x;
    double y = m.y;
    double t = 2 * (4.0 * atan(1)) * grade / 360.0;
    m.x = x * cos(t) - y * sin(t);
    m.y = x * sin(t) + y * cos(t);
    normalizare();
  }

  void deseneaza(CPunct p, double lungime) 
  {
    double x, y;
    p.getxy(x, y);
    glColor3f(1.0, 0.1, 0.1);
    glBegin(GL_LINE_STRIP);
      glVertex2d(x, y);
      glVertex2d(x + m.x * lungime, y + m.y * lungime);
    glEnd();
  }

  void print(FILE *fis)
  {
    fprintf(fis, "%+fi %+fj", C2coord::m.x, C2coord::m.y);
  }

private:
  void normalizare()
  {
    double d = sqrt(C2coord::m.x * C2coord::m.x + C2coord::m.y * C2coord::m.y);
    if (d != 0.0) 
    {
      C2coord::m.x = C2coord::m.x * 1.0 / d;
      C2coord::m.y = C2coord::m.y * 1.0 / d;
    }
  }
};

class CArborePerronSus
{
public:
  void arborePerron(double lungime, 
                    int nivel, 
                    double factordiviziune, 
                    CPunct p, 
                    CVector v)
  {
    assert(factordiviziune != 0);
    CPunct p1, p2;
    if (nivel == 0) 
    {
    }
    else
    {
      v.rotatie(-45);
      v.deseneaza(p, lungime);
      p1 = v.getDest(p, lungime);
      arborePerron(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);

      v.rotatie(90);
      v.deseneaza(p, lungime);
      p1 = v.getDest(p, lungime);
      p2 = p1;

      v.rotatie(30);
      v.deseneaza(p1, lungime);
      p1 = v.getDest(p1, lungime);
      arborePerron(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);

      p1 = p2;
      v.rotatie(-60);
      v.deseneaza(p1, lungime);
      p1 = v.getDest(p1, lungime);
      p2 = p1;

      v.rotatie(20);
      v.deseneaza(p1, lungime/2);
      p1 = v.getDest(p1, lungime/2);
      arborePerron(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);

      p1 = p2;
      v.rotatie(-125);
      v.deseneaza(p1, lungime/2);
      p1 = v.getDest(p1, lungime/2);
      arborePerron(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);
    }
  }

  void afisare(double lungime, int nivel)
  {
    CVector v(0.0, -1.0);
    CPunct p(0.0, 1.0);

    v.deseneaza(p, 0.25);
    p = v.getDest(p, 0.25);
    arborePerron(lungime, nivel, 0.4, p, v);
  }
};


void Display1() {
  CArborePerronSus cap;

  char c[3];
  glPushMatrix();
  glLoadIdentity();
  glScaled(0.4, 0.4, 1);
  cap.afisare(1, nivel);
  glPopMatrix();
  nivel++;

}

class CDreptunghi {
public:
  CDreptunghi() : stanga(-0.9, 0.9),
                  dreapta(0.9, -0.9),
                  latura(1.8){
    deseneaza();
    deseneaza_intern();
  }
  CDreptunghi(CPunct stanga_dat, CPunct dreapta_dat, double latura_dat) : 
                  stanga(stanga_dat),
                  dreapta(dreapta_dat),
                  latura(latura_dat){
  }
  ~CDreptunghi(){}
  void deseneaza() {
    CVector v(1, 0);
    v.deseneaza(stanga, latura);  
    v.rotatie(-90);
    v.deseneaza(stanga, latura);
    v.rotatie(+180);
    v.deseneaza(dreapta, latura);
    v.rotatie(90);
    v.deseneaza(dreapta, latura); 
  }
  void deseneaza_intern() {
    double noua_latura = latura / 3;
    double x, y;
    stanga.getxy(x, y);
    CPunct intern_stanga(x + noua_latura, y - noua_latura);
    dreapta.getxy(x, y);
    CPunct intern_dreapta(x - noua_latura, y + noua_latura);
    CDreptunghi intern(intern_stanga, intern_dreapta, noua_latura);
    intern.deseneaza();
  }
  void afisare(int nivel) {
    std::cout<<"afisare\n";
    if (nivel != 0) {
      double noua_latura = latura / 3;
      double x, y;
      stanga.getxy(x, y);
      for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
          double x, y;
          stanga.getxy(x, y);
          CPunct intern_stanga(x + noua_latura * j, y - noua_latura * i);
          CPunct intern_dreapta(x + noua_latura * (j + 1), y - noua_latura * (i + 1));
          CDreptunghi intern(intern_stanga, intern_dreapta, noua_latura);
          // std::cout << "************\n";
          // std::cout << "x: " << x + noua_latura * j <<"\n" <<"y: " << y - noua_latura * i <<"\n";
          // std::cout << "x: " << x + noua_latura * (j + 1) <<"\n" <<"y: " << y - noua_latura * (i + 1)<<"\n";
          // std::cout << "************\n";
          if (i != 1 || j != 1) {
            intern.deseneaza_intern();
            intern.afisare(nivel - 1);
          }
        }
      }
      CDreptunghi dreptunghi();
      deseneaza_intern();
    }

  }
protected:
  CPunct stanga, dreapta;
  double latura;
};


void Display2() {
  CDreptunghi dreptunghi;
  dreptunghi.afisare(nivel);
  ++nivel;
}



int segments[20];
CPunct afiseazaSageata(CVector v, CPunct p, double lungime, int k) {
  if (k == nivel) {
    v.deseneaza(p, lungime);
    return v.getDest(p, lungime);
  } else {
    ++segments[k];
    int c = 1;
    if (segments[k] % 2 == k % 2) {
      c = -1;
    }

    double noua_latura = lungime / 3.0 * 1.5;

    v.rotatie(60 * c);
    p = afiseazaSageata(v, p, noua_latura, k + 1);

    v.rotatie(-60 * c);
    p = afiseazaSageata(v, p, noua_latura, k + 1);

    v.rotatie(-60 * c);
    return afiseazaSageata(v, p, noua_latura, k + 1);
  }
}

void Display3() {
  for (int i = 0; i < 20; ++i) segments[i] = 0;
  CVector v(1.0, 0);
  v.rotatie(30);
  CPunct p(-0.8, -0.97);
  double lungime = 2;
  afiseazaSageata(v, p, lungime, 0);
  ++nivel;
  std::cout<<"*** "<<segments[2]<<"\n";
}

class CComplex {
public:
  CComplex() : re(0.0), im(0.0) {}
  CComplex(double re1, double im1) : re(re1 * 1.0), im(im1 * 1.0) {}
  CComplex(const CComplex &c) : re(c.re), im(c.im) {}
  ~CComplex() {}

  CComplex &operator=(const CComplex &c) 
  {
    re = c.re;
    im = c.im;
    return *this;
  }

  double getRe() {return re;}
  void setRe(double re1) {re = re1;}
  
  double getIm() {return im;}
  void setIm(double im1) {im = im1;}

  double getModul() {return sqrt(re * re + im * im);}

  int operator==(CComplex &c1)
  {
    return ((re == c1.re) && (im == c1.im));
  }

  CComplex pow2() 
  {
    CComplex rez;
    rez.re = powl(re * 1.0, 2) - powl(im * 1.0, 2);
    rez.im = 2.0 * re * im;
    return rez;
  }

  friend CComplex operator+(const CComplex &c1, const CComplex &c2);
  friend CComplex operator*(CComplex &c1, CComplex &c2);

  void print(FILE *f) 
  {
    fprintf(f, "%.20f%+.20f i", re, im);
  }

private:
  double re, im;
};

CComplex operator+(const CComplex &c1, const CComplex &c2) 
{
  CComplex rez(c1.re + c2.re, c1.im + c2.im);
  return rez;
}

CComplex operator*(CComplex &c1, CComplex &c2) 
{
  CComplex rez(c1.re * c2.re - c1.im * c2.im,
                c1.re * c2.im + c1.im * c2.re);
  return rez;
}

class CMandelbrot {
public:
  CMandelbrot() {
    nriter = NRITER_JF;
    modmax = 2;
  }
  
  ~CMandelbrot() {}

  double getmodmax() {return modmax;}
  void setnriter(int v) {assert(v <= NRITER_JF); nriter = v;}
  int getnriter() {return nriter;}


  // testeaza daca x apartine multimii Julia-Fatou Jc
  // returneaza 0 daca apartine, -1 daca converge finit, +1 daca converge infinit
  int isIn(CComplex &x) {
    int rez = 0;
    CComplex z0, z1;
    z0 = c;

    for (int i = 1; i < nriter; i++) {
      z1 = z0 * z0 + x;
      if (z1.getModul() > modmax) {
        rez = 1;
        break;
      }
      z0 = z1;
    }
    return rez;
  }

  void display(double xmin, double ymin, double xmax, double ymax) {
    glPushMatrix();
    glLoadIdentity();
    glEnable(GL_COLOR_MATERIAL);
    glBegin(GL_POINTS); 
    glColor3f(1.0, 0.1, 0.1);
    for(double x = xmin; x <= xmax; x+=MX_JF)
      for(double y = ymin; y <= ymax; y+=MY_JF)  {
        CComplex z(x, y);
        int r = isIn(z);
        if (r == 0) {
          glVertex2f(x/2,y/2);
        }
      }
    fprintf(stdout, "STOP\n");
    glEnd();
    glPopMatrix();
  }

private:
  CComplex c;
  int nriter;
  double modmax;

};

class CMandelbrotNotInSetColoring {
public:
  CMandelbrotNotInSetColoring() {
    nriter = NRITER_JF;
    modmax = 2;
  }
  
  ~CMandelbrotNotInSetColoring() {}

  double getmodmax() {return modmax;}
  void setnriter(int v) {assert(v <= NRITER_JF); nriter = v;}
  int getnriter() {return nriter;}


  // testeaza daca x apartine multimii Julia-Fatou Jc
  // returneaza 0 daca apartine, -1 daca converge finit, +1 daca converge infinit
  double isIn(CComplex &x) {
    CComplex z0, z1;
    z0 = c;

    for (int i = 0; i < nriter; i++) {
      z1 = z0 * z0 + x;
      if (z1.getModul() > modmax) {
        return 1.0 * i;
      }
      z0 = z1;
    }
    return 1.0 * (nriter - 1);
  }

  void display(double xmin, double ymin, double xmax, double ymax) {
    glPushMatrix();
    glLoadIdentity();
    glBegin(GL_POINTS); 
    for(double x = xmin; x <= xmax; x+=MX_JF)
      for(double y = ymin; y <= ymax; y+=MY_JF)  {
        CComplex z(x, y);
        double r = isIn(z);
        glColor3f(r/nriter, 0.1, 0.1);
        glVertex3d(x/2,y/2,0);    
      }
    fprintf(stdout, "STOP\n");
    glEnd();
    glPopMatrix();
  }

private:
  CComplex c;
  int nriter;
  double modmax;
};

void Display4() {
  CMandelbrot mundelbrot;
  mundelbrot.setnriter(20);
  mundelbrot.display(-2, -2, 2, 2);
}

void Display5() {
  CMandelbrotNotInSetColoring mundelbrot;

  glColor3f(1.0, 0.1, 0.1);
  mundelbrot.setnriter(20);
  mundelbrot.display(-2, -2, 2, 2);
}

void Init(void) {

   glClearColor(1.0,1.0,1.0,1.0);

   glLineWidth(1);

   glPointSize(3);

   glPolygonMode(GL_FRONT, GL_LINE);
}

void Display(void) 
{
  switch(prevKey) 
  {
    case '1':
      glClear(GL_COLOR_BUFFER_BIT);
      Display1();
      break;
    case '2':
      glClear(GL_COLOR_BUFFER_BIT);
      Display2();
      break;
    case '3':
      glClear(GL_COLOR_BUFFER_BIT);
      Display3();
      break;
    case '4':
      glClear(GL_COLOR_BUFFER_BIT);
      Display4();
      break;
    case '5':
      glClear(GL_COLOR_BUFFER_BIT);
      Display5();
      break;
    default:
      break;
  }

  glFlush();
}

void Reshape(int w, int h) 
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

void KeyboardFunc(unsigned char key, int x, int y) 
{
   prevKey = key;
   if (key == 27) // escape
      exit(0);
   glutPostRedisplay();
}

void MouseFunc(int button, int state, int x, int y) 
{
}

int main(int argc, char** argv) 
{
  glutInit(&argc, argv);

  glutInitWindowSize(dim, dim);

  glutInitWindowPosition(100, 100);

  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);

  glutCreateWindow (argv[0]);

  Init();

  glutReshapeFunc(Reshape);

  glutKeyboardFunc(KeyboardFunc);

  glutMouseFunc(MouseFunc);

  glutDisplayFunc(Display);

  glutMainLoop();

  return 0;
}


