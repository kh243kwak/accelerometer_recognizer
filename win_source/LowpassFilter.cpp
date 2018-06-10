//////////////////////////////////////////////////////////////////////////
// 수신받은 데이터를 필터링하는 부분.
// 2007년 8월 2일 황승희 구현.
// 2007년 8월 10일 곽근환 수정.
// 2007년 9월 4일 이진섭 수정.
//////////////////////////////////////////////////////////////////////////

#include "LowpassFilter.h"

//////////////////////////////////////////////////////////////////////////
// 전처리 필터링 함수
// 입력데이터 개수, 입력데이터, 출력데이터
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

// 필터처리하는 함수.
// 입력데이터, 출력데이터, 데이터 갯수
void fun_filter(int *data, double *result, int count)
{
	int sampling = count;
	int i=0, j=0;
	double temp = 0.0, average=0.0;
	double input[FILTER_LENGTH];
	
	// 필터계수
	// 제일 완만한 값
	double Filter[FILTER_LENGTH] = {
		0.03283474219465,  0.03309835783034,  0.03825692367928,  0.04318363965487,
			0.04875391126805,  0.05084948296643,  0.05136242453101,  0.05319881528978,
			0.05828198139189,  0.05955515807612,  0.06118347107438,  0.05955515807612,
			0.05828198139189,  0.05319881528978,  0.05136242453101,  0.05084948296643,
			0.04875391126805,  0.04318363965487,  0.03825692367928,  0.03309835783034,
			0.03283474219465
	};
	
	// 필터처리를 위한 동적배열 할당
	//input=(double*)malloc(sizeof(double)*FILTER_LENGTH);
	
	/* =======================================================================
	필터처리는 주파수영역에서 통과시키고 싶지않은 주파수를 구해 
	필터계수라는 상수값을 곱하여 필터처리를 한다. 
	하지만 우리는 시간 영역에서의 신호값을 받기 때문에 FFT를 통하여 
	시간 영역의 값을 주파수영역으로 바꿔야하지만..
	convolution을 이용하면 주파수영역에서의 곱과 같은 효과를 
	얻을 수 있다. 그래서 우리는 convolution을 사용하여
	사간영역에서의 신호값으로 필터처리를 하였다.
    ======================================================================= */
	
	// 필터 계수만큼 Convolution시킴.
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
	
	// 쓰레기값 제거
	j=FILTER_LENGTH;
	for(i=j; i<sampling; i++)
	{
		result[i-j] = result[i];
	}
	
	// 진폭 증가 소스 /////////////////////////////////////////////////////////
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

