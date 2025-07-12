#include "equatorial-coordinates-service.h"
#define HTTP_RETRY 10

TrackedObject::TrackedObject(String type, String name)
  : type(type), name(name) {
}

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
  : doc(2048) {
  this->http.setConnectTimeout(2000);
  this->http.setTimeout(5000);
}

EquatorialCoordinates EquatorialCoordinatesService::compute(GPSData gpsData, TrackedObject trackedObject) {
  Serial.println("A");

  String homeAssistantURL;
  homeAssistantURL += F("http://192.168.1.169:22666/horizontal-coordinates/");
  homeAssistantURL += trackedObject.type;
  homeAssistantURL += F("/");
  homeAssistantURL += trackedObject.name;
  homeAssistantURL += F("?latitude=");
  homeAssistantURL += gpsData.getLatitudeInDegrees();
  homeAssistantURL += F("&longitude=");
  homeAssistantURL += gpsData.getLongitudeInDegrees();
  homeAssistantURL += F("&elevation=");
  homeAssistantURL += gpsData.getElevation();
  Serial.println("A1");
  this->http.end();
  this->http.begin(homeAssistantURL);
  Serial.println("A2");
  int httpCode;
  int retry = 0;

  do {
    Serial.println("A3");
    yield();
    httpCode = this->http.GET();
    retry++;
    Serial.println("B");
  } while (httpCode <= 0 && retry < HTTP_RETRY);

  if (httpCode > 0) {
    Serial.println("C");
    String response = this->http.getString();
    Serial.println("C1");
    this->http.end();
    Serial.println("C2");
    DeserializationError error = deserializeJson(this->doc, response);
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      Serial.println("D");
      return EquatorialCoordinates();
    }
    Serial.println("C3");
    return EquatorialCoordinates(
      this->doc["altitude"],
      this->doc["azimuth"],
      this->doc["distance"]);

  } else {
    String error;
    error += F("KO -> code erreur = ");
    error += String(httpCode);
    Serial.println(error);
    Serial.println("");
    Serial.println("E");
    this->http.end();
    return EquatorialCoordinates();
  }
}
