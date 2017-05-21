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
/*��������SBF14*/
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
    //ʹ��H�����ϵ��������Ӧ������Ƶ��ϵͳ����HF(n)
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
    FILE *fp1;//�����ļ���ָ�룬���ڴ򿪶�ȡ���ļ�
    char text[1024];//����һ���ַ������飬���ڴ洢��ȡ���ַ�
    fp1 = fopen(filePath, "r");//ֻ����ʽ���ļ�

    fgets(text, MAXCOUNT, fp1);//��ȡ��һ�С�������Ƶ��
    fs = (int)StrToNum(text);
    fgets(text, MAXCOUNT, fp1);//��ȡ�ڶ��С������ݶ���
    SegCount = (int)StrToNum(text);
    for (int i = 0; fgets(text, MAXCOUNT, fp1) != NULL; i++)//���ж�ȡfp1��ָ���ļ��е����ݵ�text��
    {
        ecd[i] = StrToNum(text);
        //printf("%.5f\n",ecd[i]);//�������Ļ
        //fputs(text, fp2);//������д��fp2��ָ���ļ���
    }
    fclose(fp1);//�ر��ļ�a.txt���д򿪾�Ҫ�йر�
}

void SaveData(char* filePath, COMPLEX ecdf[], int SegCount, int SegLength){
    FILE *fp;//�����ļ���ָ�룬���ڴ�д�������ļ�
    int total = SegCount*SegLength;
    fp = fopen(filePath, "w");//д��ʽ���ļ�
    for (int i = 0; i<total; i++)//���ж�ȡfp1��ָ���ļ��е����ݵ�text��
    {
        fprintf(fp,"%.5f\n",ecdf[i].r);//������ļ�
        //fputs(text, fp);//������д��fp2��ָ���ļ���
    }
    fclose(fp);//�ر��ļ�b.txt
}

void main(int argC, char* argV){
    FILE *fp2;//�����ļ���ָ�룬���ڴ�д�������ļ�
    char text[1024];//����һ���ַ������飬���ڴ洢��ȡ���ַ�
    double ecd[MAX_SIZE];
    COMPLEX ECD[MAX_SIZE];
    COMPLEX ECDF[MAX_SIZE];
    COMPLEX ecdf[MAX_SIZE];
    double H[10][2][5];
    double b[100];
    char* filePath = "d:\\ShaoBF\\DataForSignal\\119_heart1.txt";
    //��ȡ�ļ�����
    GetDataFromFile(filePath, fs, SegCount, ecd);

    debug = true;

    //���������IIR�˲�������ϵ��H
    int bandType = BANDSTOP;//LOWPASS;//BANDPASS;HIGHPASS;//
    double db1 = 3;
    double db2 = 15;
    double f1 = 45;
    double f2 = 50;
    double f3 = 60;
    double f4 = 65;
    IIR(H, bandType, db1, db2, fs, f3, f4, f1, f2);

    //�����ź���FFT�õ�ECD��n����
    for (int i = 0; i < SegCount*SegLength; i++){
        ECD[i] = ecd[i];
    }
    for (int i = SegCount*SegLength; i < MAX_SIZE; i++){
        ECD[i] = 0;
    }
    fft(ECD, MAX_SIZE, 1);


    //ECDF=ECD��h
    Resp(ECDF, ECD, H);

    //ECDF��IFFT�õ�ecdf��Ϊ����
    fft(ecdf, MAX_SIZE, -1);

    //ecdf������ļ�
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