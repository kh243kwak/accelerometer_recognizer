#include "serial_comm.h"

HANDLE	g_handle, hFile;	// 스레드 핸들, COM port 핸들
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
	
	// COM port 열기
	hFile = CreateFile(com,
				GENERIC_READ | GENERIC_WRITE,
				0,				// 비공유
				0,				// 시큐리티 속성:사용안함
				OPEN_EXISTING,	// 기존 파일 오픈
				0, 0 );			// 속성, 템플레이트
	
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
	
	// 지정한 통신 디바이스의 현재 DCB 설정 얻기
	// DCB : Device Control Block 디바이스 제어 블럭
	GetCommState(hFile , &dcb);			
											
	// 설정을 변경
	dcb.BaudRate = baudrate;	
	
	// DCB의 지정에 따라 통신 디바이스 구성
	// 하드웨어와 제어 설정 초기화
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
	DWORD	dwByte;				// 파일에서 읽은 바이트수가 리턴될 변수
	char	szRecv[2];			// short형의 x, y, z, reserve를 각각 받기 위한 2바이트 버퍼
	char    Buffer[8];			// x, y, z, reserve를 모두받기 위한 8바이트 버퍼
	int     buf_cnt = 0;		// 버퍼안에 1바이트 문자 갯수
	int		cnt = 0;			// 데이터세트 갯수
	int     ch, nRet, mode, i;	// mode : 숫자, 영문, 한글 변경 플래그
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
		// 한 문자 수신
		nRet = ReadFile(hFile, szRecv, 1, &dwByte, 0);
		
		// ReadFile()은 성공하면 0이외를 반환, 타임아웃도 성공
		if(dwByte == 1)
		{	
			ch = szRecv[0];		
			Buffer[buf_cnt] = ch;
			
			// 버퍼에 8개의 문자가 다 차면
			if(buf_cnt == 7)
			{	
				// 버퍼의 내용을 좌표 구조체에 복사
				data = (struct coordinate *)Buffer;
				
				// 숫자 시작
				if((data->x == 7) && (data->y == 7) && (data->z == 7))	//7,7,7	
				{		
					// 모드를 숫자로 변경
					mode = NUMBER;
					bMouse = FALSE;

					// 버퍼 초기화, 초기 데이터 갯수 초기화
					buf_cnt = 0;
					for(int j = 0 ; j < 8 ; j++)
					{
						Buffer[j] = NULL;
					}
					cnt = 0;	// 초기 데이터 갯수 초기화
					
					// 넉넉하게 메모리를 할당하고
					input_data = (coordinate *)malloc(sizeof(coordinate) * MAX_DATA_SIZE);
					
					if(input_data == NULL)
						MessageBox(hWnd, "Memory Alloc failed", "Error", MB_OK);
					
					// 상태 에디트 출력(시작)
					static int i = 0;
					char str[10];

					sprintf(str, "%d Word", i++);
					SendMessage(hListStatus, LB_ADDSTRING, 0, (LPARAM)str);
					SendMessage(hListStatus, LB_ADDSTRING, 0, (LPARAM)"Start");
					
					int nCount = (int)SendMessage(hListStatus, LB_GETCOUNT, 0, 0);  
					SendMessage(hListStatus, LB_SETTOPINDEX, nCount-1, 0);
				}	
				// 영문자 시작
				else if((data->x == 8) && (data->y == 8) && (data->z == 8)) //8,8,8
				{
					// 모드를 영문자로 변경
					mode = ENGLISH;
					bMouse = FALSE;
					
					// 버퍼 초기화, 초기 데이터 갯수 초기화
					buf_cnt = 0;
					for(int j = 0 ; j < 8 ; j++)
					{
						Buffer[j] = NULL;
					}
					cnt = 0;	// 초기 데이터 갯수 초기화

					// 넉넉하게 메모리를 할당하고
					input_data = (coordinate *)malloc(sizeof(coordinate) * MAX_DATA_SIZE);
					
					if(input_data == NULL)
						MessageBox(hWnd, "Memory Alloc Failed", "Error", MB_OK);
					
					// 상태 에디트 출력(시작)
					static int i = 0;
					char str[10];

					sprintf(str, "%d Word", i++);
					SendMessage(hListStatus, LB_ADDSTRING, 0, (LPARAM)str);
					SendMessage(hListStatus, LB_ADDSTRING, 0, (LPARAM)"Start");
					
					int nCount = (int)SendMessage(hListStatus, LB_GETCOUNT, 0, 0);  
					SendMessage(hListStatus, LB_SETTOPINDEX, nCount-1, 0);
				}
				// 한글 시작
				else if((data->x == 9) && (data->y == 9) && (data->z == 9)) // 9,9,9
				{
					// 모드를 한글로 변경
					mode = HANGUL;
					bMouse = FALSE;

					// 버퍼 초기화, 초기 데이터 갯수 초기화
					buf_cnt = 0;
					for(int j = 0 ; j < 8 ; j++)
					{
						Buffer[j] = NULL;
					}
					cnt = 0;	// 초기 데이터 갯수 초기화

					// 넉넉하게 메모리를 할당하고
					input_data = (coordinate *)malloc(sizeof(coordinate) * MAX_DATA_SIZE);
					
					if(input_data == NULL)
						MessageBox(hWnd, "Memory Alloc Failed", "Error", MB_OK);
					
					// 상태 에디트 출력(시작)
					static int i = 0;
					char str[10];

					sprintf(str, "%d Word", i++);
					SendMessage(hListStatus, LB_ADDSTRING, 0, (LPARAM)str);
					SendMessage(hListStatus, LB_ADDSTRING, 0, (LPARAM)"Start");
					
					int nCount = (int)SendMessage(hListStatus, LB_GETCOUNT, 0, 0);  
					SendMessage(hListStatus, LB_SETTOPINDEX, nCount-1, 0);
				}
				// 마우스 시작
				else if((data->x == 1) && (data->y == 1) && (data->z == 1))
				{	
					bMouse = TRUE;	
					
					// 버퍼 초기화
					buf_cnt = 0;
					for(int j = 0 ; j < 8 ; j++)
					{
						Buffer[j] = NULL;
					}
				}
				// 마우스 왼쪽 버튼을 눌렀을 때 
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
				// 마우스 왼쪽 버튼을 뗐을 때 or 문자인식 때는 화살표 왼쪽
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
				// 마우스 오른쪽 버튼을 눌렀을 때 
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
				// 마우스 오른쪽 버튼을 뗐을 때 or 문자인식 때는 화살표 오른쪽
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
				// 끝
				else if((data->x == 2) && (data->y == 2) && (data->z == 2))
				{	
					// 버퍼 초기화
					buf_cnt = 0;
					for(int j = 0 ; j < 8 ; j++)
					{
						Buffer[j] = NULL;
					}

					if(bMouse == FALSE)
					{	
						// 상태 에디트 출력(끝)
						SendMessage(hListStatus, LB_ADDSTRING, 0, (LPARAM)"End");
						SendMessage(hListStatus, LB_ADDSTRING, 0, (LPARAM)"================================");
						
						int nCount = (int)SendMessage(hListStatus, LB_GETCOUNT, 0, 0);  
						SendMessage(hListStatus, LB_SETTOPINDEX, nCount-1, 0);
						
						// 받은 데이터로 바로 전처리과정 실행 후 인식
						if(cnt < 1000)
						{
							if((mode == HANGUL) && ((cnt > 0) && (cnt < 65)))
								recognize(cnt, input_data, mode, TRUE);		// 한글에서 . 인식
							else
								recognize(cnt, input_data, mode, FALSE);	// 그 외 문자
						}
						else
						{
							cnt = 0;
							MessageBox(hWnd, "Data Overflow", "Error", MB_OK);
						}

						free(input_data);
					}
				}
				// 데이터를 보내는 중
				else if(data->reserved == 0)
				{					
					// 마우스 모드
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
					// 문자인식 모드
					else
					{		
						// 데이터 셋 갯수가 1000개를 넘어가면 리셋
						if(cnt >= 1000)
							;
						else 
						{
							// 인식과정 함수에 넘길 구조체에 가속도값 저장
							input_data[cnt].x = data->x;
							input_data[cnt].y = data->y;
							input_data[cnt].z = data->z;
						
							// 가속도값 세트 증가
							cnt++;
						}		
						// 버퍼 초기화
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
				// 버퍼 카운트 증가
				buf_cnt++;
			}
		}		
	}
	return 0;
}