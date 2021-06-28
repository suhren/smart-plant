/*
 * blink.ino
 * 
 * This simple test sketch will blink the built-in LED on the ESP32
 * and write a message to the serial output.
 */

// The GPIO2 pin is connected to a built-in LED 
int LED_PIN = 2;


/*
 * The setup function is run only once in the beginning.
 */
void setup()
{
  // Set the LED pin as an output
  pinMode(LED_PIN, OUTPUT);
  
  // Set up the baud rate for the serial communication
  Serial.begin(115200);
}

/*
 * The loop function is run repeatedly when the processor is running.
 */
void loop()
{
  // First turn the LED on
  Serial.println("Blink on!");
  digitalWrite(LED_PIN, HIGH);
  delay(500);
  // Then turn the LED off
  Serial.println("Blink off!");
  digitalWrite(LED_PIN, LOW);
  delay(500);
}
