#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>
#include "drivers.h"

//led ����
void led_open(unsigned char port_num)
{

	switch (port_num){

		case 1:
		DDRA = 0xff;
		PORTA = 0x00;
		_delay_ms(500);	//��ʱ500mS
		break;

		case 2:
		DDRB = 0xff;
		PORTB = 0x00;
		_delay_ms(500);	//��ʱ500mS
		break;

		case 3:
		DDRC = 0xff;
		PORTC = 0x00;
		_delay_ms(500);	//��ʱ500mS
		break;

		case 4:
		DDRD = 0xff;
		PORTD = 0x00;
		_delay_ms(500);	//��ʱ500mS
		break;

		case 5:
		DDRE = 0xff;
		PORTE = 0x00;
		_delay_ms(500);	//��ʱ500mS
		break;

	}

}

void led_close(unsigned char port_num)
{

	switch (port_num){
		case 1: 
		PORTA = 0xff;
		_delay_ms(500);	//��ʱ500mS
		break;
		
		case 2: 
		PORTB = 0xff;
		_delay_ms(500);	//��ʱ500mS
		break;

		case 3: 
		PORTC = 0xff;
		_delay_ms(500);	//��ʱ500mS
		break;

		case 4: 
		PORTD = 0xff;
		_delay_ms(500);	//��ʱ500mS
		break;

		case 5: 
		PORTE = 0xff;
		_delay_ms(500);	//��ʱ500mS
		break;
	}
}

//����ܾ�̬��ʾ
#define HC595_SCK 4
#define HC595_RCK 5
#define HC595_RST 6
#define HC595_DAT 7

//��HC595����һ���ֽ�
void HC595_send_byte(unsigned char byte)
{

	unsigned char i,temp;
	for(i=0;i<=7;i++)
	{
		temp = byte & 1 << i;
		if(temp)
		{
			PORTA |= 1 << HC595_DAT;		//������
		}
		else
		{
			PORTA &= ~(1 << HC595_DAT);
		}
		PORTA |= 1 << HC595_SCK;		//SCK(11�ţ�������
		PORTA &= ~(1 << HC595_SCK);
	}

	PORTA |= 1 << HC595_RCK;			//RCK(12��)	������
	PORTA &= ~(1 << HC595_RCK);
}


//��ʾһ����num
void SMG_ShowNum(unsigned char seg)
{
	HC595_send_byte(seg);
}


//�������
void SMG_Shownums(unsigned char seg[10])
{

    unsigned char num;
    DDRA = 0xFF;        //�������
    PORTA = 0X0F; 		//��ʼ������λΪ�ߵ�ƽ��ʹ����λ�����
	PORTA &= ~(1 << HC595_RST);		//HC595�ĸ�λ�ˣ�����������λ
    while(1)
    {
		for(num=0;num<10;num++)
		{
			SMG_ShowNum(seg[num]);
			_delay_ms(1000);
		}
    }
}

//��̬���
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
                DAת��ԭ��:
  ��PWM����ռ�ձ�,�����PWM�źž�������ĵ�
  ��ͨ�˲���·��õ�ƽ�ȵ�ֱ���ź�,���DAת��.
  ռ�ձ�Խ���ʱ��,�����ֱ����ѹ��Խ��,��֮,
  �����ֱ����ѹ��ԽС
*********************************************/
//PWM0(OC0)��ʼ��

void PWM_INIT(void)
{

    TCCR0A=0x75; //��λ����ģʽ,1024��Ƶ
    TCNT0=0x00;
}

//PWM0���
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
   ����˵��:
   AVR��Ƭ�� ADCת������
   ���ȵ���ADC_Init()������ADC���г�ʼ��,
   Ȼ��Ϳ�����read_adc(uchar adc_input)����������AD����ֵ
   �β�adc_inputΪADCͨ��ѡ��,0~7����һ��
   ���ص�����Ϊ10λ���� value = 1024* U/Vref 
   ����,UΪ�����ѹֵ,VrefΪ�ο���ѹֵ.
   ������ý�����͵�������ʾ
 ***********************************************************/


//#define ADC_VREF_TYPE 0xC0 //ʹ��2.56V �ڲ��ο���ѹ 
#define ADC_VREF_TYPE 0x00 //ʹ��AREF pin�ο���ѹ

void ADC_Init()		//�˿ڳ�ʼ��
{

    DDRA=0x00;		//PA,��8λADC�˿�����Ϊ����ģʽ  
    ADMUX=ADC_VREF_TYPE & 0xff;//ʹ��AREF pin �ڲ��ο���ѹ
    ADCSRA=0x84;     //ʹ��ADC,ʱ��16��Ƶ(����16M��16/16=1M ��ADCʱ��)   
} 

//��ȡADCת��ֵ
//adc_inputΪADCͨ��
unsigned int read_adc(unsigned char adc_input)	
{

    ADMUX=adc_input | (ADC_VREF_TYPE & 0xff);
    ADCSRA|=1<<6;				//��ʼת��
    while ((ADCSRA & 0x10)==0);	//�ȴ�ADCת������
    ADCSRA|=0x10;
    return ADCW;				//����ת�����,ʮλ����
}

unsigned char data; //������յ����ַ�

void USART_Init(void)
{

    UCSR0A=0x00;
    UCSR0B=0x98;  //����ʹ�ܣ����ս����ж�ʹ�ܣ�����ʹ��
    UCSR0C=0x86;
    UBRR0H=0x00;
    UBRR0L=0x67;     //16M����,9600������
    //UBRRL=0x05; //  ����11.0592M����  11520������
    //UBRRL=0x47; //  ����11.0592M����  9600������
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
			value = read_adc(adc[i]);    //��ȡͨ��i�Ĳ���ֵ
			_delay_ms(1000);
		}
	}
}


//�ⲿ�ж�
//�ⲿ�жϳ�ʼ������
void EXT_INT0_Init(void)
{

	EICRA |= 1<<6;   //ʹ���ⲿ�ж�0
	MCUCR=0X02;     //INT0Ϊ�½��ش���
	asm("sei");  //��ȫ���ж�
}



void interrupt()
{
	DDRA |= 1;     //LED0���
	DDRB |= 1 << 3;     //���������
	DDRD &= ~(1 << 2);     //�ⲿ�ж�0�ܽ�(PD.2)��������
	PORTD |= 1 << 2;    //ʹ������
	EXT_INT0_Init();//��ʼ���ⲿ�ж�0
	while(1); 	    //�ȴ��жϵ���
}

//300


