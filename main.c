#include <stdio.h>
#include <conio.h> // getch()
#include <string.h>
#include "mymath.h"
#include "data.h"

int main()
{
    char buff;                  // 读入字符
    char operand = '\0';        // 保存运算符
    char queue[9] = "________"; // 待显示字符
    printf("%s", queue);
    int count = 0; // 记录输入位数
    int mode = -2; // 输入模式 -1:num1, 0:operand, 1:num2
    int num[3] = {0, 0, 0};
    int jump = 0; // 跳过最后的打印
    while (buff = getch())
    {
        count++;
        if (buff >= '0' && buff <= '9') // 读到数字
        {
            PushQueue(queue, buff);
        }
        else if (buff == '\n' || buff == '\r') // 一个元素输入完毕
        {
            mode++;

            if (mode) // 将queue转为int
            {
                num[mode > 0 ? mode : 0] = Str2Int(queue);
                strcpy(queue, "________");
            }
            if (mode == 1)
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
                default:
                    break;
                }

                Int2Str(num[2], queue);
                printf("\r%s", queue);
                strcpy(queue, "________");
                jump = 1;
                mode = -2;
            }
        }
        else // 读到符号
        {
            operand = buff;
        }

        // 输出
        if (!jump)
        {
            printf("\r%s", queue);
        }else
        {
            jump = 0;
        }
        
    }
    return 0;
}