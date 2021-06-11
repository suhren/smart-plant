#include "DHT.h"

// Define the DHT sensor pin
#define DHTPIN 15

//The sensor is of the DHT22 type
#define DHTTYPE DHT22

//Create an instance of DHT sensor
DHT dht(DHTPIN, DHTTYPE);


void setup()
{
  Serial.begin(115200);
  Serial.println("DHT22 sensor!");
  
  //call begin to start sensor
  dht.begin();
}


void loop()
{  
  // Use the functions which are supplied by library.
  float h = dht.readHumidity();
  
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  // Print the result to Terminal
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C ");
  
  // We delay a little bit for next read
  delay(2000);
}
