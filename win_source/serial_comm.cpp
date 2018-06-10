#include "serial_comm.h"

HANDLE	g_handle, hFile;	// ������ �ڵ�, COM port �ڵ�
HWND   hEdit;
BOOL   bMouse = FALSE;

int     mean_x;
int     mean_y;
int     mean_z;

BOOL Open(int baudrate, int com_port)
{
	HWND hListStatus;
	char str[30];
	DWORD nThreadId;
	char com[20];

	sprintf(com, "COM%d", com_port);
	
	// COM port ����
	hFile = CreateFile(com,
				GENERIC_READ | GENERIC_WRITE,
				0,				// �����
				0,				// ��ť��Ƽ �Ӽ�:������
				OPEN_EXISTING,	// ���� ���� ����
				0, 0 );			// �Ӽ�, ���÷���Ʈ
	
	hListStatus = GetDlgItem(hWnd, IDC_LIST_STATUS);
	hEdit = GetDlgItem(hWnd, IDC_EDIT_TEXT);

	if(hFile == INVALID_HANDLE_VALUE)
		MessageBox(0, "Can't Open COM port!","Caution", MB_ICONERROR);
	else
	{
		sprintf(str, "Open Port %s Success", com);
		SendMessage(hListStatus, LB_ADDSTRING, 0, (LPARAM)str);
		SendMessage(hListStatus, LB_ADDSTRING, 0, (LPARAM)"================================");
	}

	DCB dcb;
	
	// ������ ��� ����̽��� ���� DCB ���� ���
	// DCB : Device Control Block ����̽� ���� ��
	GetCommState(hFile , &dcb);			
											
	// ������ ����
	dcb.BaudRate = baudrate;	
	
	// DCB�� ������ ���� ��� ����̽� ����
	// �ϵ����� ���� ���� �ʱ�ȭ
	SetCommState(hFile , &dcb);			

	COMMTIMEOUTS	cTimeout;

	cTimeout.ReadIntervalTimeout         = 1000;	
	cTimeout.ReadTotalTimeoutMultiplier  = 0;
	cTimeout.ReadTotalTimeoutConstant    = 1000;
	cTimeout.WriteTotalTimeoutMultiplier = 0;
	cTimeout.WriteTotalTimeoutConstant   = 0;

	SetCommTimeouts(hFile, &cTimeout);

	CloseHandle(g_handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RecvData, NULL, 0, &nThreadId));

	return TRUE;	
}

DWORD WINAPI RecvData(void * dummy)					
{
	DWORD	dwByte;				// ���Ͽ��� ���� ����Ʈ���� ���ϵ� ����
	char	szRecv[2];			// short���� x, y, z, reserve�� ���� �ޱ� ���� 2����Ʈ ����
	char    Buffer[8];			// x, y, z, reserve�� ��ιޱ� ���� 8����Ʈ ����
	int     buf_cnt = 0;		// ���۾ȿ� 1����Ʈ ���� ����
	int		cnt = 0;			// �����ͼ�Ʈ ����
	int     ch, nRet, mode, i;	// mode : ����, ����, �ѱ� ���� �÷���
	HWND    hListStatus;
	int     cal_cnt =  0;
	struct coordinate *data;
	coordinate *input_data;
	
	LVITEM LI;
	LI.mask = LVIF_TEXT;

	hListStatus = GetDlgItem(hWnd, IDC_LIST_STATUS);
	
	while(1) 
	{		
		if(Sending_flag == 0)
		{			
			CloseHandle(hFile);
			return 0;		
		}
		// �� ���� ����
		nRet = ReadFile(hFile, szRecv, 1, &dwByte, 0);
		
		// ReadFile()�� �����ϸ� 0�ܸ̿� ��ȯ, Ÿ�Ӿƿ��� ����
		if(dwByte == 1)
		{	
			ch = szRecv[0];		
			Buffer[buf_cnt] = ch;
			
			// ���ۿ� 8���� ���ڰ� �� ����
			if(buf_cnt == 7)
			{	
				// ������ ������ ��ǥ ����ü�� ����
				data = (struct coordinate *)Buffer;
				
				// ���� ����
				if((data->x == 7) && (data->y == 7) && (data->z == 7))	//7,7,7	
				{		
					// ��带 ���ڷ� ����
					mode = NUMBER;
					bMouse = FALSE;

					// ���� �ʱ�ȭ, �ʱ� ������ ���� �ʱ�ȭ
					buf_cnt = 0;
					for(int j = 0 ; j < 8 ; j++)
					{
						Buffer[j] = NULL;
					}
					cnt = 0;	// �ʱ� ������ ���� �ʱ�ȭ
					
					// �˳��ϰ� �޸𸮸� �Ҵ��ϰ�
					input_data = (coordinate *)malloc(sizeof(coordinate) * MAX_DATA_SIZE);
					
					if(input_data == NULL)
						MessageBox(hWnd, "Memory Alloc failed", "Error", MB_OK);
					
					// ���� ����Ʈ ���(����)
					static int i = 0;
					char str[10];

					sprintf(str, "%d Word", i++);
					SendMessage(hListStatus, LB_ADDSTRING, 0, (LPARAM)str);
					SendMessage(hListStatus, LB_ADDSTRING, 0, (LPARAM)"Start");
					
					int nCount = (int)SendMessage(hListStatus, LB_GETCOUNT, 0, 0);  
					SendMessage(hListStatus, LB_SETTOPINDEX, nCount-1, 0);
				}	
				// ������ ����
				else if((data->x == 8) && (data->y == 8) && (data->z == 8)) //8,8,8
				{
					// ��带 �����ڷ� ����
					mode = ENGLISH;
					bMouse = FALSE;
					
					// ���� �ʱ�ȭ, �ʱ� ������ ���� �ʱ�ȭ
					buf_cnt = 0;
					for(int j = 0 ; j < 8 ; j++)
					{
						Buffer[j] = NULL;
					}
					cnt = 0;	// �ʱ� ������ ���� �ʱ�ȭ

					// �˳��ϰ� �޸𸮸� �Ҵ��ϰ�
					input_data = (coordinate *)malloc(sizeof(coordinate) * MAX_DATA_SIZE);
					
					if(input_data == NULL)
						MessageBox(hWnd, "Memory Alloc Failed", "Error", MB_OK);
					
					// ���� ����Ʈ ���(����)
					static int i = 0;
					char str[10];

					sprintf(str, "%d Word", i++);
					SendMessage(hListStatus, LB_ADDSTRING, 0, (LPARAM)str);
					SendMessage(hListStatus, LB_ADDSTRING, 0, (LPARAM)"Start");
					
					int nCount = (int)SendMessage(hListStatus, LB_GETCOUNT, 0, 0);  
					SendMessage(hListStatus, LB_SETTOPINDEX, nCount-1, 0);
				}
				// �ѱ� ����
				else if((data->x == 9) && (data->y == 9) && (data->z == 9)) // 9,9,9
				{
					// ��带 �ѱ۷� ����
					mode = HANGUL;
					bMouse = FALSE;

					// ���� �ʱ�ȭ, �ʱ� ������ ���� �ʱ�ȭ
					buf_cnt = 0;
					for(int j = 0 ; j < 8 ; j++)
					{
						Buffer[j] = NULL;
					}
					cnt = 0;	// �ʱ� ������ ���� �ʱ�ȭ

					// �˳��ϰ� �޸𸮸� �Ҵ��ϰ�
					input_data = (coordinate *)malloc(sizeof(coordinate) * MAX_DATA_SIZE);
					
					if(input_data == NULL)
						MessageBox(hWnd, "Memory Alloc Failed", "Error", MB_OK);
					
					// ���� ����Ʈ ���(����)
					static int i = 0;
					char str[10];

					sprintf(str, "%d Word", i++);
					SendMessage(hListStatus, LB_ADDSTRING, 0, (LPARAM)str);
					SendMessage(hListStatus, LB_ADDSTRING, 0, (LPARAM)"Start");
					
					int nCount = (int)SendMessage(hListStatus, LB_GETCOUNT, 0, 0);  
					SendMessage(hListStatus, LB_SETTOPINDEX, nCount-1, 0);
				}
				// ���콺 ����
				else if((data->x == 1) && (data->y == 1) && (data->z == 1))
				{	
					bMouse = TRUE;	
					
					// ���� �ʱ�ȭ
					buf_cnt = 0;
					for(int j = 0 ; j < 8 ; j++)
					{
						Buffer[j] = NULL;
					}
				}
				// ���콺 ���� ��ư�� ������ �� 
				else if( (data->x == 6) && (data->y == 6) && (data->z == 6) )
				{
					if(bMouse == TRUE)
						MouseLeft();

					buf_cnt = 0;
					for(i = 0 ; i < 8 ; i++)
					{
						Buffer[i] = NULL;
					}
				}
				// ���콺 ���� ��ư�� ���� �� or �����ν� ���� ȭ��ǥ ����
				else if( (data->x == 5) && (data->y == 5) && (data->z == 5) )
				{
					if(bMouse == TRUE)
						MouseLeftUp();
					else 
						push_left();

					buf_cnt = 0;
					for(i = 0 ; i < 8 ; i++)
					{
						Buffer[i] = NULL;
					}
				}
				// ���콺 ������ ��ư�� ������ �� 
				else if( (data->x == 4) && (data->y == 4) && (data->z == 4) )
				{
					if(bMouse == TRUE)
						MouseRight();
					
					buf_cnt = 0;
					for(i = 0 ; i < 8 ; i++)
					{
						Buffer[i] = NULL;
					}
				}
				// ���콺 ������ ��ư�� ���� �� or �����ν� ���� ȭ��ǥ ������
				else if( (data->x == 3) && (data->y == 3) && (data->z == 3) )
				{
					if(bMouse == TRUE)
						MouseRightUp();
					else 
						push_right();
					
					buf_cnt = 0;
					for(i = 0 ; i < 8 ; i++)
					{
						Buffer[i] = NULL;
					}
				}
				// ��
				else if((data->x == 2) && (data->y == 2) && (data->z == 2))
				{	
					// ���� �ʱ�ȭ
					buf_cnt = 0;
					for(int j = 0 ; j < 8 ; j++)
					{
						Buffer[j] = NULL;
					}

					if(bMouse == FALSE)
					{	
						// ���� ����Ʈ ���(��)
						SendMessage(hListStatus, LB_ADDSTRING, 0, (LPARAM)"End");
						SendMessage(hListStatus, LB_ADDSTRING, 0, (LPARAM)"================================");
						
						int nCount = (int)SendMessage(hListStatus, LB_GETCOUNT, 0, 0);  
						SendMessage(hListStatus, LB_SETTOPINDEX, nCount-1, 0);
						
						// ���� �����ͷ� �ٷ� ��ó������ ���� �� �ν�
						if(cnt < 1000)
						{
							if((mode == HANGUL) && ((cnt > 0) && (cnt < 65)))
								recognize(cnt, input_data, mode, TRUE);		// �ѱۿ��� . �ν�
							else
								recognize(cnt, input_data, mode, FALSE);	// �� �� ����
						}
						else
						{
							cnt = 0;
							MessageBox(hWnd, "Data Overflow", "Error", MB_OK);
						}

						free(input_data);
					}
				}
				// �����͸� ������ ��
				else if(data->reserved == 0)
				{					
					// ���콺 ���
					if(bMouse == TRUE)
					{
						if(cal_cnt < 100)
						{
							mean_x = mean_x + data->x;
							mean_y = mean_y + data->y;
							mean_z = mean_z + data->z;
							
							buf_cnt = 0;
							for(i = 0 ; i < 8 ; i++)
							{
								Buffer[i] = NULL;
							}							
						} 
						else if( cal_cnt == 100)
						{
							mean_x = mean_x / 100;
							mean_y = mean_y / 100;
							mean_z = mean_z / 100;
							
							cal_cnt++;
						} 
						else if(cal_cnt > 100)
						{
							POINT before;
							GetCursorPos(&before);
							MoveMouse((int)(before.x), (int)(before.y), data->x, data->y, data->z);
						}
						cal_cnt++;
						
						buf_cnt = 0;
						for(i = 0 ; i < 8 ; i++)
						{
							Buffer[i] = NULL;
						}	
					}
					// �����ν� ���
					else
					{		
						// ������ �� ������ 1000���� �Ѿ�� ����
						if(cnt >= 1000)
							;
						else 
						{
							// �νİ��� �Լ��� �ѱ� ����ü�� ���ӵ��� ����
							input_data[cnt].x = data->x;
							input_data[cnt].y = data->y;
							input_data[cnt].z = data->z;
						
							// ���ӵ��� ��Ʈ ����
							cnt++;
						}		
						// ���� �ʱ�ȭ
						buf_cnt = 0;
						for(int j = 0 ; j < 8 ; j++)
						{
							Buffer[j] = NULL;
						}
					}
				}
			}	//if(buf_cnt == 7) 
			else
			{
				// ���� ī��Ʈ ����
				buf_cnt++;
			}
		}		
	}
	return 0;
}