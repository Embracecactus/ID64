//串口收发程序  物联171李健4026
#include<reg52.h>	 
sbit BUZZ=P1^2;
sbit LED=P1^1;
unsigned char jiema(unsigned char  *buff);
unsigned char Combuf[16];
unsigned char Compointer=0;
void Send_char(unsigned char SData) ;
void Init(void)
{
	TMOD=0X20;//time2八位自动重装
	TH1=0XFD;		//9600波特率
	TL1=0XFD;
	PCON=0;
	SCON=0X50;
	ES=1;
	EA=1;
	TR1=1;

}


void main(void)
{	unsigned char i;
	Init();
	BUZZ=0;		
	while(1)
   	{
		if(Compointer>=11) 
		{	 
		 	if(jiema(Combuf+2))
		 	{	 BUZZ=1;   //	成功蜂鸣器响LED亮
				 LED=0;	   //
		   		 for(i=0;i<5;i++)
		   		 Send_char(Combuf[i+2]);
		 	}
			else
			{	
			   	for(i=0;i<11;i++)
				Send_char(Combuf[i]);
			}
				Compointer=0;
		 }
	 }
			  	
}      
void Send_char(unsigned char SData)
{
	SBUF=SData;
	while(TI==0);
	TI=0;
}
void Uart(void) interrupt 4
{
	if (RI)
	{
		Combuf[Compointer++] =SBUF;
    	if(Compointer>=16)
		Compointer=0;
		RI=0;

	}
	 
}
unsigned char jiema(unsigned char  *buff)	//解码函数
{	
	unsigned char *p=buff;	   
	unsigned char temp=0,a[64],b[10], HL, LL ;		//n用于控制循环右移次数，temp用于循环右移交换
	int t=0,i,s=0,j=0,m=0,n=0;									//a[64]存放2进制id卡号，HL16进制高4位LL低4位   s用于二进制循环														//j行偶校验标志位，m列偶校验标志位
    for(i=1;i<=8;i++)
 	{	
	    HL=(*buff)/16;		   //16进制高4位
	  	LL=(*buff)%16;		   //16进制低4位
		t=8*i-1;   //存入数组的起始位置
		for(s=0;s<4;s++)
		{								 //16进制转换成2进制
			a[t]=LL%2;
			LL=LL/2;
			t--;
		}
		for(s=0;s<4;s++)
		{
			a[t]=HL%2;
			HL=HL/2;
			t--;
		}
   		buff++;	
    }
	while((!(a[0]==1&&a[1]==1&&a[2]==1&&a[3]==1&&a[4]==1&&a[5]==1&&a[6]==1&&a[7]==1&&a[8]==1&&a[63]==0))&&n<=64)
	{
		temp=a[63];
		for(i=63;i>0;i--)
		a[i]=a[i-1];																	//进行循环右移找出正确卡号
		a[0]=temp;
		n++;
     }
	if(n<=64)
	{	
		for(i=2;i<=11;i++)
		{
	 		if((a[5*i-1]+a[5*i]+a[5*i+1]+a[5*i+2]+a[5*i+3])%2==0)	   //行偶校验
	    		j++ ;
		}	
	 	for(i=0;i<4;i++)
		{
	   		 if(((a[9+i]+a[14+i]+a[19+i]+a[24+i]+a[29+i]+a[34+i]+a[39+i]+a[44+i]+a[49+i]+a[54+i]+a[59+i])%2==0))  //列偶校验
	  			m++ ;
		}	
		if(j==10&&m==4)
		{
							  //校验通过生成ID40卡号
 	 		for(i=2;i<=11;i++)
     		{
				b[i-2]=a[5*i-1]*2*2*2+a[5*i]*2*2+a[5*i+1]*2+a[5*i+2];   
	    	}  
	   		for(i=0;i<5;i++)									
	   		{
	   			*p=16*b[2*i]+b[2*i+1];
			 	 p++;
	  		 }
	   			return 1; 	//成功返回1
	 	}	
	 	else
		{					   //校验失败返回0
	 		return 0;
	 
	 	}
	}
	else
	{
	  return 0;		 //未寻找到正确卡号
	
	}

}
