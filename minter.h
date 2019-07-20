#ifndef MINTER_H
#define MINTER_H

#if defined(ARDUINO)
#include "Arduino.h"
#else
#include <iostream>
#define String std::string
#endif

#include <QList.h> //https://github.com/SloCompTech/QList

struct Status{
uint64_t height;
String datetime;
String network;
};
struct BalancePair{
String coin;
float amount;
String amountStr;
};
struct Wallet{
uint64_t id;
String address;
uint32_t count_txs;
QList<BalancePair> balance;
};

class Minter
{
public:
    Minter();
};

#endif // MINTER_H
