#include<stc15.h>
#include<intrins.h>

#include <stdio.h>
#include "getch.h" // getch()
#include <string.h>
#include "mymath.h"
#include "data.h"
#include "led.h"


sbit	P_HC595_SER   = P4^0;	// ER	  data input
sbit	P_HC595_RCLK  = P5^4;	// RCLk	  store clock
sbit	P_HC595_SRCLK = P4^3;	// SRCLK  Shift data clock

unsigned int num[]={0x003F,0x0006,0x005B,0x004F,0x0066,0x006D,0x007D,0x0007,0x007F,0x006F, 0x007F, 0x007C, 0x0039, 0x005E, 0x0079, 0x0071};
unsigned int ID[8] = {0, 0, 0, 0, 0, 0, 0, 0};
unsigned char dig[8] = {0x00fe, 0x00fd, 0x00fb, 0x00f7, 0x00ef, 0x00df, 0x00bf, 0x007f};
unsigned int datalist[8];
int count=0;
int s=0;

void scan(int col, unsigned temp);

//软件延时
void delay(int i)
{
	int j,k;
    for(j=i;j>0;j--);
		for(k=0;k<30;k++);
}		
//学号转十六位数据
void change(unsigned int *a)
{	
	int m;
	unsigned char b=0xFE;//1111 1110 
	unsigned int c;
	for(m=0;m<8;m++)
	{
		c=b;			  //0000 0000 1111 1110 
		datalist[m]= num[a[m]]+(c<<8);		  // 1111 1110 0000 0000 +num[temp]
		b=_crol_(b,1);   		
	}	 						
}

//向HC595发送两个字节函数（前8位为位码 0为选中数码管，后8位为段码）
void Send_595(unsigned int  dat)
{       
    int i;
    for(i=0;i<16;i++)
    {	
	    P_HC595_SRCLK = 0;
        dat <<= 1;
        P_HC595_SER = CY; 
        P_HC595_SRCLK = 1; //上升沿有效	
		_nop_();  //延时一个时钟周期
    }
 //位移寄存器完毕，转移到存储寄存器
  	P_HC595_RCLK = 1;//	上升沿有效	延迟两个时钟周期
	_nop_();
	_nop_();	

}

//数码管显示函数
void display()
{	  
	int p;
	for(p=0;p<8;p++)
	{
		    P_HC595_RCLK = 0;
			Send_595(datalist[p]);
	     	
	}
    P_HC595_RCLK = 0;
  	Send_595(0xFFFF);
	delay(s);

}

unsigned short Get_ADC10bitResult(unsigned char channel)  //channel = 0~7
{
    ADC_RES = 0;
    ADC_RESL = 0;

    ADC_CONTR = (ADC_CONTR & 0xe0) | 0x08 | channel;    //start the ADC
    _nop_();
    _nop_();
    _nop_();
    _nop_();

    while((ADC_CONTR & 0x10) == 0)  ;   //wait for ADC finish
    ADC_CONTR &= ~0x10;     //清除ADC结束标志
    return  (((unsigned short)ADC_RES << 2) | (ADC_RESL & 3));
}

void main()
{	  
	unsigned temp;
	unsigned short nAdc;
	EA=1; //开中断
	EX0=1;//开外部中断0
	EX1=1;//开外部中断1
	IT0=1;//外部中断0设为脉冲负跳变有效
	IT1=1;//外部中断1设为脉冲负跳变有效
    change(ID);
   	P1ASF = 0x10;       //P1.4做ADC
	ADC_CONTR = 0xE0;   //90T, ADC power on 
	/////////////////////////////////////////////////////////////////////////////   
	   char buff;                  // 读入字符
	   char operand = '\0';        // 保存运算符
	   char queue[9] = "________"; // 待显示字符
	   int count = 0;              // 记录输入位数
	   int mode = -1;              // 输入模式 -1:num1, 0:operand, 1:num2
	   double num[3] = {0, 0, 0}, memory = 0;
	   int jump = 0; // 跳过最后的打印
	   int dp = 9;   // 小数点位置
   
	   display(queue, dp);
   
	   while (buff = sh_getch())
	   {
		   count++;
	   if (buff == 3 || buff == 26) // ctrl+c or ctrl+z
	   {
		   printf("\n");
		   return 0;
	   } 
		   else if (buff == 'r') // r，即reset
		   {
			   mode = -1;
			   strcpy(queue, "________");
			   dp = 9;
		   }
		   else if (buff == '\b') // 退格
		   {
			   if (mode)
			   {
				   PopQueue(queue);
				   dp++;
				   num[mode > 0 ? mode : 0] = Str2Num(queue, dp);
			   }
			   else
			   {
				   operand = '\0';
			   }
		   }
		   else if (buff >= '0' && buff <= '9' && mode) // 读到数字
		   {
			   if (dp < 9)
			   {
				   dp--;
			   }
			   PushQueue(queue, buff);
			   num[mode > 0 ? mode : 0] = Str2Num(queue, dp);
		   }
		   else if (buff == '\n' || buff == '\r') // 一个元素输入完毕
		   {
   
			   if (mode) // 清空输出
			   {
				   strcpy(queue, "________");
				   dp = 9;
			   }
   
			   mode++;
   
			   if (mode == 2)
			   {
				   switch (operand)
				   {
				   case '+':
					   num[2] = num[0] + num[1];
					   break;
				   case '-':
					   num[2] = num[0] - num[1];
					   break;
				   case '*':
					   num[2] = num[0] * num[1];
					   break;
				   case '/':
					   num[2] = num[0] / num[1];
					   break;
				   case 's':
					   num[2] = sin(num[0]);
					   break;
				   case 'c':
					   num[2] = cos(num[0]);
					   break;
				   case 'l':
					   num[2] = log(num[1]) / log(num[0]);
					   break;
				   case 'p':
					   num[2] = pow(num[0], num[1]);
					   break;
				   default:
					   break;
				   }
   
				   dp = Num2Str(num[2], queue);
				   if (dp == -1) // 溢出
				   {
					   strcpy(queue, "Error___");
				   }
   
				   display(queue, dp);
				   strcpy(queue, "________");
				   jump = 1; // 跳过最后的输出
				   mode = -1;
				   dp = 9;
			   }
		   }
		   else if (buff == '-' && mode) // 输入数字时输入负号
		   {
			   num[mode > 0 ? mode : 0] = Negative(queue, num[mode > 0 ? mode : 0]);
			   num[mode > 0 ? mode : 0] = Str2Num(queue, dp);
		   }
		   else if (buff == '.' && mode) // 输入数字时输入小数点
		   {
			   dp = 8;
		   }
		   else if (buff == 'v') // v，即MC
		   {
			   memory = 0;
		   }
		   else if (buff == 'b' && mode) // b，即MR
		   {
			   num[mode > 0 ? mode : 0] = memory;
			   dp = Num2Str(num[mode > 0 ? mode : 0], queue);
		   }
		   else if (buff == 'n') // n, 即M-
		   {
			   // -1 -> 2, 0 -> 0, 1 -> 1
			   memory -= num[mode < 0 ? 2 : mode];
		   }
		   else if (buff == 'm') // m, 即M+
		   {
			   // -1 -> 2, 0 -> 0, 1 -> 1
			   memory += num[mode < 0 ? 2 : mode];
		   }
		   else if (!mode) // 读到符号
		   {
			   operand = buff;
		   }
   
		   // 输出
		   if (!jump)
		   {
			   display(queue, dp);
		   }
		   else
		   {
			   jump = 0;
		   }
	   }



	   
	while(1){
		int i = 0, num2 = 0;
		for(i = 0; i < 10; i++){
			nAdc += Get_ADC10bitResult(4);
		}
		nAdc /= 10;
		num2 = nAdc / 0x44;
		ID[7] = num2 - 1;
		P0 = 0xf0;
        temp = P0 & 0xf0;
		//KeyScan();
        if(temp != 0xf0)
        {
            delay(5);
            if(temp != 0xf0)
            {
				temp = P0;
				P17 = !P17;
                switch(temp)
                {
					case 0xe0:
						scan(0, temp);
						break;
					case 0xd0:
						scan(1, temp);
						break;
					case 0xb0:
						scan(2, temp);
						break;
					case 0x70:
						scan(3, temp);
						break;
                }
            }
        }else{
				ID[0] = ID[1] = ID[2] = ID[3] = 0;
			}
		
		change(ID);
		display();
	}
}

void scan(int col, unsigned temp)
{
	int row, i = 0;
	for(row = 1; row <= 8; row *= 2)
	{
		P0 = 0xf0 + row;
		temp = P0;
		if(temp == 0xf0 + row)
		{
			ID[0] = ID[1] = ID[2] = 0;
			ID[3] = col * 4 + i;
			break;
		}
		i++;
	}
}

void X1_ISR(void)interrupt 2
{
   s=s+30;
}

void display(char str[9], int dp)//显示到led上
{
	int i;
	double numContent = Str2Num(str,dp);
	int numContent_int = int(numContent); 
    //printf("\b \r");
    if (dp == -1)
    {
        dp = 8;
    }
    
    for (i = 0; i < 8; i++)
    {
        if (i < dp)
        {
			int temp = numContent_int/10;
			ID[i] = numContent_int - temp * 10;
			numContent_int /= 10;		
        }else if(i == dp)
        {
            
        }
        else
        {
            printf("%c", str[i - 1]);
        }        
    }
}
