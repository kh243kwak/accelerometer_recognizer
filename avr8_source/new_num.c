#include <iom8.h>
#include <macros.h>
#define BUFSIZE 8

#define KOR			9
#define ENG			8
#define NUM			7

#define MOUSE		1
#define END			2

#define MOUSE_L		6
#define MOUSE_R		4

#define E_MOUSE_L	5 
#define E_MOUSE_R	3 

unsigned int language_flag = 0;
unsigned int mouse_mode_flag = 0;

typedef struct _XYZ
{
 	unsigned int x;
 	unsigned int y;
 	unsigned int z;
	unsigned int reserve;			
}XYZ,*pXYZ;

XYZ data;
unsigned char buf[BUFSIZE];

void port_init(void)
{
	DDRD &= 0xFB;    // Set PD0 Input, other is Output
	PORTD = 0x00;    // Using External pull up
}

void uart0_init(void)
{
    UCSRA = 0x00;
    UCSRC = 0x06;
    UBRRL = 0x07; //set baud rate lo // 57600
    UBRRH = 0x00; //set baud rate hi
    UCSRB = 0x18;
}

void TX0_USART(unsigned char *dat,int len) {
    int i;
    for(i=0;i<len;i++) {
        while((UCSRA & 0x20)==0x00); //UDR0 empty??
		UDR=dat[i];
    }
    while((UCSRA & 0x20)==0x00); //until translate, wait
}


//call this routine to initialize all peripherals
void init_devices(void)
{

	port_init();
	uart0_init();
 
	MCUCR = 0x00;
	TIMSK = 0x00; //timer interrupt sources
}

void delay(int n)
{
	volatile int i,j;
	for(i=1;i<n;i++)
	{
    	for(j=1;j<600;j++);
	}
}

unsigned int ch0 = 0,ah0,al0;//,ch2,ch4;
unsigned int ch2 = 0,ah2,al2;//,ch2,ch4;
unsigned int ch4 = 0 ,ah4,al4;//,ch2,ch4;

void get_adc_data(void)  // Sensing Accelerometer Data from ADC Sensor
{
	unsigned int i;
	unsigned int a;
	ch0=0;ah0=0;al0=0;
	ch2=0;ah2=0;al2=0;
	ch4=0;ah4=0;al4=0;
	for(i=0;i<16;i++){
		ADCSR &= 0x7f;
		ADMUX = 0x40;
		ADCSR |= 0xC0;

		while((ADCSR & 0x10) != 0x10)
			;
		al0 = ADCL;
		ah0 = ADCH;
		ch0 += al0+ah0*256;

		ADCSR &= 0x7f;
		ADMUX = 0x41;
		ADCSR |= 0xC0;

		while((ADCSR & 0x10) != 0x10)
			;
		al2 = ADCL;
		ah2 = ADCH;
		ch2 += al2+ah2*256;

		ADCSR &= 0x7f;
		ADMUX = 0x42;
		ADCSR |= 0xC0;
		
		while((ADCSR & 0x10) != 0x10)
			;
		al4 = ADCL;
		ah4 = ADCH;
		ch4 += al4+ah4*256;
	}

	ch0 = ch0>>4;
	ch0 &=0xfffe;
	ch2 = ch2>>4;
	ch2 &=0xfffe;
	ch4 = ch4>>4;
	ch4 &=0xfffe;

	data.x = ch0;
	data.y = ch2;
	data.z = ch4;  
	data.reserve = 0x0000;
	
}

void start_kor()   // Korean start bit
{
  data.x = KOR;
  data.y = KOR;
  data.z = KOR;
  data.reserve = 0x0000;
}

void start_eng()   // English start bit
{
  data.x = ENG;
  data.y = ENG;
  data.z = ENG;
  data.reserve = 0x0000;
}

void start_num()   // Number start bit
{
  data.x = NUM;
  data.y = NUM;
  data.z = NUM;
  data.reserve = 0x0000;
}

void start_mouse() // Mouse start bit
{
  data.x=MOUSE;
  data.y=MOUSE;
  data.z=MOUSE;
  data.reserve=0x0000;
}

void start_mouse_l() // Mouse L_buttonstart bit
{
  data.x=MOUSE_L;
  data.y=MOUSE_L;
  data.z=MOUSE_L;
  data.reserve=0x0000;
}

void start_mouse_r() // Mouse R_button start bit
{
  data.x=MOUSE_R;
  data.y=MOUSE_R;
  data.z=MOUSE_R;
  data.reserve=0x0000;
}

void end()
{
  data.x=END;
  data.y=END;
  data.z=END;
  data.reserve=0x0000;
}

void end_mouse_l()
{
  data.x=E_MOUSE_L;
  data.y=E_MOUSE_L;
  data.z=E_MOUSE_L;
  data.reserve=0x0000;
}

void end_mouse_r()
{
  data.x=E_MOUSE_R;
  data.y=E_MOUSE_R;
  data.z=E_MOUSE_R;
  data.reserve=0x0000;
}

void language_led()		  // 언어 선택 led(기본 한국어) 
{
  ADCSR = 0x05;
  DDRC &= 0x38;
  
  if((PIND & 0x08) && (language_flag==0))		// 버튼을 눌렀다 띠면 영어로  
  {
    PORTC = 0x28;
	language_flag = 1;
  }
  else if((PIND & 0x08) && (language_flag==1))  // 숫자로
  {
    PORTC = 0x30;
	language_flag = 2;
  }
  else if((PIND & 0x08) && (language_flag==2))  // 한글로
  {
    PORTC = 0x18;
	language_flag = 0;
  }
}

void mode_led()				 // 펜인지 마우스인지 led (기본 펜)
{ 
  DDRB &= 0x04;
  DDRD &= 0x80;
  
  if((PIND&0x10) && (mouse_mode_flag==0))
  {
    PORTD = 0x00;
	PORTB = 0x04;  
	mouse_mode_flag = 1;
  }
  else if((PIND&0x10) && (mouse_mode_flag==1))
  {
    PORTB = 0x00;
	PORTD = 0x80;
	mouse_mode_flag = 0;
  }
}

void mouse_left()
{
  start_mouse_l();
  memcpy(buf,data,sizeof(data));
  TX0_USART(buf,BUFSIZE);
  
  if(!(PIND&0x40)) 
  {
    while(!(PIND&0x40))
  	{
      if(PIND&0x40)
	  {
	    end_mouse_l();
	  	memcpy(buf,data,sizeof(data));
      	TX0_USART(buf,BUFSIZE);
	  }
    }
  }
  else if(PIND&0x40)
  {
    end_mouse_l();
	memcpy(buf,data,sizeof(data));
    TX0_USART(buf,BUFSIZE);
  }
}

void mouse_right()
{
  start_mouse_r();
  memcpy(buf,data,sizeof(data));
  TX0_USART(buf,BUFSIZE);
  
  if(!(PIND&0x20)) 
  {
    while(!(PIND&0x20))
  	{
      if(PIND&0x20)
	  {
	    end_mouse_r();
	  	memcpy(buf,data,sizeof(data));
      	TX0_USART(buf,BUFSIZE);
	  }
    }
  }
  else if(PIND&0x20)
  {
    end_mouse_r();
	memcpy(buf,data,sizeof(data));
    TX0_USART(buf,BUFSIZE);
  }
}

void pen_write()
{
  ADMUX = 0x00;		  // ADC init
  ADCSR = 0x85;       // ADC enable, 125kHz
  
  if(language_flag==0 && mouse_mode_flag==0)   	   		// Hangle LED
  		start_kor();
  else if(language_flag==1 && mouse_mode_flag==0)		// English LED
  	   	start_eng();
  else if(language_flag==2 && mouse_mode_flag==0)		// Number LED
   	    start_num();
  else if(mouse_mode_flag==1)		// Mouse Mode
  	    start_mouse();

  memcpy(buf,data,sizeof(data));
  TX0_USART(buf,BUFSIZE);
  
  while(!(PIND&0x04)) 
  {
    get_adc_data();
	memcpy(buf,data,sizeof(data));  // Send data to PC through Bluetooth UART 
	TX0_USART(buf,BUFSIZE);

	if((PIND&0x04))
	{
      end();                        // Send End-of-data
	  memcpy(buf,data,sizeof(data));
      TX0_USART(buf,BUFSIZE);
	}
  }
}

void main()
{

	init_devices();
	
	DDRC = 0x38;		 
    PORTC = 0x18;		 // Set Default LED
	
	DDRB = 0x04;		 
	PORTB = 0x00;		 
	
	DDRD = 0x80;		 
	PORTD = 0x80;		 
	
	while(1)
	{  
	    if(!(PIND&0x04)) 		  // Input Start
		{
		  pen_write();
		}
		
		if(!(PIND&0x10))		  // Mode LED
		{
		  mode_led();
		}
		
		if(!(PIND&0x08))		  // Change Input Mode
	    {
		  if(mouse_mode_flag==0)
		  {
		    language_led();
		  }
		}
		
		if(!(PIND&0x20))		  // Mouse Right Click
		{
		  if(mouse_mode_flag==1)
		  {
		    mouse_right();
		  }
		}
		
		if(!(PIND&0x40))		  // Mouse Left Click
		{
		  if(mouse_mode_flag==1)
		  {
		    mouse_left();
		  }
		}
	}
}
