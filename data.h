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
    int ret = 0, i;
    for (i = 0; i < 8; i++)
    {
        if (str[i] != '_')
        {
            ret += str[i] - 48;
            ret *= 10;
        }
    }
    return ret / 10;
}

// 将整数转为 9-1 位字符串
char * Int2Str(int num, char * ret)
{
    int i;
    ret[8] = '\0';
    for (i = 7; i >= 0; i--)
    {
        ret[i] = num % 10 + '0';
        num /= 10;
    }
    return ret;
}

#endif