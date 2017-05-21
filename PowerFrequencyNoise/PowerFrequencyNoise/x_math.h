#pragma once 
#pragma execution_character_set("utf-8")

#include<float.h>
#include<math.h>
#include<conio.h>
#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include "complex.h"
#include<stdarg.h>

#define NAMEDIF abcdefghijklmnopqrstuvwxyz012345678901234567890
//#define M_PI 3.1415926
const double M_PI = 4 * atan(1.0);

enum band_type{ LOWPASS = 1, HIGHPASS, BANDPASS, BANDSTOP };
//1：滤波器通带类型枚举

enum win_type{ RECTANG = 1, BARTLETT, HANNING, HAMMING, BLACKMAN };
//2：FIR滤波器所加窗口类型的枚举


//可变参数宏定义
#define _VARG(x) #@x
#define _COMPLEX(A,n,data) \
COMPLEX A[n];{double c[n*2+2]={_VARG(\0##data##\0)};\
for(int i=0;i<n;i++)A[i].r=c[2*i+1],A[i].i=c[2*i+2];}

////////////////////

double abs(COMPLEX a);
COMPLEX sqrt(COMPLEX z);
double polyval(double a[], int n, double s);

COMPLEX polyval(COMPLEX a[], int n, COMPLEX s);
COMPLEX polyval(double a[], int n, COMPLEX s);

void conv(double y[], double x[], int N, double h[], int M);
void conv(COMPLEX y[], COMPLEX x[], int N, COMPLEX h[], int M);

#define polymul(y,x,N,h,M) conv(y,x,N,h,M)

void polycoef(COMPLEX a[], COMPLEX b[], int N);
void polycoef(double a[], double b[], int N);

void polyder(COMPLEX a[], int n, int m);
void polyder(double a[], int n, int m);

void dft(COMPLEX idata[], COMPLEX data[], int n, int isign);
void fft(COMPLEX data[], int n, int isign);
double* cv(double *x, int N, double *h, int M);
double* pAdd(double* a, double* b, int M);
double* pReplace(double *y, int K, double* x, int L, double* a, int M, double* b, int N);
int freqImage(double *e, int K, double *f, int L, double *a, int M, double *b, int N, double *c, int P, double *d, int Q);
void cpoly_coef(COMPLEX b[], int n, COMPLEX a[]);
void btwNormal(double d[], int N);
int  btwOrder(int bandType, double db1, double db2, double fs, double f1, double f2, ...);
int btwAf(double b[][2], int order);
double btwC23(double c[2][3], int bandType, int N, double db1, double fs, double f1, double f2, ...);
void btwAf2Df(double H[][2][5], int L, double b[][2], double c[2][3]);
double btw20lgHs(double f, double *p, int *N, double *fc);
double btw20lgHs(double f, double fc, double p[], int N);
double btw20lgHz(double f, double *fs, double H[][2][5], int *L);
double btw20lgHz(double f, double fs, double H[][2][5], int L);
void firWin(int winType, double w[], int N);
void firDesgin(double b[], int L, int bandType, int winType, double fs, double fL, ...);
double firAbs(double f, double fs, double b[], int L);
double firPhase(double f, double fs, double b[], int L);
void polyLag(COMPLEX a[], int m, COMPLEX *x, double r_err, int polish);

//func在[a,b]区间划分成2^N个梯形做积分,积分值输出为y，需声明为double;
//形参x无需声明，它只标明函数那个参数作自变量。
//用法如：double y;_Trapz(y,t,atan2(t,7.12),0,4,11);
#define _iTrapz(y,a,b,N,func,x) \
{double x,sum##NAMEDEF,del##NAMEDEF,it##NAMEDEF=1.0;int i##NAMEDEF,j##NAMEDEF;\
x=a,y=func,x=b,y=y+func,y=0.5*(b-a)*y;\
for(i##NAMEDEF=2;i##NAMEDEF<=N;i##NAMEDEF++)\
{del##NAMEDEF=(b-a)/it##NAMEDEF,x=a+0.5*del##NAMEDEF,sum##NAMEDEF=0.0,j##NAMEDEF=1;\
for(;j##NAMEDEF<=it##NAMEDEF;j##NAMEDEF++,x+=del##NAMEDEF)sum##NAMEDEF+=func;\
y=0.5*(y+sum##NAMEDEF*del##NAMEDEF);it##NAMEDEF=it##NAMEDEF+it##NAMEDEF;}\
}


//double y做输出。
//积分值精确到6位有效数字。
#define _iTrap(y,a,b,func,x) \
{double oldy##NAMEDEF;int kk##NAMEDEF;\
_iTrapz(oldy##NAMEDEF,a,b,1,func,x);\
_iTrapz(y,a,b,2,func,x);\
for(kk##NAMEDEF=3;(fabs(y-oldy##NAMEDEF)>=fabs((1.0e-6)*y))&&kk##NAMEDEF<20;kk##NAMEDEF++)\
{oldy##NAMEDEF=y;_iTrapz(y,a,b,kk##NAMEDEF,func,x);}}

double iTrapz(double a, double b, int N, double(*func)(double));
double iTrap(double a, double b, double(*func)(double));
double wal(double t, int n);
double Wal(double t, int n[1]);
double* WAL(int N);

//22:double coef是输出。
#define _WalCoef(coef,k,T,fx,t) \
{if(T<=0.0||k<0){printf("_WalCoef()的T或则k参数输入错误！");getch();exit(0);}\
else _iTrap(coef,0.0,T,fx*wal(t/T,k),t);\
coef=coef/T;}

double WalSer(double t, double *T, double a[], int n[1]);
double WalSer(double t, double T, double a[], int n);

//double coef是输出。
#define _fsinCoef(coef,k,T,fx,x) \
{double w##NAMEDEF=M_PI*2.0/T;\
if(T<=0.0||k<0){printf("fsinCoef()的T或则k参数输入错误！");getch();exit(0);}\
else {_iTrap(coef,0.0,T,fx*sin(w##NAMEDEF*x*k),x);\
coef=coef*2/T;}}


#define _fcosCoef(coef,k,T,fx,x) \
{double w##NAMEDEF=M_PI*2.0/T;\
if(T<=0.0||k<0){printf("fsinCoef()的T或则k参数输入错误！");getch();exit(0);}\
else {_iTrap(coef,0.0,T,fx*cos(w##NAMEDEF*x*k),x);\
coef=coef*2/T;}\
if(k==0)coef=coef/2;}

double FouSer(double t, double *T, double a[], double b[], int *N);
double FouSer(double t, double T, double a[], double b[], int N);

//函数卷积
#define _iTConv(s,ti,a,N,func1,func2,x) \
{double x,s1,s2=0,del,it=1.0;int i,j;\
x=a,s1=func1,x=ti-a,s1=s1*func2,x=ti-a,s2=func1,x=a,s2=s2*func2,s=s1+s2,s=0.5*(ti-2*a)*s;\
for(i=2;i<=N;i++)\
{del=(ti-2*a)/it,x=a+0.5*del,s2=0.0,j=1;\
for(;j<=it;j++,x+=del){s1=func1;x=ti-x;s2+=func2*s1;x=ti-x;}\
s=0.5*(s+s2*del);it=it+it;}\
}

//已知被卷两函数的非零起点：t>=a，f1(x)=0,f2(x)=0时，卷积分区间为[a,t-a]
#define _iConv(y,ti,a,fx1,fx2,x) \
{double oldy;int kk;\
_iTConv(oldy,ti,a,1,fx1,fx2,x);\
_iTConv(y,ti,a,2,fx1,fx2,x);\
for(kk=3;(fabs(y-oldy)>=fabs((1.0e-6)*y))&&kk<20;kk++)\
{oldy=y;_iTConv(y,ti,a,kk,fx1,fx2,x);}}


#define _fTConv(y,t,a,b,N,fx1,fx2,x) \
{double x,s1,s2=0,del,it=1.0;int i,j;\
x=a,s1=fx1,s2=fx2,x=b,y=s1*fx2+s2*fx1,y=0.5*(b-a)*y;\
for(i=2;i<=N;i++)\
{del=(b-a)/it,x=a+0.5*del,s2=0.0,j=1;\
for(;j<=it;j++,x+=del){s1=fx1;x=t-x;s2+=fx2*s1;x=t-x;}\
y=0.5*(y+s2*del);it=it+it;}\
}

//已知被卷两函数中任一函数的非零区间：只在[a，b]，f1(x)!=0,此外f1(x)=0。
#define _fConv(y,t,a,b,fx1,fx2,x) \
{double oldy;int kk;\
_fTConv(oldy,t,a,b,1,fx1,fx2,x);\
_fTConv(y,t,a,b,2,fx1,fx2,x);\
for(kk=3;(fabs(y-oldy)>=fabs((1.0e-6)*y))&&kk<20;kk++)\
{oldy=y;_fTConv(y,t,a,b,kk,fx1,fx2,x);}}

