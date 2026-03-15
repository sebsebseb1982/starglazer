#include <UrlEncode.h>
#include "equatorial-coordinates-service.h"
#include "debug.h"
#include "secrets.h"
#define HTTP_RETRY 3

EquatorialCoordinates::EquatorialCoordinates() {
}

EquatorialCoordinates::EquatorialCoordinates(
  float altitude,
  float azimuth,
  float distance) {
  this->altitude = altitude;
  this->azimuth = azimuth;
  this->distance = distance;
}

EquatorialCoordinatesService::EquatorialCoordinatesService()
  : doc() {
  this->http.setConnectTimeout(2000);
  this->http.setTimeout(15000);
}

EquatorialCoordinates EquatorialCoordinatesService::compute(GPSData gpsData, ObjectToWatch *trackedObject) {
  // TODO à factoriser avec src/api/starglaze-api.cpp
  String skyfieldAPIURL;
  skyfieldAPIURL += STARGLAZE_API_HOST;
  skyfieldAPIURL += F("/horizontal-coordinates/");
  skyfieldAPIURL += trackedObject->type;
  skyfieldAPIURL += F("/");
  skyfieldAPIURL += urlEncode(trackedObject->code);
  skyfieldAPIURL += F("?latitude=");
  skyfieldAPIURL += gpsData.latitudeInDegrees;
  skyfieldAPIURL += F("&longitude=");
  skyfieldAPIURL += gpsData.longitudeInDegrees;
  skyfieldAPIURL += F("&elevation=");
  skyfieldAPIURL += gpsData.elevation;
  this->http.end();
  this->http.begin(skyfieldAPIURL);
  int httpCode;
  int retry = 0;

  do {
    //yield();
    httpCode = this->http.GET();
    retry++;
  } while (httpCode <= 0 && httpCode != HTTPC_ERROR_READ_TIMEOUT && retry < HTTP_RETRY);

  if (httpCode > 0) {
    String response = this->http.getString();
    this->http.end();
    DeserializationError error = deserializeJson(this->doc, response);
    if (error) {
        DEBUG_PRINT(F("deserializeJson() failed: "));
        DEBUG_PRINTLN(error.c_str());
      return EquatorialCoordinates();
    }
    return EquatorialCoordinates(
      this->doc["altitude"],
      this->doc["azimuth"],
      this->doc["distance"]);

  } else {
    String error;
    error += F("KO -> code erreur = ");
    error += String(httpCode);
      DEBUG_PRINTLN(error);
    this->http.end();
    return EquatorialCoordinates();
  }
}
