#ifndef _LOWPASSFILTER_H
#define _LOWPASSFILTER_H

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <commctrl.h>
#include "Recognize.h"

#define FILTER_LENGTH		21			// ���� ���
#define MAX_FILTER_LENGTH	1000		// ������ �ִ� ����	

//////////////////////////////////////////////////////////////////////////
// Variable
//////////////////////////////////////////////////////////////////////////

// �Է� x, y, z ��ǥ
typedef struct
{
	int x, y, z;
} coordinate1;										

// ���� ó���� x, y, z ��ǥ
typedef struct 
{
	double x;
	double y;
	double z;
} filtered_coordinate;

// ���͸� ������ ����
int cnt_filtered_data = 0;		

//////////////////////////////////////////////////////////////////////////
// Operation
//////////////////////////////////////////////////////////////////////////

// ����ó���ϴ� �Լ�.
void fun_filter(int *data, double *result, int count);			

#endif