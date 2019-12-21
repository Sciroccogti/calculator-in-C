#include <stdio.h>
#include <conio.h> // getch()
#include <string.h>
#include "mymath.h"
#include "data.h"
#include "led.h"

// 要显示的字符：0-9，负号-，空格_，小数点.

int main()
{
    char buff;                  // 读入字符
    char operand = '\0';        // 保存运算符
    char queue[9] = "________"; // 待显示字符
    int count = 0;              // 记录输入位数
    int mode = -1;              // 输入模式 -1:num1, 0:operand, 1:num2
    double num[3] = {0, 0, 0};
    int jump = 0; // 跳过最后的打印
    int dp = 9;   // 小数点位置

    display(queue, dp);

    while (buff = getch())
    {
        count++;
        if (buff >= '0' && buff <= '9') // 读到数字
        {
            if (dp < 9)
            {
                dp--;
            }
            PushQueue(queue, buff);
            num[mode > 0 ? mode : 0] = Str2Int(queue, dp);
        }
        else if (buff == '\n' || buff == '\r') // 一个元素输入完毕
        {

            if (mode) // 将queue转为int
            {
                strcpy(queue, "________");
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

                dp = Int2Str(num[2], queue);
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
            num[mode > 0 ? mode : 0] = Str2Int(queue, dp);
        }
        else if (buff == '.' && mode) // 输入数字时输入小数点
        {
            dp = 8;
        }
        else // 读到符号
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
    return 0;
}