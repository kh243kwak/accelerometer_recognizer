//////////////////////////////////////////////////////////////////////////
// Regulation Data
// Author : Jinsub Lee and Kuenhwan Kwak.
//////////////////////////////////////////////////////////////////////////

#include "Regulation.h"


int regulation(int count, processing_data *data, processing_data *final_data)
{

	processing_data output[200];
	int i;


	double rate;
	
	LVITEM LI;	
	LI.mask = LVIF_TEXT;


	number_of_regulated_data = count;


	for(i = 0 ; i < LENGTH ; i++)
	{
		output[i].x = 0;
		output[i].y = 0;
		output[i].z = 0;
	}

	rate = (double)LENGTH / (double)number_of_regulated_data;

	output[0] = data[0];
	output[LENGTH-1] = data[number_of_regulated_data-1];
    
	for(i = 0 ; i < number_of_regulated_data; i++)
	{
		output[(int(i * rate))].x = data[i].x;
		output[(int(i * rate))].y = data[i].y;
		output[(int(i * rate))].z = data[i].z;
	}  
    
	for(i = 0 ; i < LENGTH ; i++)
	{	

		if(output[i].x == 0) 
		{
			int before_index = i;
			int after_index = i; 


			double before_value_x;               
			double after_value_x;                
			
			double before_value_y;               
			double after_value_y;

			double before_value_z;
			double after_value_z;


			while(1)
			{
				if(output[before_index].x != 0)
					break;
				before_index--;
			}


			while(1)
			{
				if(output[after_index].x != 0)
					break;
				after_index++;
			}                                  

			before_value_x = output[before_index].x;
			after_value_x = output[after_index].x;			

			before_value_y = output[before_index].y;
			after_value_y = output[after_index].y;	

			before_value_z = output[before_index].z;
			after_value_z = output[after_index].z;   

			output[i].x = linear_interpol(before_index,after_index,before_value_x,after_value_x, i);
			output[i].y = linear_interpol(before_index,after_index,before_value_y,after_value_y, i);
			output[i].z = linear_interpol(before_index,after_index,before_value_z,after_value_z, i);
                                                  			
		}
	}

	for(i = 0; i < LENGTH; i++)
	{
		final_data[i].x = output[i].x;
		final_data[i].y = output[i].y;
		final_data[i].z = output[i].z;
	}             	
	return LENGTH;
}

double linear_interpol(int x1,int x2, double y1, double y2, double x)
{
	double result;

	result = (y2-y1)/(x2-x1)*(x-x1) + y1;

	return result;	
}
