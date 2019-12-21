#ifndef DATA_H
#define DATA_H

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

// 将 9-1 位字符串转为整数
int Str2Int(char str[9])
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
    return sign * ret / 10;
}

// 将整数转为 9-1 位字符串
char *Int2Str(int num, char *ret)
{
    int i, isNegative = 0;
    ret[8] = '\0';

    if (num < 0)
    {
        num = -num;
        isNegative = 1;
    }

    for (i = 7; i >= 0; i--)
    {
        if (!num)
        {
            if (isNegative)
            {
                ret[i] = '-';
                isNegative = 0;
            }
            else
            {
                ret[i] = '_';
            }
        }
        else
        {
            ret[i] = num % 10 + '0';
        }
        num /= 10;
    }
    return ret;
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