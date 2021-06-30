// Default library
#include <Arduino.h>
// Libraries used by the sensor
#include "DHT.h"
// Libraries used by the server
#include <ESPAsyncWebServer.h>
#include "AsyncJson.h"
#include "ArduinoJson.h"
// File containing WIFI credentials
#include "credentials.h"

// Baud rate used by the ESP32 for serial communication
#define BAUD_RATE 115200

/**
 * Sensor setup
 */

// Pin (digital) used by the DHT temperature and humidity sensor communication
#define DHT_PIN 15
#define DHT_TYPE DHT22
// Pin (analog) used by the VMA303 moisture sensor
#define VMA303_PIN 35

DHT dht(DHT_PIN, DHT_TYPE);

/**
 * WiFi setup
 */

void setupWiFi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.waitForConnectResult() != WL_CONNECTED);
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
}

/**
 * Server setup
 */

AsyncWebServer server(80);

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "application/json", "{\"message\":\"Not found\"}");
}

void requestRoot(AsyncWebServerRequest *request) {
    request->send(200, "application/json", "{\"message\":\"Welcome\"}");
}

boolean isInteger(String str) {
    for (int i = 0; i < str.length(); i++)
        if (!isDigit(str.charAt(i)))
            return false;
    return true;
}

void requestRead(AsyncWebServerRequest *request) {
    DynamicJsonDocument data(1024);

    int numParams = request->params();

    if (numParams > 0) {
        int n = 1;  // Number of measurements
        int t = 10; // Time between measurements in milliseconds

        for (int i = 0; i < numParams; i++) {
            AsyncWebParameter *p = request->getParam(i);
            String name = p->name();
            String value = p->value();

            if (!isInteger(value))
                continue;

            if (name == "n")
                n = value.toInt();

            else if (name == "t")
                t = value.toInt();
        }

        for (int i = 0; i < n; i++) {
            data["temperature"][i] = dht.readTemperature();
            data["humidity"][i] = dht.readHumidity();
            data["moisture"][i] = analogRead(VMA303_PIN);
            delay(t);
        }
    }
    else {
        data["temperature"] = dht.readTemperature();
        data["humidity"] = dht.readHumidity();
        data["moisture"] = analogRead(VMA303_PIN);
    }

    String response;
    serializeJson(data, response);
    request->send(200, "application/json", response);
}

void setupServer() {
    server.on("/", HTTP_GET, requestRoot);
    server.on("/read", HTTP_GET, requestRead);
    server.onNotFound(notFound);
    server.begin();
}

/**
 * Main program
 */

void setup() {
    Serial.begin(BAUD_RATE);
    dht.begin();
    setupWiFi();
    setupServer();
}

void loop() {

}
