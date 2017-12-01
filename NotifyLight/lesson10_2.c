#include <reg52.h>
#include <intrins.h>
sbit LED1=P1^1;//左一
sbit LED2=P1^2;//左二
sbit LED3=P1^3;//左三
sbit PWMOUT1 = P2^5;    //指示灯0  红
sbit PWMOUT2 = P2^4;   //指示灯1
sbit PWMOUT3 = P2^3;   //指示灯3   绿
unsigned char tmp;
sbit 	RL1=P1^0;
unsigned char HighRH = 0;//高电平重载值高八位
unsigned char HighRL = 0;//高电平重载值低八位
unsigned char LowRH = 0; //低电平重载值高八位
unsigned char LowRL = 0; //低电平重载值低八位
void ConfigPWM(unsigned int fr, unsigned char dc);
void ClosePWM();
void Delay_ms( unsigned int m);
void delay(unsigned char y);
void init();
void send(); //发送回馈信息，目前无效不知道什么原因
void ctrl();
void bluetooth();

void delay(unsigned char y) 
{ 
   unsigned char j,k; 
   for(j=y;j>0;j--) 
   for(k=0;k<57;k--); 
} 
//**********************************************************
//延时函数，延时1s
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
    EA = 1;     //开总中断
	init();
    while (1)
    {	 
	    if(1==RL1)  	{Delay_ms(2000);	LED1=!LED1;	}  //光敏电阻没光取反
		else            { LED1=1;	}
 		Delay_ms(2000);	

	    if(RI==1)                     // 是否有数据到来
        {
            RI = 0;
            tmp = SBUF;                   // 暂存接收到的数据
            ctrl();
        }  
		//Delay_ms(2000);
//	    PWMOUT1=0;	//rgb变换颜色	   
//	 	Delay_ms(2000);	
//		PWMOUT1=1;
//		PWMOUT3=0;
//		Delay_ms(2000);
//		PWMOUT3=1;
//		PWMOUT2=0;
//		Delay_ms(2000);
//		PWMOUT2=1;
        ConfigPWM(100, 10);    //频率100Hz，占空比10%
        for (i=0; i<40000; i++);
        ClosePWM();
        ConfigPWM(100, 50);    //频率100Hz，占空比40%
        for (i=0; i<40000; i++);
        ClosePWM();
        ConfigPWM(100, 90);    //频率100Hz，占空比90%
        for (i=0; i<40000; i++);
        ClosePWM();            //关闭PWM，相当于占空比100%	 
        for (i=0; i<40000; i++);
    }




}


void ConfigPWM(unsigned int fr, unsigned char dc) //频率，占空比
{
	unsigned long tmp;//中间变量
	unsigned int high, low;

	tmp = 11059200/12/fr; //  /12为频率，/fr为一个周期[高电平+低电平]所需计数值（100hz计数值）
	high = (tmp*dc)/100;//高电平所需计数值
	low = tmp - high; //低电平所需计数值
	high = 65536 - high + 12;//定时器重载值进行补偿12
	low = 65536 - low + 12;
	HighRH = (unsigned char)(high >>8);//高电平高八位
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

    if (LED1==1 && LED2==1  )  //当前输出为高电平时，装载低电平值并输出低电平
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
    else              //当前输出为低电平时，装载高电平值并输出高电平
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



void init() //初始化
 {
  TMOD = 0x20;                        // 定时器1工作于8位自动重载模式, 用于产生波特率
          TH1=TL1=0xFD;                                // 波特率9600
          
          SCON = 0x50;                        // 设定串行口工作方式1 允许接收,相当于下面三句
         
          PCON =0x00;                        // 波特率不倍增
          TR1 = 1;
    EA=1;
    ES=1;
    PWMOUT1=0;                            //初始化设置3个指示灯全亮
    PWMOUT2=0;
    PWMOUT3=0;
 }
 
void send() //发送回馈信息，目前无效不知道什么原因
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
void ctrl()   //根据接受信息做出操作
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