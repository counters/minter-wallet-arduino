#include "minterapi.h"

MinterApi::MinterApi()
{
    http.setTimeout(HTTP_TIMEOUT_MS);
}
/*
MinterApi::MinterApi(String nodeUrl)
{
    this->nodeUrl = nodeUrl;
}
 */
MinterApi::MinterApi(const String host, const uint16_t port)
{
    // this->schema = schema;
    this->host = host;
    this->port = port;
    http.setTimeout(HTTP_TIMEOUT_MS);
}

Status MinterApi::getStatus()
{
    String json;
    const uint8_t statusHttp = get(MAPI_STATUS, "", json);
    Status status;
    if (statusHttp == HTTP_RESULT_STATUS)
    {
#ifdef MINTERAPI_DEBUG
        Serial << "json:" << endl
               << json << endl;
#endif
        // Status status;
        const Status status = parseStatus(json);
    }
    return status;
}

uint8_t MinterApi::getAddress(const String address, const uint64_t height, Wallet &ResWallet)
{
    uint8_t status=MINTERAPI_ERROR;
#if defined(ARDUINO)
    const String _height = String((uint32_t)height);
#else
    char _height[16] = {0};
    //        ultoa(height, _height, 10);
    snprintf(_height, 16, "%llu", height);
#endif
    // String params; //?address=Mx&height=0
    // params.operator+=("address=").operator+=(address).operator+=("&height=").operator+=(_height);
    const String params = "address=" + address + "&height=" + _height;
    String json;
    if (get(MAPI_ADDRESS, params, json) == HTTP_RESULT_STATUS)
    {
#ifdef MINTERAPI_DEBUG
        Serial << "json:" << endl << json << endl;
#endif
        if ( parseWallet(json, ResWallet)==MINTERAPI_JSON_STATUS_OK ){
           status=MINTERAPI_OK; 
           ResWallet.address = address;
        }
    }
    return status;
}

Status MinterApi::parseStatus(const String json)
{
    Status status;
#if ArduinoJson == 5
    JsonObject &root = jsonBuffer.parseObject(json);
    if (root.success())
    {
#elif ArduinoJson == 6
    DeserializationError error = deserializeJson(jsonBuffer, json, DeserializationOption::NestingLimit(100));
    if (!error)
    {
#endif
        JsonObject &result = root["result"];

#if defined(ARDUINO)
        String height = result["latest_block_height"].as<String>();
        //  status.height = atoll(height);
        const char *strheight = height.c_str();
        status.height = strtoull(strheight, NULL, 10);
        status.datetime = result["latest_block_time"].as<String>();
        status.network = result["tm_status"]["node_info"]["network"].as<String>();
#else
        status.height = result["latest_block_height"].as<uint64_t>();
        status.datetime.append(result["latest_block_time"].as<String>());
        status.network.append(result["tm_status"]["node_info"]["network"].as<String>());
#endif

        return status;
    }
    else
    {
#ifdef MINTERAPI_DEBUG
        Serial << "parseObject() failed" << endl;
#endif
        return status;
    }
}

uint8_t MinterApi::parseWallet(const String json, Wallet &wallet)
{
    uint8_t status = MINTERAPI_JSON_STATUS_ERROR;
    jsonBuffer.clear();
   
    JsonObject &root = jsonBuffer.parseObject(json);
    if (root.success())
    {
        JsonObject &result = root["result"];
        wallet.count_txs = result["transaction_count"].as<uint32_t>();

        JsonObject &balance = result["balance"];
        for (JsonPair p : balance)
        {
            const char *key = p.key;
            JsonVariant value = p.value;
            String amount = value.as<String>();
#ifdef MINTERAPI_DEBUG
            // Serial << key << "\t" << amount << "\t" << minterMatch.getAmount(amount) << endl;
#endif
            BalancePair balancePair = {key, (float)minterMatch.getAmount(amount), amount};
            wallet.balance.push_front(balancePair);
        }

#ifdef MINTERAPI_DEBUG // #endif
        // printWallet(wallet);
#endif
        status = MINTERAPI_JSON_STATUS_OK;
    }
    else
    {
#ifdef MINTERAPI_DEBUG
        Serial << "parseObject() failed" << endl;
#endif
    }
    return status;
}

uint8_t MinterApi::get(const String method, const String params, String &content)
{
    const String getUrl = String("/") + method + String("?") + params;
#ifdef _MINTERAPI_DEBUG
    // url = url.operator+=("/").operator+=(method).operator+=("?").operator+=(params);
    Serial << "MinterApi::get(" << method << "," << params << ") " << getUrl << endl;
#endif
    uint8_t status = http.get(this->host, this->port, getUrl, content);
    return status;
}
