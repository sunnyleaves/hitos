
//led ����
void led_open(unsigned char port_num);

void led_close(unsigned char port_num);

//����ܾ�̬��ʾ

//��HC595����һ���ֽ�
void HC595_send_byte(unsigned char byte);

//��ʾһ����num
void SMG_ShowNum(unsigned char seg);
//�������
void SMG_Shownums(unsigned char );

//��̬���
void SMG_Display(unsigned int dat);

/********************************************
                DAת��ԭ��:
  ��PWM����ռ�ձ�,�����PWM�źž�������ĵ�
  ��ͨ�˲���·��õ�ƽ�ȵ�ֱ���ź�,���DAת��.
  ռ�ձ�Խ���ʱ��,�����ֱ����ѹ��Խ��,��֮,
  �����ֱ����ѹ��ԽС
*********************************************/
//PWM0(OC0)��ʼ��

void PWM_INIT(void);

//PWM0���
void PWM0_OUT(unsigned char num);

void DA(unsigned char Duty_Cycle[],unsigned char num);


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
 
void ADC_Init();	//�˿ڳ�ʼ��

//��ȡADCת��ֵ
//adc_inputΪADCͨ��
unsigned int read_adc(unsigned char adc_input);

void USART_Init(void);

void adc(unsigned char num,unsigned char adc[]);


//�ⲿ�ж�
//�ⲿ�жϳ�ʼ������
void EXT_INT0_Init(void);

void interrupt();



//70
