
#define _DEBUG_

#ifdef _DEBUG_
#include <Streaming.h> // http://arduiniana.org/libraries/streaming/
#endif
//#define LCD2040
#define LCD128X64

#ifdef LCD128X64
#include <Wire.h> // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h"
SSD1306Wire display(0x3c, D1, D2);
#endif

#include "BTCSECURE_Minter_128x64.h"

#define DEMO_DURATION 5000
long timeSinceLastModeSwitch = 0;

// #define _COIN_MAX_DISPLAY 10

#if defined(ARDUINO_ESP8266_NODEMCU)
// ARDUINO_ESP8266_NODEMCU
#endif
#if defined(ESP8266)
// ESP8266
#endif

// #include <gmpxx.h>
#include "minterapi.h"
#if defined(ARDUINO)
// #include "Arduino.h"
#else
#define String std::string
#define Serial std::cout
#define endl std::endl
#endif

// #define ASYNC_TCP_SSL_ENABLED 1

#include <ESP8266WiFi.h> //https://github.com/esp8266/Arduino

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager
#define TRIGGER_PIN D3

// #include <ESP8266mDNS.h>
// #include "ESPAsyncTCP.h"
// #include "SyncClient.h"
MinterApi minterApi("minter-msk.interchain.zone", 80);

#include "printminter.h"
PrintMinter printMinter;

    String walletAddress;
   

void setup()
{
  // MinterApi minterApi("https://minter-node-2.testnet.minter.network:8841");

  Serial.begin(115200);
  Serial.println("test_Minter_LCD v0.0.6");

  //  pinMode(TRIGGER_PIN, INPUT);
  pinMode(TRIGGER_PIN, INPUT_PULLUP);
#ifdef LCD128X64
  display.init();

  display.flipScreenVertically();
  display.clear();

  display.clear();
  drawBMImage();
  display.display();

#endif
 walletAddress = "Mx0903ab168597a7c86ad0d4b72424b3632be0af1b";

  String ssid = String(WiFi.SSID());
  // String pass = WiFi.psk();
  if (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
#ifdef _DEBUG_
    Serial << "WiFi Failed!" << endl;
#endif
  }
#ifdef LCD128X64

#endif

#ifdef _DEBUG_
  Serial << "WiFi ssid: " << ssid << ", pass:****** , ip:" << WiFi.localIP() << endl;
#endif
 
}



void displayCoins(Wallet &wallet, const int16_t startY = 0)
{
  uint16_t length = wallet.balance.length();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);

  for (uint16_t i = 0; i < length; i++)
  {
    const int16_t y = startY + (i * 11);
    const String strCoin = wallet.balance.at(i).coin;
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, y, strCoin);
    display.drawString(75, y, String(wallet.balance.at(i).amount, 4));
#ifdef __DEBUG_ // #endif
    Serial
        << "coin: " << strCoin
        << ", amount: " << wallet.balance.at(i).amount
        << ", amountStr: " << wallet.balance.at(i).amountStr
        << endl;
#endif
  }
}

void drawBMImage()
{
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  display.drawXbm(0, 0, BM_Logo_width, BM_Logo_height, BM_Logo_bits);
}

void loop()
{
  if (digitalRead(TRIGGER_PIN) == LOW)
  {
    WiFiManager wifiManager;
    //wifiManager.resetSettings();
    // WiFi.mode(WIFI_STA);
    if (!wifiManager.startConfigPortal("OnDemandAP"))
    {
#ifdef _DEBUG_ // #endif
      Serial.println("failed to connect and hit timeout");
#endif
      delay(3000);
      //reset and try again, or maybe put it to deep sleep
      ESP.reset();
      delay(5000);
    }
#ifdef _DEBUG_ // #endif
    Serial.println("connected...yeey :)");
#endif
  }
  if (millis() - timeSinceLastModeSwitch > DEMO_DURATION)
  {
    timeSinceLastModeSwitch = millis();
    Wallet wallet;

    String MxStr = printMinter.Mx(walletAddress);

    if (minterApi.getAddress(walletAddress, 0, wallet) == MINTERAPI_OK)
    {
#ifdef _DEBUG_ // #endif
      minterApi.printWallet(wallet, "main ");
#endif
      display.clear();
      displayCoins(wallet, 15);
    }
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0, 0, MxStr);
    display.display();
  }
  // counter++;
  // delay(10);
}