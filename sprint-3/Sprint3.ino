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

// Nick TODO:
// read in color array over BT
// send chosen color to app

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


#define FACTORYRESET_ENABLE         1
#define MINIMUM_FIRMWARE_VERSION    "0.6.6"
#define MODE_LED_BEHAVIOUR          "MODE"


//===============================================
//=================  PIN CONFIG =================

#define NUMPIXELS 6
#define hallSensor1 2   //named SDA on feather;
#define hallSensor2 3   //named SCL on feather;
#define cap1SendPin1 10
#define cap1SendPin2 11
#define cap2SendPin1 12
#define cap2SendPin2 13
#define NEOPIN 5

//===============================================
//==============  HALL EFFECT VARS ==============

 //Assign variables for interrupts.
 volatile long revCounter1 = 0;
 volatile long revCounter2 = 0;
 
 bool clockwise = false;
 bool counterClockwise = false;
 bool triggerH1 = false;
 bool triggerH2 = false;

//===============================================
//======== CAP SENSOR VARS AND THRESHOLDS =======

#define MINTOUCH 20
#define MAXTOUCH 1000
#define MINTIME 2

CapacitiveSensor   cs1= CapacitiveSensor(cap1SendPin1,cap1SendPin2);        // 47K resistor between pins (a,b), a is send b is receive or "sensor"
CapacitiveSensor   cs2= CapacitiveSensor(cap2SendPin1,cap2SendPin2);

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

#define COLORCOUNT 1
#define RGB 3
char* incomingHexColors[1];
// char* incomingHexColors[8]= {"#ff0000", "#9d00ff", "#ff9900",  "#00ffff", "#ffff00"};
uint32_t convertedHexRGB[COLORCOUNT][RGB]; // 2D Array for storing converted HEX to RGB

uint8_t red;
uint8_t green;
uint8_t blue;

unsigned long timeout = 0;
uint8_t blinkOrSolid = 0;
int colorSetCopy = -1;
int colorSet = 0;   // Used to cycle through colors

// Create the bluefruit object, either software serial...uncomment these lines
/*
SoftwareSerial bluefruitSS = SoftwareSerial(BLUEFRUIT_SWUART_TXD_PIN, BLUEFRUIT_SWUART_RXD_PIN);
Adafruit_BluefruitLE_UART ble(bluefruitSS, BLUEFRUIT_UART_MODE_PIN,
                      BLUEFRUIT_UART_CTS_PIN, BLUEFRUIT_UART_RTS_PIN);
*/

/* ...or hardware serial, which does not need the RTS/CTS pins. Uncomment this line */
// Adafruit_BluefruitLE_UART ble(Serial1, BLUEFRUIT_UART_MODE_PIN);

/* ...hardware SPI, using SCK/MOSI/MISO hardware SPI pins and then user selected CS/IRQ/RST */
Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

/* ...software SPI, using SCK/MOSI/MISO user-defined SPI pins and then user selected CS/IRQ/RST */
//Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_SCK, BLUEFRUIT_SPI_MISO,
//                             BLUEFRUIT_SPI_MOSI, BLUEFRUIT_SPI_CS,
//                             BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);


// A small helper
void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

void setup()
{
    Serial.begin(115200);                         //  Set baud rate for serial comm
  
    //===============================================
    //============ Initialize NeoPixels =============

    pixel.begin();
    for(uint8_t i=0; i<NUMPIXELS; i++) {
        pixel.setPixelColor(i, pixel.Color(0,0,0)); // off
    }
    pixel.setBrightness(10);
    pixel.show();

    //===============================================
    //=============== CAP SENSE INIT ================

    cs1.set_CS_AutocaL_Millis(0xFFFFFFFF);    //  turn off autocalibrate on channel 1 - just as an example
    cs2.set_CS_AutocaL_Millis(0xFFFFFFFF);

    //===============================================
    //================= Assign I/O ==================

    pinMode(hallSensor1, INPUT_PULLUP);
    pinMode(hallSensor2, INPUT_PULLUP);

    //===============================================
    //=============== Interrupt Init ================

    attachInterrupt(digitalPinToInterrupt(hallSensor1), hallInterupt1, RISING);
    attachInterrupt(digitalPinToInterrupt(hallSensor2), hallInterupt2, RISING);

    // hexToRGB();

  /* Initialise the module */
  Serial.print(F("Initialising the Bluefruit LE module: "));
  if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  Serial.println( F("OK!") );

  if ( FACTORYRESET_ENABLE )
  {
    /* Perform a factory reset to make sure everything is in a known state */
    Serial.println(F("Performing a factory reset: "));
    if ( ! ble.factoryReset() ){
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

  ble.verbose(false);  // debug info is a little annoying after this point!

  /* Wait for connection */
  while (! ble.isConnected()) {
      delay(500);
  }

  // LED Activity command is only supported from 0.6.6
  if ( ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) )
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
//    checkForTouch();
//    checkTimer();
//    spinDirection();
//    colorSelector();
}

void checkForUserInput(){
  // Check for user input
  char inputs[BUFSIZE+1];

  if ( getUserInput(inputs, BUFSIZE) )
  {
    // Send characters to Bluefruit
    Serial.print("[Send] ");
    Serial.println(inputs);

    ble.print("AT+BLEUARTTX=");
    ble.println(inputs);

    // check response stastus
    if (! ble.waitForOK() ) {
      Serial.println(F("Failed to send?"));
    }
  }

  // Check for incoming characters from Bluefruit
  ble.println("AT+BLEUARTRX");
  ble.readline();
  if (strcmp(ble.buffer, "OK") == 0) {
    // no data
    return;
  }
  // Some data was found, its in the buffer
  Serial.print(F("[Recv] ")); 
  Serial.println(ble.buffer);
  ble.waitForOK();
  // Color
  if (ble.buffer[1] == 'C') {
    red = ble.buffer[2];
    green = ble.buffer[3];
    blue = ble.buffer[4];
    Serial.println(red);
    Serial.println(green);
    Serial.println(blue);
    Serial.print ("RGB #");
    if (red < 0x10) Serial.print("0");
    Serial.print(red, HEX);
    if (green < 0x10) Serial.print("0");
    Serial.print(green, HEX);
    if (blue < 0x10) Serial.print("0");
    Serial.println(blue, HEX);
    newColorMap = true;
  }
  if(ble.buffer[0]=='#'){
    Serial.println("Update my color");
    incomingHexColors[0] = ble.buffer;
    hexToRGB();
  }
  if(ble.buffer[0]=='B'){
    Serial.println("Update partner color");
    partnersColor=true;
  }

//  for(int i = 0; i < 8; i++){
////    strcpy((*incomingHexColors),ble.buffer);
//    incomingHexColor[i] = ble.buffer;
//    Serial.println(incomingHexColors[i]);
//  }
}

bool getUserInput(char buffer[], uint8_t maxSize)
{
  // timeout in 100 milliseconds
  TimeoutTimer timeout(100);

  memset(buffer, 0, maxSize);
  while( (!Serial.available()) && !timeout.expired() ) 
  { 
    checkForTouch();
    checkTimer();
    spinDirection();
    colorSelector();
  }

  if ( timeout.expired() ) return false;

  delay(2);
  uint8_t count=0;
  do
  {
    count += Serial.readBytes(buffer+count, maxSize);
    delay(2);
  } while( (count < maxSize) && (Serial.available()) );

  return true;
}

void checkTimer(){
  static unsigned long timer = 0;
  static bool on = false;
  if(millis() < timeout){
    if(colorSet != colorSetCopy){
      colorSetCopy = colorSet;
      on = false;
      timer = 0;
      }  
      if(blinkOrSolid==1){
        if(!on){
          setColor();
          showColor();
          on = true;
        }
      }
    else{
        if(millis() > timer){
          on = !on;
          if(on){
            setColor();
            showColor();
          }
          else{
            turnOffPixels();
          }
          timer = millis()+1000;
        }      
      }      
    }
  else{   
    if(on){   
      turnOffPixels();
      on = false;
    }
    timer = 0;
  }
}

void colorSelector(){
  if(swiped){
    if(colorSet==(COLORCOUNT-1)){
      colorSet=0;
      timeout = 0;
    }
    else{
        colorSet++;
    }
    Serial.println("TOUCH");
    timeout = millis()+3000;
    blinkOrSolid = 0;
  }  
}

void checkForTouch(){
    startTime = millis();
    incomingCapData =  map(cs1.capacitiveSensor(30),0,60,0,100);
    incomingCapData2 = map(cs2.capacitiveSensor(30),0,60,0,100);
    elapsedTime = millis()-startTime;
    swiped = false;
    switch(touchState)
    {
        case TOUCH_FILTER:
            if((elapsedTime > MINTIME) && (incomingCapData > MINTOUCH) || (incomingCapData2 > MINTOUCH) && (incomingCapData < MAXTOUCH) || (incomingCapData2 < MAXTOUCH)){  // Check if touch is between thresholds & over minimum time
                touchState++;
            }
            break;
        case ZONE_1:
            if(incomingCapData > 0 && incomingCapData < 100){
                touchState++;
            }
            break;
        case ZONE_2:
            if(incomingCapData2 > 0 && incomingCapData2 < 100){
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

void hexToRGB(){

    //===============================================
    //============ HEX to RGB Conversion ============

    for(int i = 0; i < COLORCOUNT; i++){
        String hexstring = String(incomingHexColors[i]);
        long number = strtol( &hexstring[1], NULL, 16);
        int r = number >> 16;
        int g = number >> 8 & 0xFF;
        int b = number & 0xFF;
        convertedHexRGB[i][0]=r;
        convertedHexRGB[i][1]=g;
        convertedHexRGB[i][2]=b;
    }
    //===============================================
    colorChange = false;
}

void setColor(){
   Serial.println("IN SET COLOR NOW");
   if(newColorMap){ 
     Serial.println("IN NEWCOLORMAP");
     for(uint8_t i=0; i<NUMPIXELS; i++) {
         pixel.setPixelColor(i,pixel.Color(red,green,blue));
     }
   }
  // if(partnersColor){
  //   for(uint8_t i=0; i<NUMPIXELS; i++) {
  //       pixel.setPixelColor(i,pixel.Color(convertedHexRGB[colorSet][0],convertedHexRGB[colorSet][1],convertedHexRGB[colorSet][2]));
  //   }
  //   partnersColor=false;
  // }

  if(counterClockwise){
    Serial.println("IN COUNTER_CLOCKWISE");
    for(uint8_t i=0; i<NUMPIXELS; i++) {
        pixel.setPixelColor(i,pixel.Color(255, 0, 255));
    }
    counterClockwise=false;
  }
//  else{
//    Serial.println("ELSE COLORSET");
//    for(uint8_t i=0; i<NUMPIXELS; i++) {
//        pixel.setPixelColor(i,pixel.Color(convertedHexRGB[colorSet][0],convertedHexRGB[colorSet][1],convertedHexRGB[colorSet][2]));
//    }
//  }
}

void showColor(){
    pixel.show();
}

void turnOffPixels(){
    for(uint8_t i=0; i<NUMPIXELS; i++) {
        pixel.setPixelColor(i, pixel.Color(0,0,0)); // off
  }
  pixel.show();
}

void spinDirection(){
    if ((triggerH1) && (revCounter2 > 0) && (revCounter1 > revCounter2)){
        clockwise = true;
        Serial.println("CLOCKWISE");
        timeout = millis()+5000;
        blinkOrSolid = 1;
        revCounter1 = 0;
        revCounter2 = 0;
        triggerH1 = false;
        triggerH2 = false;
    }
    if ((triggerH2 && revCounter1 > 0) && (revCounter2 > revCounter1)){
        counterClockwise = true;
        Serial.println("COUNTER_CLOCKWISE");
        Serial.println(counterClockwise);
        timeout = millis()+5000;
        blinkOrSolid = 1;
        revCounter1 = 0;
        revCounter2 = 0;
        triggerH2 = false;
        triggerH1 = false;
    }
}

void hallInterupt1()
{
    triggerH1 = true;
    Serial.println("1st HALL EFFECT sensor TRIGGERED");
    revCounter1++;
    Serial.println(revCounter1);
}
void hallInterupt2()
{
    triggerH2 = true;
    Serial.println("2nd HALL EFFECT sensor TRIGGERED");
    revCounter2++;
    Serial.println(revCounter2);
}