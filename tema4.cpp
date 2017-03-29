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
unsigned char last_prevKey;
int nivel = 0;



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
    
    void deseneaza_dreapta(pair<int, int> x, pair<int,int> y) {
        glColor3f(0.1, 0.1, 0.1);
        glLineWidth(3);
        glBegin(GL_LINES);
        glVertex2d(-0.8+distanta_linie*x.first, -0.8+distanta_coloana*x.second);
        glVertex2d(-0.8+distanta_linie*y.first, -0.8+distanta_coloana*y.second);
        glEnd();
    }
    
private:
    int linii, coloane;
    float distanta_linie, distanta_coloana;
};


vector<pair<int, int>> AfisareSegmentDreapta3(int x0, int y0, int xn, int yn) {
    double m = float(yn - y0)/(xn-x0);
    cout << m << endl;
    vector<pair<int,int>> M(100);
    
    if((x0 < xn) && (y0 <  yn)) {
        int dx = xn - x0;
        int dy = yn - y0;

        int d = 2*dy - dx;
        int dE = 2*dy;
        int dNE = 2*(dy-dx);

        int x = x0, y = y0;


        M.push_back(make_pair(x,y));
        while(x < xn) {
            if(d<0) {
                d += dE;
                x++;
            }else {
                d += dNE;
                x++;
                y++;
            }
            //cout << x << " " << y << endl;
            M.push_back(make_pair(x,y));
        }
    } else {
        //
    }
    return M;

}

void Display1() {
    pair<int, int> x0 = make_pair(0,0);
    pair<int, int> y0 = make_pair(15,7);
    
//    pair<int, int> x0 = make_pair(0,15);
//    pair<int, int> y0 = make_pair(15,10);
    
    GrilaCarteziana gc(15, 15);
    glPushMatrix();
    glLoadIdentity();
    gc.deseneaza();
    gc.adauga_pixel(x0.first, x0.second);
    gc.adauga_pixel(y0.first, y0.second);
    gc.deseneaza_dreapta(x0, y0);
    
    
    vector<pair<int, int>> M = AfisareSegmentDreapta3(x0.first, x0.second, y0.first, y0.second);
    for(vector<pair<int,int>>::iterator it = M.begin(); it != M.end(); ++it) {
        gc.adauga_pixel(it->first, it->second);
    }
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
    
    switch(prevKey)
    {
        case '1':
            glClear(GL_COLOR_BUFFER_BIT);
            Display1();
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



