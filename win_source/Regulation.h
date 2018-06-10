#ifndef _REGULATION_H
#define _REGULATION_H

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <commctrl.h>
#include "Recognize.h"

#define LENGTH 200 

//////////////////////////////////////////////////////////////////////////
// Variable
//////////////////////////////////////////////////////////////////////////

struct coordinate_regulated{                                    
	double x;
	double y;
	double z;
};

int number_of_regulated_data;                          

//////////////////////////////////////////////////////////////////////////
// Operation
//////////////////////////////////////////////////////////////////////////

double linear_interpol(int x1,int x2, double y1, double y2, double x);

#endif