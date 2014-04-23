#include <stdint.h>
#include <SPI.h>
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

#define BUFFER_MAX 17
#define SCREEN_LENGTH 16
#define MAX_LINE_COUNT 11
#define NEWLINE 0xa

#define X_START 220
#define Y_START 10

#define YP A1  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 7   // can be a digital pin
#define XP 6   // can be a digital pin

#define MINPRESSURE 900
#define MAXPRESSURE 1000
#define PUSHMAX 5

int incomingByte = 0;
char buffer[BUFFER_MAX+1];
int index = 0;
int charCount = 0;
int x = X_START;
int y = Y_START;
int lineCount = 0;
int pushCount = 0;
boolean blank = false;
unsigned long startPush;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

void setup() {
  while(!Serial) {
  }

  Tft.init();
  Tft.setDisplayDirect(UP2DOWN);
  sendWelcome();
  startPush = millis();

  Serial.begin(9600);
  
  resetBuffer();
}

void loop() {
  TSPoint p = ts.getPoint();

  if (p.z > MINPRESSURE && p.z < MAXPRESSURE && millis() < startPush + 2000) {
    pushCount = pushCount + 1;

    if (pushCount == PUSHMAX) {
      pushCount = 0;
      resetBuffer();
      resetScreen();
      sendWelcome();
    }
  }

  if (pushCount > 0 && millis() >= startPush + 2000) {
    pushCount = 0;
  }

  if (pushCount == 0) {
    startPush = millis();
  }

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
        if (blank == true) {
          x = X_START;
        } else {
          x = x - 20;
        }
      }

      Tft.drawString(buffer, x, y, 2, WHITE);
      lineCount = lineCount + 1;
      blank = false;

      memset(buffer, 0, sizeof(buffer));


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
  blank = true;
}

void resetBuffer() {
  buffer[0] = '>';
  buffer[1] = ' ';
  index = 2;
}

void sendWelcome() {
  Tft.drawString("> zettajs.io", x, y, 2, WHITE);
  lineCount = lineCount + 1;
  blank = false;
}
