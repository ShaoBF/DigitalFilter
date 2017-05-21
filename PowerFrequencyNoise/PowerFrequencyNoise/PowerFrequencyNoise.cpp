#pragma once 
#pragma execution_character_set("utf-8")

#include <stdio.h>
#include <stdlib.h>
#include "x_math.h"
#include "x_graph.h"

#define MAXCOUNT 128
#define MAX_SIZE 32768

extern WIN2 win2;
extern WIN3 win3;

int fs = 0, SegCount = 0, SegLength = 1024;
bool debug=false;
double StrToNum(char* str);
void FIRBS(double H[], double fp1, double fp2, double fs1, double fs2, double fs, int n, int length);
/*以下来自SBF14*/
void PrintH(double H[10][2][5], int L){
    int i, j, k;
    for (i = 0; i < L; i++){
        printf("h%d=\n", i);
        for (j = 0; j < 2; j++){
            for (k = 0; k < 5; k++){
                printf("%.3f ", H[i][j][k]);
            }
            printf("\n");
        }
    }
}

void IIR(double H[][2][5], int bandType, double db1, double db2, double fs, double f1, double f2, double f3, double f4){
    int order, L;
    double b[10][2], c[2][3];
    //double H[10][2][5]; 

    order = btwOrder(bandType, db1, db2, fs, f1, f2, f3, f4);

    L = btwAf(b, order);

    btwC23(c, bandType, order, db1, fs, f1, f2, f3, f4);

    btwAf2Df(H, L, b, c);
    if (debug == true){
        printf("order=%d\n", order);
        printf("L=%d\n", L);
        printf("b=\n");
        for (int j = 0; j < 2; j++){
            for (int i = 0; i < order / 2; i++){
                printf("%.3f ",b[i][j]);
            }
            printf("\n");
        }
        PrintH(H, L);
    }
    _getch();
}
void Resp(COMPLEX Y[], COMPLEX X[], double H[][2][5]){
    COMPLEX HF[MAX_SIZE];
    COMPLEX zj;
    double omega = -2 * M_PI / MAX_SIZE;
    double *a = &H[0][0][0];
    double *b = &H[0][1][0];
    COMPLEX num = 0, den = 0;
    //使用H数组的系数计算相应点数的频域系统函数HF(n)
    for (int i = 0; i < MAX_SIZE; i++){
        num = 0;
        den = 0;
        //H(z)=sum(a[i]*z^i)/sum(b[i]*z^i),z=e^(-jwn)
        //HF = (H[0][0][0] + H[0][0][1] * z + H[0][0][2] * z*z + H[0][0][3] * z*z*z + H[0][0][4] * z*z*z*z) /
        //    (H[0][1][0] + H[0][1][1] * z + H[0][1][2] * z*z + H[0][1][3] * z*z*z + H[0][1][4] * z*z*z*z);
        for (int j = 0; j < 5; j++){
            zj = COMPLEX(cos(omega*i*j), sin(omega*i*j));
            num += a[j] * zj;
            den += b[j] * zj;
        }
        HF[i] = num / den;
        Y[i] = X[i] * HF[i];
    }
}

void GetDataFromFile(char* filePath, int& fs, int& SegCount, double ecd[]){
    FILE *fp1;//定义文件流指针，用于打开读取的文件
    char text[1024];//定义一个字符串数组，用于存储读取的字符
    fp1 = fopen(filePath, "r");//只读方式打开文件

    fgets(text, MAXCOUNT, fp1);//读取第一行——抽样频率
    fs = (int)StrToNum(text);
    fgets(text, MAXCOUNT, fp1);//读取第二行——数据段数
    SegCount = (int)StrToNum(text);
    for (int i = 0; fgets(text, MAXCOUNT, fp1) != NULL; i++)//逐行读取fp1所指向文件中的内容到text中
    {
        ecd[i] = StrToNum(text);
        //printf("%.5f\n",ecd[i]);//输出到屏幕
        //fputs(text, fp2);//将内容写到fp2所指向文件中
    }
    fclose(fp1);//关闭文件a.txt，有打开就要有关闭
}

void SaveData(char* filePath, COMPLEX ecdf[], int SegCount, int SegLength){
    FILE *fp;//定义文件流指针，用于打开写操作的文件
    int total = SegCount*SegLength;
    fp = fopen(filePath, "w");//写方式打开文件
    for (int i = 0; i<total; i++)//逐行读取fp1所指向文件中的内容到text中
    {
        fprintf(fp,"%.5f\n",ecdf[i].r);//输出到文件
        //fputs(text, fp);//将内容写到fp2所指向文件中
    }
    fclose(fp);//关闭文件b.txt
}

void main(int argC, char* argV){
    FILE *fp2;//定义文件流指针，用于打开写操作的文件
    char text[1024];//定义一个字符串数组，用于存储读取的字符
    double ecd[MAX_SIZE];
    COMPLEX ECD[MAX_SIZE];
    COMPLEX ECDF[MAX_SIZE];
    COMPLEX ecdf[MAX_SIZE];
    double H[10][2][5];
    double b[100];
    char* filePath = "d:\\ShaoBF\\DataForSignal\\119_heart1.txt";
    //获取文件数据
    GetDataFromFile(filePath, fs, SegCount, ecd);

    debug = true;

    //计算所需的IIR滤波器函数系数H
    int bandType = BANDSTOP;//LOWPASS;//BANDPASS;HIGHPASS;//
    double db1 = 3;
    double db2 = 15;
    double f1 = 45;
    double f2 = 50;
    double f3 = 60;
    double f4 = 65;
    IIR(H, bandType, db1, db2, fs, f3, f4, f1, f2);

    //输入信号做FFT得到ECD（n）。
    for (int i = 0; i < SegCount*SegLength; i++){
        ECD[i] = ecd[i];
    }
    for (int i = SegCount*SegLength; i < MAX_SIZE; i++){
        ECD[i] = 0;
    }
    fft(ECD, MAX_SIZE, 1);


    //ECDF=ECD×h
    Resp(ECDF, ECD, H);

    //ECDF做IFFT得到ecdf即为所求
    fft(ecdf, MAX_SIZE, -1);

    //ecdf输出到文件
    filePath = "d:\\ShaoBF\\FileTest\\b.txt";
    SaveData(filePath, ecdf, SegCount, SegLength);
    system("pause");
    return;
}

/*
 * Swap string to number(double)
 *
 * @return The result number
 *
 * @param str the string
 */
double StrToNum(char* str){
    double num=0;
    bool negative = false;
    bool fraction = false;
    int decimal = 1;
    for (int i = 0; str[i] != '\n'; i++){
        if (str[i] == '-'){
            negative = true;
        }
        else if (str[i] == '.'){
            fraction = true;
        }
        else{
            if (!fraction){
                num = num * 10 + (str[i] - '0');
            }
            else{
                double number = str[i] - '0';
                number /= pow(10.0, decimal++);
                num += number;
            }
        }
    }
    if (negative)
        num = -num;
    return num;
}