#include "dtw.h"

#define FILTER_LENGTH	21							


void fun_filter(long int Coordi_Cnt, struct coordi * Coordi_Arr)
{
	int i=0, j=0; //, a, b, c;
	double tempx = 0.0, tempy = 0.0, tempz = 0.0, average=0.0;
	double *inputx; double *inputy; double *inputz;

	// filter coefficient array
	double Filter[FILTER_LENGTH] = {
    0.03283474219465,  0.03309835783034,  0.03825692367928,  0.04318363965487,
    0.04875391126805,  0.05084948296643,  0.05136242453101,  0.05319881528978,
    0.05828198139189,  0.05955515807612,  0.06118347107438,  0.05955515807612,
    0.05828198139189,  0.05319881528978,  0.05136242453101,  0.05084948296643,
    0.04875391126805,  0.04318363965487,  0.03825692367928,  0.03309835783034,
    0.03283474219465
	};


	// Allocation filter data for calculate
	inputx=(double*)malloc(sizeof(double)*FILTER_LENGTH);
	inputy=(double*)malloc(sizeof(double)*FILTER_LENGTH);
	inputz=(double*)malloc(sizeof(double)*FILTER_LENGTH);

	
	for(i=0; i<Coordi_Cnt; i++)
	{ 
		  tempx=0; tempy=0;  tempz=0; 
		  

		  if(i<FILTER_LENGTH)
		  { 
			  inputx[i]=Coordi_Arr[i].x; 
			  inputy[i]=Coordi_Arr[i].y;
			  inputz[i]=Coordi_Arr[i].z; 
			  for(j=0;j<=i;j++)
			  {
				  tempx+=inputx[i-j]*Filter[j]; 
				  tempy+=inputy[i-j]*Filter[j]; 
				  tempz+=inputz[i-j]*Filter[j];
			  }
		 }

		  else
		  {
			  for(j=0;j<FILTER_LENGTH-1;j++)
			  {
				  inputx[j]=inputx[j+1]; 
				  inputy[j]=inputy[j+1]; 
				  inputz[j]=inputz[j+1]; 
			  }
				  
			  inputx[FILTER_LENGTH-1]=Coordi_Arr[i].x; 
			  inputy[FILTER_LENGTH-1]=Coordi_Arr[i].y; 
			  inputz[FILTER_LENGTH-1]=Coordi_Arr[i].z; 
				  
			  for(j=0;j<FILTER_LENGTH;j++) 
			  {
				  tempx+=inputx[j]*Filter[j]; 
				  tempy+=inputy[j]*Filter[j]; 
				  tempz+=inputz[j]*Filter[j]; 
			  }
		 }
		  
		 Coordi_Arr[i].x = tempx; 
		 Coordi_Arr[i].y = tempy; 
		 Coordi_Arr[i].z = tempz;
    } 
	


	j=FILTER_LENGTH;
	for(i=j; i<Coordi_Cnt; i++)
	{
		Coordi_Arr[i-j].x = Coordi_Arr[i].x;
		Coordi_Arr[i-j].y = Coordi_Arr[i].y;
		Coordi_Arr[i-j].z = Coordi_Arr[i].z;
	}

	free(inputx);
	free(inputy);
	free(inputz);
}

