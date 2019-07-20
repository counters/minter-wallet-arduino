#ifndef HTTP_H
#define HTTP_H

#define HTTP_TIMEOUT_MS 50
#define HTTP_RESULT_STATUS 200 //200
#define HTTP_RESULT_ERROR 0

#define HTTP_DEBUG
// #define ARDUINO

#if defined(ARDUINO)
#include "Arduino.h"
#ifdef HTTP_DEBUG
#include <Streaming.h>
#endif
#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager
#define TRIGGER_PIN D3

#include <ESP8266mDNS.h>
// #include "ESPAsyncTCP.h"
#include "SyncClient.h"
#else
#include <iostream>
#include <string>
#include <curl/curl.h>
#define String std::string
#endif
#include <ArduinoJson.h>

#include "minter.h"

class Http
{
private:
#ifdef ARDUINO

#else
    CURL *curl;
    CURLcode res;
    std::string readBuffer;
    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
    {
        ((std::string *)userp)->append((char *)contents, size * nmemb);
        return size * nmemb;
    }
#endif
    // StaticJsonBuffer<20000> jsonBuffer;
    //   char* json;
uint16_t timeout_ms = HTTP_TIMEOUT_MS;
public:
    Http();
    Http(const uint16_t timeout_ms);
     //_timeout_ms
    void setTimeout(const uint16_t timeout_ms) 
    {
        this->timeout_ms = timeout_ms;
    }
    uint8_t get(const String host, const uint16_t port, const String getUrl, String& result);
    String testGet()
    {

#ifdef ARDUINO

#else
        curl = curl_easy_init();
        if (curl)
        {
            curl_easy_setopt(curl, CURLOPT_URL, "https://minter-node-2.testnet.minter.network:8841/status");
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
            //       std::cout << readBuffer << std::endl;
            return readBuffer;
        }
#endif
        return "";
    }
};

#endif // HTTP_H
