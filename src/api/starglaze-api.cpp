#include "starglaze-api.h"
#include "secrets.h"

StarGlazeAPI::StarGlazeAPI()
{
    this->http.setConnectTimeout(2000);
    this->http.setTimeout(5000);
}

boolean StarGlazeAPI::health()
{
    String skyfieldAPIURL;
    skyfieldAPIURL += F(STARGLAZE_API_HOST);
    skyfieldAPIURL += F("/catalog");
    this->http.end();
    this->http.begin(skyfieldAPIURL);
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

void StarGlazeAPI::catalog()
{
}