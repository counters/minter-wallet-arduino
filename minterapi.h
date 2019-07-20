#ifndef MINTERAPI_H
#define MINTERAPI_H

#define __MINTERAPI_DEBUG
#define MINTERAPI_JSON_STATUS_OK 1
#define MINTERAPI_JSON_STATUS_ERROR 0

#define MINTERAPI_OK 1
#define MINTERAPI_ERROR 0

#ifdef MINTERAPI_DEBUG
#include <Streaming.h>
#endif

#if defined(ARDUINO)
#include "Arduino.h"
#else
#include <string>
#include <stdlib.h>
#include <stdio.h>
#define String std::string
#define Serial std::cout
#define endl std::endl
#endif
#include "minter.h"
#include "http.h"

#include "mintermatch.h"

#define ArduinoJson 5

#include <ArduinoJson.h>

#define MAPI_BLOCK "block"
#define MAPI_NODE "candidate"
#define MAPI_ADDRESS "address"
#define MAPI_COIN "coin_info"
#define MAPI_STATUS "status"
#define MAPI_ESTIMATE_COIN_BUY "estimate_coin_buy"
#define MAPI_ESTIMATE_COIN_SELL "estimate_coin_sell"

#define MAPI_DEFAULT_SCHEME "http"

class MinterApi
{
private:
  MinterMatch minterMatch;
  Http http;
  //  String nodeUrl;
  String host;
  uint16_t port;

#if ArduinoJson == 5
  DynamicJsonBuffer jsonBuffer;
  //  StaticJsonBuffer<20000> jsonBuffer;
#elif ArduinoJson == 6
  DynamicJsonDocument jsonBuffer;
//   StaticJsonDocument<200> doc2;
#endif

  Status parseStatus(const String json);
  uint8_t parseWallet(const String json, Wallet &ResWallet);
  uint8_t get(const String method, const String params, String &content);

public:
  MinterApi();
  // MinterApi(String nodeUrl);
  MinterApi(const String host, const uint16_t port = 80 /* , String schema=MAPI_DEFAULT_SCHEME */);
  Status getStatus();
  uint8_t getAddress(const String address, const uint64_t height, Wallet &ResWallet);

#ifdef MINTERAPI_DEBUG
  void printWallet(Wallet &wallet, const char * prefix= "Print ")
  {
    Serial << prefix << "Wallet: " << wallet.address << ", count_txs: " << wallet.count_txs
           << ", length: " << wallet.balance.length() << endl;
    printBalance(wallet.balance, prefix);
  }
  void printBalance(QList<BalancePair> &balance, const char * prefix)
  {
    Serial << prefix ;
    for (uint16_t i = 0; i < balance.length(); i++)
    {
      Serial << "coin: " << balance.at(i).coin
             << ", amount: " << balance.at(i).amount
             << ", amountStr: " << balance.at(i).amountStr
             << endl;
    }
  }
#endif
};

#endif // MINTERAPI_H
