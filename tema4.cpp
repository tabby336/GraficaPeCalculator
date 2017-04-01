#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <float.h>
#include  <iostream>
#include <vector>

using namespace std;

#include <GLUT/glut.h>

// dimensiunea ferestrei in pixeli
#define dim 600
#define NRITER_JF 5000
#define MODMAX_JF 10000000

#define MX_JF 0.005
#define MY_JF 0.005

unsigned char prevKey;

class GrilaCarteziana {
public:
    GrilaCarteziana(int l, int c) {
        this->linii = l ;
        this->coloane = c;
        this->distanta_linie = 1.6/linii;
        this->distanta_coloana = 1.6/coloane;
    }
    
    void deseneaza() {
        glColor3f(1.0, 0.1, 0.1);
        
        for(int i = 0; i <= linii; ++i) {
            glBegin(GL_LINES);
            glVertex2d(-0.8, -0.8+distanta_linie*i);
            glVertex2d(0.8, -0.8+distanta_linie*i);
            glEnd();
        }
        
        for(int i = 0; i <= coloane; ++i) {
            glBegin(GL_LINES);
            glVertex2d(-0.8+distanta_linie*i, -0.8);
            glVertex2d(-0.8+distanta_linie*i, 0.8);
            glEnd();
        }
    }
    
    void adauga_pixel(int i, int j) {
        if(i > this->linii || i < 0 )
            return;
        if(j > this->coloane || j < 0)
            return;
        
        float _i = -0.8 + distanta_linie*i;
        float _j = -0.8 + distanta_coloana*j;
        
        float radius = 0.035;
        const float DEG2RAD = 3.14159/180;
        glColor3f(0.1, 1.0, 0.1);
        glPolygonMode(GL_FRONT, GL_FILL);
        glBegin(GL_POLYGON);
        
        for (int i=0; i <= 360; i++) {
            float degInRad = i*DEG2RAD;
            glVertex2f(cos(degInRad)*radius + _i,sin(degInRad)*radius + _j);
        }
        
        glEnd();
    }
    
    void print_grosime_verticala(int i,int j,int grosime) {
        int upper = j+grosime/2; ;
        if(!(grosime%2)) {
            upper--;
        }
        for(int k = j - grosime/2; k <= upper; ++k) {
            adauga_pixel(i, k);
        }
    }
    
    void print_grosime_orizontala(int i, int j, int grosime) {
        int upper = i+grosime/2; ;
        if(!(grosime%2)) {
            upper--;
        }
        for(int k = i - grosime/2; k <= upper; ++k) {
            adauga_pixel(k, j);
        }

        
    }
    
    void deseneaza_dreapta(pair<int, int> x, pair<int,int> y) {
        glColor3f(0.1, 0.1, 0.1);
        glLineWidth(3);
        glBegin(GL_LINES);
        glVertex2d(-0.8+distanta_linie*x.first, -0.8+distanta_coloana*x.second);
        glVertex2d(-0.8+distanta_linie*y.first, -0.8+distanta_coloana*y.second);
        glEnd();
    }
    
    
    void AfisareSegmentDreapta3(int x0, int y0, int xn, int yn, int grosime = 1) {
        // ordonare dupa x
        int aux;
        if (x0 > xn) {
            aux = x0;
            x0 = xn;
            xn = aux;
            
            aux = y0;
            y0= yn;
            yn = aux;
        }

        int x = x0, y = y0;
        
        // panta
        double m = float(yn - y0)/(xn-x0);
        int dx = xn - x0;
        
        if(y0 <  yn) {
            int dy = yn - y0;
            
            int dE = 2*dy;
            int dNE = 2*(dy-dx);
            int dN = -2*dx;
            
            if( fabs(m) < 1) { //panta < 1
                int d = 2*dy - dx;
                print_grosime_verticala(x, y, grosime);
                while(x < xn) {
                    if(d<0) {
                        d += dE;
                        x++;
                    }else {
                        d += dNE;
                        x++;
                        y++;
                    }
                    print_grosime_verticala(x,y,grosime);
                }
            } else { //panta > 1
                int d = 2*dx - dy;
                print_grosime_orizontala(x,y,grosime);
                while(y < yn) {
                    if(d<0) {
                        d += dNE;
                        y++;
                        x++;
                    } else {
                        d += dN;
                        y++;
                        
                    }
                    print_grosime_orizontala(x,y,grosime);
                }
            }
        } else { // y0 > yn
            
            
            int dy = y0 - yn;
            
            int dE = 2*dy;
            int dNE = 2*(dy-dx);
            int dS = 2*dx;
            int dSE = -dNE;

            if( fabs(m) < 1) { //panta < 1
                int d = 2*dy - dx;
                print_grosime_verticala(x,y, grosime);
                while ( x < xn ) {
                    if(d < 0) {
                        d += dE;
                        x++;
                    } else {
                        d += dNE;
                        y--;
                        x++;
                    }
                    print_grosime_verticala(x,y, grosime);
                }
            } else { //panta < 1
                int dx = xn - x0;
                int dy = y0 - yn;
                int d = 2*dx - dy;
                
                print_grosime_orizontala(x,y,grosime);
                while ( y > yn ) {
                    if(d < 0) {
                        d += dS;
                        y--;
                    } else {
                        d += dSE;
                        x++;
                        y--;
                    }
                    print_grosime_orizontala(x,y,grosime);
                }
            }
        }
    }
    

    
private:
    int linii, coloane;
    float distanta_linie, distanta_coloana;
};

// abs(panta < 1) si x0 < xn si y0 < yn
void Display1() {
    pair<int, int> x0 = make_pair(0,0);
    pair<int, int> y0 = make_pair(15,7);
    int grosime = 4;
    
    GrilaCarteziana gc(15, 15);
    glPushMatrix();
    glLoadIdentity();
    gc.deseneaza();
    gc.deseneaza_dreapta(x0, y0);
    gc.AfisareSegmentDreapta3(x0.first, x0.second, y0.first, y0.second, grosime);

    glPopMatrix();
}

// abs(panta > 1) si x0 < xn si y0 < yn
void Display2() {
    pair<int, int> x0 = make_pair(0,0);
    pair<int, int> y0 = make_pair(7,15);
    int grosime = 2;
    
    GrilaCarteziana gc(15, 15);
    glPushMatrix();
    glLoadIdentity();
    gc.deseneaza();
    gc.AfisareSegmentDreapta3(x0.first, x0.second, y0.first, y0.second, grosime);

    
    gc.deseneaza_dreapta(x0, y0);
    glPopMatrix();
}

void Display3() {
    pair<int, int> x0 = make_pair(0,15);
    pair<int, int> y0 = make_pair(15,10);
    int grosime = 3;
    
    GrilaCarteziana gc(15, 15);
    glPushMatrix();
    glLoadIdentity();
    gc.deseneaza();
//    gc.print_grosime(x0.first, x0.second,grosime);
    gc.AfisareSegmentDreapta3(x0.first, x0.second, y0.first, y0.second, grosime);

    gc.deseneaza_dreapta(x0, y0);
    glPopMatrix();
}


void Display4() {
    pair<int, int> x0 = make_pair(0,15);
    pair<int, int> y0 = make_pair(6,0);
    
    GrilaCarteziana gc(15, 15);
    glPushMatrix();
    glLoadIdentity();
    gc.deseneaza();
    gc.AfisareSegmentDreapta3(x0.first, x0.second, y0.first, y0.second);
    
    gc.deseneaza_dreapta(x0, y0);
    glPopMatrix();
}

void Init(void) {
    
    glClearColor(1.0,1.0,1.0,1.0);
    
    glLineWidth(1);
    
    glPointSize(3);
    
    glPolygonMode(GL_FRONT, GL_LINE);
}

void Display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    switch(prevKey)
    {
        case '1':
            Display1();
            break;
        case '2':
            Display2();
            break;
        case '3':
            Display3();
            break;
        case '4':
            Display4();
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