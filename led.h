#ifndef LED_H
#define LED_H
#include <stdio.h>

void display(char str[9], int dp)
{
    int i;
    printf("\b \r");
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