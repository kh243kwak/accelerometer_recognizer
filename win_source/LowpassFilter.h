#ifndef _LOWPASSFILTER_H
#define _LOWPASSFILTER_H

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <commctrl.h>
#include "Recognize.h"

#define FILTER_LENGTH		21			// 필터 계수
#define MAX_FILTER_LENGTH	1000		// 데이터 최대 갯수	

//////////////////////////////////////////////////////////////////////////
// Variable
//////////////////////////////////////////////////////////////////////////

// 입력 x, y, z 좌표
typedef struct
{
	int x, y, z;
} coordinate1;										

// 필터 처리된 x, y, z 좌표
typedef struct 
{
	double x;
	double y;
	double z;
} filtered_coordinate;

// 필터링 데이터 갯수
int cnt_filtered_data = 0;		

//////////////////////////////////////////////////////////////////////////
// Operation
//////////////////////////////////////////////////////////////////////////

// 필터처리하는 함수.
void fun_filter(int *data, double *result, int count);			

#endif