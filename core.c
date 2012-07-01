#include <avr/io.h>
#include "core.h"
//#include <util/delay.h>



//��ֹ�ж�
void  IntEnter(void)
{
	SPCR &= SPIE;
}

//�ָ������ж�
void  IntLeave(void)
{
	SPCR &= ~SPIE;
}


//����ʼ����
void  StartTaskRunning()
{
;//��cur_tcb����ڿ�ʼִ�г���
}

//�������
void  OS_Scheduler(void)
{
;//����ǰ����״̬�洢�����ҽ��л�����Ӧ�Ľ���
}


//���̿�ṹ��
typedef struct Os_tcb
{

	unsigned char *tstack;
	unsigned char tmessage;
	unsigned char ttime_delay;
	unsigned char tstatus;

}os_tcb;

os_tcb tcb[Tasknum],*cur_tcb;//�������ȼ�������Խ�����ȼ�Խ��



//����ָ������״̬
char *task_show_status(unsigned char index){
	if(index >= Tasknum)
		return "Error";
	os_tcb *tmp_tcb;
	tmp_tcb = &tcb[index];
	char *ch;
	if(tmp_tcb->tstatus & TS_DELAY)
		ch="Task delaying!";
	if(tmp_tcb->tstatus & TS_SUSPEND)
		ch="Task is suspended!";
	if(tmp_tcb->tstatus & TS_MESSAGE)
		ch="Task is waiting for message!";
	if(tmp_tcb->tstatus & TS_LOCK)
		ch="Task is locked and it can't be interrupted!";
	if(tmp_tcb->tstatus == 0)
		ch="Task is ready!";
	return ch;
}

//����ָ������ջ�ռ�����

char *task_show_stack(unsigned char index){
	if(index >= Tasknum)
		return "Error";
	os_tcb *tmp_tcb;
	tmp_tcb = &tcb[index];
	return tmp_tcb->tstack;
}

//����ָ��������ʱʱ��Ƭ
char task_show_delay(unsigned char index){
	if(index >= Tasknum)
		return -1;
	os_tcb *tmp_tcb;
	tmp_tcb = &tcb[index];
	return tmp_tcb->ttime_delay;
}

//��ʼ�����н���״̬��ȫ������
void task_state_init(){

	unsigned char i;
	os_tcb *tmp_tcb;
	tmp_tcb = tcb;
	for(i = 0;i < Tasknum;i++,tmp_tcb++)
	{
		tmp_tcb->tstatus = TS_SUSPEND;
	}

}

//��ʱʱ�䴦�� ��--��
void task_delay_dec()
{

	unsigned char i;
	os_tcb *tmp_tcb;
	tmp_tcb = tcb;

	for(i = 0;i < Tasknum;i++,tmp_tcb++)
	{

		if(tmp_tcb->ttime_delay != 0)
		{
			tmp_tcb->ttime_delay--;
		}

		if(tmp_tcb->ttime_delay == 0)
		{
			tmp_tcb->tstatus &= ~TS_DELAY;
		}
	}
}

//�л���ǰ����
unsigned char current_task_switch()
{

	if((cur_tcb->tstatus & TS_LOCK) == 0)
	{
		unsigned char i;
		os_tcb *tmp_tcb;
		tmp_tcb = tcb;

		for(i = 0;i < Tasknum;i++,tmp_tcb++)
		{
			if(tmp_tcb->tstatus == 0)
			{
				cur_tcb = tmp_tcb;
				OS_Scheduler();
				return 1;
			}
		}

	}

	return 0;
}


//�������̣����ȼ�Ϊpriority������ԽС����Խ�ߣ��������Ϊentry,
unsigned char task_create(void (*entry)(void), unsigned char priority)
{

	unsigned char *tmp_stack;
	unsigned char i;
	
	tmp_stack = _Stack[priority] + 64;//����
	*tmp_stack = '\0';
	*(--tmp_stack) = (unsigned char)((unsigned int)entry);//�������
	*(--tmp_stack) = (unsigned char)((unsigned int)entry>>8);
	--tmp_stack;

	for(i=0;i<32;i++)
	{
		 *(--tmp_stack) = '0';
	}
	*(--tmp_stack) = 1 << 7;    //SREG
	*(--tmp_stack) = (unsigned char)((unsigned int)_Stack[priority]);
	*(--tmp_stack) = (unsigned char)((unsigned int)_Stack[priority]>>8);

	os_tcb *tmp_tcb;

	tmp_tcb = &tcb[priority];
	tmp_tcb -> tstatus = 0x00;
	tmp_tcb -> tmessage = 0x00;
	tmp_tcb -> ttime_delay = 0x00;
	tmp_tcb -> tstack = tmp_stack;

	return 1;
}

//����ǰ���̷�����ʱʱ��Ƭ
unsigned char task_delay(unsigned char time)
{

	if(time > 0)
	{

		IntEnter();
		cur_tcb->ttime_delay = time;
		cur_tcb->tstatus = TS_DELAY;
		IntLeave();
		OS_Scheduler();

		return 1;
	}

	return 0;
}

//���̹���
unsigned char task_suspend(unsigned char priority)
{

	if(priority < Tasknum)
	{

		os_tcb *tmp_tcb;
		tmp_tcb = &tcb[priority];

		IntEnter();
		tmp_tcb->tstatus |= TS_SUSPEND;
		IntLeave();

		if(cur_tcb == tmp_tcb)
		{
			OS_Scheduler();
		}

		return 1;
	}

	return 0;
}

//���ѽ���
unsigned char task_awake(unsigned char priority)
{

	if(priority < Tasknum){

		os_tcb *tmp_tcb;
		tmp_tcb = &tcb[priority];

		IntEnter();
		tmp_tcb->tstatus &= ~TS_SUSPEND;
		IntLeave();
		OS_Scheduler();

		return 1;
	}

	return 0;
}

//�ȴ���Ϣ
unsigned char task_wait_message(void)
{
	
	IntEnter();
	cur_tcb -> tstatus |= TS_MESSAGE;
	IntLeave();
	
	OS_Scheduler();
	return cur_tcb -> tmessage;
}

//������Ϣ
unsigned char task_send_message(unsigned char priority,unsigned char message)
{

	if(priority < Tasknum){
		os_tcb *tmp_tcb;
		tmp_tcb = &tcb[priority];

		IntEnter();
		tmp_tcb -> tmessage = message;
		tmp_tcb -> tstatus &= ~TS_MESSAGE;
		IntLeave();
		OS_Scheduler();

		return 1;
	}

	return 0;
}

//��������
void task_lock(void)
{

	IntEnter();
	cur_tcb->tstatus |= TS_LOCK;
	IntLeave();

}

//�������
void task_unlock(void)
{

	IntEnter();
	cur_tcb->tstatus &= ~TS_LOCK;
	IntLeave();
	
	OS_Scheduler();
	
}

//�ГQ���Ⱥˣ���ʼִ�г���
unsigned char start(void){
	
	unsigned char i;
	os_tcb *tmp_tcb;
	tmp_tcb = tcb;
	SPCR |= 1 << SPIE;
	for(i = 0;i < Tasknum;i++,tmp_tcb++)
	{
		if(tmp_tcb -> tstatus == 0){
			cur_tcb = tmp_tcb;
			StartTaskRunning();
			return 1;
		}
	}
	return 0;
}

void Usart0_Init(void)
{

    UCSR0B = 0x00; //disable while setting baud rate
    UCSR0A = 0x00;
    UCSR0C = (1<<UCSZ00) | (1<<UCSZ01);
    UBRR0L = (fosc/16/(baud+1))%256; //set baud rate lo
    UBRR0H = (fosc/16/(baud+1))/256; //set baud rate hi
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);
}

void Usart0_Putc(unsigned char ch)
{

    while(!(UCSR0A & (1 << UDRE0))){   //�ȴ����ͻ�������
    	OS_Scheduler();
    	task_delay_dec(1);
	}

    UDR0 = ch;                       //��������
}

void Usart0_Puts(char *s) 
{

	while (*s) 
	{
		Usart0_Putc(*s); 
		s++; 
	}
	//Usart0_Putc(0x0a);//�س����� 
	//Usart0_Putc(0x0d); 	
}

//��ӡ�ַ��Ķ����Ʊ���
void Usart0_Putcb(unsigned char ch){
	unsigned char i;
	for(i = 0;i < 8;i++){
		if(ch & 1)
			Usart0_Putc('1');
		else
			Usart0_Putc('0');
		ch >>= 1;
	}
}

//��ӡ�ַ����Ķ����Ʊ���
void Usart0_Putsb(char *s){
	while(*s){
		Usart0_Putcb(*s); 
		Usart0_Putc(' ');
		s++; 
	}
}
unsigned char Usart0_Getc(void)
{

    while (!(UCSR0A & (1<<RXC0))){
    	OS_Scheduler();
    	task_delay_dec(1);
	}

    return UDR0;
}

void Usart1_Init(void)
{

    UCSR1B = 0x00; //disable while setting baud rate
    UCSR1A = 0x00;
    UCSR1C = (1<<UCSZ10) | (1<<UCSZ11);
    UBRR1L = (fosc/16/(baud+1))%256; //set baud rate lo
    UBRR1H = (fosc/16/(baud+1))/256; //set baud rate hi
    UCSR1B = (1<<RXEN0)|(1<<TXEN0);
}

void Usart1_Putc(unsigned char ch)
{

    while(!(UCSR1A & (1 << UDRE1))){   //�ȴ����ͻ�������
    	OS_Scheduler();
    	task_delay_dec(1);
	}
    UDR1 = ch;                       //��������
}

void Usart1_Puts(char *s) 
{

	while (*s) 
	{
		Usart1_Putc(*s); 
		s++; 
	}
	//Usart1_Putc(0x0a);//�س����� 
	//Usart1_Putc(0x0d); 
}

unsigned char Usart1_Getc(void)
{

	while (!(UCSR1A & (1<<RXC1))){
		OS_Scheduler();
		task_delay_dec(1);
	}
	return UDR1;
}

void Usart2_Init(void)
{

	UCSR2B = 0x00; //disable while setting baud rate
	UCSR2A = 0x00;
	UCSR2C = (1<<UCSZ20) | (1<<UCSZ21);
	UBRR2L = (fosc/16/(baud+1))%256; //set baud rate lo
	UBRR2H = (fosc/16/(baud+1))/256; //set baud rate hi
	UCSR2B = (1<<RXEN0)|(1<<TXEN0);
}

void Usart2_Putc(unsigned char ch)
{

	while(!(UCSR2A & (1 << UDRE2))){   //�ȴ����ͻ�������
		OS_Scheduler();
		task_delay_dec(1);
	}

	UDR2 = ch;                       //��������
}

void Usart2_Puts(char *s) 
{

	while (*s) 
	{
		Usart0_Putc(*s); 
		s++; 
	}
	//Usart2_Putc(0x0a);//�س����� 
	//Usart2_Putc(0x0d); 
}

unsigned char Usart2_Getc(void)
{

	while (!(UCSR2A & (1<<RXC2))){
		OS_Scheduler();
		task_delay_dec(1);
	}
	return UDR2;
}

void Usart3_Init(void)
{

	UCSR3B = 0x00; //disable while setting baud rate
	UCSR3A = 0x00;
	UCSR3C = (1<<UCSZ30) | (1<<UCSZ31);
	UBRR3L = (fosc/16/(baud+1))%256; //set baud rate lo
	UBRR3H = (fosc/16/(baud+1))/256; //set baud rate hi
	UCSR3B = (1<<RXEN0)|(1<<TXEN0);
}

void Usart3_Putc(unsigned char ch)
{

	while(!(UCSR3A & (1 << UDRE3))){   //�ȴ����ͻ�������
		OS_Scheduler();
		task_delay_dec(1);
	}
	UDR3 = ch;                       //��������
}

void Usart3_Puts(char *s) 
{

	while (*s)
	{
		Usart3_Putc(*s); 
		s++; 
	}
	//Usart3_Putc(0x0a);//�س����� 
	//Usart3_Putc(0x0d); 
}



unsigned char Usart3_Getc(void)
{

	while (!(UCSR3A & (1<<RXC3))){
		OS_Scheduler();
		task_delay_dec(1);
	}
	return UDR3;
}

//520
