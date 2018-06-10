//////////////////////////////////////////////////////////////////////////
// ���Ź��� �����͸� ���͸��ϴ� �κ�.
// 2007�� 8�� 2�� Ȳ���� ����.
// 2007�� 8�� 10�� ����ȯ ����.
// 2007�� 9�� 4�� ������ ����.
//////////////////////////////////////////////////////////////////////////

#include "LowpassFilter.h"

//////////////////////////////////////////////////////////////////////////
// ��ó�� ���͸� �Լ�
// �Էµ����� ����, �Էµ�����, ��µ�����
int precedure_filtering(int cnt, coordinate* in_data, processing_data *out_data)
{
	int sampling = cnt;
	int i=0, count=0;					// value
	
	int data[MAX_FILTER_LENGTH];		// save data array
	double result[MAX_FILTER_LENGTH];	// array for file value	
	
	LVITEM LI;	
	LI.mask = LVIF_TEXT;
	
	// initialization array
	for(i=0; i<sampling; i++)
	{
		data[i] = 0;
		result[i] = 0.0;
	}
	
	// save X result
	for(i=0; i<sampling; i++)
		data[i] = in_data[i].x;	

	fun_filter(data, result, cnt);

	
	for(i = 0 ; i < sampling ; i++)
		out_data[i].x = result[i];
	
	// save Y result
	for(i=0; i<sampling; i++)
		data[i] = in_data[i].y;	

	fun_filter(data, result, cnt);

	for(i = 0 ; i < sampling ; i++)
		out_data[i].y = result[i];
	
	// save Z result
	for(i=0; i<sampling; i++)
		data[i] = in_data[i].z;	

	fun_filter(data, result, cnt);

	for(i = 0 ; i < sampling ; i++)
		out_data[i].z = result[i];

	for(i = 0; i< sampling; i++)
	{
		data[i] = 0;
		result[i] = 0;
	}

	return (sampling - FILTER_LENGTH);
}

// ����ó���ϴ� �Լ�.
// �Էµ�����, ��µ�����, ������ ����
void fun_filter(int *data, double *result, int count)
{
	int sampling = count;
	int i=0, j=0;
	double temp = 0.0, average=0.0;
	double input[FILTER_LENGTH];
	
	// ���Ͱ��
	// ���� �ϸ��� ��
	double Filter[FILTER_LENGTH] = {
		0.03283474219465,  0.03309835783034,  0.03825692367928,  0.04318363965487,
			0.04875391126805,  0.05084948296643,  0.05136242453101,  0.05319881528978,
			0.05828198139189,  0.05955515807612,  0.06118347107438,  0.05955515807612,
			0.05828198139189,  0.05319881528978,  0.05136242453101,  0.05084948296643,
			0.04875391126805,  0.04318363965487,  0.03825692367928,  0.03309835783034,
			0.03283474219465
	};
	
	// ����ó���� ���� �����迭 �Ҵ�
	//input=(double*)malloc(sizeof(double)*FILTER_LENGTH);
	
	/* =======================================================================
	����ó���� ���ļ��������� �����Ű�� �������� ���ļ��� ���� 
	���Ͱ����� ������� ���Ͽ� ����ó���� �Ѵ�. 
	������ �츮�� �ð� ���������� ��ȣ���� �ޱ� ������ FFT�� ���Ͽ� 
	�ð� ������ ���� ���ļ��������� �ٲ��������..
	convolution�� �̿��ϸ� ���ļ����������� ���� ���� ȿ���� 
	���� �� �ִ�. �׷��� �츮�� convolution�� ����Ͽ�
	�簣���������� ��ȣ������ ����ó���� �Ͽ���.
    ======================================================================= */
	
	// ���� �����ŭ Convolution��Ŵ.
	for(i=0; i<sampling; i++)
	{ 
		temp=0; 
		
		if(i<FILTER_LENGTH) { 
			input[i]=data[i]; 
			for(j=0;j<=i;j++)
				temp+=input[i-j]*Filter[j]; 
		}
		else {
			for(j=0;j<FILTER_LENGTH-1;j++)
				input[j]=input[j+1]; 
			
			input[FILTER_LENGTH-1]=data[i]; 
			
			for(j=0;j<FILTER_LENGTH;j++) 
				temp+=input[j]*Filter[j]; 
		} 		
		result[i] = temp;
    } 
	
	// �����Ⱚ ����
	j=FILTER_LENGTH;
	for(i=j; i<sampling; i++)
	{
		result[i-j] = result[i];
	}
	
	// ���� ���� �ҽ� /////////////////////////////////////////////////////////
	average = result[0];
	for(i=0; i<sampling; i++)
	{
		result[i] = result[i] - average;
	}
	
	for(i=0; i<sampling; i++)
	{
		result[i] = result[i] * 2 + average;
	}
	//////////////////////////////////////////////////////////////////////////
}

