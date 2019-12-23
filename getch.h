/*
 * error        => 以后再说
 * 跨平台的丑陋从这里开始
 * __GNUC        => linux 平台特殊操作
 * __MSC_VER    => window 平台特殊操作
 */
#ifdef __GNUC__  // 下面是依赖GCC编译器实现

#include <termio.h>
/*
 * 得到用户输入的一个字符
 *        : 返回得到字符
 */
int 
sh_getch(void) {
    int cr;
    struct termios nts, ots;

    if (tcgetattr(0, &ots) < 0) // 得到当前终端(0表示标准输入)的设置
        return EOF;

    nts = ots;
    cfmakeraw(&nts); // 设置终端为Raw原始模式，该模式下所有的输入数据以字节为单位被处理
    if (tcsetattr(0, TCSANOW, &nts) < 0) // 设置上更改之后的设置
        return EOF;

    cr = getchar();
    if (tcsetattr(0, TCSANOW, &ots) < 0) // 设置还原成老的模式
        return EOF;

    return cr;
}

#elif _MSC_VER // 下面是依赖Visual Studio编译器实现

#include <conio.h>

// window 上用_getch 替代了getch, 这里为了让其回来
#define sh_getch    _getch

#else
    #error "error : Currently only supports the Visual Studio and GCC!"
#endif
