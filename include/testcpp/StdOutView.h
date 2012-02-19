#ifndef STDOUTVIEW_H__
#define STDOUTVIEW_H__

#include "detail/TextStreamTestView.h"

#include <string>
#include <iostream>

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

}

#endif /* STDOUTVIEW_H */
