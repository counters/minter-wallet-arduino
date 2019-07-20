#include "http.h"

Http::Http()
{
  setTimeout(HTTP_TIMEOUT_MS);
}
Http::Http(const uint16_t timeout_ms)
{
  setTimeout(timeout_ms);
}
uint8_t Http::get(const String host, const uint16_t port, const String getUrl, String& result)
{
uint8_t status = HTTP_RESULT_ERROR;
#ifdef HTTP_DEBUG
  Serial << F("Http::get(") << host << "," << port << "," << getUrl << ")" << endl;
#endif
#ifdef ARDUINO
  /**/
  SyncClient client;
  if (!client.connect(host.c_str(), port))
  {
#ifdef HTTP_DEBUG
      Serial << F("Connect Failed") << endl;
#endif
  }
  const String httpHeaderGet = "GET " + getUrl + " HTTP/1.1\r\nHost: " + host + "\r\nConnection: close\r\n\r\n";
  const char *httpHeaderGetChar = httpHeaderGet.c_str();
  client.setTimeout(timeout_ms);
  if (client.printf(httpHeaderGetChar) > 0)
  {
    while (client.connected() && client.available() == 0)
    {
      delay(1);
    }
    // Skip HTTP headers
    const char endOfHeaders[] = "\r\n\r\n";
    if (!client.find(endOfHeaders))
    {
#ifdef HTTP_DEBUG
      Serial << F("Http::get Invalid response") << endl;
#endif
      // return;
    }
    else
    {
      String _out = "";
      while (client.available())
      {
        const char thisChar = client.read();
        _out += thisChar;
      }
      if (client.connected())
      {
        client.stop();
      }
      status = HTTP_RESULT_STATUS;
      result =  _out;
    }
  }
  else
  {
    client.stop();
#ifdef HTTP_DEBUG
      Serial << F("Send Failed") << endl;
#endif
    while (client.connected())
      delay(0);
  }

#else
  curl = curl_easy_init();
  if (curl)
  {
    String url =

    String readBuffer;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    //
    return readBuffer;
  }
#endif
  return status;
}
