#ifndef _CORE_TAG_
#define _CORE_TAG_


#define TS_DELAY    0x01        //任务处于延时状态
#define TS_SUSPEND  0x02        //任务处于休眠状态
#define TS_MESSAGE  0x04        //任务处于等待消息状态
#define TS_LOCK     0x08        //任务处于锁定状态


#define Tasknum 8	//任务优先级，数字越低优先级越高
#define STACK_LEN 64			//任务的堆栈大小 */
unsigned char _Stack[Tasknum][STACK_LEN];

#define Function_Nest_Depth 8         /* 函数嵌套调用的最大层数 */

#define SYSTEM_FREQ         3686400    /* 定义单片运行的频率 单位MHz */
#define TASK_SWITCH_FREQ    200        /* 任务每秒钟切换次数 */
#define CLOCK_PRESCALER     256        /* 时钟预分频因子 1 8 64 256 1024 */

#define fosc 16000000 //晶振8MHZ 
#define baud 9600     //波特率 

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



//禁止中断
void  IntEnter(void);

//恢复允许中断
void  IntLeave(void);

//任务开始运行
void  StartTaskRunning();

//任务调度
void  OS_Scheduler(void);
//延时时间处理 “--”
void task_delay_dec();

//切换当前进程
unsigned char current_task_switch();

unsigned char task_create(void (*entry)(void), unsigned char priority);

unsigned char task_delay(unsigned char time);

//进程挂起
unsigned char task_suspend(unsigned char priority);

//唤醒进程
unsigned char task_awake(unsigned char priority);

//等待消息
unsigned char task_wait_message(void);

//发送消息
unsigned char task_send_message(unsigned char priority,unsigned char message);

//锁定进程
void task_lock(void);

//解除锁定
void task_unlock(void);

unsigned char start(void);

void Usart0_Init(void);

void Usart0_Putc(unsigned char ch);

void Usart0_Puts(char *s);

//打印字符的二进制编码
void Usart0_Putcb(unsigned char ch);

//打印字符串的二进制编码
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

