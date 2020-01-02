#ifndef STCLED_H
#define STCLED_H

#include <stc15.h>
#include <intrins.h>

sbit P_HC595_SER = P4 ^ 0;   // ER	  data input
sbit P_HC595_RCLK = P5 ^ 4;  // RCLk	  store clock
sbit P_HC595_SRCLK = P4 ^ 3; // SRCLK  Shift data clock

unsigned int num[] = {0x003F, 0x0006, 0x005B, 0x004F, 0x0066,
                      0x006D, 0x007D, 0x0007, 0x007F, 0x006F,         // 0-9
                      0x007F, 0x007C, 0x0039, 0x005E, 0x0079, 0x0071, // A-F
                      0x0040, 0x0000, 0x0050, 0x005C,                 // -, _, r, o:16-19
                      0x00BF, 0x0086, 0x00DB, 0x00CF, 0x00E6,
                      0x00ED, 0x00FD, 0x0087, 0x00FF, 0x00EF}; // 0.-9.:20-29
unsigned int ID[8] = {0, 0, 0, 0, 0, 0, 0, 0};
unsigned char dig[8] = {0x00fe, 0x00fd, 0x00fb, 0x00f7, 0x00ef, 0x00df, 0x00bf, 0x007f};
unsigned int datalist[8];
int count = 0;
int s = 0;

// 要显示的字符：0-9，负号-，空格_，小数点.，E，r，o。

void scan(int col, unsigned temp);

//软件延时
void delay(int i)
{
    int j, k;
    for (j = i; j > 0; j--)
        ;
    for (k = 0; k < 30; k++)
        ;
}

//学号转十六位数据
void change(unsigned int *a)
{
    int m;
    unsigned char b = 0xFE; //1111 1110
    unsigned int c;
    for (m = 0; m < 8; m++)
    {
        c = b;                              //0000 0000 1111 1110
        datalist[m] = num[a[m]] + (c << 8); // 1111 1110 0000 0000 +num[temp]
        b = _crol_(b, 1);
    }
}

//向HC595发送两个字节函数（前8位为位码 0为选中数码管，后8位为段码）
void Send_595(unsigned int dat)
{
    int i;
    for (i = 0; i < 16; i++)
    {
        P_HC595_SRCLK = 0;
        dat <<= 1;
        P_HC595_SER = CY;
        P_HC595_SRCLK = 1; //上升沿有效
        _nop_();           //延时一个时钟周期
    }
    //位移寄存器完毕，转移到存储寄存器
    P_HC595_RCLK = 1; //	上升沿有效	延迟两个时钟周期
    _nop_();
    _nop_();
}

//数码管显示函数
void display_stc()
{
    int p;
    for (p = 0; p < 8; p++)
    {
        P_HC595_RCLK = 0;
        Send_595(datalist[p]);
    }
    P_HC595_RCLK = 0;
    Send_595(0xFFFF);
    delay(s);
}

unsigned short Get_ADC10bitResult(unsigned char channel) //channel = 0~7
{
    ADC_RES = 0;
    ADC_RESL = 0;

    ADC_CONTR = (ADC_CONTR & 0xe0) | 0x08 | channel; //start the ADC
    _nop_();
    _nop_();
    _nop_();
    _nop_();

    while ((ADC_CONTR & 0x10) == 0)
        ;               //wait for ADC finish
    ADC_CONTR &= ~0x10; //清除ADC结束标志
    return (((unsigned short)ADC_RES << 2) | (ADC_RESL & 3));
}

int str2led(char ch)
{
    int ret = 14; // 默认显示E
    if (ch >= '0' && ch <= '9')
    {
        ret = ch - '0';
    }
    else
    {
        switch (ch)
        {
        case '-':
            ret = 16;
            break;
        case '_':
            ret = 17;
            break;
        case 'r':
            ret = 18;
            break;
        case 'o':
            ret = 19;
            break;
        default:
            break;
        }
    }
    return ret;
}

void display(char str[9], int dp)
{
    int i;
    // printf("\b \r");
    if (dp == -1) // 整数输出模式
    {
        dp = 9;
    }

    for (i = 0; i < 9; i++)
    {
        if (i < dp)
        {
            ID[i] = str2led(str[i]);
            // printf("%c", str[i]);
        }
        else if (i == dp)
        {
            ID[i] = str2led(str[i]) + 20;
            // printf(".");
        }
        else
        {
            ID[i] = str2led(str[i - 1]);
            // printf("%c", str[i - 1]);
        }
    }
    change(ID);
    display_stc();
}

#endif