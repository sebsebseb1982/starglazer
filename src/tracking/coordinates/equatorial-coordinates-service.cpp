#include "equatorial-coordinates-service.h"
#define HTTP_RETRY 10

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
  this->http.setTimeout(5000);
}

EquatorialCoordinates EquatorialCoordinatesService::compute(GPSData gpsData, TrackedObject *trackedObject) {
  String skyfieldAPIURL;
  skyfieldAPIURL += F("http://192.168.1.169:22666/horizontal-coordinates/");
  skyfieldAPIURL += trackedObject->type;
  skyfieldAPIURL += F("/");
  skyfieldAPIURL += trackedObject->name;
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
  } while (httpCode <= 0 && retry < HTTP_RETRY);

  if (httpCode > 0) {
    String response = this->http.getString();
    this->http.end();
    DeserializationError error = deserializeJson(this->doc, response);
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
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
    Serial.println(error);
    this->http.end();
    return EquatorialCoordinates();
  }
}
