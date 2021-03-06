#pragma once 
#pragma execution_character_set("utf-8")

#include "x_math.h"







double abs(COMPLEX a){
    double x, y, ans, temp; x = fabs(a.r); y = fabs(a.i);
    if (x == 0.0)ans = y;
    else if (y == 0.0)ans = x;
    else if (x > y){ temp = y / x; ans = x*sqrt(1.0 + temp*temp); }
    else { temp = x / y; ans = y*sqrt(1.0 + temp*temp); }
    return ans;
}

//(-INF)开方与matlab不一致，应为INFi,但这里结果为INF+INFi
COMPLEX sqrt(COMPLEX z){
    double ab, alp;
    ab = sqrt(abs(z)); alp = atan2(z.i, z.r);
    return COMPLEX(ab*cos(0.5*alp), ab*sin(0.5*alp));
}



//求复数多项式a[0]+a[1]s+a[2]s^2+...+a[n-1]s^(n-1)的复数值。类似matlab的polyval
COMPLEX polyval(COMPLEX a[], int n, COMPLEX s)
{
    int i = n - 2; 
    COMPLEX sum = a[n - 1]; 
    for (; i >= 0; i--)
        sum = s*sum + a[i]; 
    return sum;
}

COMPLEX polyval(double a[], int n, COMPLEX s)
{
    int i = n - 2;
    COMPLEX sum = a[n - 1];
    for (; i >= 0; i--)
        sum = s*sum + a[i];
    return sum;
}

double polyval(double a[], int n, double s)
{
    int i = n - 2;
    double sum = a[n - 1];
    for (; i >= 0; i--)
        sum = s*sum + a[i];
    return sum;
}

//多项式乘法即卷积，计算 y[L]=X[N]@h[M],L=M+N-1,y[]无须清0，matlab
void conv(double y[], double x[], int N, double h[], int M)
{
    int n, k, j, L;
    L = M + N - 1;
    for (n = 0; n < L; n++)
        for (k = (n >= M) ? n - M + 1 : 0, j = (N > n) ? n : N - 1, y[n] = 0; k <= j; k++)
            y[n] += x[k] * h[n - k];
}

void conv(COMPLEX y[], COMPLEX x[], int N, COMPLEX h[], int M)
{
    int n, k, j, L;
    L = M + N - 1;
    for (n = 0; n < L; n++)
        for (k = (n >= M) ? n - M + 1 : 0, j = (N > n) ? n : N - 1, y[n] = COMPLEX(0, 0); k <= j; k++)
            y[n] += x[k] * h[n - k];
}

#define polymul(y,x,N,h,M) conv(y,x,N,h,M)

//由一组根得到相应多项式的系数，a[]是输出系数，b[]是输入的根。
//a[0]+a[1]s+...+a[N]s^N=(s-b[0])(s-b[1])...(s-b[N-1])
void polycoef(COMPLEX a[], COMPLEX b[], int N)
{
    int i, j;
    COMPLEX tb[2], *c = new COMPLEX[N];//从堆分配数组并由构造函数初始化为0。
    a[0] = -b[0], a[1] = COMPLEX(1, 0);
    for (i = 1; i < N; i++){
        tb[0] = -b[i]; tb[1] = COMPLEX(1, 0);
        for (j = 0; j < i + 1; j++)c[j] = a[j];
        conv(a, c, i + 1, tb, 2);
    }
    delete[N]c;
}

void polycoef(double a[], double b[], int N)
{
    int i, j;
    double tb[2], *c = new double[N];//从堆分配数组并由构造函数初始化为0。
    a[0] = -b[0], a[1] = 1;
    for (i = 1; i < N; i++){
        tb[0] = -b[i]; tb[1] = 1;
        for (j = 0; j < i + 1; j++)c[j] = a[j];
        conv(a, c, i + 1, tb, 2);
    }
    delete[N]c;
}

void polyder(COMPLEX a[], int n, int m)
//11:对n-1次复数多项式a[0]+a[1]S+...+a[n-1]s^(n-1)求m阶导
{
    int i, j;
    for (i = 1; i <= m; i++)for (j = 0; j < n - i; j++)a[j] = a[j + 1] * (j + 1);
    for (i = n - 1; i >= n - m; i--)a[i] = COMPLEX(0, 0);
}

void polyder(double a[], int n, int m)
//11:对n-1次实数多项式a[0]+a[1]x+...+a[n-1]x^(n-1)求m阶导
{
    int i, j;
    for (i = 1; i <= m; i++)for (j = 0; j < n - i; j++)a[j] = a[j + 1] * (j + 1);
    for (i = n - 1; i >= n - m; i--)a[i] = 0;
}







/////////////变换函数////////////////////////////////////
//13:离散傅立叶变换
//idata[]存放变换的结果，data[]是需变换的数组，两数组长度都为n,
//isign=1做正变换，isign=-1做反变换。
void dft(COMPLEX idata[], COMPLEX data[], int n, int isign)
{
    int j, k;
    COMPLEX temp;
    for (j = 0; j < n; j++){
        idata[j] = COMPLEX(0.0, 0.0);
        for (k = 0; k < n; k++){
            idata[j] = idata[j] + data[k] * COMPLEX(cos(M_PI * 2 * j*k / n), sin(-isign*M_PI * 2 * j*k / n));
        }
        if (isign == -1)idata[j] = idata[j] * 1.0 / n;
    }
}

//13:离散傅立叶变换
//data[]既是输入数组又是输出数组，数组长度n必须是2的幂次。
//isign=1做正变换，isign=-1做反变换。
void fft(COMPLEX data[], int n, int isign)
{
    int i, j, k, step;
    COMPLEX ctemp, w, wp;
    double theta;
    for (i = 1; i < n; i++)
    {
        j = n + i; k = 0;
        while (j > 1){
            k = (j & 1 == 1) ? (k << 1) + 1 : k << 1;
            j = j >> 1;
        }
        if (k > i)
        {
            ctemp = data[i];
            data[i] = data[k];
            data[k] = ctemp;
        }
    }
    //Here begins the Danielson_Lanczos setion of the routine
    //Step=2,4,8,... is the distance between W(k/N) to W(k/N) in column
    for (step = 2; step <= n; step *= 2){
        for (theta = -isign*M_PI * 2 / step,
            wp = COMPLEX(cos(theta), sin(theta)),
            w = COMPLEX(1.0, 0.0),
            k = 0;
        k < step / 2;
        k++,
            w = w*wp){
            for (i = k; i < n; i += step){
                j = i + step / 2;
                ctemp = w*data[j];
                data[j] = data[i] - ctemp;
                data[i] = data[i] + ctemp;
            }
        }
    }
    if (isign == -1)for (i = 0; i < n; i++)data[i] = data[i] * 1.0 / n;
}


/////////////////



/*y[0]..y[L-1],x[0]..x[N-1],h[0]..h[M-1];general:L=M+N-1;
as:0<=k<=M+N-2,0<=n-k<=M-1,0<=k<=N-1
so:K>=0,K>=n-M+1;
	 k<=N-1,K<=n;
x=x@h;
*/
double* cv(double *x, int N, double *h, int M)
{
    int n, k, kk, L;
    double *y;
    y = (double*)malloc((unsigned)(M + N - 1)*sizeof(double));
    if (!y){ printf("allocation error in conv()/n"); _getch(); exit(1); }
    L = M + N - 1;
    for (n = 0; n < L; n++)
        for (k = (n >= M) ? n - M + 1 : 0, kk = (N > n) ? n : N - 1, y[n] = 0; k <= kk; k++)
            y[n] += x[k] * h[n - k];
    for (n = 0; n < L; n++)x[n] = y[n];
    free(y);
    return x;
}


/*here :a[]=a[]+b[M]*/
double* pAdd(double* a, double* b, int M)
{
    int i;
    for (i = 0; i < M; i++)a[i] = a[i] + b[i];
    return a;
}


/*y[]=x0A<L-1>+x1A<L-2>B<1>+...+x[L-2]A<1>B<L-2>+x[L-1]B<L>
here:  k=(M>N)?M:N;
	   K=(K-1)*(L-1)+1;
*/
double* pReplace(double *y, int K, double* x, int L, double* a, int M, double* b, int N)
{
    int i, j;
    double *g;
    g = (double*)malloc((unsigned)K*sizeof(double));
    /*h=(double*)malloc((unsigned)K*sizeof(double));*/
    if (!g){ printf("allocation error in pReplace.\n"); _getch(); exit(1); }

    for (j = 0; j < K; j++)y[j] = 0.0;

    for (i = 0; i < L; i++)
    {
        for (g[0] = x[i], j = 1; j < K; j++)g[j] = 0.0;
        for (j = 1; j <= i; j++)cv(g, K - M + 1, a, M);
        for (j = 1; j <= L - 1 - i; j++)cv(g, K - N + 1, b, N);
        pAdd(y, g, K);
    }
    return y;
}

/*here:s=c(t)/d(t)
	   e(t)/f(t)=a(s)/b(s)=a[c(t)/d(t)]/b[c(t)/d(t)]
	   a[0]..a[M],
	   b[0]..b[N],
	   c[0]..c[P],
	   d[0]..d[Q],
	   e[0]..e[K],
	   f[0]..f[L],
here:setting K,L with
	   if(M>N){K=(M-1)*(P-1)+1;
			  L=(N-1)*(P-1)+(M-N)*(Q-1)+1;}
	   else  {K=(M-1)*(P-1)+(N-M)*(Q-1)+1;
			  L=(N-1)*(P-1)+1;}
*/
int freqImage(double *e, int K, double *f, int L,
    double *a, int M, double *b, int N,
    double *c, int P, double *d, int Q)
{
    if (((M > N) && (K < (M - 1)*(P - 1) + 1 || L < (N - 1)*(P - 1) + (M - N)*(Q - 1) + 1))
        || ((M <= N) && (K < (M - 1)*(P - 1) + (N - M)*(Q - 1) + 1 || L < (N - 1)*(P - 1) + 1)))
    {
        printf("e[],f[]'s dimension K,L too short.\n\
               			 please resetting K,L\n"); _getch(); exit(1);
    }
    pReplace(e, K, a, M, c, P, d, Q);
    pReplace(f, L, b, N, c, P, d, Q);
    if (M < N)for (L = 1; L <= N - M; L++)cv(e, K - Q + 1, d, Q);
    else if (M > N)for (K = 1; K <= M - N; K++)cv(f, L - Q + 1, d, Q);

    if (M > N){
        K = (M - 1)*(P - 1) + 1;
        L = (N - 1)*(P - 1) + (M - N)*(Q - 1) + 1;
    }
    else  {
        K = (M - 1)*(P - 1) + (N - M)*(Q - 1) + 1;
        L = (N - 1)*(P - 1) + 1;
    }
    return (K > L) ? K : L;
}

/*12:s[0]..s[n-1] is     roots
a[0]..a[n] is coefficents
  p0=a0=1;
  p1=p0*(s-b0);
  p(k+1)=pk*(s-bk)=a0  a1    a2..    ak
					  -a0bk -a1bk.. -a(k-1)bk -akbk
*/
void cpoly_coef(COMPLEX b[], int n, COMPLEX a[])
{
    int k, i;
    a[0] = COMPLEX(1.0, 0.0);
    for (k = 0; k < n; k++)
    {
        a[k + 1] = COMPLEX(0.0, 0.0);
        for (i = k; i >= 0; i--)
            a[i + 1] = a[i + 1] - a[i] * b[k];
    }
}


/*notice:please declare double d[N],N=filter_order+1 */
void btwNormal(double d[], int N)
{
    int k;
    //COMPLEX *s,*a;
    COMPLEX *s, *a;
    double pi;
    N = N - 1;
    //s=(COMPLEX*)malloc((unsigned)N*sizeof(COMPLEX));
    //a=(COMPLEX*)malloc((unsigned)(N+1)*sizeof(COMPLEX));
    s = new COMPLEX[N];
    a = new COMPLEX[N + 1];
    if (!s || !a)
    {
        printf("allocatioin faile in normalBtw()."); _getch(); exit(1);
    }
    pi = 4 * atan(1.0);
    for (k = 0; k < N; k++)s[k] = COMPLEX(-sin(pi*(2 * k + 1) / (2 * N)), cos(pi*(2 * k + 1) / (2 * N)));
    cpoly_coef(s, N, a);
    for (k = 0; k <= N; k++)d[k] = a[k].r;
    //free(a);free(s);
    delete a;
    delete s;
}





//int _cdecl orderBtw()//bandType,db1,db2,fs,f1,f2,f3,f4)
//int bandType;
//double db1,db2,fs,f1,f2,f3,f4;
int  btwOrder(int bandType, double db1, double db2, double fs, double f1, double f2, ...)
{
    double pi, omga1, omga2, omga3, omga4, p1, p2, p;
    int N;
    va_list arg;//
    va_start(arg, f2);
    double f3 = va_arg(arg, double);
    double f4 = va_arg(arg, double);


    if (bandType < 1
        || bandType>4
        || db1 < 0
        || db2 < db1
        || f1 < 0
        || f2 < f1
        || fs < f2)
    {
        printf("input argument error in orderBtw()\n"); _getch(); exit(0);
    }
    pi = 4 * atan(1.0);

    /*********for low_pass and high_pass***********/
    omga1 = tan(pi*f1 / fs);
    omga2 = tan(pi*f2 / fs);
    if (bandType < 3)p = omga2 / omga1;

    /*********for band_pass and band_stop***********/
    else{
        if (f3 < f2
            || f4 < f3
            || fs < f4)
        {
            printf("Input argument error in orderBtw()\n");
            _getch(); exit(1);
        }
        omga3 = tan(pi*f3 / fs);
        omga4 = tan(pi*f4 / fs);
        if (bandType == 3)
        {
            p1 = (omga1*omga1 - omga3*omga2) / (omga1*(omga3 - omga2));
            p2 = (omga4*omga4 - omga3*omga2) / (omga4*(omga3 - omga2));
            p1 = -p1;
            p = (p1 < p2) ? p1 : p2;
        }
        else if (bandType == 4)
        {
            p1 = -omga2*(omga4 - omga1) / (omga2*omga2 - omga4*omga1);
            p2 = -omga3*(omga4 - omga1) / (omga3*omga3 - omga4*omga1);
            p2 = -p2;
            p = (p1 < p2) ? p1 : p2;
        }
    }
    p1 = pow(10, db1 / 10) - 1;
    p2 = pow(10, db2 / 10) - 1;

    p = 0.5*log10(p2 / p1) / log10(p);
    N = (int)ceil(p);
    return N;
}

int btwAf(double b[][2], int order)
/*notice:please declare double d[N],N=filter_order */
{
    int k, M, L;
    double pi;

    pi = 4 * atan(1.0);
    M = order / 2; L = M;
    if (order != M * 2){ b[M][0] = 0; b[M][1] = 1; L++; }

    for (k = 0; k < M; k++){
        b[k][0] = 1; b[k][1] = (-2)*cos((2 * k + order + 1)*pi / (2 * order));
    }

    return L;
}

//double getc23(c,bandType,N,db1,fs,f1,f2,f3,f4)
//int bandType,N;
//double c[2][3],db1,fs,f1,f2,f3,f4;
double btwC23(double c[2][3], int bandType, int N, double db1, double fs, double f1, double f2, ...)
{
    double fc, pi, namda;
    va_list arg;//
    va_start(arg, f2);
    double f3 = va_arg(arg, double);
    double f4 = va_arg(arg, double);

    pi = 4 * atan(1.0);
    if (bandType == LOWPASS)
    {
        c[0][0] = 1; c[0][1] = -c[0][0]; c[0][2] = 0.0;
        c[1][0] = tan(pi*f1 / fs); c[1][1] = c[1][0]; c[1][2] = 0.0;
        fc = f1;
    }
    else if (bandType == HIGHPASS)
    {
        c[0][0] = tan(pi*f2 / fs); c[0][1] = c[0][0]; c[0][2] = 0.0;
        c[1][0] = 1; c[1][1] = -1; c[1][2] = 0.0;
        fc = f2;
    }
    else if (bandType == BANDPASS)
    {
        double U, L;
        U = tan(pi*f3 / fs); L = tan(pi*f2 / fs);
        c[0][0] = 1 + U*L; c[0][1] = 2 * (U*L - 1); c[0][2] = 1 + U*L;
        c[1][0] = U - L; c[1][1] = 0; c[1][2] = L - U;
        fc = f2;
    }
    else if (bandType == BANDSTOP)
    {
        double U, L;
        U = tan(pi*f4 / fs); L = tan(pi*f1 / fs);
        c[1][0] = 1 + U*L; c[1][1] = 2 * (U*L - 1); c[1][2] = 1 + U*L;
        c[0][0] = U - L; c[0][1] = 0; c[0][2] = L - U;
        fc = f1;
    }

    namda = pow(10, db1 / 10) - 1;
    namda = pow(1 / namda, 0.5 / N);
    c[1][0] = namda*c[1][0];
    c[1][1] = namda*c[1][1];
    c[1][2] = namda*c[1][2];

    return fc;
}


/*jiang s=(a+b/z+c/zz)/(d+e/z+f/zz) dairu

h(s)=1/(m*ss+n*s+1) de xinshi zhong
dedao:
h(1/z)=[dd+2de/z+(ee+2df)/zz+2ef/zzz+ff/zzzz]
/{m[aa+2ab/z+(bb+2ac)/zz+2bc/zzz+cc/zzzz]
+[dd+2de/z+(ee+2df)/zz+2ef/zzz+ff/zzzz]
+n[ad+(ae+bd)/z+(cd+af+be)/zz+(bf+ce)/zzz+cf/zzzz]}

=[]/
[maa+dd+nad
+(2mab+2de+nae+nbd)/z
+(mbb+2mac+2df+ee+n(cd+af+be))/zz
+(2mbc+2ef+nbf+nce)/zzz
+(mcc+ff+ncf)/zzzz]
****/
void btwAf2Df(double H[][2][5], int L, double b[][2], double c[2][3])
{
    int i;
    for (i = 0; i < L; i++)
    {
        H[i][0][0] = c[1][0] * c[1][0];
        H[i][0][1] = 2 * c[1][0] * c[1][1];
        H[i][0][2] = c[1][1] * c[1][1] + 2 * c[1][0] * c[1][2];
        H[i][0][3] = 2 * c[1][1] * c[1][2];
        H[i][0][4] = c[1][2] * c[1][2];
        H[i][1][0] = b[i][0] * c[0][0] * c[0][0] + c[1][0] * c[1][0] + b[i][1] * c[0][0] * c[1][0];
        H[i][1][1] = 2 * b[i][0] * c[0][0] * c[0][1] + 2 * c[1][0] * c[1][1] + b[i][1] * (c[0][0] * c[1][1] + c[0][1] * c[1][0]);
        H[i][1][2] = b[i][0] * (c[0][1] * c[0][1] + 2 * c[0][0] * c[0][2]) + 2 * c[1][0] * c[1][2]
            + c[1][1] * c[1][1] + b[i][1] * (c[0][2] * c[1][0] + c[0][0] * c[1][2] + c[0][1] * c[1][1]);
        H[i][1][3] = 2 * b[i][0] * c[0][1] * c[0][2] + 2 * c[1][1] * c[1][2]
            + b[i][1] * (c[0][1] * c[1][2] + c[0][2] * c[1][1]);
        H[i][1][4] = b[i][0] * c[0][2] * c[0][2] + c[1][2] * c[1][2] + b[i][1] * c[0][2] * c[1][2];
    }
}




double btw20lgHs(double f, double *p, int *N, double *fc)
{
    int i, M;
    double tem;
    COMPLEX s, *a;
    M = N[0];
    //a=(COMPLEX*)malloc((unsigned)M*sizeof(COMPLEX));
    a = new COMPLEX[M];
    for (i = 0; i < M; i++)a[M - 1 - i] = COMPLEX(p[i], 0.0);
    s = COMPLEX(0.0, f / fc[0]);
    tem = 20 * log10(1 / abs(polyval(a, M, s)));
    //free(a);
    delete a;
    return tem;
}

double btw20lgHs(double f, double fc, double p[], int N)
{
    int i, M;
    double tem;
    COMPLEX s, *a;
    M = N;
    //a=(COMPLEX*)malloc((unsigned)M*sizeof(COMPLEX));
    a = new COMPLEX[M];
    for (i = 0; i < M; i++)a[M - 1 - i] = COMPLEX(p[i], 0.0);
    s = COMPLEX(0.0, f / fc);
    tem = 20 * log10(1 / abs(polyval(a, M, s)));
    //free(a);
    delete a;
    return tem;
}


double btw20lgHz(double f, double *fs, double H[][2][5], int *L)
{
    double wT, sum;
    int i;

    wT = 8 * f*atan(1.0) / fs[0];
    COMPLEX z_1(cos(wT), -sin(wT));

    for (sum = 0, i = 0; i < L[0]; i++)
    {
        sum = sum + 20 * log10(abs(
            polyval(&(H[i][0][0]), 5, z_1)
            / polyval(&(H[i][1][0]), 5, z_1)
            ));
    }
    return sum;
}

double btw20lgHz(double f, double fs, double H[][2][5], int L)
{
    double wT, sum;
    int i;

    wT = 8 * f*atan(1.0) / fs;
    COMPLEX z_1(cos(wT), -sin(wT));

    for (sum = 0, i = 0; i < L; i++)
    {
        sum = sum + 20 * log10(abs(
            polyval(&(H[i][0][0]), 5, z_1)
            / polyval(&(H[i][1][0]), 5, z_1)
            ));
    }
    return sum;
}

////////////////////
void firWin(int winType, double w[], int N)
{
    int i; double pi;
    if (winType<RECTANG || winType>BLACKMAN)
    {
        printf("filter window type input error in filerWin()");
        _getch(); exit(1);
    }
    pi = 4 * atan(1.0);
    if (winType == RECTANG)
    {
        for (i = 0; i < N; i++)w[i] = 1.0;
    }
    else if (winType == BARTLETT)
    {
        for (i = 0; i < N; i++)w[i] = 1.0 - fabs(1 - 2 * i / (N - 1.0));
    }
    else if (winType == HANNING)
    {
        for (i = 0; i < N; i++)w[i] = 0.5*(1.0 - cos(2 * i*pi / (N - 1)));
    }
    else if (winType == HAMMING)
    {
        for (i = 0; i < N; i++)w[i] = 0.54 - 0.46*cos(2 * i*pi / (N - 1));
    }
    else if (winType == BLACKMAN)
    {
        for (i = 0; i < N; i++)
            w[i] = 0.42 - 0.5*cos(2 * i*pi / (N - 1)) + 0.08*cos(4 * i*pi / (N - 1));
    }
}

void firDesgin(double b[], int L, int bandType, int winType, double fs, double fL, ...)
{
    int i, M, lim, mid;
    double pi, wc1, wc2, *w, s;

    va_list arg; va_start(arg, fL);
    double fH = va_arg(arg, double);

    pi = 4 * atan(1.0);
    for (i = 0; i < L; i++)b[i] = 0.0;

    M = L - 1;
    lim = (int)((L - 1) / 2);
    if (M / 2.0 == lim){ mid = lim; lim = lim - 1; }
    else mid = 0;

    /*here:f0=fs/2 ,so mormalization as fellow */
    wc1 = pi*fL / (fs / 2); if (bandType >= BANDPASS)wc2 = pi*fH / (fs / 2);

    w = (double*)malloc((unsigned)L*sizeof(double));
    firWin(winType, w, L);

    if (bandType == LOWPASS)
    {
        for (i = 0; i <= lim; i++)
        {
            s = i - M / 2.0;
            b[i] = w[i] * sin(wc1*s) / (pi*s);
            b[L - 1 - i] = b[i];
        }
        if (mid)b[mid] = wc1 / pi;
    }
    else if (bandType == HIGHPASS)
    {
        for (i = 0; i <= lim; i++)
        {
            s = i - M / 2.0;
            b[i] = w[i] * (sin(pi*s) - sin(wc1*s)) / (pi*s);
            b[L - 1 - i] = b[i];
        }
        if (mid)b[mid] = 1 - wc1 / pi;
    }
    else if (bandType == BANDPASS)
    {
        for (i = 0; i < lim; i++)
        {
            s = i - M / 2.0;
            b[i] = w[i] * (sin(wc2*s) - sin(wc1*s)) / (pi*s);
            b[L - i - 1] = b[i];
        }
        if (mid)b[mid] = (wc2 - wc1) / pi;
    }
    else if (bandType == BANDSTOP)
    {
        for (i = 0; i < lim; i++)
        {
            s = i - M / 2.0;
            b[i] = w[i] * (sin(wc1*s) + sin(pi*s) - sin(wc2*s)) / (pi*s);
            b[L - i - 1] = b[i];
        }
        if (mid)b[mid] = (pi + wc1 - wc2) / pi;
    }
    free((double*)w);
}


double firAbs(double f, double fs, double b[], int L)
{
    double WT = 4 * atan(1.0)*f / (fs / 2);
    COMPLEX sum = polyval(b, L, COMPLEX(cos(WT), -sin(WT)));
    return abs(sum);
}


double firPhase(double f, double fs, double b[], int L)
{
    double WT = 4 * atan(1.0)*f / (fs / 2);
    COMPLEX sum = polyval(b, L, COMPLEX(cos(WT), -sin(WT)));
    return atan2(sum.i, sum.r);
}


/////////////////多项式laguerre拉盖尔法求根///////////////////////
void polyLag(COMPLEX a[], int m, COMPLEX *x, double r_err, int polish)
/*9:just gets one root of the complex polynomial by Laguerre's way
it called by rt_lgs()*/
#define MAXIT 100
#define EPSS 6.0e-8
//dcomplex a[];
//dcomplex *x;
//int m,polish;
//double r_err;
{
    int j, iter;
    double err, abx;
    COMPLEX p, p1, p2, G, G2, H, gp, gm, sq, dx, x1;
    //void promtError();
    for (iter = 1; iter <= MAXIT; iter++)
    {
        //p1=p2=complex(0.0,0.0);
        for (p = a[0], abx = abs(*x), err = abs(p), j = 1; j <= m; j++){
            p2 = *x*p2 + p1;
            p1 = *x*p1 + p;
            p = *x*p + a[j];
            err = abs(p) + abx*err;
        }

        abx = abs(p); err *= EPSS;
        if (abx <= err)return;//如果相对误差满足EPPS则终止。
        G = p1 / p;
        G2 = G*G;
        H = G2 - 2.0*p2 / p;
        sq = sqrt((m*H - G2)*(m - 1));
        gp = G + sq;
        gm = G - sq;
        if (abs(gp) < abs(gm))gp = gm;//模大复数放在gp中。
        dx = m / gp;//COMPLEX((double)m,0.0)/gp;
        x1 = *x - dx;
        if ((*x).r == x1.r && (*x).i == x1.i)return;//当dx很小，被*x-dx忽略时，终止。
        *x = x1;
        if (!polish)
            if (abs(dx) <= r_err*abs(*x))return;
    }
    printf("Too many iterations in routine RT_LG1()");
}




/////////////积分函数/////////////////////////////////////

double iTrapz(double a, double b, int N, double(*func)(double))
{
    double s, sum, del, x, it = 1.0; int i, j;
    s = (*func)(a)+(*func)(b), s = 0.5*(b - a)*s;
    for (i = 2; i <= N; i++)
    {
        del = (b - a) / it, x = a + 0.5*del, sum = 0.0, j = 1;
        for (; j <= it; j++, x += del)sum += (*func)(x);
        s = 0.5*(s + sum*del); it = it + it;
    }
    return s;
}


double iTrap(double a, double b, double(*func)(double))
{
    double y, oldy; int kk;
    oldy = iTrapz(a, b, 1, func);
    y = iTrapz(a, b, 2, func);
    for (kk = 3; (fabs(y - oldy) >= fabs((1.0e-6)*y)) && kk < 20; kk++)
    {
        oldy = y; y = iTrapz(a, b, kk, func);
    }
    return y;
}



////wal函数
double wal(double t, int n)
//19
{
    int p2r = 1, wl = 1;
    double q;
    t = t - (int)t;/*t into [0,1)*/
    if (t < 0)t = 1.0 + t;
    do{
        q = fmod(p2r*t, 2.0);
        if (n & 1 == 1 && 0.5 < q&&q < 1.5)wl = -wl;
        n >>= 1; p2r <<= 1;
    } while (n != 0);
    return (double)wl;
}

double Wal(double t, int n[1])
//20
{
    return wal(t, n[0]);
}


double* WAL(int N)
//21
{
    int M, p, q, a, i, k, *b, m, n;
    double *c, cl, ch;
    /*when M=0,1,p=1;*/
    for (M = N, p = 1; M > 1; M >>= 1, p <<= 1);
    b = (int*)malloc((unsigned)p*sizeof(int));
    for (b[0] = 1, i = 1; i < p; i++, i++)
    {
        b[i - 1] = b[0]; b[i] = -b[0]; b[0] = -b[0];
    }
    b[0] = 1;
    for (M = N, q = 1; M > 1; M >>= 1, q <<= 1)
    {
        if (M & 1 == 1)
        {
            a = p / q;
            m = 1;
            n = 3;
            for (; m < q; m += 4, n += 4)
            {
                i = m*a;
                k = n*a;
                k = (k < p) ? k : p;
                for (; i < k; i++)b[i] = -b[i];
            }
        }
    }
    c = (double*)malloc((unsigned)(N + N + 2)*sizeof(double));
    cl = 0;
    for (M = b[0], k = 0, i = 1; i < p; i++)
    {
        if (M != b[i]){
            ch = (double)(i) / (2 * p);
            if (M == 1){ c[k + 1] = ch; c[k] = cl; }
            else{ c[k + 1] = cl; c[k] = ch; }
            cl = ch;
            M = b[i]; k++; k++;
        }
    }
    ch = 0.5;
    if (M == 1){ c[k + 1] = ch; c[k] = cl; }
    else{ c[k + 1] = cl; c[k] = ch; }
    k++; k++;
    if ((N & 1) == 0){
        k--; k--;
        if (c[k + 1] > c[k]){ c[k + 1] = c[k + 1] + c[k + 1] - c[k]; }
        else{ c[k] = c[k] + c[k] - c[k + 1]; }
        i = k - 1;
        k++; k++;
        for (; i >= 0; i--, k++){ c[k] = 1 - c[i]; }
    }
    else{
        for (i = k - 1; i >= 0; i--, i--, k++, k++)
        {
            c[k] = 1 - c[i - 1];
            c[k + 1] = 1 - c[i];
        }
    }
    free(b);
    return c;
}

//walsh级数和Foureie级数



double WalSer(double t, double *T, double a[], int n[1])
//23
{
    int i;
    double sum;
    for (sum = 0.0, i = 0; i < n[0]; i++)
    {
        sum = (wal(t / T[0], i) > 0) ? sum + a[i] : sum - a[i];
    }
    return sum;
}

double WalSer(double t, double T, double a[], int n)
//23
{
    int i;
    double sum;
    for (sum = 0.0, i = 0; i < n; i++)
    {
        sum = (wal(t / T, i) > 0) ? sum + a[i] : sum - a[i];
    }
    return sum;
}

//,10,sin,&k,&T,
//		fx,arg1,arg2,arg3,arg4,arg5);


double FouSer(double t, double *T, double a[], double b[], int *N)
{
    int i; double sum;
    for (sum = 0, i = 0; i < N[0]; i++)
        sum += a[i] * cos(i*M_PI * 2 / T[0] * t) + b[i] * sin(i*M_PI * 2 / T[0] * t);
    return sum;
}


double FouSer(double t, double T, double a[], double b[], int N)
{
    int i; double sum;
    for (sum = 0, i = 0; i < N; i++)
        sum += a[i] * cos(i*M_PI * 2 / T*t) + b[i] * sin(i*M_PI * 2 / T*t);
    return sum;
}
