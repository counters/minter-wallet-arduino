#include "mintermatch.h"


String MinterMatch::NormalePip(String bigInt, uint8_t &length)
{
    // const uint8_t length = bigInt.length();
    if ( PIP_BIP_LENGHT >= length)
    {
        const uint8_t _length = PIP_BIP_LENGHT - bigInt.length();
        for (uint8_t i = 0; i <= _length; i++)
        {
            bigInt = "0" + bigInt;
        }
        length = bigInt.length();
    }
    return bigInt;
}

float MinterMatch::getAmount(String bigInt)
{
    uint8_t stringLength = bigInt.length();
    bigInt = NormalePip(bigInt, stringLength);

    const uint8_t moduloLength = stringLength - PIP_BIP_LENGHT;
#if defined(ARDUINO)
   const String modulo = bigInt.substring(moduloLength);
    const String integer = bigInt.substring(0, moduloLength);
#else
    const String modulo = bigInt.substr(moduloLength);
    const String integer = bigInt.substr(0, moduloLength);
#endif
    // const String  numFloat = String(integer.operator+=(".").operator+=(modulo.substring(0, PIP_PRECISION)));
    const String  numFloat = integer+"."+modulo;
    // numFloat.concat(integer, ",", modulo);
    //         std::cout<<"numFloat "<< numFloat <<std::endl;
    const float numF = atof(numFloat.c_str());
    //         sscanf(numFloat.c_str(), "%f", &numF);
    //          const float numF = numFloat.toFloat();
    return numF;
}
