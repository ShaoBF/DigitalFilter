#pragma once 
#pragma execution_character_set("utf-8")

#include "complex.h"


#include <windows.h>  //This lib MUST be included 
#include <conio.h>
#include <tchar.h>
#include<stdlib.h>
#include <stdio.h>
#include<math.h>

extern "C" {WINBASEAPI HWND WINAPI GetConsoleWindow(); }
#define WHITE RGB(255,255,255)
#define BLACK RGB(0,0,0)

#define RED RGB(255,0,0)
#define BLUE RGB(0,0,255)
#define GREEN RGB(0,0x80,0)
#define CYAN RGB(0,0xFF,0xFF)
#define MAGENTA RGB(0xFF,0,0xFF)
#define GRAY RGB(0x80,0x80H,0x80)

#define LIGHTRED RGB(255,0xB6,0xC1)
#define LIGHTBLUE RGB(0xad,0xd8,0xe6)
#define LIGHTGREEN RGB(0x90,0xee,0x90)
#define LIGHTCYAN RGB(0xe0,0xFF,0xFF)
#define LIGHTMAGENTA RGB(0xFF,0x14,0x93)
#define LIGHTGRAY RGB(0xd3,0xd3,0xd3)

#define DARKGRAY RGB(0xa9,0xa9,0xa9)
#define YELLOW RGB(0xf0,0xe6,0x8c)
//全局声明GetConsoleWindow()函数调用方式。

#define LRTB_SPACE 25    //设置默认的上下左右边框留白。 

typedef struct tagWIN2{
    HWND hwnd;
    HDC hdc;
    RECT rect, rect2;
    char xstr[50], ystr[50];
    int xo, yo;
    double x1, y1, x2, y2;
    double xk, yk, dxPerPixel, dyPerPixel;
}WIN2;



typedef struct tagWIN3{
    HWND hwnd;
    HDC hdc;
    RECT rect;//,rect2;
    //int xo,yo;//moveto3,linTo3用它做0点。
    char xstr[50], ystr[50], zstr[50];
    double x1, y1, z1, x2, y2, z2;
    double dxPerPixel, dyPerPixel, dzPerPixel;//dxPerPixel=1/xk,dyPerPixel=1/yk,dzPerPixel=1/zk.
    double xPixels, yPixels, zPixels, gridPixels;//xPixels为坐标轴的长。
    //double tx1,tx2,tx3,tx4,ty1,ty2,ty3,ty4,tz1,tz2,tz3,tz4,tc1,tc2,tc3,tc4;
    double t[4][4];//t[0][]=x行，t[1][]为y行，t[2][]--z,t[3]--常数行。
}WIN3;


#define ux2(x) (int)(win2.xo+x*win2.xk)
#define uy2(y) (int)(win2.yo-y*win2.yk)

#define _plotxy2(t,func)\
{double t;\
t=win2.x1;\
moveto2(t,func);\
for(;t<win2.x2;t=t+win2.dxPerPixel)lineto2(t,func);\
t=win2.x2;lineto2(t,func);}\


typedef union{
    struct{ double x, y, z, c; };
    double A[4];
}POINT3, *PPOINT3;


typedef struct{
    int x, y;
}SCR2, *PSCR2;


#define ROUND(d) int(floor(d+0.5))

#define SHOWWIN \
SendMessage(HWND_BROADCAST,WM_PAINT,0,0);_getch();

//system("pause");


#define DRAWXYZ \
{POINT ca;SelectObject(win3.hdc,hpen1);line3(0,0,0,40,0,0);\
ca=getxy3();TextOut(win3.hdc,ca.x,ca.y,"x",1);\
SHOWWIN;\
SelectObject(win3.hdc,hpen2);line3(0,0,0,0,40,0);\
ca=getxy3();TextOut(win3.hdc,ca.x,ca.y,"y",1);\
SHOWWIN;\
SelectObject(win3.hdc,hpen3);line3(0,0,0,0,0,40);\
ca=getxy3();TextOut(win3.hdc,ca.x,ca.y,"z",1);\
SHOWWIN;}

///变量t无需要先声明。
#define plotxy2(penColor,penWidth,t,func) \
{HPEN pen=CreatePen(PS_SOLID,penWidth,penColor),oldpen=(HPEN)SelectObject(win2.hdc,pen);\
_plotxy2(t,func);\
SelectObject(win2.hdc,oldpen);\
DeleteObject(pen);\
}

///////////////////////////////////////////////////////////
#define _MULRX \
double temp[4][4];\
mMul((double*)RX,(double*)win3.t,4,4,4,(double*)temp);\
int i,j;\
for(i=0;i<4;i++)for(j=0;j<4;j++)win3.t[i][j]=temp[i][j];
////////////////////////////////////////////////////////////

#define ux3(x,y,z) (int)(x*win3.t[0][0]+y*win3.t[1][0]+z*win3.t[2][0]+win3.t[3][0]+0.5)
#define uy3(x,y,z) (int)(-x*win3.t[0][1]-y*win3.t[1][1]-z*win3.t[2][1]-win3.t[3][1]+0.5)

void lineto1(HDC hdc, COLORREF bkcolor, COLORREF linecolor, int x, int y);
void purfleline(HDC hdc, COLORREF bkcolor, COLORREF linecolor, int x0, int y0, int x1, int y1, int cx, int cy);
void purfleline(HDC hdc, COLORREF bkcolor, COLORREF linecolor, POINT a, POINT b, POINT ref);
void arrowline(HDC hdc, int linewidth, COLORREF bkcolor, COLORREF linecolor, int ax, int ay, int bx, int by);
void arrowline(HDC hdc, int linewidth, COLORREF bkcolor, COLORREF linecolor, POINT a, POINT b);
void rectline(HDC hdc, int width, COLORREF bkcolor, COLORREF linecolor, double ax, double ay, double bx, double by);
void coord2(double x1, double y1, double x2, double y2);
void frame2(char xstr[], char ystr[]);
void frame2(char xstr[], char ystr[], int space);
void frame2();
void moveto2(double x, double y);
void lineto2(double x, double y);
void line2(double x1, double y1, double x2, double y2);
void plot2(double(*functionName)(double));
void plot2(double(*functionName)(double), double dt);
void moveto3(double x, double y, double z);
void moveto3(POINT3 a);
void lineto3(double x, double y, double z);
void lineto3(POINT3 a);
void line3(double x1, double y1, double z1, double x2, double y2, double z2);
void line3(POINT3 a, POINT3 b);
void mMul(double* A, double* B, int m, int n, int p, double* C);
POINT3 pt3(double x, double y, double z);
void shift3(double dx, double dy, double dz);
void shift3(POINT3 p[], int n, double dx, double dy, double dz);
void rx3(POINT3 p[], int n, double a);
void rx3(double a);
void sxyz3(double sx, double sy, double sz);
void ry3(POINT3 p[], int n, double a);
void ry3(double a);
void rz3(POINT3 p[], int n, double a);
void rz3(double a);
POINT getxy3();
void rv3(double w, double x1, double y1, double z1, double x2, double y2, double z2);
void rv3(POINT3 C[], int n, double w, double x1, double y1, double z1, double x2, double y2, double z2);
void testRot();
void rv3();
void coord3(double x1, double y1, double z1, double x2, double y2, double z2);
void coord3(POINT3 a, POINT3 b);
void frame3(COLORREF bkcolor, COLORREF dotLineColor, char *xstr, char *ystr, char *zstr);
void frame3(char *xstr, char *ystr, char *zstr);
void frame3();
void arrow3(COLORREF linecolor, double ax, double ay, double az, double bx, double by, double bz, int h, int d2);
void xyz3(COLORREF axiesColor);
void initgraph(char *title, int x, int y, int width, int higth);
void plot3(COLORREF color, double(*fxy)(double, double));
void plot3(COLORREF gridcolor, COLORREF linecolor, COMPLEX p[], int M);
void initgd(char *path);
void closegraph();
void setvp2(int scrx, int scry, int width, int heigth, char *title, char *notice);
void xy2(COLORREF color);
void window2(char *title, double x1, double y1, double x2, double y2, char *xAxis, char *yAxis, COLORREF framecolor, COLORREF xyColor);
void window2(char *title, double x1, double y1, double x2, double y2, char *xAxis, char *yAxis);
void clearviewport(COLORREF bkcolor);
void setvp3(int scrx, int scry, int width, int heigth, char *title, char *notice);
void window3(char *title, double x1, double y1, double z1, double x2, double y2, double z2, char *xAxis, char *yAxis, char *zAxis, COLORREF framecolor, COLORREF xyColor);
void window3(char *title, double x1, double y1, double z1, double x2, double y2, double z2, char *xAxis, char *yAxis, char *zAxis);
void clear3();
void instKeyCtr3();
void instKeyCtr2();
BOOL keyCtr2(double x[1], double a, double b, int n);
BOOL keyCtr3(double x[1], double a, double b, int n);
void clearvp2();
void instKeyCtr(double x[1], double a, double b, int N);
BOOL keyCtr(double x[1], double a, double b, int n);


class VERTEX{
public:
    double x;
    double y;
    double z;
public:
    VERTEX();
    virtual ~VERTEX();
    VERTEX(double x1, double y1, double z1){ x = x1; y = y1; z = z1; }
};

class EDGE{
public:
    int eNumber;
    int *eIndex;
public:
    EDGE();
    virtual ~EDGE();
};

class box{
public:
    POINT3 v[8];
    //	LINESET  l[12];
    //	FACESET   f[6];
    double x0, y0, z0;
    double len;
public:
    box();
    virtual ~box();
};

enum USEWIN{ USE_CONSOLE, USE_WINDOW };

class winx{
public:
    HINSTANCE hinstance;
    HWND hwnd0;
    HDC hdc0;
    RECT rect0;
    //	int xo,yo;
    //	double x1,y1,x2,y2;
    //	double xk,yk,dxPerPixel,dyPerPixel;
public:
    winx(USEWIN win = USE_CONSOLE, char *title = "xxgc实验平台", int x0 = 0, int y0 = 0, int width = 800, int height = 600){
        hinstance = (HINSTANCE)GetModuleHandle(NULL);
        if (win == USE_CONSOLE){
            system("color f0");//设置控制台窗口背景为白色，前景为黑色。

            CONSOLE_CURSOR_INFO cursor_info = { 1, 0 };
            SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
            //隐藏控制台光标，避免干扰图象

            hwnd0 = GetConsoleWindow();
            hdc0 = GetDC(hwnd0);//通过窗口句柄得到该窗口的设备场境句柄

            SetConsoleTitle(title);//修改窗口标题。
            SetWindowPos(hwnd0, HWND_TOPMOST, x0, y0, width, height, SWP_SHOWWINDOW);
        }
    };
};




///模仿函数方式，变量x,y需要先声明。
#define _plotxyz3(x,y,fxy) \
{double x,y,grid=win3.gridPixels*win3.dyPerPixel;\
for(y=win3.y1;y<=win3.y2;y=y+grid)\
{x=win3.x1,moveto3(x,y,fxy);\
for(x=win3.x1;x<=win3.x2;x=x+win3.dxPerPixel)\
lineto3(x,y,fxy);}\
y=win3.y2;\
{x=win3.x1,moveto3(x,y,fxy);\
for(x=win3.x1;x<=win3.x2;x=x+win3.dxPerPixel)\
lineto3(x,y,fxy);}\
grid=win3.gridPixels*win3.dxPerPixel;\
for(x=win3.x1;x<=win3.x2;x=x+grid)\
{y=win3.y1,moveto3(x,y,fxy);\
for(y=win3.y1;y<=win3.y2;y=y+win3.dyPerPixel)\
lineto3(x,y,fxy);}\
x=win3.x2;\
{y=win3.y1,moveto3(x,y,fxy);\
for(y=win3.y1;y<=win3.y2;y=y+win3.dyPerPixel)\
lineto3(x,y,fxy);}\
}

///模仿函数方式，变量x,y需要先声明。
#define plotxyz3(penColor,penWidth,x,y,fxy) \
{HPEN hPen=CreatePen(PS_SOLID, penWidth, penColor);\
HPEN hOldPen=(HPEN)SelectObject(win3.hdc, hPen);\
_plotxyz3(x,y,fxy);\
SelectObject(win2.hdc, hOldPen);\
DeleteObject(hPen);\
}
