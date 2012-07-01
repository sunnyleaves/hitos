#ifndef _CORE_TAG_
#define _CORE_TAG_


#define TS_DELAY    0x01        //��������ʱ״̬
#define TS_SUSPEND  0x02        //����������״̬
#define TS_MESSAGE  0x04        //�����ڵȴ���Ϣ״̬
#define TS_LOCK     0x08        //����������״̬


#define Tasknum 8	//�������ȼ�������Խ�����ȼ�Խ��
#define STACK_LEN 64			//����Ķ�ջ��С */
unsigned char _Stack[Tasknum][STACK_LEN];

#define Function_Nest_Depth 8         /* ����Ƕ�׵��õ������� */

#define SYSTEM_FREQ         3686400    /* ���嵥Ƭ���е�Ƶ�� ��λMHz */
#define TASK_SWITCH_FREQ    200        /* ����ÿ�����л����� */
#define CLOCK_PRESCALER     256        /* ʱ��Ԥ��Ƶ���� 1 8 64 256 1024 */

#define fosc 16000000 //����8MHZ 
#define baud 9600     //������ 

#if (CLOCK_PRESCALER == 1)
#define TCCR0_VALUE 0x09
#endif
#if (CLOCK_PRESCALER == 8)
#define TCCR0_VALUE 0x0A
#endif
#if (CLOCK_PRESCALER == 64)
#define TCCR0_VALUE 0x0B
#endif
#if (CLOCK_PRESCALER == 256)
#define TCCR0_VALUE 0x0C
#endif
#if (CLOCK_PRESCALER == 1024)
#define TCCR0_VALUE 0x0D
#endif

#define TCB_LENGTH          5

#define TCNT0_RELOAD_VALUE  ((SYSTEM_FREQ/CLOCK_PRESCALER/TASK_SWITCH_FREQ)-1)



//��ֹ�ж�
void  IntEnter(void);

//�ָ������ж�
void  IntLeave(void);

//����ʼ����
void  StartTaskRunning();

//�������
void  OS_Scheduler(void);
//��ʱʱ�䴦�� ��--��
void task_delay_dec();

//�л���ǰ����
unsigned char current_task_switch();

unsigned char task_create(void (*entry)(void), unsigned char priority);

unsigned char task_delay(unsigned char time);

//���̹���
unsigned char task_suspend(unsigned char priority);

//���ѽ���
unsigned char task_awake(unsigned char priority);

//�ȴ���Ϣ
unsigned char task_wait_message(void);

//������Ϣ
unsigned char task_send_message(unsigned char priority,unsigned char message);

//��������
void task_lock(void);

//�������
void task_unlock(void);

unsigned char start(void);

void Usart0_Init(void);

void Usart0_Putc(unsigned char ch);

void Usart0_Puts(char *s);

//��ӡ�ַ��Ķ����Ʊ���
void Usart0_Putcb(unsigned char ch);

//��ӡ�ַ����Ķ����Ʊ���
void Usart0_Putsb(char *s);

unsigned char Usart0_Getc(void);

void Usart1_Init(void);

void Usart1_Putc(unsigned char ch);

void Usart1_Puts(char *s);

unsigned char Usart1_Getc(void);


void Usart2_Init(void);

void Usart2_Putc(unsigned char ch);

void Usart2_Puts(char *s);

unsigned char Usart2_Getc(void);


void Usart3_Init(void);

void Usart3_Putc(unsigned char ch);

void Usart3_Puts(char *s);

unsigned char Usart3_Getc(void);

#endif

//120

