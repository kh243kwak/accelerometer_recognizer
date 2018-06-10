#include "Recognize.h"

extern int file_load(int, processing_data *, int);
extern HWND hWnd;


int vowel[5] = {0,};
int count = 0;
int consonant = 0;	
int flag = 0;
HWND Front_Parent, Front_Child;

void recognize(int raw_count, coordinate *input_data, int mode, BOOL spot)
{
	int number, filter_cnt, normal_cnt, regul_cnt, i;
	processing_data *data, *final_data;

	if(spot == FALSE)
	{
		data = (processing_data *)malloc(sizeof(processing_data) * raw_count);
		final_data = (processing_data *)malloc(sizeof(processing_data) * LENGTH);		
		
		// LFP 
		filter_cnt = precedure_filtering(raw_count, input_data, data);

		// normalization
		normal_cnt = normalization(filter_cnt, data);
		
		// regulation
		regul_cnt = regulation(normal_cnt, data, final_data);
		
		// DTW Alorightm
		number = file_load(regul_cnt, final_data, mode);
		
		for(i = 0; i < raw_count; i++)
		{
			input_data[i].x = 0;
			input_data[i].y = 0;
			input_data[i].z = 0;
		}
		
		for(i = 0; i < raw_count; i++)
		{
			data[i].x = 0.0;
			data[i].y = 0.0;
			data[i].z = 0.0;
		}
		for(i = 0; i < regul_cnt; i++)
		{
			final_data[i].x = 0.0;
			final_data[i].y = 0.0;
			final_data[i].z = 0.0;
		}
		
		free(data);
		free(final_data);
	}

	resultprint(number, mode, spot);
}

// Print out recognition result 
void resultprint(int number, int mode, BOOL spot)
{
	Front_Parent = GetForegroundWindow();
	Front_Child = FindWindowEx(Front_Parent, GetFocus(), NULL, NULL);
	if(Front_Child == NULL)
		MessageBox(hWnd, "에디트를 찾을 수 없습니다.", "Error", MB_OK);

	init_context(mode);
		
	if(mode == NUMBER)
	{
		number_print(number);
	}		
	else if(mode == ENGLISH)
	{
		english_print(number);
	}	
	else if((mode == HANGUL) && !spot)
	{		
		consonant_print(number);
	}
 	else if((mode == HANGUL) && spot)
 	{
		consonant = 0;
 		vowel[count++] = 1;	
 	}	

	vowel_composition(number);
}

void init_context(int mode)
{
	HIMC hIMC;
	DWORD dwConversion;
	DWORD dwSentence;

	if(!(hIMC = ImmGetContext(Front_Parent)))
		MessageBox(hWnd, "Error", "Error", MB_OK);
 			
	ImmGetConversionStatus( hIMC, &dwConversion, &dwSentence);

	if(mode == HANGUL)
	{
		if(!(dwConversion & IME_CMODE_HANGEUL)) 
			keybd_event(VK_HANGEUL, 0, 0, 0);	
	}
	else if(mode == ENGLISH)
	{
		if((dwConversion & IME_CMODE_HANGEUL)) 
			keybd_event(VK_HANGEUL, 0, 0, 0);	
	}

	ImmReleaseContext(Front_Parent, hIMC);
}

void number_print(int number)
{

	SHORT key = VkKeyScan('0');

	keybd_event(key + number, 0, KEYEVENTF_EXTENDEDKEY, 0);
	keybd_event(key + number, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
}

void english_print(int number)
{

	SHORT key = VkKeyScan('A');
	
	keybd_event(VK_SHIFT, 0, KEYEVENTF_EXTENDEDKEY, 0);					
	keybd_event(key + (number-1), 0, KEYEVENTF_EXTENDEDKEY, 0);
	keybd_event(key + (number-1), 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
	keybd_event(VK_SHIFT, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);		
}

void consonant_print(int number)
{
    // This function was Korean Consonant
}

void vowel_composition(int number)
{
	// This function was Korean Vowel 
}

void push_left()
{
	int i;

	consonant = 0;

	for(i=0; i<=4; i++)
		vowel[i] = 0;
	count = 0;

	keybd_event(VK_LEFT, 0, 0, 0);
	keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);
}

void push_right()
{
	int i;

	consonant = 0;

	for(i=0; i<=4; i++)
		vowel[i] = 0;
	count = 0;

	keybd_event(VK_RIGHT, 0, 0, 0);
	keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);
}