#pragma once 
#pragma execution_character_set("utf-8")

#include "complex.h"

COMPLEX::COMPLEX(double r0, double i0){ 
    r = r0; i = i0; 
}
COMPLEX COMPLEX::operator+(COMPLEX a){ return COMPLEX(r + a.r, i + a.i); };
COMPLEX COMPLEX::operator + (double a){ return COMPLEX(r + a, i); };
void COMPLEX::operator+=(COMPLEX a){ r += a.r, i += a.i; };
void COMPLEX::operator+=(double a){ r += a; };

COMPLEX COMPLEX::operator-(COMPLEX a){ return COMPLEX(r - a.r, i - a.i); };
COMPLEX COMPLEX::operator-(double a){ return COMPLEX(r - a, i); };
void COMPLEX::operator-=(COMPLEX a){ r -= a.r, i -= a.i; };
void COMPLEX::operator-=(double a){ r -= a; };
COMPLEX COMPLEX::operator-(){ return COMPLEX(-r, -i); };


COMPLEX COMPLEX::operator*(COMPLEX a){ return COMPLEX(r*a.r - i*a.i, i*a.r + r*a.i); };
COMPLEX COMPLEX::operator*(double a){ return COMPLEX(r*a, i*a); };
void COMPLEX::operator*=(COMPLEX a){ *this = *this*a; }
void COMPLEX::operator*=(double a){ r *= a, i *= a; };


COMPLEX COMPLEX::operator/(COMPLEX a){
    double s, den;
    if (fabs(a.r) == INF || fabs(a.i) == INF){ return COMPLEX(r / INF, i / INF); }
    else if (fabs(a.r)>fabs(a.i)){ s = a.i / a.r; den = a.r + s*a.i; return COMPLEX((r + s*i) / den, (i - s*r) / den); }
    else if (a.r == a.i&&a.r == 0){ den = 0; return COMPLEX(r / den, i / den); }
    else{ s = a.r / a.i; den = a.i + s*a.r; return COMPLEX((r*s + i) / den, (i*s - r) / den); }
};

COMPLEX COMPLEX::operator/(double a){ return COMPLEX(r / a, i / a); };

void COMPLEX::operator/=(COMPLEX a){ *this = *this / a; }
void COMPLEX::operator/=(double a){ r /= a, i /= a; }

//保证：（实数）运算符 （复数），也能正常重载*。
COMPLEX operator+(double a, COMPLEX b){ return COMPLEX(a + b.r, b.i); };
COMPLEX operator-(double a, COMPLEX b){ return COMPLEX(a - b.r, -b.i); };
COMPLEX operator*(double a, COMPLEX b){ return COMPLEX(b.r*a, b.i*a); };
COMPLEX operator/(double a, COMPLEX b){ double c = a / (b.r*b.r + b.i*b.i); return COMPLEX(b.r*c, -b.i*c); };
