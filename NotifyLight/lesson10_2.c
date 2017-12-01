#include <reg52.h>
#include <intrins.h>
sbit LED1=P1^1;//��һ
sbit LED2=P1^2;//���
sbit LED3=P1^3;//����
sbit PWMOUT1 = P2^5;    //ָʾ��0  ��
sbit PWMOUT2 = P2^4;   //ָʾ��1
sbit PWMOUT3 = P2^3;   //ָʾ��3   ��
unsigned char tmp;
sbit 	RL1=P1^0;
unsigned char HighRH = 0;//�ߵ�ƽ����ֵ�߰�λ
unsigned char HighRL = 0;//�ߵ�ƽ����ֵ�Ͱ�λ
unsigned char LowRH = 0; //�͵�ƽ����ֵ�߰�λ
unsigned char LowRL = 0; //�͵�ƽ����ֵ�Ͱ�λ
void ConfigPWM(unsigned int fr, unsigned char dc);
void ClosePWM();
void Delay_ms( unsigned int m);
void delay(unsigned char y);
void init();
void send(); //���ͻ�����Ϣ��Ŀǰ��Ч��֪��ʲôԭ��
void ctrl();
void bluetooth();

void delay(unsigned char y) 
{ 
   unsigned char j,k; 
   for(j=y;j>0;j--) 
   for(k=0;k<57;k--); 
} 
//**********************************************************
//��ʱ��������ʱ1s
//**********************************************************
void Delay_ms( unsigned int m)
{
	unsigned int j;
	unsigned int i;
	for(i = 0 ; i < m ; i++)
	{
		for(j = 0 ; j < 123 ; j++);
	}
}
    
 
void main()
{
    unsigned int i;	 	     
    EA = 1;     //�����ж�
	init();
    while (1)
    {	 
	    if(1==RL1)  	{Delay_ms(2000);	LED1=!LED1;	}  //��������û��ȡ��
		else            { LED1=1;	}
 		Delay_ms(2000);	

	    if(RI==1)                     // �Ƿ������ݵ���
        {
            RI = 0;
            tmp = SBUF;                   // �ݴ���յ�������
            ctrl();
        }  
		//Delay_ms(2000);
//	    PWMOUT1=0;	//rgb�任��ɫ	   
//	 	Delay_ms(2000);	
//		PWMOUT1=1;
//		PWMOUT3=0;
//		Delay_ms(2000);
//		PWMOUT3=1;
//		PWMOUT2=0;
//		Delay_ms(2000);
//		PWMOUT2=1;
        ConfigPWM(100, 10);    //Ƶ��100Hz��ռ�ձ�10%
        for (i=0; i<40000; i++);
        ClosePWM();
        ConfigPWM(100, 50);    //Ƶ��100Hz��ռ�ձ�40%
        for (i=0; i<40000; i++);
        ClosePWM();
        ConfigPWM(100, 90);    //Ƶ��100Hz��ռ�ձ�90%
        for (i=0; i<40000; i++);
        ClosePWM();            //�ر�PWM���൱��ռ�ձ�100%	 
        for (i=0; i<40000; i++);
    }




}


void ConfigPWM(unsigned int fr, unsigned char dc) //Ƶ�ʣ�ռ�ձ�
{
	unsigned long tmp;//�м����
	unsigned int high, low;

	tmp = 11059200/12/fr; //  /12ΪƵ�ʣ�/frΪһ������[�ߵ�ƽ+�͵�ƽ]�������ֵ��100hz����ֵ��
	high = (tmp*dc)/100;//�ߵ�ƽ�������ֵ
	low = tmp - high; //�͵�ƽ�������ֵ
	high = 65536 - high + 12;//��ʱ������ֵ���в���12
	low = 65536 - low + 12;
	HighRH = (unsigned char)(high >>8);//�ߵ�ƽ�߰�λ
	HighRL = (unsigned char)high;
	LowRH = (unsigned char)(low >> 8);
	LowRL = (unsigned char)low;
	TMOD &= 0xF0;
	TMOD |= 0x01;
	TH0 = HighRH;
	TL0 = HighRL;
	ET0 = 1;
	TR0 = 1;
	LED1=1;	 	  
	LED2=1;	     	  
	LED3=1;	   
//	PWMOUT1 = 0;
//	Delay_ms();
//	PWMOUT2 = 0;
//	Delay_ms();
//	PWMOUT3 = 0;
	
}
void ClosePWM()
{
	TR0 = 0;
	ET0 = 0;
	LED1=1;	    
	LED2=1;	    
	LED3=1;	    
//	PWMOUT1 = 0;
//	Delay_ms();
//	PWMOUT2 = 0;
//	Delay_ms();
//	PWMOUT3 = 0;
}
void InterruptTimer0() interrupt 1
{

    if (LED1==1 && LED2==1  )  //��ǰ���Ϊ�ߵ�ƽʱ��װ�ص͵�ƽֵ������͵�ƽ
    {
        TH0 = LowRH;
        TL0 = LowRL;
		LED1=0;	    
	    LED2=0;	    		 
	    LED3=0;	    
//        PWMOUT1 = 1;
//		Delay_ms();
//		PWMOUT2 = 1;
//		Delay_ms();
//		PWMOUT3 = 1;
    }
    else              //��ǰ���Ϊ�͵�ƽʱ��װ�ظߵ�ƽֵ������ߵ�ƽ
    {
        TH0 = HighRH;
        TL0 = HighRL;
		LED1=1;	    
     	LED2=1;	    
    	LED3=1;	    
//        PWMOUT1 = 0;
//		Delay_ms();
//	    PWMOUT2 = 0;
//		Delay_ms();
//	    PWMOUT3 = 0;
    }
	  
}



void init() //��ʼ��
 {
  TMOD = 0x20;                        // ��ʱ��1������8λ�Զ�����ģʽ, ���ڲ���������
          TH1=TL1=0xFD;                                // ������9600
          
          SCON = 0x50;                        // �趨���пڹ�����ʽ1 �������,�൱����������
         
          PCON =0x00;                        // �����ʲ�����
          TR1 = 1;
    EA=1;
    ES=1;
    PWMOUT1=0;                            //��ʼ������3��ָʾ��ȫ��
    PWMOUT2=0;
    PWMOUT3=0;
 }
 
void send() //���ͻ�����Ϣ��Ŀǰ��Ч��֪��ʲôԭ��
{
 unsigned char a;
 a=tmp;
 TI=0;
 SBUF=a;
 while(TI==0);
 TI=0;
 //led3=1;
 //delay(1000);
}
void ctrl()   //���ݽ�����Ϣ��������
{
        switch(tmp)
   {
     case '1':	        PWMOUT1=0;                           send();       break;     
     case '2':          PWMOUT1=1;                           send();       break;

     case '3':	        PWMOUT2=0;                           send();       break;     
     case '4':          PWMOUT2=1;                           send();       break; 

	 case '5':	        PWMOUT3=0;                           send();       break;     
     case '6':          PWMOUT3=1;                           send();       break;

     default:           PWMOUT1&&PWMOUT2&&PWMOUT3==1;    send();       break;
   }
}