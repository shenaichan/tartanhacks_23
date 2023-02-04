// tartanhacks project 2023 lauren and shenai

int flexRead = 0;

const int flexPin = A0;

const int waitTime = 11000;

unsigned long timer = 0;

unsigned long servoTimer = 0;

const int servoWait = 3;

const int UP  = 1;

const int DOWN  = 0;

int servoDirection = 1;

unsigned long heartVal = 0;

unsigned long checkHeartTimer = 0;

const int heartWait = 1000;

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

const int servoPin = 9;

/*************************************************** 
  This is an example for the Adafruit VS1053 Codec Breakout

  Designed specifically to work with the Adafruit VS1053 Codec Breakout 
  ----> https://www.adafruit.com/products/1381

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

// include SPI, MP3 and SD libraries
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>

// define the pins used
#define CLK 13       // SPI Clock, shared with SD card
#define MISO 12      // Input data, from VS1053/SD card
#define MOSI 11      // Output data, to VS1053/SD card
// Connect CLK, MISO and MOSI to hardware SPI pins. 
// See http://arduino.cc/en/Reference/SPI "Connections"

// These are the pins used for the music maker shield
#define SHIELD_RESET  -1      // VS1053 reset pin (unused!)
#define SHIELD_CS     7      // VS1053 chip select pin (output)
#define SHIELD_DCS    6      // VS1053 Data/command select pin (output)

// These are common pins between breakout and shield
#define CARDCS 4     // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ 3       // VS1053 Data request, ideally an Interrupt pin

Adafruit_VS1053_FilePlayer musicPlayer = 
  // create shield-example object!
  Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);

/*
  Heart beat plotting!
  By: Nathan Seidle @ SparkFun Electronics
  Date: October 20th, 2016
  https://github.com/sparkfun/MAX30105_Breakout

  Shows the user's heart beat on Arduino's serial plotter

  Instructions:
  1) Load code onto Redboard
  2) Attach sensor to your finger with a rubber band (see below)
  3) Open Tools->'Serial Plotter'
  4) Make sure the drop down is set to 115200 baud
  5) Checkout the blips!
  6) Feel the pulse on your neck and watch it mimic the blips

  It is best to attach the sensor to your finger using a rubber band or other tightening
  device. Humans are generally bad at applying constant pressure to a thing. When you
  press your finger against the sensor it varies enough to cause the blood in your
  finger to flow differently which causes the sensor readings to go wonky.

  Hardware Connections (Breakoutboard to Arduino):
  -5V = 5V (3.3V is allowed)
  -GND = GND
  -SDA = A4 (or SDA)
  -SCL = A5 (or SCL)
  -INT = Not connected

  The MAX30105 Breakout can handle 5V or 3.3V I2C logic. We recommend powering the board with 5V
  but it will also run at 3.3V.
*/

#include <Wire.h>
#include "MAX30105.h"

MAX30105 particleSensor;
  
void setup() {

  // Initialize sensor
  if (particleSensor.begin() == false)
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }

  particleSensor.setup(); //Configure sensor. Use 6.4mA for LED drive

  heartVal = particleSensor.getIR();
  
  myservo.attach(servoPin);  // attaches the servo on pin 9 to the servo object
  
  Serial.begin(9600);
  Serial.println("Adafruit VS1053 Simple Test");

  if (! musicPlayer.begin()) { // initialise the music player
     Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
     while (1);
  }
  Serial.println(F("VS1053 found"));
  
   if (!SD.begin(CARDCS)) {
    Serial.println(F("SD failed, or not present"));
    while (1);  // don't do anything more
  }

  // list files
//  printDirectory(SD.open("/"), 0);
  
  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(0,0);

  // If DREQ is on an interrupt pin (on uno, #2 or #3) we can do background
  // audio playing
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int
  
  // Play one file, don't return until complete
//  Serial.println(F("Playing track 001"));
//  musicPlayer.playFullFile("/purr.mp3");
  // Play another file in the background, REQUIRES interrupts!
//  Serial.println(F("Playing track 002"));
//  musicPlayer.startPlayingFile("/track001.mp3");
}

void loop() {
  flexRead = map(analogRead(flexPin), 250, 540, 0, 1000);
  Serial.print("flex is: ");
  Serial.println(flexRead);

  if (millis() - checkHeartTimer >= heartWait) {
    checkHeartTimer = millis();
    heartVal = particleSensor.getIR(); //Send raw data to var
  }
  Serial.print("heart is: ");
  Serial.println(heartVal);

  if ((flexRead < 500) || (flexRead > 800)) {
    if (millis() - timer >=  waitTime) {
      musicPlayer.startPlayingFile("/purr.mp3");
      timer = millis();
    }
  }

//  if (millis() - servoTimer >= 1) {
    servoTimer = millis();
    if (heartVal >= 50000) {
      
      if (servoDirection == UP) {
        if (musicPlayer.playingMusic) {
          pos += 15;
        }
        else {
          pos += 15;
        }
      }
      else {
        if (musicPlayer.playingMusic) {
          pos -= 15;
        }
        else {
          pos -= 15;
        }
      }
      if (pos >= 180) {
        servoDirection = DOWN;
      }
      else if (pos <= 0) {
        servoDirection = UP;
      }
      myservo.write(pos);
    }
//  } 
}
