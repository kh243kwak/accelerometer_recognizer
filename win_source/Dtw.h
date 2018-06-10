/*******************************************************
	Modify		: 2007. 09. 05 
	Copyright	:  
	Amender		: Hwang Seung Hee
	Name		: DTW - DTW.H
*******************************************************/

#ifndef _DTW_H
#define _DTW_H

#include<windows.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include "resource.h"


#define MAXCHARS		255		// ���� ��� �˻��� ���� ���ڿ� �迭 ����
#define DBTABLECOUNT	5		// DB ���̺� ����
#define MAXCHARCOUNT	30
#define NUMBERCOUNT		10		// ���� ����
#define ENGLISHCOUNT	26		// ������ ����
#define HANGULCOUNT		16		// �ѱ� ����
#define DBCOUNT			200		// �����ͺ��̽��� ����� ������ ����
#define INPUTCOUNT		200		
#define NUMBER	0
#define ENGLISH 1
#define HANGUL	2
//////////////////////////////////////////////////////////////////////////
// Variable
//////////////////////////////////////////////////////////////////////////

// DTW ó���� x, y, z ��ǥ
typedef struct processing_data
{
	double x;
	double y;
	double z;
} processing_data;

HANDLE handle;

int Number[DBTABLECOUNT+1];
int Number_cnt = 1;
int last[MAXCHARCOUNT];							//��ǥ ������
double total_value[MAXCHARCOUNT];				//�����ڳ����� �Ÿ����� ���ؼ� ���߿� 5�γ����� ���� ��������ã��
double comresult[MAXCHARCOUNT*DBTABLECOUNT+1];
int comcount = 1;

//////////////////////////////////////////////////////////////////////////
// Operation
//////////////////////////////////////////////////////////////////////////

// LCS 5�� ���� �������� (���� DTW) - ��Ʈ��ŷ�� �� �ʿ䰡 ������ �ӵ��� ������ ��귮�� �پ���
double dtw1(long int, long int, processing_data *, processing_data *);
double dtw2(long int, long int, processing_data *, processing_data *);
double dtw3(long int, long int, processing_data *, processing_data *);

// DB�� ��ǥ���� �Էµ������� ��ǥ���� ���̸� ���ϴ� �Լ�.
double distance(int, int, processing_data *, processing_data *);
 		
// 
void fileopen(char *); 

//
int file_load(int, processing_data *, int);

//
double Database_Open(char *, int, processing_data *, int);

//
processing_data *CreateDataBase(char *);

#endif