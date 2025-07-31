#include "starglaze-api.h"
#include "secrets.h"

StarGlazeAPI::StarGlazeAPI()
{
    this->http.setConnectTimeout(2000);
    this->http.setTimeout(5000);
}

boolean StarGlazeAPI::health()
{
    String url;
    url += STARGLAZE_API_HOST;
    url += F("/catalog");
    this->http.end();
    this->http.begin(url);
    int httpCode;
    int retry = 0;

    do
    {
        httpCode = this->http.GET();
        retry++;
    } while (httpCode <= 0 && retry < HTTP_RETRY);

    this->http.end();

    return httpCode == HTTP_CODE_OK;
}

Catalog StarGlazeAPI::getCatalog()
{
    String url;
    url += STARGLAZE_API_HOST;
    url += F("/catalog");
    this->http.end();
    this->http.begin(url);
    int httpCode;
    int retry = 0;

    do
    {
        httpCode = this->http.GET();
        retry++;
    } while (httpCode <= 0 && retry < HTTP_RETRY);

    if (httpCode > 0)
    {
        String response = this->http.getString();
        this->http.end();
        DeserializationError error = deserializeJson(this->doc, response);
        if (error)
        {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.c_str());
            return Catalog();
        }
        // TODO

        Serial.println(this->doc.size());
        return Catalog();
    }
    else
    {
        String error;
        error += F("KO -> code erreur = ");
        error += String(httpCode);
        Serial.println(error);
        this->http.end();
        return Catalog();
    }
}