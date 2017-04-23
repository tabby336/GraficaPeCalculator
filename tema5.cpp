#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <float.h>
#include <iostream>
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

struct muchie {
    pair<int, int> vi;
    pair<int, int> vf;
};

struct poligon {
    int len = 0;
    muchie muchii[100];
};

struct intersectie {
    int ymax;
    double xmin;
    double ratia;
};

struct intersectii {
    int len = 0;
    intersectie i[100];
};


struct ET {
    intersectii intersectii[100];
};

class GrilaCarteziana {
public:
    GrilaCarteziana(int l, int c, bool pc=true) {
        this->linii = l ;
        this->coloane = c;
        this->distanta_linie = 1.6/linii;
        this->distanta_coloana = 1.6/coloane;
        this->primul_cadran = pc;
        
        if (!primul_cadran) {
            this->x_origine = -0.8 + linii/2 * distanta_linie;
            this->y_origine = -0.8 + coloane/2 * distanta_coloana;
        } else {
            this->x_origine = -0.8;
            this->y_origine = -0.8;
        }
    }
    
    void deseneaza_grila() {
        glLineWidth(1);
        glColor3f(0.1, 0.1, 0.1);
        
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
        if(!pixel_valid(i,j)) {
            //cout << pixel_valid(i, j) << endl;
            return;
        }
        
        float _i = x_origine + distanta_linie*i;
        float _j = y_origine + distanta_coloana*j;
        
        float radius = 0.02;
        const float DEG2RAD = 3.14159/180;
        glColor3f(0.1, 0.1, 0.1);
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
    
    void AfisarePuncteCerc3 (int x, int y) {
//        adauga_pixel(x, y); // octanul 2
//        adauga_pixel(-x, -y);
//        adauga_pixel(-x, y);
//        adauga_pixel(x, -y);
        //if ( x != y ) {
            //adauga_pixel(y, x);
        print_grosime_orizontala(y,x,3); //octanul 1
//            adauga_pixel(-y, -x);
//            adauga_pixel(-y, x);
//            adauga_pixel(y, -x);
        //}
    }
    
    void deseneaza_cerc(int radius) {
        deseneaza_elipsa(radius, radius);
    }
    
    void deseneaza_elipsa(int razaX, int razaY) {
        const float DEG2RAD = 3.14159/180;
        glColor3f(1.0, 0.1, 0.1);
        glLineWidth(4);
        glBegin(GL_LINE_STRIP);
        
        int count = 0;
        for (int i=0; i <= 360; i++) {
            float degInRad = i*DEG2RAD;
            double _i = cos(degInRad)*distanta_linie*razaX;
            double _j = sin(degInRad)*distanta_coloana*razaY;
            if(primul_cadran && (_i < 0 || _j < 0)) {
                glEnd();
                return;
            }
            glVertex2f(_i + x_origine, _j + y_origine);
        }
        glEnd();
    }
    
    void AfisareCerc4(int raza, int grosime) {
        int x = 0, y = raza;
        int d = 1 - raza;
        int dE = 3, dSE = -2*raza + 5;
        
        AfisarePuncteCerc3(x, y);
        while ( y > x ) {
            if(d<0) {
                d += dE;
                dE += 2;
                dSE += 2;
                
            } else {
                d += dSE;
                dE += 2;
                dSE += 4;
                y--;
            }
            x++;
            AfisarePuncteCerc3(x, y);
        }
        
    }
    
    
    void ColorarePoligon() {
        glBegin(GL_POLYGON_MODE);
        poligon p = citire_poligon();
        for(int i = 0; i < p.len; ++i) {
            //adauga_pixel(poligon[i].first, poligon[i].second);
            deseneaza_dreapta(p.muchii[i].vi, p.muchii[i].vf);
        }
        
        pair<int, int> DOM_SCAN(99,0);

        for(int i = 0; i < p.len; ++i) {
            if(p.muchii[i].vf.second < DOM_SCAN.first )
                DOM_SCAN.first = p.muchii[i].vf.second;
            if(p.muchii[i].vi.second < DOM_SCAN.first )
                DOM_SCAN.first = p.muchii[i].vi.second;
            
            if(p.muchii[i].vf.second > DOM_SCAN.second )
                DOM_SCAN.second = p.muchii[i].vf.second;
            if(p.muchii[i].vi.second > DOM_SCAN.second )
                DOM_SCAN.second = p.muchii[i].vi.second;
        }
        
        ET et = initializareET(p,DOM_SCAN);
        ET ssm = calculssm(p, et,DOM_SCAN);
        
        for(int i = DOM_SCAN.first; i <= DOM_SCAN.second; ++i) {
            if(ssm.intersectii[i].len) {
                cout << "Intersectii de: " << i << " cu len: " << ssm.intersectii[i].len << endl;
                for (int j = 0; j < ssm.intersectii[i].len; ++j) {
                    cout << ssm.intersectii[i].i[j].ratia << endl;
                    cout << ssm.intersectii[i].i[j].xmin << endl;
                    cout << ssm.intersectii[i].i[j].ymax << endl;
                    //adauga_pixel(ceil(ssm.intersectii[i].i[j].xmin), ssm.intersectii[i].i[j].ymax);
                    adauga_pixel(ceil(ssm.intersectii[i].i[j].xmin), i);
                    cout << endl;
                }
                cout << endl;
            }
        }
    
        glEnd();
    }
    
    void umpleteelipsa(int a, int b) {
        int xi = 0;
        int x = 0;
        int y = b;
        double fxpyp = 0.0;
        double deltaE, deltaSE, deltaS;
        
        //regiunea 1
        for(int i = 0; i <= y; ++i) {
            adauga_pixel(-x, -i);
        }
        while(a*a*(y-0.5) >= b*b*(x+1)) {
//            cout << y << " " << y-0.5 << " " << a*a*(y-0.5) << endl;
//            cout << x << " " << x+1 << " " << b*b*(x+1) << endl;
            deltaE = b*b*(2*x+1);
            deltaSE = b*b*(2*x+1) + a*a*(-2*y+1);
            if(fxpyp + deltaE <= 0.0) {
                cout << "E: ";
                // E este in interior
                fxpyp += deltaE;
                x++;
                // ssm.setare
                cout << x << " " << y << endl;
            }
            else if(fxpyp + deltaSE <= 0.0) {
                // SE este in interior
                cout << "SE: ";
                fxpyp += deltaSE;
                x++;
                y--;
                // ssm.adaugare
                cout << x << " " << y << endl;
            }
            for(int i = 0; i <= y; ++i) {
                adauga_pixel(-x, -i);
            }
            //adauga_pixel(-x, -y);
        }
        
        // regiunea 2
        while(y > 0) {
            deltaSE = b*b*(2*x+1) + a*a*(-2*y+1);
            deltaS = a*a*(-2*y+1);
            if(fxpyp + deltaSE <= 0.0) {
                // SE este in interior
                fxpyp += deltaSE;
                x++;
                y--;
            } else {
                fxpyp += deltaS;
                y--;
            }
            for(int i = 0; i <= y; ++i) {
                adauga_pixel(-x, -i);
            }
        }
    }
    
private:
    int linii, coloane;
    double x_origine, y_origine;
    float distanta_linie, distanta_coloana;
    bool primul_cadran;
    
    poligon citire_poligon() {
        FILE *in = fopen("poligon.txt", "r");
        int num = 0, x, y;
        fscanf(in, "%d", &num);
        vector<pair<int,int>> poligon;
        for (int i = 0; i < num; ++i) {
            fscanf(in, "%d%d", &x, &y);
            poligon.push_back(make_pair(x, y));
        }
        poligon.push_back(poligon[0]);
        
        
        struct poligon p;
        p.len = num;
        for (int i = 0; i < num; ++i) {
            p.muchii[i].vi = poligon[i];
            p.muchii[i].vf = poligon[i+1];
        }
        return p;
    }
    
    ET initializareET(poligon p, pair<int,int> DOM_SCAN) {
        
        ET et;
        for(int i = 0; i < 99; ++i) {
            et.intersectii[i].len = 0;
        }
        bool change;
        
        for (int i = 0; i < p.len; i++) {
            // pentru fiecare muchie din poligon
//            cout << "Muchia: " << endl;
//            cout << p.muchii[i].vi.first << " " << p.muchii[i].vi.second << endl;
//            cout << p.muchii[i].vf.first << " " << p.muchii[i].vf.second << endl;
            
            
            if ( p.muchii[i].vi.second != p.muchii[i].vf.second) {
                // ... care nu este orizontala
                
                // ymin si ymax
                int ym = p.muchii[i].vi.second < p.muchii[i].vf.second ? p.muchii[i].vi.second : p.muchii[i].vf.second;
                int yM = p.muchii[i].vi.second > p.muchii[i].vf.second ? p.muchii[i].vi.second : p.muchii[i].vf.second;
                //cout << "ym si yM: " << ym << " " << yM << endl;
                
                // xmin si xmax
                int xm = (ym == p.muchii[i].vi.second) ? p.muchii[i].vi.first : p.muchii[i].vf.first;
                int xM = (yM == p.muchii[i].vi.second) ? p.muchii[i].vi.first : p.muchii[i].vf.first;
                //cout << "xm si xM:" << xm << " " << xM << endl;
                
                // et ??
                int len = et.intersectii[ym].len;
                et.intersectii[ym].i[len].ymax = yM;
                et.intersectii[ym].i[len].xmin = xm;
                et.intersectii[ym].i[len].ratia = (double)(xm - xM)/(ym - yM);
                et.intersectii[ym].len += 1;
            }
            
            /*
             sortarea in ordinea crescatoare conform cu 
             xm a intersectiilor dreptei de scanare cu
             muchiile poligonului
             */
            for (int i = DOM_SCAN.first; i <= DOM_SCAN.second; ++i) {
                do {
                    change = false;
                    if(et.intersectii[i].len == 0) {
                        break;
                    }
                    for(int j = 0; j+1 < et.intersectii[i].len; ++j) {
                        if(et.intersectii[i].i[j].xmin > et.intersectii[i].i[j+1].xmin) {
                            //swap
                            int auxi;
                            double auxd;
                            
                            // swap xmin
                            auxd = et.intersectii[i].i[j].xmin;
                            et.intersectii[i].i[j].xmin = et.intersectii[i].i[j+1].xmin;
                            et.intersectii[i].i[j+1].xmin = auxd;
                            
                            // swap ratia
                            auxd = et.intersectii[i].i[j].ratia;
                            et.intersectii[i].i[j].ratia = et.intersectii[i].i[j+1].ratia;
                            et.intersectii[i].i[j+1].ratia = auxd;
                            
                            // swap ymax
                            auxi = et.intersectii[i].i[j].ymax;
                            et.intersectii[i].i[j].ymax = et.intersectii[i].i[j+1].ymax;
                            et.intersectii[i].i[j+1].ymax = auxi;
                            
                            change = true;
                        }
                    }
                } while(change);
            }
            
        }
        return et;
    }
    
    bool pixel_valid(int i, int j) {
        if(primul_cadran) {
            return !(i > linii || i < 0 || j > coloane || j < 0 );
        } else {
            int xmin = -linii/2;
            int xmax = linii - xmin;
            
            int ymin = -coloane/2;
            int ymax = coloane - ymin;
        
            return !(i > xmax || i < xmin || j > ymax || j < ymin);
        }
    }
    
    ET calculssm(poligon p, ET et, pair<int,int> DOM_SCAN) {
        ET ssm;
        intersectii aet;
        int y, k;
        
        // lui y i se atribuie o valoare care nu este in DOM_SCAN
        y = -1;
        // se determina y = min{ y' | et(y') != \emptyset}
        for(int i = DOM_SCAN.first; i <= DOM_SCAN.second; i++) {
            if(et.intersectii[i].len != 0) {
                y = i;
                break;
            }
        }
        if(y == -1) {
            return ;
        }
        
        do {
            for(int i = aet.len; i < aet.len + et.intersectii[y].len; i++) {
                //cout << i << endl;
                aet.i[i].ratia = et.intersectii[y].i[i - aet.len].ratia;
                aet.i[i].xmin = et.intersectii[y].i[i - aet.len].xmin;
                aet.i[i].ymax = et.intersectii[y].i[i - aet.len].ymax;
            }
            aet.len += et.intersectii[y].len;
            
            // eliminarea varfurilor cu ymax == y
            for(int i = 0; i < aet.len; ++i) {
                if(aet.i[i].ymax == y) {
                    for(int j = i; j+1 < aet.len; ++j) {
                        aet.i[j].ratia = aet.i[j+1].ratia;
                        aet.i[j].xmin = aet.i[j+1].xmin;
                        aet.i[j].ymax = aet.i[j+1].ymax;
                    }
                    aet.len -= 1;
                }
            }
            
            // sortarea aet cf. cheii xmin
            int k = aet.len;
            while (k >= 2) {
                for(int i = 0; i+1 < k; ++i) {
                    if(aet.i[i].xmin > aet.i[i+1].xmin) {
                        double auxd;
                        int auxi;
                        
                        // swap ratia
                        auxd = aet.i[i].ratia;
                        aet.i[i].ratia = aet.i[i+1].ratia;
                        aet.i[i+1].ratia = auxi;
                        
                        // swap xmin
                        auxd = aet.i[i].xmin;
                        aet.i[i].xmin = aet.i[i+1].xmin;
                        aet.i[i+1].xmin = auxd;
                        
                        
                        // swap ymax
                        auxi = aet.i[i].ymax;
                        aet.i[i].ymax = aet.i[i+1].ymax;
                        aet.i[i+1].ymax = auxi;
                        
                    }
                }
                --k;
            }
            
            // copy aet in ssm
            ssm.intersectii[y].len = aet.len;
            for(int i = 0; i < aet.len; ++i) {
                ssm.intersectii[y].i[i].ratia = aet.i[i].ratia;
                ssm.intersectii[y].i[i].xmin = aet.i[i].xmin;
                ssm.intersectii[y].i[i].ymax = aet.i[i].ymax;
            }
            
            ++y;
            
            // reactualizarea punctelor de intersectie pentru noua dreapta de scanare
            for(int i = 0; i < aet.len; ++i) {
                if(aet.i[i].ratia != 0) {
                    aet.i[i].xmin += aet.i[i].ratia;
                    //adauga_pixel(aet.i[i].xmin, aet.i[i].ymax);
                }
            }
            
            
        }while((aet.len || et.intersectii[y].len) && y <= DOM_SCAN.second);
        
        return ssm;
    }
};


void Display1() {
    int grosime = 3;
    
    GrilaCarteziana gc(15, 15);
    glPushMatrix();
    glLoadIdentity();
    gc.deseneaza_grila();
    gc.adauga_pixel(0, 0);
    gc.deseneaza_cerc(13);
    gc.adauga_pixel(-1, -1);
    gc.AfisareCerc4(13, grosime);
    
    glPopMatrix();
}

void Display2() {
    int grosime = 3;
    
    GrilaCarteziana gc(28, 28, false);
    glPushMatrix();
    glLoadIdentity();
    gc.deseneaza_grila();
    gc.adauga_pixel(0, 0);
    gc.deseneaza_elipsa(14, 8);
    gc.umpleteelipsa(14, 8);
    glPopMatrix();
}


void Display3() {
    GrilaCarteziana gc(15, 15, true);
    glPushMatrix();
    glLoadIdentity();
    gc.deseneaza_grila();
    gc.ColorarePoligon();
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



