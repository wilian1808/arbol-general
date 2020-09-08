// llamamos a las librerias que utilizaremos
#include <windows.h>
#include <windowsx.h>
#include <algorithm>
#include <math.h>
#include <iostream>
#include <vector>
#include <commctrl.h>
#include <stdio.h>
#include "resource.h"

using namespace std;

// declaramos variables de ambito global
HINSTANCE hInst;
int pxi,pyi,pxf,pyf;

// creamos una funcion que no retorna nada llamada 'SeTPixel2'
// esta funcion recibe 4 parametros
// el primer parametro es el 'hdc' el segundo y tercero los valores de 'x' y 'y'
// y el ultimo para metro es el 'color'
// esta funcion ayuda a calcular los pixeles que usaremos para dibujar el circulo
void SetPixel2(HDC hdc,int x,int y,COLORREF Color)
{
    // creamos una condicional donde comparamos los valors 'x' y 'y'
    // con los valores globales de 'pxi','pyi','pxf','pyf'
    if(x>pxi && x<pxf && y>pyi && y<pyf)
    {
        // si la condicional se cumple llamamos recursivamente a la funcion 'SetPixel'
        // y le pasamos los valores en los parametros
        SetPixel(hdc, x, y, Color);
    }
}

// creamos una funcion que no retorna nada llamada 'Draw8Points'
// esta funcion recibe 6 parametros la primera es el valor del 'hdc'
// la segunda y tercera son los valors de 'xc' y 'yc' el cuarto el valor de 'a'
// y por ultimo parametro tenemos el valor del 'color'
// esta funcion la usaremos dibujar un circulo con ayuda del algoritmo
// circulo de Bresenham
void Draw8Points(HDC hdc,int xc,int yc, int a, int b,COLORREF Color)
{   
    // dentro la funcion llamamos 8 veces a la funcion 'SetPixel'
    // y le pasamos los valores de los parametros correspondientes
    SetPixel2(hdc, xc+a, yc+b, Color);
    SetPixel2(hdc, xc-a, yc+b, Color);
    SetPixel2(hdc, xc-a, yc-b, Color);
    SetPixel2(hdc, xc+a, yc-b, Color);
    SetPixel2(hdc, xc+b, yc+a, Color);
    SetPixel2(hdc, xc-b, yc+a, Color);
    SetPixel2(hdc, xc-b, yc-a, Color);
    SetPixel2(hdc, xc+b, yc-a, Color);
}

// creamos la funcion que no retorna nada llamada 'CircleBresenham'
// esta funcion recibe 6 parametros el primero es el valor del 'hdc'
// el segundo y el tercero son 'xc' y 'yc' el cuarto el valor de 'R'
// el quinto es valor de 'rrr' y por ultimo el valor del 'color'
// esta funcion hace uso del algoritmo llamado: circulo de Bresenham
// que nos ayuda a dibujar un circulo
void CircleBresenham(HDC hdc,int xc,int yc, int R,int rrr, COLORREF color)
{   
    // declaramos variables y le asignamos los valores de los parametros
    int x=0,y=R;
    int d=1-R;

    // llamamos a la funcion 'Draw8Points' 
    // y le pasamos los valores en sus parametros
    Draw8Points(hdc,xc,yc,x,y,color);

    // creamos un ciclo while que se ejecutara cuando x sea menor que y
    while(x<y)
    {   
        // creamos una condicional que compara si el valor de 'd' es menor que 0
        // si se cumpla la condicional incrementamos el valor de 'd'
        // segun el resultado de 2*x+2
        if(d<0)
            d+=2*x+2;
        else
        {   
            // si la condicion no se cumple entonces incrementamos el valor de 'd'
            // segun el resultado de: 2*(x-y)+5 y restamos el valor de 'y' segund 'rrr' 
            d+=2*(x-y)+5;
            y-=rrr;
        }
        // incrementamos el valor de 'x' segun 'rrr'
        x+=rrr;
        // y llamamos a la funcion 'Draw8Points' pasandole los valores de sus parametros
        Draw8Points(hdc,xc,yc,x,y,color);
    }
}

// creamos la clase 'Punto'
class Punto
{
public:
    // definimos las propiedades 'x' y 'y' de tipo float
    float x,y;
    // definimos el constructor de la clase 'Punto' que recibe
    // dos parametros de tipo float '_x' y '_y'
    Punto(float _x=0,float _y=0)
    {   
        // asignampos los valores de los parametros a las propiedades
        x=_x;
        y=_y;
    }
};

// creamos la clase 'Nodo'
class Nodo
{
public:
    // definimos las propiedades de la clase
    // 'P' es una propieda de tipo 'Punto'
    Punto P;
    // definimos 'V' que esun vector de 'Nodos'
    vector < Nodo * > V;
    // creamos el constructor de la clase 'Nodo' que recibe como parametro
    // un punto con los valores de (0, 0)
    Nodo(Punto PP=Punto(0,0))
    {
        // asignamos el valor del parametro al valor de nuestra propiedad 'P'
        P=PP;
    }
};

// creamos una funcion que retorna un 'booleano' llamado 'sortbysec'
bool sortbysec(const pair<float,Punto> &a, const pair<float,Punto> &b)
{
    // esta funcion retorna true o false segun si a.first es menor que b.first
    return (a.first< b.first);
}

// creamos la clase 'VoronoiTree'
class VoronoiTree
{   
public:
    // declaramos las propiedades de la clase 'VoronoiTree'
    // primero una 'Raiz' de tipo Nodo
    Nodo *Raiz;
    // el segundo un 'Hi' de tipo entero
    int Hi;
    // y dos vevtores 'Ps' y 'C' de tipo Punto y DWORD
    vector <Punto> Ps;
    vector<DWORD> C;

    // definimos el constructor de la clase 'VoronoiTree'
    VoronoiTree()
    {   //Raiz=new Nodo(Punto(-1000,-1000));
        Hi=2;
    }

    void CrearColores()
    {
        C.clear();
        for (int i=0;i<Ps.size();i++)
        {   DWORD Col = RGB(rand() % 50 + 200, rand() % 100 + 150, rand() % 100 + 150);
            C.push_back(Col);
        }
    }

    static int DistanceSqrd(const Punto& point, int x, int y)
    {
        int xd = x - point.x;
        int yd = y - point.y;
        return (xd * xd) + (yd * yd);
    }
    /*
    void CrearSitios(HDC hdc)
    {   int w = pxf, h = pyf, d;
        for (int hh = pyi; hh < h; hh++)
        {   for (int ww = pxi; ww < w; ww++)
            {   int ind = -1, dist = INT_MAX;
                for (int it = 0; it < Ps.size(); it++)
                {   const Punto& p = Ps[it];
                    d = DistanceSqrd(p, ww, hh);
                    if (d < dist) {
                    dist = d;
                    ind = it;
                }
            }
            if (ind > -1)
                SetPixel(hdc, ww, hh, C[ind]);
            else
                cout<<"Increible que haya pasado"; // should never happen!
            }
        }
    }*/

    float distP(Punto P1,Punto P2)
    {
        return (sqrt((P1.x-P2.x)*(P1.x-P2.x)+(P1.y-P2.y)*(P1.y-P2.y)));
    }

    void Mostrar(HDC hdc)
    {
        for(int i=0;i<Ps.size();i++)
            CircleBresenham(hdc,Ps[i].x,Ps[i].y,2,1,RGB(255,0,0));
    }

    void Crear(HDC hdc,vector <Punto> PP)
    {
        if(PP.size()>0)
        {
            int d=PuntoCentral(hdc,PP);
            Punto R=PP[d];
            PP[d]=PP[0];
            PP[0]=R;
            Raiz=new Nodo(PP[0]);
            PP.erase(PP.begin(),PP.begin()+1);
            Ins(hdc,Raiz,PP,Hi); //AQUI
            CircleBresenham(hdc,Raiz->P.x,Raiz->P.y,4,1,RGB(0,0,255));
        }
    }

    void Ins(HDC hdc,Nodo *&R,vector <Punto> PP,int D)
    {
        if(PP.size()>D)
        {
            vector < int > May(D,0);
            vector < vector <Punto> > X(D);
            vector < pair <float, Punto> > Pss;
            for(int i=0;i<PP.size();i++)
            {
                Pss.push_back(pair <float, Punto>(distP(R->P,PP[i]),PP[i]));
            }

            sort(Pss.begin(), Pss.end(),sortbysec);
            for(int i=D;i<Pss.size();i++)
            {
                int min=0;
                for(int j=1;j<D;j++)
                {
                    if(distP(Pss[i].second,Pss[j].second)<distP(Pss[i].second,Pss[min].second))
                        min=j;
                }
                X[min].push_back(Pss[i].second);
            }

            for(int i=0;i<D;i++)
            {
                R->V.push_back(new Nodo(Pss[i].second));
            }

            for(int i=0;i<R->V.size();i++)
            {
                Ins(hdc,R->V[i],X[i],D);
            }
        }
        else if(PP.size()>0)
        {
            for(int i=0;i<PP.size();i++)
                R->V.push_back(new Nodo(PP[i]));
        }
    }

    int PuntoCentral(HDC hdc,vector <Punto> PP)
    {
        Punto M;
        int N=PP.size();
        if(N>0)
        {
            for(int i=0;i<N;i++)
            {
                M.x+=PP[i].x/N;
                M.y+=PP[i].y/N;
            }
            int d=0;
            for(int i=1;i<N;i++)
            {
                if(distP(M,PP[i])<distP(M,PP[d]))
                    d=i;
            }
            //CircleBresenham(hdc,Ps[d].x,Ps[d].y,4,1,RGB(0,0,255));
            return d;
        }
        return -1;
    }

    void MostrarZ(HDC hdc)
    {
        if (Raiz!=NULL)
        {
            MosZ(hdc,Raiz);
        }
    }

    void MosZ(HDC hdc,Nodo *R)
    {
        if (R!=NULL)
        {
            if(R->V.size()>0)
            {
                for(int i=0;i<R->V.size();i++)
                {   //cout<<Raiz->P.x<<" - "<<Raiz->P.y<<endl;
                    MoveToEx(hdc,R->P.x,R->P.y,NULL);
                    LineTo(hdc,R->V[i]->P.x,R->V[i]->P.y);
                    MosZ(hdc,R->V[i]);
                }
            }
        }
    }
};

VoronoiTree V;

BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {   case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc;
            hdc=BeginPaint(hwndDlg,&ps);
            Rectangle(hdc,pxi,pyi,pxf,pyf);

            if(V.Ps.size()>0)
            {  // V.CrearColores();
               // V.CrearSitios(hdc);
            }
            V.Mostrar(hdc);
            V.Crear(hdc,V.Ps);
            V.MostrarZ(hdc);
            //A.Crear1(hdc);
            //A.MostrarZ(hdc);
            EndPaint(hwndDlg,&ps);
        }
        return TRUE;
        case WM_LBUTTONDOWN:
        {
            long xPos = GET_X_LPARAM(lParam);
            long yPos = GET_Y_LPARAM(lParam);
            if(xPos<pxf&&xPos>pxi&&yPos<pyf&&yPos>pyi)
                V.Ps.push_back(Punto((float)xPos,(float)yPos));
            InvalidateRect(hwndDlg,NULL,true);
        }
        return TRUE;
        case WM_INITDIALOG:
        {
            pxi=10,pyi=50,pxf=pxi+450,pyf=pyi+450;
        }
        return TRUE;
        case WM_MOUSEMOVE:
        {
            long xPos = GET_X_LPARAM(lParam);
            long yPos = GET_Y_LPARAM(lParam);
            char Cad[20];
            sprintf(Cad,"%i",xPos);
            SetDlgItemText(hwndDlg,EDITX,Cad);
            sprintf(Cad,"%i",yPos);
            SetDlgItemText(hwndDlg,EDITY,Cad);
            if(xPos<pxf&&xPos>pxi&&yPos<pyf&&yPos>pyi)
            { //  InvalidateRect(hwndDlg,NULL,true);
            }
        }
        return TRUE;
        case WM_CLOSE:
        {
            EndDialog(hwndDlg, 0);
        }
        return TRUE;

        case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
                case PONER:
                {
                    int H=GetDlgItemInt(hwndDlg,HIJOS,NULL,NULL);
                    V.Hi=H;
                    InvalidateRect(hwndDlg,NULL,true);
                }
                return TRUE;
            }
        }
        return TRUE;
    }
    return FALSE;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    hInst=hInstance;
    InitCommonControls();
    return DialogBox(hInst, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)DlgMain);
}
