/**************************************************************
 * Blynk is a platform with iOS and Android apps to control
 * Arduino, Raspberry Pi and the likes over the Internet.
 * You can easily build graphic interfaces for all your
 * projects by simply dragging and dropping widgets.
 *
 *   Downloads, docs, tutorials: http://www.blynk.cc
 *   Blynk community:            http://community.blynk.cc
 *   Social networks:            http://www.fb.com/blynkapp
 *                               http://twitter.com/blynk_app
 *
 * Blynk library is licensed under MIT license
 * This example code is in public domain.
 *
 **************************************************************
 * Control a color gradient on NeoPixel strip using a slider!
 *
 * For this example you need NeoPixel library:
 *   https://github.com/adafruit/Adafruit_NeoPixel
 *
 * App dashboard setup:
 *   Slider widget (0...500) on V1
 *
 **************************************************************/

#define BLYNK_PRINT Serial
#include <SPI.h>
#include <Ethernet.h>
//#include <BlynkSimpleEthernet.h>
#include <Adafruit_NeoPixel.h>

#define PIN 8



Adafruit_NeoPixel strip = Adafruit_NeoPixel(120, PIN, NEO_GRB + NEO_KHZ800);

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "6303c5e9d44b4e98b566d362b50189c6";
int PartyPin = 2;
int PartyStatus = 0;
int RedPin = 3;
int RedStatus = 0;
int BluePin = 5;
int BlueStatus = 0;
int WhitePin = 6;
int WhiteStatus = 0;
//int PulsePin = ;
//int PulseStatus = 0;
int cWheelPin = 7;
int cWheelStatus = 0;
int RValue = 0;
int GValue = 0;
int BValue = 0;
unsigned long previousMillis = 0;
int neoPixel_j = 0;
unsigned long currentMillis = 0;
int Pwait = 100;
int Brightness = 0;


void setup()
{
  //WiFi.init();
  Serial.begin(9600);
  Blynk.begin(auth);
  strip.begin();
  strip.show();
}

//Read in the Virtual pins for sliders
BLYNK_WRITE(1)
{
  int shift = param.asInt();
  for (int i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, Wheel(shift & 255));
    // OR: strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + shift) & 255));
  }
  strip.show();
}
BLYNK_WRITE(2)
{
  RValue = param.asInt();
}
BLYNK_WRITE(3)
{
  GValue = param.asInt();
}
BLYNK_WRITE(4)
{
  BValue = param.asInt();
}
BLYNK_WRITE(5)
{
  Brightness = param.asInt()
}
void loop()
{
  Blynk.run();
  PartyStatus = digitalRead(PartyPin);
  RedStatus = digitalRead(RedPin);
  BlueStatus = digitalRead(BluePin);
  WhiteStatus = digitalRead(WhitePin);
  cWheelStatus = digitalRead(cWheelPin);
  Serial.println(cWheelStatus);
  strip.setBrightness(Brightness);
  
  // Set up buttons
  if (PartyStatus == 1) {
    for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
        if(millis() - previousMillis > Pwait){
        
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
        }
        strip.show();
       previousMillis = currentMillis;
      }
      }
    }
  
  }else if (RedStatus == 1){
     solidColor(strip.Color(255,0,0), 1); //red
  }else if (BlueStatus ==1){
    solidColor(strip.Color(0,0,255),1);
  }else if (WhiteStatus ==1){
    solidColor(strip.Color(125,125,125),1);
  }else if (cWheelStatus == 1){
    solidColor(strip.Color(RValue,GValue,BValue),1);
  }else{
    solidColor(strip.Color(0,0,0),1); //turns strip off
  }

}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if (WheelPos < 85) {
    return strip.Color(WheelPos * 10, 255 - WheelPos * 10, 0);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 10, 0, WheelPos * 10);
  } else {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 10, 255 - WheelPos * 10);
  }
}

void solidColor(uint32_t c, uint8_t wait) 
{
 if(millis() - previousMillis > wait) 
 {
   allColor(c);
   strip.show(); 
   previousMillis = currentMillis; 
 }
}

void softBlink(uint32_t c, uint8_t brightness, uint8_t wait) 
{
 if(millis() - previousMillis > wait) 
 {
   allColor(c);
   uint16_t i;//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
   int b = (neoPixel_j * brightness) / brightness;
   strip.setBrightness(b);
   strip.show(); 
   neoPixel_j = (neoPixel_j + 1) % brightness;
   previousMillis = currentMillis; 
 }
}


void allColor(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
  }
}
