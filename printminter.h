#ifndef PRINTMINTER_H
#define PRINTMINTER_H

#if defined(ARDUINO)
#include "Arduino.h"
#else
#include <string>
#include <stdlib.h>
#include <stdio.h>
#define String std::string
#define Serial std::cout
//#define endl std::endl
#endif

#define _PRINTMINTER_SPACE_ "..."

#define _PRINTMINTER_FIRST_ 6
#define _PRINTMINTER_LAST_ 4
#define _PRINTMINTER_MX_LENGHT_ 42

#define PRINTMINTER_MX_FROM_START (_PRINTMINTER_MX_LENGHT_ - _PRINTMINTER_LAST_)

class PrintMinter
{
public:
    PrintMinter();
    String Mx(String mx)
    {

#if defined(ARDUINO)
        const String str = mx.substring(0, _PRINTMINTER_FIRST_) + _PRINTMINTER_SPACE_ + mx.substring(PRINTMINTER_MX_FROM_START, _PRINTMINTER_MX_LENGHT_);
#else
        const String str = mx.substr(0, _PRINTMINTER_FIRST_) + _PRINTMINTER_SPACE_ + mx.substr(PRINTMINTER_MX_FROM_START, _PRINTMINTER_MX_LENGHT_);
#endif
        return str;
    }
};

#endif // PRINTMINTER_H
