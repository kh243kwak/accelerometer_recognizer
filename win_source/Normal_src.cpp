//////////////////////////////////////////////////////////////////////////
// Sensor Data Normalization
// Author : Seunghee Hwang, Jinsub Lee, Kuenhwan Kwak
//////////////////////////////////////////////////////////////////////////

#include "Nomalization.h"


int normalization(int cnt_filtered_data, processing_data* data)
{
	int sampling = cnt_filtered_data;
	double * x;
	double * y;
	double * z;
	int i;

	LVITEM LI;	
	LI.mask = LVIF_TEXT;
	
	x = (double*)malloc((sizeof(double))*sampling);
	y = (double*)malloc((sizeof(double))*sampling);
	z = (double*)malloc((sizeof(double))*sampling);
	
	
	for(i = 0 ; i < sampling ; i++)
	{
		x[i] = data[i].x;
		y[i] = data[i].y;
		z[i] = data[i].z;
	}	

	P_rms(x, cnt_filtered_data);
	P_rms(y, cnt_filtered_data);
	P_rms(z, cnt_filtered_data);
	
	for(i = 0 ; i < sampling ; i++)
	{
		data[i].x = x[i] * 300;
		data[i].y = y[i] * 300;
		data[i].z = z[i] * 300;
	}

	free(x);
	free(y);
	free(z);
	
	return sampling;
}

// root mean square 
void P_rms(double *input, int cnt_filtered_data)		
{
	int i=0; 
	int sampling = cnt_filtered_data;

	double sum_pow = 0.0;		
	double rms_val = 0.0;	

	// calculate total square value
	for(i=0; i<sampling; i++)
	{
		sum_pow = sum_pow + pow(input[i],2);
	}

	// P = ¡î[(¢²A[n]^2)/N
	rms_val = sqrt(sum_pow / sampling);

	// normalization
	for(i=0; i<sampling; i++)	
		input[i] = input[i] / rms_val;
}
