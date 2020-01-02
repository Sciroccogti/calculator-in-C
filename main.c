#include <stdio.h>
#include "getch.h" // getch()
#include <string.h>
#include "mymath.h"
#include "data.h"
#include "stcled.h"

int main()
{
    char buff;                  // 读入字符
    char operand = '\0';        // 保存运算符
    char queue[9] = "________"; // 待显示字符
    int count = 0;              // 记录输入位数
    int mode = -1;              // 输入模式 -1:num1, 0:operand, 1:num2
    double num[3] = {0, 0, 0}, memory = 0;
    int jump = 0; // 跳过最后的打印
    int dp = 9;   // 小数点位置
    unsigned temp;
    unsigned short nAdc;
    EA = 1;  //开中断
    EX0 = 1; //开外部中断0
    EX1 = 1; //开外部中断1
    IT0 = 1; //外部中断0设为脉冲负跳变有效
    IT1 = 1; //外部中断1设为脉冲负跳变有效
    change(ID);
    P1ASF = 0x10;     //P1.4做ADC
    ADC_CONTR = 0xE0; //90T, ADC power on

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
    return 0;
}
