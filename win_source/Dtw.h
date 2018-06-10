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


#define MAXCHARS		255		// 현재 경로 검색을 위한 문자열 배열 길이
#define DBTABLECOUNT	5		// DB 테이블 개수
#define MAXCHARCOUNT	30
#define NUMBERCOUNT		10		// 숫자 갯수
#define ENGLISHCOUNT	26		// 영문자 갯수
#define HANGULCOUNT		16		// 한글 갯수
#define DBCOUNT			200		// 데이터베이스에 저장된 데이터 갯수
#define INPUTCOUNT		200		
#define NUMBER	0
#define ENGLISH 1
#define HANGUL	2
//////////////////////////////////////////////////////////////////////////
// Variable
//////////////////////////////////////////////////////////////////////////

// DTW 처리된 x, y, z 좌표
typedef struct processing_data
{
	double x;
	double y;
	double z;
} processing_data;

HANDLE handle;

int Number[DBTABLECOUNT+1];
int Number_cnt = 1;
int last[MAXCHARCOUNT];							//투표 마지막
double total_value[MAXCHARCOUNT];				//각숫자끼리의 거리값을 더해서 나중에 5로나눈후 제일 작은값을찾음
double comresult[MAXCHARCOUNT*DBTABLECOUNT+1];
int comcount = 1;

//////////////////////////////////////////////////////////////////////////
// Operation
//////////////////////////////////////////////////////////////////////////

// LCS 5번 국부 연속제약 (끝점 DTW) - 백트래킹을 할 필요가 없으며 속도가 빠르고 계산량도 줄어든다
double dtw1(long int, long int, processing_data *, processing_data *);
double dtw2(long int, long int, processing_data *, processing_data *);
double dtw3(long int, long int, processing_data *, processing_data *);

// DB의 좌표값과 입력데이터의 좌표값의 길이를 구하는 함수.
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