//�����շ�����  ����171�4026
#include<reg52.h>	 
sbit BUZZ=P1^2;
sbit LED=P1^1;
unsigned char jiema(unsigned char  *buff);
unsigned char Combuf[16];
unsigned char Compointer=0;
void Send_char(unsigned char SData) ;
void Init(void)
{
	TMOD=0X20;//time2��λ�Զ���װ
	TH1=0XFD;		//9600������
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
		 	{	 BUZZ=1;   //	�ɹ���������LED��
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
unsigned char jiema(unsigned char  *buff)	//���뺯��
{	
	unsigned char *p=buff;	   
	unsigned char temp=0,a[64],b[10], HL, LL ;		//n���ڿ���ѭ�����ƴ�����temp����ѭ�����ƽ���
	int t=0,i,s=0,j=0,m=0,n=0;									//a[64]���2����id���ţ�HL16���Ƹ�4λLL��4λ   s���ڶ�����ѭ��														//j��żУ���־λ��m��żУ���־λ
    for(i=1;i<=8;i++)
 	{	
	    HL=(*buff)/16;		   //16���Ƹ�4λ
	  	LL=(*buff)%16;		   //16���Ƶ�4λ
		t=8*i-1;   //�����������ʼλ��
		for(s=0;s<4;s++)
		{								 //16����ת����2����
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
		a[i]=a[i-1];																	//����ѭ�������ҳ���ȷ����
		a[0]=temp;
		n++;
     }
	if(n<=64)
	{	
		for(i=2;i<=11;i++)
		{
	 		if((a[5*i-1]+a[5*i]+a[5*i+1]+a[5*i+2]+a[5*i+3])%2==0)	   //��żУ��
	    		j++ ;
		}	
	 	for(i=0;i<4;i++)
		{
	   		 if(((a[9+i]+a[14+i]+a[19+i]+a[24+i]+a[29+i]+a[34+i]+a[39+i]+a[44+i]+a[49+i]+a[54+i]+a[59+i])%2==0))  //��żУ��
	  			m++ ;
		}	
		if(j==10&&m==4)
		{
							  //У��ͨ������ID40����
 	 		for(i=2;i<=11;i++)
     		{
				b[i-2]=a[5*i-1]*2*2*2+a[5*i]*2*2+a[5*i+1]*2+a[5*i+2];   
	    	}  
	   		for(i=0;i<5;i++)									
	   		{
	   			*p=16*b[2*i]+b[2*i+1];
			 	 p++;
	  		 }
	   			return 1; 	//�ɹ�����1
	 	}	
	 	else
		{					   //У��ʧ�ܷ���0
	 		return 0;
	 
	 	}
	}
	else
	{
	  return 0;		 //δѰ�ҵ���ȷ����
	
	}

}
