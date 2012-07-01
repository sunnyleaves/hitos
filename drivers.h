
//led 驱动
void led_open(unsigned char port_num);

void led_close(unsigned char port_num);

//数码管静态显示

//向HC595发送一个字节
void HC595_send_byte(unsigned char byte);

//显示一个数num
void SMG_ShowNum(unsigned char seg);
//依次输出
void SMG_Shownums(unsigned char );

//动态输出
void SMG_Display(unsigned int dat);

/********************************************
                DA转换原理:
  用PWM控制占空比,输出的PWM信号经过外面的的
  低通滤波电路后得到平稳的直流信号,完成DA转换.
  占空比越大的时候,输出的直流电压就越高,反之,
  输出的直流电压就越小
*********************************************/
//PWM0(OC0)初始化

void PWM_INIT(void);

//PWM0输出
void PWM0_OUT(unsigned char num);

void DA(unsigned char Duty_Cycle[],unsigned char num);


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
 
void ADC_Init();	//端口初始化

//读取ADC转换值
//adc_input为ADC通道
unsigned int read_adc(unsigned char adc_input);

void USART_Init(void);

void adc(unsigned char num,unsigned char adc[]);


//外部中断
//外部中断初始化函数
void EXT_INT0_Init(void);

void interrupt();



//70
