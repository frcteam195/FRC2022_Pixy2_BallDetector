#include <stdio.h>
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_DotStar.h>
#include <Adafruit_SleepyDog.h>

//#define DEBUG
#define DATAPIN INTERNAL_DS_DATA
#define CLOCKPIN INTERNAL_DS_CLK
#define NUMPIXELS 1
#define RED_DIO_PIN 0
#define BLUE_DIO_PIN 1
#include <Pixy2.h>

Adafruit_DotStar pixels(NUMPIXELS, DATAPIN, CLOCKPIN , DOTSTAR_BGR);

Pixy2 pixy;
void setup()
{
  Watchdog.enable(2000);

  #ifdef DEBUG
  Serial.begin(9600);
  Serial.print("Starting...\n");
  #endif

  pixels.begin();
  pixels.clear();

  pinMode(RED_DIO_PIN, OUTPUT);
  pinMode(BLUE_DIO_PIN, OUTPUT);

  pixy.init();
  pixy.setLamp(1, 0);

  Watchdog.reset();
}

int block;
bool isRed = false;
bool isBlue = false;

void turnLEDRedON()
{
  digitalWrite(RED_DIO_PIN, LOW);
  digitalWrite(BLUE_DIO_PIN, HIGH);
  pixy.setLED(255, 0, 0);
  #ifdef DEBUG
  Serial.print("I see red\n");
  #endif

  for (int i = 0; i < NUMPIXELS; i++)
  {
    pixels.setPixelColor(i, pixels.Color(150, 0, 0));
  }
}

void turnLEDBlueON()
{
  digitalWrite(RED_DIO_PIN, LOW);
  digitalWrite(BLUE_DIO_PIN, HIGH);
  pixy.setLED(0, 0, 255);
  #ifdef DEBUG
  Serial.print("I see blue\n");
  #endif

  for (int i = 0; i < NUMPIXELS; i++)
  {
    pixels.setPixelColor(i, pixels.Color(0, 0, 150));
  }
}

void turnLEDsOFF()
{
  digitalWrite(RED_DIO_PIN, LOW);
  digitalWrite(BLUE_DIO_PIN, LOW);
  pixy.setLED(0, 0, 0);
  #ifdef DEBUG
  Serial.print("I don't see anything\n");
  #endif

  for (int i = 0; i < NUMPIXELS; i++)
  {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
  }
}

int timeRed = 0;
int timeBlue = 0;

void loop()
{
  //set_color(block);

  //pixels.show();
  int myTime = millis();
  pixy.ccc.getBlocks(false);

  block = pixy.ccc.blocks->m_signature;

  if (block == 1) //if it is a red block
  {
    timeRed = myTime; //time red is the current time
    turnLEDRedON();
    isRed = true;
    isBlue = false;
  }

  if (block == 2)
  {
    timeBlue = myTime;
    turnLEDBlueON();
    isBlue = true;
    isRed = false;
  }

  if (isRed == true && block != 1)
  {
    if (myTime - timeRed > 100)
    {
      turnLEDsOFF();
      isRed = false;
    }

    else
    {
      turnLEDRedON();
      isRed = true;
    }
  }

  if (isBlue == true && block != 2)
  {
    if (myTime - timeBlue > 100)
    {
      turnLEDsOFF();
      isBlue = false;
    }

    else
    {
      turnLEDBlueON();
      isBlue = true;
    }
  }

  pixels.show();
  Watchdog.reset();
}