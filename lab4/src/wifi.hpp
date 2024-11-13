#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#define ssid "Wokwi-GUEST"
#define password ""

// TODO: Configurable endpoint
String apiEndpoint = "http://242e-35-221-190-215.ngrok-free.app/update";


// Function to make an HTTP request to the API endpoint with sensor value
void uploadSensorValues(const char* sensorName, int sensorValue) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    char fullUrl[128];
    
    // Construct the URL using sprintf
    sprintf(fullUrl, "%s/%s/%d", apiEndpoint.c_str(), sensorName, sensorValue);
    printf("Sending: %s\n", fullUrl);
    
    http.begin(fullUrl);
    http.setTimeout(300);
    printf("2 get\n");
    int httpResponseCode = http.GET();
    printf("3 get\n");
    if (httpResponseCode > 0) {
      printf("Sensor: %s, Value: %d, HTTP Status: %d\n", sensorName, sensorValue, httpResponseCode);
    } else {
      printf("Sensor: %s, Error code: %d\n", sensorName, httpResponseCode);
    }
    http.end();
  } else {
    printf("WiFi not connected\n");
  }
}


// WiFi Initialization
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  printf("Connecting to WiFi ..\n");
  while (WiFi.status() != WL_CONNECTED) {
    printf(".");
    delay(1000);
  }
  printf("\nConnected to WiFi\n");
  printf("IP Address: %s\n", WiFi.localIP().toString().c_str());
}

void setup() {
  initWiFi();

  // TODO
}

void loop() {
  // Empty, as FreeRTOS is managing tasks
}