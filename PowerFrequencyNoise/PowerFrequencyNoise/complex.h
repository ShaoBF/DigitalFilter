#pragma once 
#pragma execution_character_set("utf-8")

#include<float.h>
#include <math.h>

const double INFS = 0.0;//zInfinitesimal=+0.0;
//const double fInfinitesimal=-0.0;
const double INF = 1.0 / INFS;//zInf=1.0/zInfinitesimal;
//const double NINF=-1.0/INFS;//fInf=-1.0/fInfinitesimal;

class COMPLEX{
public:
    double r;
    double i;

public:
    //COMPLEX(void){r=0;i=0;};
    COMPLEX(double r0 = 0, double i0 = 0);
    COMPLEX operator+(COMPLEX a);
    COMPLEX operator+(double a);
    void operator+=(COMPLEX a);
    void operator+=(double a);

    COMPLEX operator-(COMPLEX a);
    COMPLEX operator-(double a);
    void operator-=(COMPLEX a);
    void operator-=(double a);
    COMPLEX operator-();


    COMPLEX operator*(COMPLEX a);
    COMPLEX operator*(double a);
    void operator*=(COMPLEX a);
    void operator*=(double a);


    COMPLEX operator/(COMPLEX a);

    COMPLEX operator/(double a);

    void operator/=(COMPLEX a);
    void operator/=(double a);
};

//保证：（实数）运算符 （复数），也能正常重载*。
COMPLEX operator+(double a, COMPLEX b);
COMPLEX operator-(double a, COMPLEX b);
COMPLEX operator*(double a, COMPLEX b);
COMPLEX operator/(double a, COMPLEX b);
