#include <stdint.h>
#include <SPI.h>
//#include <Bridge.h>
//#include <WiFi.h>
#include "PubSubClient.h"
#include "TouchScreen.h"
#include "TFT.h"

#ifdef SEEEDUINO
  #define YP A2   // must be an analog pin, use "An" notation!
  #define XM A1   // must be an analog pin, use "An" notation!
  #define YM 14   // can be a digital pin, this is A0
  #define XP 17   // can be a digital pin, this is A3 
#endif

#ifdef MEGA
  #define YP A2   // must be an analog pin, use "An" notation!
  #define XM A1   // must be an analog pin, use "An" notation!
  #define YM 54   // can be a digital pin, this is A0
  #define XP 57   // can be a digital pin, this is A3 
#endif 

char ssid[] = "Loft21";
char pass[] = "silkylotus997";

//int status = WL_IDLE_STATUS;

//WiFiClient client;

#define BUFFER_MAX 17
#define SCREEN_LENGTH 16
#define MAX_LINE_COUNT 11
#define NEWLINE 0xa

#define X_START 220
#define Y_START 10

int incomingByte = 0;
char buffer[BUFFER_MAX+1];
int index = 0;
int charCount = 0;
int x = X_START;
int y = Y_START;
int lineCount = 0;

void setup() {
  while(!Serial) {
  }

  //Bridge.begin();
  Tft.init();
  Tft.setDisplayDirect(UP2DOWN);
  Tft.drawString("> Science, bitch!", x, y, 2, WHITE);
  lineCount = lineCount + 1;

  Serial.begin(9600);
  
  /*if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present.");
    while(true);
  }*/
  
  Serial.println("Starting...");
  Serial.println();

  resetBuffer();
  
  /*while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    
    status = WiFi.begin(ssid, pass);
    
    delay(10000);
  }
  
  printWifiStatus();*/
}

void loop() {
  if (Serial.available() > 0) {
    incomingByte = Serial.read();

    if (incomingByte != NEWLINE) {
      buffer[index] = incomingByte;
    }

    index = index + 1;

    if (index == BUFFER_MAX || incomingByte == NEWLINE) {

      if (lineCount == MAX_LINE_COUNT) {
        resetScreen();
      } else {
        x = x - 20;
      }

      Serial.print("I received: ");
      Serial.println(buffer);

      Tft.drawString(buffer, x, y, 2, WHITE);
      memset(buffer, 0, sizeof(buffer));

      lineCount = lineCount + 1;

      if (incomingByte == NEWLINE) {
        resetBuffer();
      } else {
        index = 0;
      }
    }
  }
}

void resetScreen() {
  x = X_START;
  Tft.paintScreenBlack();
  lineCount = 0;
}

void resetBuffer() {
  buffer[0] = '>';
  buffer[1] = ' ';
  index = 2;
}

/*
void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}*/
