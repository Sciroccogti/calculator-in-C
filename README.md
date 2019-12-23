# calculator-in-C

C语言实现的命令行计算器，为单片机实验做移植准备

![](https://progress-bar.dev/70/?title=done)

## 使用方法

1. 输入第一个数字（记为x），按下回车，此时数字被存储，显示界面清空；
2. 输入运算符，并按下回车（为模拟数码管，此时运算符并不会输出）：
   1. +、-、*、/
   2. s：sin(x)
   3. c：cos(x)
   4. l：$log_x(y)$
   5. p：$x^y$
3. 输入第二个数字（记为y），按下回车，此时数字被存储，运算结果输出在显示界面。
4. 存储功能：保存 *当前* 或 *按下回车前* **显示**的数字
   1. `z`代表 *MC*
   2. `x`代表 *MR*
   3. `c`代表 *M-*
   4. `v`代表 *M+*

注意：
*   按下`r`键重置运算，但是不会清除存储；
*   输入数字过程中按下`-`则改变当前数字符号，按下退格则删除末尾字符；
*   输入字符上限为8位（包括负号），实际存储数字以按下回车前的显示字符为准；
*   命令行版本中的`_`用于表示数码管中的 *熄灭*。

![](calculator1.gif)

## 实验要求

利用STC实验箱，设计并实现计算器。键盘布局如下。

0|1|2|3
---|---|---|---
4|5|6|7
8|9|.|=
+|-|×|÷

* 	基本要求：计数器具备加减乘除四则运算功能。通过键盘输入，结果显示在8位LED数码管上。
* 	附加要求1：判断运算结果的合理性，对异常结果进行标识。
* 	附加要求2：增加“Backspace”（当前位取消）和“Reset”（复位）功能。“Backspace”取消当前输入的数字；“Reset”则清除当前运算，重新等待第一个运算数据。
* 	附加要求3：增加“MC”、“MR”、“M-”和“M+”功能。
* 	附加要求4：增加“sin(x)”、“cos(x)”、“logx(y)”和“x^y”功能。
* 	附加的其他功能。
> 附加功能所需的按键可以从外部中断、ADC键盘中选择。

## 评分标准

- [ ] 完成初步的方案设计。（20％）
- [x] 完成-128~127（8bit）四则运算。（10％）
- [x] 完成8位数字四则运算。（10％）
- [x] 完成带小数的8位四则运算。（10％）
- [x] 附加功能1-4。（各5%，计20%）
- [x] 程序编写质量。（10％）
- [ ] 实验报告。（10％）
- [ ] 独立完成或主要完成人（每个小组只能有一个主要完成人）。（10%）
- [ ] 附加的其他功能酌情加分。

## 环境配置

针对命令行阶段，本项目建议使用 *GCC* 编译运行。
