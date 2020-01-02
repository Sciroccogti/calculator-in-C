#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stc15.h>
#include <intrins.h>
#include "stcled.h"

int scan(int col, unsigned temp)
{
    int row, i = 0;
    for (row = 1; row <= 8; row *= 2)
    {
        P0 = 0xf0 + row;
        temp = P0;
        if (temp == 0xf0 + row)
        {
            int = col * 4 + i;
            break;
        }
        i++;
    }
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

char sh_getch()
{
    unsigned temp;
    char ret;
    P0 = 0xf0;
    temp = P0 & 0xf0;
    //KeyScan();
    if (temp != 0xf0) // 若矩阵键盘接收到信号
    {
        delay(5);
        if (temp != 0xf0)
        {
            temp = P0;
            P17 = !P17;
            switch (temp)
            {
            case 0xe0:
                ret = scan(0, temp);
                break;
            case 0xd0:
                ret = scan(1, temp);
                break;
            case 0xb0:
                ret = scan(2, temp);
                break;
            case 0x70:
                ret = scan(3, temp);
                break;
            }
        }
        if (ret > 9) // 不是数字
        {
            switch (ret)
            {
            default:
            case 10:
                ret = '.';
                break;
            case 11:
                ret = '\n';
                break;
            case 12:
                ret = '+';
                break;
            case 13:
                ret = '-';
                break;
            case 14:
                ret = '*';
                break;
            case 15:
                ret = '/';
                break;
            }
        }
    }
    else // 若矩阵键盘无信号，监听ADC键盘
    {
        /* 以下是原代码
        unsigned short nAdc;
        int i = 0, num2 = 0;
        for (i = 0; i < 10; i++)
        {
            nAdc += Get_ADC10bitResult(4);
        }
        nAdc /= 10;
        num2 = nAdc / 0x44;
        ID[7] = num2 - 1;
        */
    }

    return ret;
}

#endif