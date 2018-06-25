#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <MqttConnector.h>

#include "init_mqtt.h"
#include "_publish.h"
#include "_receive.h"
#include "_config.h"

MqttConnector *mqtt; 
char myName[40];

void init_hardware()
{
  pinMode(relayPin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(relayPin, relayPinState);;
  // serial port initialization
  Serial.begin(57600);
  delay(10);
  Serial.println();
  Serial.println("Starting...");
}

void init_wifi() {
  WiFi.disconnect();
  delay(20);
  WiFi.mode(WIFI_STA);
  delay(50);
  const char* ssid =  WIFI_SSID.c_str();
  const char* pass =  WIFI_PASSWORD.c_str();
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.printf ("Connecting to %s:%s\r\n", ssid, pass);
    delay(300);
  }
  Serial.println("WiFi Connected.");
  digitalWrite(LED_BUILTIN, HIGH);
}

// void printBuffer(void) {
//   // Initialize the log buffer
//   // allocate memory to store 8 lines of text and 30 chars per line.
//   display.setLogBuffer(5, 30);

//   // Some test data
//   const char* test[] = {
//       "Setting Up",
//   };

//   display.println(test[0]);
//   display.drawLogBuffer(35 /*X*/, /*Y*/25);
//   display.display();
// }

// void resetpage () {
//   display.clear();
//   display.display();
//   display.drawString(30, 25, "Press button");
//   display.display();
// }

void setup()
{
  init_hardware();
  init_wifi();
  init_mqtt();

  //IN OUT PUT//
  pinMode(13,INPUT);
  pinMode(12,INPUT);
  pinMode(14,INPUT);
  pinMode(0,OUTPUT);
  pinMode(2,OUTPUT);
  pinMode(15,OUTPUT);
// Display //
  // display.init();
  // display.flipScreenVertically();
  // display.setContrast(255);
//Start Up//
  digitalWrite(0,1);
  digitalWrite(2,1);
  digitalWrite(15,0);

}

void loop()
{
  mqtt->loop();

  int a = digitalRead(12);
  int b = digitalRead(14);
  int c = digitalRead(13);
  int i = 0;
  Serial.print(a);
  Serial.println(b);
  if ((a == 1)&&(b == 0)) {
    digitalWrite(0,0);
    digitalWrite(2,0);
    digitalWrite(15,1);
    
    mqtt->sync_advpub("CMMC","/$/command", "ON", false);
    // delay(500);
    // resetpage();
  }
  if ((a == 0)&&(b == 1)) {
    digitalWrite(0,1);
    digitalWrite(2,1);
    digitalWrite(15,0);

    mqtt->sync_advpub("CMMC","/$/command", "OFF", false);
    // delay(500);
    // resetpage();
  }
  if (c == 1) {
    digitalWrite(0,1);
    digitalWrite(2,1);
    digitalWrite(15,0);

    // delay(500);
    // resetpage();
  }

}
