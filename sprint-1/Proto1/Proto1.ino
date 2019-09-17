/* Author: Nick Pourazima
 * Contact: contactmoodring@gmail.com
 * Date: April 3, 2018
 * Hardware: Adafruit Feather Bluefruit 32u4
 * Hardware (continued): NeoPixel Ring 24 
*/

// NICK - TODO
// If swiped through cap sensors and no input for 3 seconds, hold color, start countdown timer, flash color until it gets "applied"
// store current color for "current spin color"

//===============================================
//=================  LIBRARIES ==================

#include <CapacitiveSensor.h>
#include <Adafruit_NeoPixel.h>

//===============================================
//=================  PIN CONFIG =================

#define NUMPIXELS 24
#define NEOPIN 6

//===============================================
//======== CAP SENSOR VARS AND THRESHOLDS =======
#define MINTOUCH 20
#define MAXTOUCH 1000
#define MINTIME 2

CapacitiveSensor   cs_9_10 = CapacitiveSensor(9,10);        // 47K resistor between pins (a,b), a is send b is receive or "sensor" 
CapacitiveSensor   cs_11_12 = CapacitiveSensor(11,12);

long startTime = 0;
long elapsedTime = 0;
long timer = 0;

long incomingCapData = 0;
long incomingCapData2 = 0;
int previousCapData = 0;
int previousCapData2 = 0;
bool swiped = false;

//===============================================
//==== Touch Sense State Machine Definitions ====
// ============FUTURE IMPLEMENTATION ============
#define TOUCH_FILTER 0
#define ZONE_1 1
#define ZONE_2 2
// #define ABORT 3
#define REGISTER 3
uint8_t touchState = TOUCH_FILTER;


//===============================================
//== Color Selector State Machine Definitions ===

#define WAITFORTOUCH 0
#define CONVERTRGB 1
#define SETCOLOR 2
#define SHOWCOLOR 3
#define NEXTCOLOR 4
#define TURNOFF 5
uint8_t colorState = WAITFORTOUCH;
bool colorChange = true;

//===============================================
//============ NeoPixel Instantiation ===========

Adafruit_NeoPixel pixel = Adafruit_NeoPixel(NUMPIXELS, NEOPIN);

//===============================================
//============ Array of Color Options ===========
#define COLORCOUNT 8
#define RGB 3
char* incomingHexColors[8]={
    "#A0BB23","#008080","#FF0000","#00FF00",
    "#0000FF","#FFFF00","#F65D94","#FF00FF"
    };
int colorSet = 0;   // Used to cycle through colors
int convertedHexRGB[COLORCOUNT][RGB]; // 2D Array for storing converted HEX to RGB

void setup()                    
{

//===============================================
//============ Initialize NeoPixels =============

  pixel.begin();     
  for(uint8_t i=0; i<NUMPIXELS; i++) {
    pixel.setPixelColor(i, pixel.Color(0,0,0)); // off
  }
  pixel.show();

//===============================================

  cs_9_10.set_CS_AutocaL_Millis(0xFFFFFFFF);    // turn off autocalibrate on channel 1 - just as an example
  cs_11_12.set_CS_AutocaL_Millis(0xFFFFFFFF);
  Serial.begin(115200);                         //   Set baud rate for serial comm

}

void loop()                    
{
    // CAPACITIVE COLOR SELECTOR STATE MACHINE
    switch(colorState){
        case WAITFORTOUCH:
            checkForTouch();
            if(swiped){
                Serial.println("WHAT IS GOING ON?");
                colorState++;
            }
            break;
        case CONVERTRGB:
            if(colorChange){
                hexToRGB();
            }
            else{
                colorState++;
            }
            break;
        case SETCOLOR:
            setColor();
            colorState++;
            break;
        case SHOWCOLOR:
            showColor();
            delay(400); // Does not delay state machine logic, more of a feature to select color quickly
            colorState++;
            break;
        case NEXTCOLOR:
            if(colorSet==7){
                colorSet=0;
                colorState = TURNOFF;
            }
            else{
                colorSet++;
                colorState = WAITFORTOUCH;
            }
            break;
        case TURNOFF:
            turnOffPixels();
            colorState = WAITFORTOUCH;
            break; 
        default:
            turnOffPixels();
            break;
    }
}

void checkForTouch(){
    startTime = millis();
    incomingCapData =  map(cs_9_10.capacitiveSensor(30),0,60,0,100);
    incomingCapData2 = map(cs_11_12.capacitiveSensor(30),0,60,0,100);
    elapsedTime = millis()-startTime;
    swiped = false;
    switch(touchState)
    {
        case TOUCH_FILTER:
            if((elapsedTime > MINTIME) && (incomingCapData > MINTOUCH) || (incomingCapData2 > MINTOUCH) && (incomingCapData < MAXTOUCH) || (incomingCapData2 < MAXTOUCH)){  // Check if touch is between thresholds & over minimum time
                touchState++;
                Serial.println("TOUCH");
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
        // case ABORT:
        //     swiped = false; 
        //     // touchState = TOUCH_FILTER;
        //     break;
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
    for(uint8_t i=0; i<NUMPIXELS; i=i+3) {
        pixel.setPixelColor(i,pixel.Color(convertedHexRGB[colorSet][0],convertedHexRGB[colorSet][1],convertedHexRGB[colorSet][2]));
    }
    // ~~~~KEEP FOR DEBUG~~~~ 
    // for(int i = 0;i<8;i++){
    //     for(int j = 0; j<3;j++){
    //         Serial.print("  ");
    //         Serial.print(convertedHexRGB[i][j]);
    //     }
    // Serial.println();
    // }
    // uint32_t color = pixel.getPixelColor(3);
    // Serial.println(color);
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