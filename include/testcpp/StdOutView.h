#ifndef STDOUTVIEW_H__
#define STDOUTVIEW_H__

#include "detail/TextStreamTestView.h"

#include <string>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

namespace Test
{

class StdOutView: public TextStreamTestView
{
public:
    virtual void flush() {}

    virtual TextStreamTestView& operator<< (const std::string& msg)
    { std::cout << msg; return *this; }

    virtual TextStreamTestView& operator<< (int msg)
    { std::cout << msg; return *this; }

    virtual TextStreamTestView& operator<< (EndLine)
    { std::cout << std::endl; return *this; }

    virtual TextStreamTestView& operator<< (Tab)
    { std::cout << "\t"; return *this; }
};

class ColoredStdOutView : public StdOutView
{
#ifdef _WIN32
public:
    ColoredStdOutView() :
        StdOutView(),
        _csbi(),
        _stdout(GetStdHandle(STD_OUTPUT_HANDLE))
    {
        GetConsoleScreenBufferInfo(_stdout, &_csbi);
    }

protected:
    virtual TextStreamTestView& operator<< (ColorOk)
    { SetConsoleTextAttribute(_stdout, 0xA1); return *this; }

    virtual TextStreamTestView& operator<< (ColorFail)
    { SetConsoleTextAttribute(_stdout, 0x4D); return *this; }

    virtual TextStreamTestView& operator<< (ColorNormal)
    { SetConsoleTextAttribute(_stdout, _csbi.wAttributes); return *this; }

private:
    CONSOLE_SCREEN_BUFFER_INFO _csbi;
    HANDLE _stdout;

#else

protected:
    virtual TextStreamTestView& operator<< (ColorOk)
    { std::cout << "\033[92m"; return *this; }

    virtual TextStreamTestView& operator<< (ColorFail)
    { std::cout << "\033[91m"; return *this; }

    virtual TextStreamTestView& operator<< (ColorNormal)
    { std::cout << "\033[0m"; return *this; }

#endif
};

}

#endif /* STDOUTVIEW_H */
