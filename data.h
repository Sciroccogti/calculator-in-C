#ifndef DATA_H
#define DATA_H
#include <math.h>
#include <stdlib.h>
// 把 9-1 位字符串当作队列，向其后推入一个字符
void PushQueue(char *queue, char c)
{
    int i;
    for (i = 0; i < 7; i++)
    {
        queue[i] = queue[i + 1];
    }
    queue[7] = c;
}

// 把 9-1 位字符串当作队列，向弹出其末尾一个字符
void PopQueue(char *queue)
{
    int i;
    for (i = 7; i > 0; i--)
    {
        queue[i] = queue[i - 1];
    }
}

// 将 9-1 位字符串转为数字
double Str2Num(char str[9], int dp)
{
    int ret = 0, i, sign = 1;

    for (i = 0; i < 8; i++)
    {
        if (str[i] >= '0' && str[i] <= '9')
        {
            ret += str[i] - 48;
            ret *= 10;
        }
        else if (str[i] == '-')
        {
            sign = -1;
        }
    }

    return sign * ret / Pow(10, 9 - (dp > 8 ? 8 : dp));
}

// 将整数转为 9-1 位字符串，返回小数点位置，若溢出则返回-1
int Num2Str(double num, char *str)
{
    // int a = Log(num);
    int i = 0, isNegative = 0, dp = (int)(log(fabs(num) + 0.1) / log(10) + 1);
    str[8] = '\0';

    if (num < 0)
    {
        num = -num;
        isNegative = 1;
        i++;
        dp++;
        str[0] = '-';
    }

    if (dp >= 8)
    {
        return -1;
    }

    if (fabs(num) < 1) // 首位为0
    {
        str[i++] = '0';
        dp++;
    }

    num /= pow(10, dp - i - 1);

    for (; i < 8; i++)
    {
        str[i] = (int)num % 10 + '0';
	num *= 10;
    }

    return dp;
}

// 将 9-1 位字符串取相反数
int Negative(char *str, int num)
{
    int i = 0;
    while (str[i + 1] == '_')
    {
        i++;
    }

    if (num > 0) // 正数
    {
        str[i] = '-';
        num = -num;
    }
    else if (str[i + 1] == '-')
    {
        str[i + 1] = '_';
        num = -num;
    }

    return num;
}

#endif
