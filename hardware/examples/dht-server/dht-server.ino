#include <Arduino.h>

// Libraries used by the sensor
#include "DHT.h"

// Libraries used by the server
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "AsyncJson.h"
#include "ArduinoJson.h"

// File containing WIFI credentials
#include "credentials.h"

/*
 * Sensor setup
 */

// Define the DHT sensor pin
#define DHTPIN 15

//The sensor is of the DHT22 type
#define DHTTYPE DHT22

//Create an instance of DHT sensor
DHT dht(DHTPIN, DHTTYPE);


/*
 * Server setup
 */

AsyncWebServer server(80);

const char *ssid = WIFI_SSID;
const char *password = WIFI_PASSWORD;

void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "application/json", "{\"message\":\"Not found\"}");
}

void setup() {
  Serial.begin(115200);

  // Call begin to start sensor
  dht.begin();

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.printf("WiFi Failed!\n");
  }

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "application/json", "{\"message\":\"Welcome\"}");
  });

  server.on("/get-message", HTTP_GET, [](AsyncWebServerRequest *request) {
    StaticJsonDocument<100> data;
    if (request->hasParam("message"))
    {
      data["message"] = request->getParam("message")->value();
    }
    else {
      data["message"] = "No message parameter";
    }
    String response;
    serializeJson(data, response);
    request->send(200, "application/json", response);
  });

  server.on("/read", HTTP_GET, [](AsyncWebServerRequest *request) {
    StaticJsonDocument<100> data;

    float t = dht.readTemperature();
    float h = dht.readHumidity();

    data["temperature"] = t;
    data["humidity"] = h;

    String response;
    serializeJson(data, response);
    request->send(200, "application/json", response);
  });

  AsyncCallbackJsonWebHandler *handler =
    new AsyncCallbackJsonWebHandler("/post-message", [](AsyncWebServerRequest *request, JsonVariant &json) {
      StaticJsonDocument<200> data;
      if (json.is<JsonArray>())
      {
        data = json.as<JsonArray>();
      }
      else if (json.is<JsonObject>())
      {
        data = json.as<JsonObject>();
      }
      String response;
      serializeJson(data, response);
      request->send(200, "application/json", response);
      Serial.println(response);
  });

  server.addHandler(handler);
  server.onNotFound(notFound);
  server.begin();
}


void loop() {
  // put your main code here, to run repeatedly:

}
