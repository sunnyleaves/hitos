#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>
#include "drivers.h"

//led 驱动
void led_open(unsigned char port_num)
{

	switch (port_num){

		case 1:
		DDRA = 0xff;
		PORTA = 0x00;
		_delay_ms(500);	//延时500mS
		break;

		case 2:
		DDRB = 0xff;
		PORTB = 0x00;
		_delay_ms(500);	//延时500mS
		break;

		case 3:
		DDRC = 0xff;
		PORTC = 0x00;
		_delay_ms(500);	//延时500mS
		break;

		case 4:
		DDRD = 0xff;
		PORTD = 0x00;
		_delay_ms(500);	//延时500mS
		break;

		case 5:
		DDRE = 0xff;
		PORTE = 0x00;
		_delay_ms(500);	//延时500mS
		break;

	}

}

void led_close(unsigned char port_num)
{

	switch (port_num){
		case 1: 
		PORTA = 0xff;
		_delay_ms(500);	//延时500mS
		break;
		
		case 2: 
		PORTB = 0xff;
		_delay_ms(500);	//延时500mS
		break;

		case 3: 
		PORTC = 0xff;
		_delay_ms(500);	//延时500mS
		break;

		case 4: 
		PORTD = 0xff;
		_delay_ms(500);	//延时500mS
		break;

		case 5: 
		PORTE = 0xff;
		_delay_ms(500);	//延时500mS
		break;
	}
}

//数码管静态显示
#define HC595_SCK 4
#define HC595_RCK 5
#define HC595_RST 6
#define HC595_DAT 7

//向HC595发送一个字节
void HC595_send_byte(unsigned char byte)
{

	unsigned char i,temp;
	for(i=0;i<=7;i++)
	{
		temp = byte & 1 << i;
		if(temp)
		{
			PORTA |= 1 << HC595_DAT;		//数据线
		}
		else
		{
			PORTA &= ~(1 << HC595_DAT);
		}
		PORTA |= 1 << HC595_SCK;		//SCK(11脚）上升沿
		PORTA &= ~(1 << HC595_SCK);
	}

	PORTA |= 1 << HC595_RCK;			//RCK(12脚)	上升沿
	PORTA &= ~(1 << HC595_RCK);
}


//显示一个数num
void SMG_ShowNum(unsigned char seg)
{
	HC595_send_byte(seg);
}


//依次输出
void SMG_Shownums(unsigned char seg[10])
{

    unsigned char num;
    DDRA = 0xFF;        //方向输出
    PORTA = 0X0F; 		//初始化低四位为高电平，使能四位数码管
	PORTA &= ~(1 << HC595_RST);		//HC595的复位端，不能让他复位
    while(1)
    {
		for(num=0;num<10;num++)
		{
			SMG_ShowNum(seg[num]);
			_delay_ms(1000);
		}
    }
}

//动态输出
void SMG_Display(unsigned int dat)
{

	SMG_ShowNum(dat/1000);
	PORTA = (PORTA&0xf0)|1;
	_delay_ms(2);

	SMG_ShowNum((dat%1000)/100);
	PORTA = (PORTA&0xf0)|2;
	_delay_ms(2);

	SMG_ShowNum(dat%100/10);
	PORTA = (PORTA&0xf0)|4;
	_delay_ms(2);

	SMG_ShowNum(dat%10);
	PORTA = (PORTA&0xf0)|8;
	_delay_ms(2);
}

/********************************************
                DA转换原理:
  用PWM控制占空比,输出的PWM信号经过外面的的
  低通滤波电路后得到平稳的直流信号,完成DA转换.
  占空比越大的时候,输出的直流电压就越高,反之,
  输出的直流电压就越小
*********************************************/
//PWM0(OC0)初始化

void PWM_INIT(void)
{

    TCCR0A=0x75; //相位修正模式,1024分频
    TCNT0=0x00;
}

//PWM0输出
void PWM0_OUT(unsigned char num)
{
    OCR0A=num;
}

void DA(unsigned char Duty_Cycle[],unsigned char num)
{

    unsigned char i;
   	PORTB=0x00;
	DDRB |= 1 << 3;
	PWM_INIT();
	while (1)
	{
	    for(i=0;i<num;i++)
	    {
	        PWM0_OUT(Duty_Cycle[i]);
            _delay_ms(50);
	    }
	}
}



/***********************************************************
   程序说明:
   AVR单片机 ADC转换程序
   首先调用ADC_Init()函数对ADC进行初始化,
   然后就可以用read_adc(uchar adc_input)函数来读读AD采样值
   形参adc_input为ADC通道选择,0~7其中一个
   返回的数据为10位数据 value = 1024* U/Vref 
   其中,U为输入电压值,Vref为参考电压值.
   最后将所得结果发送到串口显示
 ***********************************************************/


//#define ADC_VREF_TYPE 0xC0 //使用2.56V 内部参考电压 
#define ADC_VREF_TYPE 0x00 //使用AREF pin参考电压

void ADC_Init()		//端口初始化
{

    DDRA=0x00;		//PA,即8位ADC端口设置为输入模式  
    ADMUX=ADC_VREF_TYPE & 0xff;//使用AREF pin 内部参考电压
    ADCSRA=0x84;     //使能ADC,时钟16分频(晶振16M，16/16=1M 的ADC时钟)   
} 

//读取ADC转换值
//adc_input为ADC通道
unsigned int read_adc(unsigned char adc_input)	
{

    ADMUX=adc_input | (ADC_VREF_TYPE & 0xff);
    ADCSRA|=1<<6;				//开始转换
    while ((ADCSRA & 0x10)==0);	//等待ADC转换结束
    ADCSRA|=0x10;
    return ADCW;				//返回转换结果,十位数据
}

unsigned char data; //保存接收到的字符

void USART_Init(void)
{

    UCSR0A=0x00;
    UCSR0B=0x98;  //接收使能，接收结束中断使能，发送使能
    UCSR0C=0x86;
    UBRR0H=0x00;
    UBRR0L=0x67;     //16M晶振,9600波特率
    //UBRRL=0x05; //  换用11.0592M晶振  11520波特率
    //UBRRL=0x47; //  换用11.0592M晶振  9600波特率
    asm("sei");
}




void adc(unsigned char num,unsigned char adc[])
{

	unsigned int value=0;
	unsigned char i=0;
	ADC_Init();
	USART_Init();
	while(1)
	{
		for(i=0;i<num;i++)
		{
			value = read_adc(adc[i]);    //读取通道i的采样值
			_delay_ms(1000);
		}
	}
}


//外部中断
//外部中断初始化函数
void EXT_INT0_Init(void)
{

	EICRA |= 1<<6;   //使能外部中断0
	MCUCR=0X02;     //INT0为下降沿触发
	asm("sei");  //开全局中断
}



void interrupt()
{
	DDRA |= 1;     //LED0输出
	DDRB |= 1 << 3;     //蜂鸣器输出
	DDRD &= ~(1 << 2);     //外部中断0管脚(PD.2)方向输入
	PORTD |= 1 << 2;    //使能上拉
	EXT_INT0_Init();//初始化外部中断0
	while(1); 	    //等待中断到来
}

//300


