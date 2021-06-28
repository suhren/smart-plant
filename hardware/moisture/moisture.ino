#include <Arduino.h>

// Libraries used by the sensor
#include "DHT.h"

// Libraries used by the server
#include <ESPAsyncWebServer.h>
#include "AsyncJson.h"
#include "ArduinoJson.h"

// File containing WIFI credPentials
#include "credentials.h"

/**
 * Sensor setup
 */

#define DHT_PIN 15
#define VMA303_PIN 35
#define DHT_TYPE DHT22

DHT dht(DHT_PIN, DHT_TYPE);

/**
 * WiFi setup
 */

void setupWiFi()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.waitForConnectResult() != WL_CONNECTED)
    {
        Serial.printf("WiFi Failed!\n");
    }
    Serial.printf("IP Address: %s\n", WiFi.localIP());
}

/**
 * Server setup
 */

AsyncWebServer server(80);

void notFound(AsyncWebServerRequest *request)
{
    request->send(404, "application/json", "{\"message\":\"Not found\"}");
}

void requestRoot(AsyncWebServerRequest *request)
{
    request->send(200, "application/json", "{\"message\":\"Welcome\"}");
}

void requestRead(AsyncWebServerRequest *request)
{
    StaticJsonDocument<100> data;

    data["temperature"] = dht.readTemperature();
    data["humidity"] = dht.readHumidity();
    data["moisture"] = analogRead(VMA303_PIN);

    String response;
    serializeJson(data, response);
    request->send(200, "application/json", response);
}

void setupServer()
{
    server.on("/", HTTP_GET, requestRoot);
    server.on("/read", HTTP_GET, requestRead);
    server.onNotFound(notFound);
    server.begin();
}

/**
 * Main program
 */

void setup()
{
    Serial.begin(115200);
    dht.begin();
    setupWiFi();
    setupServer();
}

void loop()
{

}
