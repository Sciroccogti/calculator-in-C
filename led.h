#ifndef LED_H
#define LED_H
#include <stdio.h>

// 要显示的字符：0-9，负号-，空格_，小数点.，E，r，o。

void display(char str[9], int dp)
{
    int i;
    printf("\b \r");
    if (dp == -1 || !dp)
    {
        dp = 9;
    }
    
    for (i = 0; i < 9; i++)
    {
        if (i < dp)
        {
            printf("%c", str[i]);
        }else if(i == dp)
        {
            printf(".");
        }
        else
        {
            printf("%c", str[i - 1]);
        }        
    }
}

#endif