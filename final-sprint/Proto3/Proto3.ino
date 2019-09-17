/* Author: Kira Mulshine, Corey Williams, Nick Pourazima
 * Contact: contactmoodring@gmail.com
 * Date: April 4, 2018
 *
 * Overall Description:
 *
 * moodRing 1 & 2 setup & paired on app, initial array of color values (moodMaps) completed
 * moodMaps sent to moodRing
 * on moodRings - color is selected via cap touch & sent over BLE
 * moodRing 2 receives moodRing1 color and is written as partnersColor
 * vis-versa for moodRing 1
 * spin detection will display personal color on clockwise
 * and partnersColor on counterClockwise
 *
 * Hardware: Adafruit Feather Bluefruit 32u4
 * Hardware (continued): NeoPixel Ring 24
*/

//===============================================
//=================  LIBRARIES ==================
#include <CapacitiveSensor.h>
#include <Adafruit_NeoPixel.h>
#include <string.h>
#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"
#include "BluefruitConfig.h"
#if SOFTWARE_SERIAL_AVAILABLE
#include <SoftwareSerial.h>
#endif

//===============================================
//=================  BT CONFIG ==================

#define FACTORYRESET_ENABLE 1
#define MINIMUM_FIRMWARE_VERSION "0.6.6"
#define MODE_LED_BEHAVIOUR "MODE"

//===============================================
//=================  PIN CONFIG =================

#define NUMPIXELS 6
#define hallSensor1 2 //named SDA on feather;
#define hallSensor2 3 //named SCL on feather;
#define cap1SendPin1 10
#define cap1SendPin2 11
#define cap2SendPin1 12
#define cap2SendPin2 13
#define NEOPIN 5

//===============================================
//==============  HALL EFFECT VARS ==============

//Assign variables for interrupts.
volatile int revCounter1 = 0;
volatile int revCounter2 = 0;

bool clockwise = false;
bool counterClockwise = false;
bool triggerH1 = false;
bool triggerH2 = false;

//===============================================
//======== CAP SENSOR VARS AND THRESHOLDS =======

#define MINTOUCH 20
#define MAXTOUCH 1000
#define MINTIME 2

CapacitiveSensor cs1 = CapacitiveSensor(cap1SendPin1, cap1SendPin2); // 47K resistor between pins (a,b), a is send b is receive or "sensor"
CapacitiveSensor cs2 = CapacitiveSensor(cap2SendPin1, cap2SendPin2);

long startTime = 0;
long elapsedTime = 0;

uint8_t incomingCapData = 0;
uint8_t incomingCapData2 = 0;
uint8_t previousCapData = 0;
uint8_t previousCapData2 = 0;

bool swiped = false;

//===============================================
//==== Touch Sense State Machine Definitions ====

#define TOUCH_FILTER 0
#define ZONE_1 1
#define ZONE_2 2
#define REGISTER 3
uint8_t touchState = TOUCH_FILTER;

//===============================================
//========== Color Selector Definitions =========

bool colorChange = true;
bool partnersColor = false;
bool newColorMap = false;

//===============================================
//============ NeoPixel Instantiation ===========

Adafruit_NeoPixel pixel = Adafruit_NeoPixel(NUMPIXELS, NEOPIN);

//===============================================
//============ Array of Color Options ===========

#define COLORCOUNT 5
#define RGB 3
char *incomingHexColors[1];
uint32_t convertedHexRGB[COLORCOUNT][RGB]; // 2D Array for storing converted HEX to RGB
uint32_t colorChoicesRGB[COLORCOUNT][RGB] = {};

uint8_t red;
uint8_t green;
uint8_t blue;

unsigned long timeout = 0;
uint8_t blinkOrSolid = 0;
int colorSetCopy = -1;
int colorSet = 0; // Used to cycle through colors

// Create the bluefruit object
Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

// A small helper
void error(const __FlashStringHelper *err)
{
  Serial.println(err);
  while (1)
    ;
}

void setup()
{
  Serial.begin(115200); //  Set baud rate for serial comm

  //===============================================
  //============ Initialize NeoPixels =============

  pixel.begin();
  for (uint8_t i = 0; i < NUMPIXELS; i++)
  {
    pixel.setPixelColor(i, pixel.Color(0, 0, 0)); // off
  }
  pixel.setBrightness(10);
  pixel.show();

  //===============================================
  //=============== CAP SENSE INIT ================

  cs1.set_CS_AutocaL_Millis(0xFFFFFFFF); //  turn off autocalibrate on channel 1 - just as an example
  cs2.set_CS_AutocaL_Millis(0xFFFFFFFF);

  //===============================================
  //================= Assign I/O ==================

  pinMode(hallSensor1, INPUT_PULLUP);
  pinMode(hallSensor2, INPUT_PULLUP);

  //===============================================
  //=============== Interrupt Init ================

  attachInterrupt(digitalPinToInterrupt(hallSensor1), hallInterupt1, RISING);
  attachInterrupt(digitalPinToInterrupt(hallSensor2), hallInterupt2, RISING);

  //===============================================
  //============ Initialize BLE Module ============
  Serial.print(F("Initialising the Bluefruit LE module: "));
  if (!ble.begin(VERBOSE_MODE))
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  Serial.println(F("OK!"));

  if (FACTORYRESET_ENABLE)
  {
    /* Perform a factory reset to make sure everything is in a known state */
    Serial.println(F("Performing a factory reset: "));
    if (!ble.factoryReset())
    {
      error(F("Couldn't factory reset"));
    }
  }

  /* Disable command echo from Bluefruit */
  ble.echo(false);

  Serial.println("Requesting Bluefruit info:");
  /* Print Bluefruit information */
  ble.info();

  Serial.println(F("Please use Adafruit Bluefruit LE app to connect in UART mode"));
  Serial.println(F("Then Enter characters to send to Bluefruit"));
  Serial.println();

  ble.verbose(false); // debug info is a little annoying after this point!

  /* Wait for connection */
  while (!ble.isConnected())
  {
    delay(500);
  }

  // LED Activity command is only supported from 0.6.6
  if (ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION))
  {
    // Change Mode LED Activity
    Serial.println(F("******************************"));
    Serial.println(F("Change LED activity to " MODE_LED_BEHAVIOUR));
    ble.sendCommandCheckOK("AT+HWModeLED=" MODE_LED_BEHAVIOUR);
    Serial.println(F("******************************"));
  }
}

void loop()
{
  checkForUserInput();
}

void checkForUserInput()
{
  // Check for user input
  char inputs[BUFSIZE + 1];

  if (getUserInput(inputs, BUFSIZE))
  {
    // Send characters to Bluefruit
    Serial.print("[Send] ");
    Serial.println(inputs);

    ble.print("AT+BLEUARTTX=");
    ble.println(inputs);

    // check response stastus
    if (!ble.waitForOK())
    {
      Serial.println(F("Failed to send?"));
    }
  }

  // Check for incoming characters from Bluefruit
  ble.println("AT+BLEUARTRX");
  ble.readline();
  if (strcmp(ble.buffer, "OK") == 0)
  {
    // no data
    return;
  }
  // Some data was found, its in the buffer
  Serial.print(F("[Recv] "));
  Serial.println(ble.buffer);
  ble.waitForOK();

  // Read buffer into char array for RGB parsing
  char *parser[14];
  int flag;

  // Incoming RGB values have packet header with first letter of emotion capitalized
  // Ex: Ergb(255, 255, 0)
  if (ble.buffer[0] == 'E')
  {
    flag = 0;
    *parser = strtok(ble.buffer, "Ergb(");
  }
  if (ble.buffer[0] == 'S')
  {
    flag = 1;
    *parser = strtok(ble.buffer, "Srgb(");
  }
  if (ble.buffer[0] == 'D')
  {
    flag = 2;
    *parser = strtok(ble.buffer, "Drgb(");
  }
  if (ble.buffer[0] == 'F')
  {
    flag = 3;
    *parser = strtok(ble.buffer, "Frgb(");
  }
  if (ble.buffer[0] == 'A')
  {
    flag = 4;
    *parser = strtok(ble.buffer, "Argb(");
  }
  // Further parse packet into RGB components and store into 2D array
  // Current max of 5 selectable colors
  char *token = strtok(*parser, ", ");
  for (int i = 0; i < 3; i++)
  {
    colorChoicesRGB[flag][i] = atoi(token);
    Serial.println(colorChoicesRGB[flag][i]);
    token = strtok(NULL, ", ");
    newColorMap = true;
  }
  //  if(ble.buffer[0]=='P'){
  //    Serial.println("Update partner color");
  //    partnersColor=true;
  //  }
}

// If BT connected and new info in buffer, execute rest of code
bool getUserInput(char buffer[], uint8_t maxSize)
{
  // timeout in 100 milliseconds
  TimeoutTimer timeout(100);

  memset(buffer, 0, maxSize);
  while ((!Serial.available()) && !timeout.expired())
  {
    checkForTouch();  // Checks cap sensors if a swipe has been detected
    checkTimer();     // Master control for timing events such as LED blink or solid 
    spinDirection();  // Checks Hall Effect sensors for spin direction
    colorSelector();  // Gets and sets color if change has occurred
  }

  //  BLE Buffer Handling
  if (timeout.expired())
    return false;

  delay(2);
  uint8_t count = 0;
  do
  {
    count += Serial.readBytes(buffer + count, maxSize);
    delay(2);
  } while ((count < maxSize) && (Serial.available()));

  return true;
}

void checkTimer() // Delay independent master controller for event timing execution
{
  static unsigned long timer = 0;
  static bool on = false;
  if (millis() < timeout)
  {
    if (colorSet != colorSetCopy)
    {
      colorSetCopy = colorSet;
      on = false;
      timer = 0;
    }
    if (blinkOrSolid == 1)  // Show solid color during spin
    {
      if (!on)
      {
        setColor();
        showColor();
        on = true;
      }
    }
    else  // Blink color during color selector mode
    {
      if (millis() > timer)
      {
        on = !on;
        if (on)
        {
          setColor();
          showColor();
        }
        else
        {
          turnOffPixels();
        }
        timer = millis() + 1000;
      }
      sendPacket();
    }
  }
  else
  {
    if (on)
    {
      turnOffPixels();
      on = false;
    }
    timer = 0;
  }
}

void sendPacket() // Sends out current color selection over BT
{
  switch(colorSet)
  {
    case 1:
      ble.print('Ergb(');
      ble.print(colorChoicesRGB[colorSet][0]);
      ble.print(colorChoicesRGB[colorSet][1]);
      ble.print(', ');
      ble.print(colorChoicesRGB[colorSet][2]);
      ble.print(')');
      break;
    case 2:
      ble.print('Srgb(');
      ble.print(colorChoicesRGB[colorSet][0]);
      ble.print(colorChoicesRGB[colorSet][1]);
      ble.print(', ');
      ble.print(colorChoicesRGB[colorSet][2]);
      ble.print(')');
      break;
    case 3:
      ble.print('Drgb(');
      ble.print(colorChoicesRGB[colorSet][0]);
      ble.print(colorChoicesRGB[colorSet][1]);
      ble.print(', ');
      ble.print(colorChoicesRGB[colorSet][2]);
      ble.print(')');
      break;
    case 4:
      ble.print('Frgb(');
      ble.print(colorChoicesRGB[colorSet][0]);
      ble.print(colorChoicesRGB[colorSet][1]);
      ble.print(', ');
      ble.print(colorChoicesRGB[colorSet][2]);
      ble.print(')');
      break;
    case 5:
      ble.print('Argb(');
      ble.print(colorChoicesRGB[colorSet][0]);
      ble.print(colorChoicesRGB[colorSet][1]);
      ble.print(', ');
      ble.print(colorChoicesRGB[colorSet][2]);
      ble.print(')');
      break;
    default:
      ble.println("Nothing to send");
  }

}

void colorSelector()  // If a swipe action is detected, cycle through next available color, then blink to await acceptance
{
  if (swiped)
  {
    if (colorSet == (COLORCOUNT - 1))
    {
      colorSet = 0;
      timeout = 0;
    }
    else
    {
      colorSet++;
    }
    Serial.println("TOUCH");
    timeout = millis() + 3000;
    blinkOrSolid = 0;
  }
}

void checkForTouch()  // Threshold cap touch sensors such that a flag is set on a swipe motion
{
  startTime = millis();
  incomingCapData = map(cs1.capacitiveSensor(30), 0, 60, 0, 100);
  incomingCapData2 = map(cs2.capacitiveSensor(30), 0, 60, 0, 100);
  elapsedTime = millis() - startTime;
  swiped = false;
  switch (touchState)
  {
  case TOUCH_FILTER:
    if ((elapsedTime > MINTIME) && (incomingCapData > MINTOUCH) || (incomingCapData2 > MINTOUCH) && (incomingCapData < MAXTOUCH) || (incomingCapData2 < MAXTOUCH))
    { // Check if touch is between thresholds & over minimum time
      touchState++;
    }
    break;
  case ZONE_1:
    if (incomingCapData > 0 && incomingCapData < 100)
    {
      touchState++;
    }
    break;
  case ZONE_2:
    if (incomingCapData2 > 0 && incomingCapData2 < 100)
    {
      touchState = REGISTER;
    }
    break;
  case REGISTER:
    swiped = true;
    touchState = TOUCH_FILTER;
    break;
  default:
    break;
  }
}

void hexToRGB()
{
  //===============================================
  //============ HEX to RGB Conversion ============

  for (int i = 0; i < COLORCOUNT; i++)
  {
    String hexstring = String(incomingHexColors[i]);
    long number = strtol(&hexstring[1], NULL, 16);
    int r = number >> 16;
    int g = number >> 8 & 0xFF;
    int b = number & 0xFF;
    convertedHexRGB[i][0] = r;
    convertedHexRGB[i][1] = g;
    convertedHexRGB[i][2] = b;
  }
  //===============================================
  colorChange = false;
}

void setColor() // Sets NeoPixel color dependending on event
{
  if (newColorMap)
  {
    for (uint8_t i = 0; i < NUMPIXELS; i++)
    {
      pixel.setPixelColor(i, pixel.Color(colorChoicesRGB[colorSet][0], colorChoicesRGB[colorSet][1], colorChoicesRGB[colorSet][2]));
    }
  }
  // CODE FOR FUTURE IMPLEMENTATION:
  // if(partnersColor){
  //   for(uint8_t i=0; i<NUMPIXELS; i++) {
  //       pixel.setPixelColor(i,pixel.Color(convertedHexRGB[colorSet][0],convertedHexRGB[colorSet][1],convertedHexRGB[colorSet][2]));
  //   }
  //   partnersColor=false;
  // }

  if (counterClockwise) // Show Partners Color
  {
    Serial.println("IN COUNTER_CLOCKWISE");
    for (uint8_t i = 0; i < NUMPIXELS; i++)
    {
      pixel.setPixelColor(i, pixel.Color(255, 0, 255)); // Default to Purple currently
    }
    counterClockwise = false;
  }
}

void showColor() // Displays registered color
{
  pixel.show();
}

void turnOffPixels()  // Sets NeoPixels to black
{
  for (uint8_t i = 0; i < NUMPIXELS; i++)
  {
    pixel.setPixelColor(i, pixel.Color(0, 0, 0)); // off
  }
  pixel.show();
}

void spinDirection()  // Detect direction of spin based on logic of Hall Effect sensor interrupts
{
  if ((triggerH1) && (revCounter2 > 0) && (revCounter1 > revCounter2))  // if H.E. 1 is trigger and the counter is higher than the other
  {
    clockwise = true;
    resetSpinDirection();
    Serial.println("CLOCKWISE");
    timeout = millis() + 2000;
    blinkOrSolid = 1;
  }
  if ((triggerH2 && revCounter1 > 0) && (revCounter2 > revCounter1))
  {
    counterClockwise = true;
    resetSpinDirection();
    Serial.println("COUNTER_CLOCKWISE");
    Serial.println(counterClockwise);
    timeout = millis() + 2000;
    blinkOrSolid = 1;
  }
}

void resetSpinDirection() // Re-initialize spin counters and H.E. triggers
{
  revCounter1 = 0;
  revCounter2 = 0;
  triggerH2 = false;
  triggerH1 = false;
}

void hallInterupt1()  //  Method called when Hall Effect sensor 1 is activated
{
  triggerH1 = true;
  revCounter1++;
  Serial.println("1st HALL EFFECT sensor TRIGGERED");
  Serial.println(revCounter1);
}
void hallInterupt2()  //  Method called when Hall Effect sensor 2 is activated
{
  triggerH2 = true;
  revCounter2++;
  Serial.println("2nd HALL EFFECT sensor TRIGGERED");
  Serial.println(revCounter2);
}