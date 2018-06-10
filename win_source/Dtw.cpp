/*******************************************************
	Modify		: 2007. 09. 05 
	Copyright	: Hyunsub Han 
	Amender		: Seunghee Hwang
	Name		: DTW - DTW.CPP
*******************************************************/

#include "dtw.h"
extern HWND hWnd;

// DTW Recognition 
int file_load(int input_count, processing_data *data, int mode) 
{	
	HWND hListTest = GetDlgItem(hWnd, IDC_LIST_TEST);
	
	char FILEDIR[512];					
	char current_path[MAXCHARS];		

	int i=0, j=0, a=0;
	double min[DBTABLECOUNT+1];			
	double lastmin=0;					
		
	int flag=0;
	int cmpcount=0;						
	int choice[MAXCHARCOUNT];			

	int count = input_count;			
	int char_count, db_count;			

	GetCurrentDirectory(MAXCHARS, current_path);

	if(mode == NUMBER)			char_count = NUMBERCOUNT;
	else if(mode == ENGLISH)	char_count = ENGLISHCOUNT;
	else						char_count = HANGULCOUNT;	
	

	if(mode == ENGLISH)			db_count = DBTABLECOUNT - 1;
	else						db_count = DBTABLECOUNT;

	Number_cnt = 1;
	comcount = 1;

	for(i=0; i <= char_count*db_count; i++)
		comresult[i] = 0;
	
	for(i=0; i <= char_count;i++)	
	{
		last[i] = 0;
		total_value[i] = 0.0;
	}
	//////////////////////////////////////////////////////////////////////////


	for(i=1; i<=db_count; i++)
	{
		char tmp_dir[30];

		if(mode == NUMBER)
		{
			strcpy(FILEDIR, current_path);
			sprintf(tmp_dir, "\\number\\data%d\\", i);
			strcat(FILEDIR, tmp_dir);
		}
		else if(mode == ENGLISH)
		{
			strcpy(FILEDIR, current_path);
			sprintf(tmp_dir, "\\english\\data%d\\", i);
			strcat(FILEDIR, tmp_dir);
		}
		else
		{
			strcpy(FILEDIR, current_path);
			sprintf(tmp_dir, "\\hangul\\data%d\\", i);
			strcat(FILEDIR, tmp_dir);	
		}

		min[i] = Database_Open(FILEDIR, count, data, mode);
	}
//----------------------------------------------------------------------------------------------
	
	for(i=1; i<=char_count;i++)
	{
		total_value[i] /= db_count;
	}
	total_value[0]=total_value[1];
	for(i=1; i<=char_count;i++) 
	{
		if(total_value[0] > total_value[i])
			total_value[0]=total_value[i];
	}
	for(i=1; i<=char_count;i++) 
	{
		if(total_value[0]==total_value[i])
			last[i]+=2;		
	}

//----------------------------------------------------------------------------------------
	lastmin=min[1];
	for(i=1;i<=db_count; i++) 
	{
		if(lastmin > min[i])
			lastmin = min[i];
	}
	
    // Take Vote
	for(j=1, a=0;j<=db_count;j++,a++){
		for(i = 1; i <= char_count; i++) {
			if(min[j] == comresult[i+(char_count*(j-1))])
				choice[j]=i;	
		}
	}

	for(i=1;i<=char_count;i++) 
	{
		for(j=1;j<=db_count;j++)
		{
			if(i==choice[j] && min[j]<=90000) 
				last[i]+=1;				
		}
	}
	for(i=1;i<=db_count; i++)	{
		if(min[i]==lastmin ) 
		{
			a=Number[i];
			last[a]+=1;			
		}		
	}

	last[0]=last[1]; 
	for (i = 1; i <=char_count; i++) {		
		if (last[0] <= last[i]) 
			last[0] = last[i]; 
	}
//---------------------------------------------------------------------------------------------------
		
	for(i=1;i<=char_count;i++) 
	{
		if(last[0]==last[i])
		{
			cmpcount++;
		}
	}
	if(cmpcount==1 && flag!=1) 
	{
		for(i=1;i<=char_count;i++)
		{
			if(last[0]==last[i])
			{				
				if((mode == NUMBER) && (i==10))		return 0;
				return i;		
			}
		}			
	}		
	else 
	{				
		for(i=1;i<=char_count; i++)	{		
			if(total_value[0]==total_value[i])
			{
				if(total_value[0]<=120000){		
					if((mode == NUMBER) && (i==10))	 return 0;
					return i;
				}
				else
				{
					if((mode == NUMBER) && (i==10))	 return 0;
					return -1;
				}
			}
		}			
	}

	return -1;
}


double Database_Open(char *FileFolder, int input_count, processing_data *input_data, int mode)
{	
	int char_count, i;					// 
	double tmp_result1[MAXCHARCOUNT];	// 
	double min = 0;						// 
	processing_data *database_data;		// 
	
	if(mode == NUMBER)			char_count = NUMBERCOUNT;
	else if(mode == ENGLISH)	char_count = ENGLISHCOUNT;
	else						char_count = HANGULCOUNT;
		
	for(i = 1; i <= char_count; i++)
	{
		char number[2];
		char filename[512];
		double result[3]={0,};
		
		
		strcpy(filename, FileFolder);
		sprintf(number, "%d", i);
		strcat(filename, number);
		strcat(filename, ".dat");

		
		database_data = CreateDataBase(filename);
		
		
		result[0] = dtw1(input_count, DBCOUNT, input_data, database_data);
		result[1] = dtw2(input_count, DBCOUNT, input_data, database_data);
		result[2] = dtw3(input_count, DBCOUNT, input_data, database_data);

		tmp_result1[i] = result[0] + result[1] + result[2]; 

		comresult[comcount] = tmp_result1[i];
				
		comcount++;
		
		free(database_data);
	}

	for(i=1;i<=char_count;i++)
	{
		if(tmp_result1[i]<70000) 
		{
			last[i]+=1;
		}
	}

	for(i=1;i<=char_count;i++)
	{
		total_value[i]+=tmp_result1[i];
	}
	
	min=tmp_result1[1];
	for (i = 1; i <=char_count; i++) {		
		if (min > tmp_result1[i]) 
			min = tmp_result1[i]; 		
	}
		
	for(i = 1; i <= char_count; i++) {
		if(min == tmp_result1[i]) {
			Number[Number_cnt] = i;
			break;
		}
	}
	
	Number_cnt++;
	
	
	return min;
}

processing_data *CreateDataBase(char *FileFolder) 
{                  
	FILE *fin;
	char buf[255];
	char sbuf[256];	
	int count = 0;
	int str_len=0, ret=0;
	double x, y, z;
	processing_data *DB_ARRAY;

	fin = fopen(FileFolder, "r");


	DB_ARRAY = (processing_data *)malloc(sizeof(processing_data) * DBCOUNT);
	

	while(fgets( buf, 255, fin ))  
	{		
		if( strcmp( buf, "\n" ) )
		{			
			str_len = strlen(buf);
			buf[str_len-1] = 0;
			sprintf(sbuf, "[%s]", buf);
			ret = sscanf(buf, "%lf %lf %lf", &x, &y, &z);
			

			if(ret == 3)
			{
				DB_ARRAY[count].x = x;
				DB_ARRAY[count].y = y;
				DB_ARRAY[count].z = z;							
				
				count++;
			}
		}  
	}

	fclose(fin);
	
	return DB_ARRAY;

}


double distance(int ref, int typed, processing_data * InData_Arr, processing_data * DBData_Arr) 
{
	double dist=0;
	double square_num1, square_num2;


	square_num1 = pow((InData_Arr[ref].x - DBData_Arr[typed].x), 2);
	square_num2 = pow((InData_Arr[ref].z - DBData_Arr[typed].z), 2);

	dist = sqrt(square_num1 + square_num2);


	return dist;
}

double dtw1(long int InData_Cnt, long int DBData_Cnt, processing_data * InData_Arr, processing_data * DBData_Arr) 
{	
	double M[201][201];
	int i=0, j=0, l=0; 
	int a=0, b=0;
	double min=0;
	double vmax[11];

	double gvalue=0;


	for(i=0; i<=InData_Cnt; i++)
	{
		for(j=0; j<=DBData_Cnt; j++)
		{
			M[i][j]=1;			
		}		
	}

	for(i=0;i<=InData_Cnt;i++)
	{		
			M[i][0]=0;
	}
	for(j=0;j<=DBData_Cnt;j++)
	{		
			M[0][j]=0;
	}
	
	M[1][1]=(distance(0,0, InData_Arr, DBData_Arr)*2);
	
	for(j=1,b=0;j<=DBData_Cnt;)
	{		
		for(i=1,a=0;i<=InData_Cnt;)
		{
			
			if(i<=InData_Cnt && j<=DBData_Cnt)
				vmax[1]=M[i][j];

			if(i+1<=InData_Cnt && j<DBData_Cnt)
			{
				M[i+1][j]=(distance(a+1, b, InData_Arr, DBData_Arr)*1)+vmax[1];
				vmax[2]=M[i+1][j];
			}

			if(i+2<=InData_Cnt && j<=DBData_Cnt)
			{
				M[i+2][j]=(distance(a+2,b, InData_Arr, DBData_Arr)*1)+vmax[2];
				vmax[3]=M[i+2][j];
			}

			if(i<=InData_Cnt && j+1<=DBData_Cnt)
			{
				M[i][j+1]=(distance(a,b+1, InData_Arr, DBData_Arr)*1)+vmax[1];
				vmax[4]=M[i][j+1];
			}

			if(i<=InData_Cnt && j+2<=DBData_Cnt)
			{
				M[i][j+2]=(distance(a,b+2, InData_Arr, DBData_Arr)*1)+vmax[4];
				vmax[5]=M[i][j+2];
			}

			if(i+1<=InData_Cnt && j+1<=DBData_Cnt)
			{
				M[i+1][j+1]=((distance(a+1,b+1, InData_Arr, DBData_Arr)*2)+vmax[1])/2;
				vmax[6]=M[i+1][j+1];
			}
			
			if(i+2<=InData_Cnt && j+1<=DBData_Cnt)
			{
				M[i+2][j+1]=((distance(a+2,b+1, InData_Arr, DBData_Arr)*2)+vmax[2])/3;
				vmax[7]=M[i+2][j+1];
			}
			
			if(i+3<=InData_Cnt && j+1<=DBData_Cnt)
			{
				M[i+3][j+1]=((distance(a+3,b+1, InData_Arr, DBData_Arr)*2)+vmax[3])/4;
				vmax[8]=M[i+3][j+1];
			}
			
			if(i+1<=InData_Cnt && j+2<=DBData_Cnt)
			{
				M[i+1][j+2]=((distance(a+1,b+2, InData_Arr, DBData_Arr)*2)+vmax[4])/3;
				vmax[9]=M[i+1][j+2];
			}
			
			if(i+1<=InData_Cnt && j+3<=DBData_Cnt)
			{
				M[i+1][j+3]=((distance(a+1,b+3, InData_Arr, DBData_Arr)*2)+vmax[5])/4;
				vmax[10]=M[i+1][j+3];
			}

			min=vmax[6];
			for (l=6; l<=10; l++) 
			{		
				if (min >= vmax[l])
					min = vmax[l]; 
			}
		

			if(min==vmax[6])
			{
				i++;
				j++;
				a++;
				b++;
				gvalue+=vmax[6];
			}
			
			else if(min==vmax[7])
			{
				i+=2;
				j++;
				a+=2;
				b++;
				gvalue+=vmax[7];
			}
			
			else if(min==vmax[8])
			{
				i+=3;
				j++;
				a+=3;
				b++;
				gvalue+=vmax[8];
			}
			else if(min==vmax[9])
			{
				i++;
				j+=2;
				a++;
				b+=2;
				gvalue+=vmax[9];
			}
			else if(min==vmax[10])
			{
				i++;
				j+=3;
				a++;
				b+=3;
				gvalue+=vmax[10];
			}
			
			if(gvalue>20000)
			{
				return gvalue;	
			}
		}
	}


	return gvalue;
}

double dtw2(long int InData_Cnt, long int DBData_Cnt, processing_data * InData_Arr, processing_data * DBData_Arr) 
{	
	double M[201][201];
	int i=0, j=0, l=0;
	int a=0,b=0;
	double min=0;
	double vmax[11];
	double gvalue=0;
		
	for(i=0; i<=InData_Cnt; i++)
	{
		for(j=0;j<=DBData_Cnt;j++)
		{
			M[i][j]=1;			
		}		
	}
	for(i=0; i<=InData_Cnt; i++)
	{		
			M[i][0]=0;
	}
	for(j=0; j<=DBData_Cnt; j++)
	{		
			M[0][j]=0;
	}
		
	for(i=0; i<11; i++)
		vmax[i] = 0;


	M[200][200]=(distance(InData_Cnt-1, DBData_Cnt-1, InData_Arr, DBData_Arr)*2);
	

	for(j=DBData_Cnt, b=InData_Cnt-1; j>=1;)
	{		
		for(i=InData_Cnt, a=DBData_Cnt-1; i>=1;)
		{		
			if(i<=InData_Cnt && j<=DBData_Cnt)
				vmax[1]=M[i][j];
			if(i-1>=1 && j>1)
			{
				M[i-1][j]=(distance(a-1, b, InData_Arr, DBData_Arr)*1)+vmax[1];
				vmax[2]=M[i-1][j];
			}
			if(i-2>=1 && j>=1){
				M[i-2][j]=(distance(a-2, b, InData_Arr, DBData_Arr)*1)+vmax[2];
				vmax[3]=M[i-2][j];
			}
			if(i>=1 && j-1>=1){
				M[i][j-1]=(distance(a, b-1, InData_Arr, DBData_Arr)*1)+vmax[1];
				vmax[4]=M[i][j-1];
			}
			if(i>=1 && j-2>=1){
				M[i][j-2]=(distance(a, b-2, InData_Arr, DBData_Arr)*1)+vmax[4];
				vmax[5]=M[i][j-2];
			}
			if(i-1>=1 && j-1>=1){
				M[i-1][j-1]=((distance(a-1, b-1, InData_Arr, DBData_Arr)*2)+vmax[1])/2;
				vmax[6]=M[i-1][j-1];
			}
			if(i-2>=1 && j-1>=1){
				M[i-2][j-1]=((distance(a-2, b-1, InData_Arr, DBData_Arr)*2)+vmax[2])/3;
				vmax[7]=M[i-2][j-1];
			}
			if(i-3>=1 && j-1>=1){
				M[i-3][j-1]=((distance(a-3, b-1, InData_Arr, DBData_Arr)*2)+vmax[3])/4;
				vmax[8]=M[i-3][j-1];
			}
			if(i-1>=1 && j-2>=1){
				M[i-1][j-2]=((distance(a-1, b-2, InData_Arr, DBData_Arr)*2)+vmax[4])/3;
				vmax[9]=M[i-1][j-2];
			}
			if(i-1>=1 && j-3>=1){
				M[i-1][j-3]=((distance(a-1, b-3, InData_Arr, DBData_Arr)*2)+vmax[5])/4;
				vmax[10]=M[i-1][j-3];
			}

			
			min=vmax[6];
			for (l = 6; l <=10; l++) {		
				if (min >= vmax[l]) 
					min = vmax[l]; 
			}
			
			if(min==vmax[6])
			{
				i--;
				j--;
				a--;
				b--;
				gvalue+=vmax[6];
			}
			else if(min==vmax[7])
			{
				i-=2;
				j--;
				a-=2;
				b--;
				gvalue+=vmax[7];
			}
			else if(min==vmax[8])
			{
				i-=3;
				j--;
				a-=3;
				b--;
				gvalue+=vmax[8];
			}
			else if(min==vmax[9])
			{
				i--;
				j-=2;
				a--;
				b-=2;
				gvalue+=vmax[9];
			}
			else if(min==vmax[10])
			{
				i--;
				j-=3;
				a--;
				b-=3;
				gvalue+=vmax[10];
			}
			if(gvalue > 25000)
			{
				return gvalue;
			}
		}		
	}
	
	return gvalue;
}

double dtw3(long int InData_Cnt, long int DBData_Cnt, processing_data * InData_Arr, processing_data * DBData_Arr)
{	
	double M[201][201];
	int i=0,j=0,l=0;	
	double vmax=0;
	double gvalue=0;


	for(i=0;i<=InData_Cnt;i++)
	{
		for(j=0;j<=DBData_Cnt;j++)
		{
			M[i][j]=1;			
		}		
	}


	for(i=0;i<=(InData_Cnt);i++)
	{		
			M[i][0]=0;
	}

	for(j=0;j<=(DBData_Cnt);j++)
	{		
			M[0][j]=0;
	}	
	

	for(j=1,i=1;j<=(DBData_Cnt)&&i<=InData_Cnt;)
	{		
		if(i+1<=InData_Cnt && j<DBData_Cnt)
		{
			M[i][j]=(distance(i-1,j-1, InData_Arr, DBData_Arr)*2);
			vmax=M[i][j];
		}		
		
		i++;
		j++;
		gvalue+=vmax;
	}
	
	return gvalue;
}
