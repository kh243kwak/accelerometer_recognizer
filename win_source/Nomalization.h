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

// root mean square 계산하는 함수
// 입력데이터, 필터처리한 데이터 갯수
void P_rms(double *input, int cnt_filtered_data);	

#endif