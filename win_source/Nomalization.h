#ifndef _NOMALIZATION_H
#define _NOMALIZATION_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#include <commctrl.h>
#include "Recognize.h"

//////////////////////////////////////////////////////////////////////////
// Operation
//////////////////////////////////////////////////////////////////////////

// root mean square ����ϴ� �Լ�
// �Էµ�����, ����ó���� ������ ����
void P_rms(double *input, int cnt_filtered_data);	

#endif