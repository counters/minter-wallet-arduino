#ifndef MINTERMATCH_H
#define MINTERMATCH_H


#if defined(ARDUINO)
#include "Arduino.h"
#else
#include <iostream>
//#include "math.h"
#include <stdlib.h>
//#include <stdio.h>
#define String std::string
#endif

//#include <gmpxx.h>

#define PIP_STR 0.000000000000000001
#define PIP_STR_DIV 1000000000000000000

#define PIP_BIP_LENGHT 18

class MinterMatch
{
     String NormalePip(String bigInt, uint8_t & length);

public:
     MinterMatch(){}
    float getAmount(String bigInt);
};

#endif // MINTERMATCH_H
