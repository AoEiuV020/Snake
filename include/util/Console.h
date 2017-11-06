#ifndef SNAKE_CONSOLE_H_
#define SNAKE_CONSOLE_H_

#include <string>

#if defined(WIN32) || defined(_WIN32)

#include <Windows.h>

#endif

#if defined(__linux__) || defined(__APPLE__)
#define LINUX_OR_APPLE
#endif

/**
 * 枚举控制台颜色，
 */
enum ConsoleColorType {
    BLACK,
    RED,
    GREEN,
    BLUE,
    YELLOW,
    CYAN,
    MAGENTA,
    WHITE,
};

/**
 * 封装控制台颜色，
 */
struct ConsoleColor {
    ConsoleColor(const ConsoleColorType foreColor_,
                 const ConsoleColorType backColor_,
                 const bool foreIntensified_ = false,
                 const bool backIntensified_ = false);

    /**
     * 前景色，也就是文字的颜色，
     */
    ConsoleColorType foreColor;
    /**
     * 背景色，
     */
    ConsoleColorType backColor;
    /**
     * 是否前景高亮，
     */
    bool foreIntensified;
    /**
     * 是否背景高亮，
     */
    bool backIntensified;
};

/**
 * 封装控制台相关方法,
 * 比如打印字符, 读取键盘输入,
 */
class Console {
public:
    /**
     * 光标跳转到指写行列，
     * @param x 列号，
     * @param y 行号，
     */
    static void setCursor(const int x = 0, const int y = 0);

    /**
     * 控制台清屏，
     */
    static void clear();

    /**
     * 控制台打印字符串，
     * @param str 字符串，
     */
    static void write(const std::string &str);

    /**
     * 控制台打印带颜色的字符串，
     * @param str 字符串，
     * @param consoleColor 颜色，
     */
    static void writeWithColor(const std::string &str, const ConsoleColor &consoleColor);

    /**
     * 获取控制台输入，
     * @return
     */
    static int getch();

    /**
     * 判断控制台是否有输入，
     * @return 有则返回非0 也就是真，没有则返回0 也就是假，
     */
    static int kbhit();

private:
#ifdef WIN32
    /**
     * windows设置控制台颜色,
     */
    static WORD setColor(const ConsoleColor &consoleColor);

    /**
     * windows重置控制台颜色,
     */
    static void resetColor(const WORD attr);
#endif
};

#endif
