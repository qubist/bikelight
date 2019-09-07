/*

  A fun and colorful bike light

  Parts:
  - Ardiuno Pro Mini 3.3v 8MHz
  - Tiny super-bright SMD LED
  - NeoPixel Ring 16 from Adafruit (https://www.adafruit.com/product/1463https://www.adafruit.com/product/1463)
  - LiPo Charger Basic from Sparkfun (https://www.sparkfun.com/products/10217https://www.sparkfun.com/products/10217)
  - small switch
  - small button
  
  The circuit:
  Pin 8 connected to NeoPixel Ring 16 Data input pin
  Pin 2 connected:
    by a button to VCC
    by a 10K ohm resistor to GND
  RAW connected to positive battery terminal of 3.7v LiIon (via LiPo Charger Basic and small switch)
  GND connected to negative battery termital
  VCC connected to Voltage in pin of NeoPixel ring
  GND connected to GND pin of NeoPixel ring
  Bright LED connected directly to VCC and GND of Arduino
  

  Will Harris-Braun

*/

#include <Adafruit_NeoPixel.h>

const byte RING_PIN = 8; // the pin connected to NeoPixel input pin
const byte ONBOARD_BUTTON_PIN = 2;

const int RAINBOW_SPEED = 5;
const int RED_RAINBOW_SPEED = 10;
const int NEOPIXEL_BRIGHTNESS = 255;
const int SWEEP_SPEED = 50;

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, RING_PIN, NEO_GRB + NEO_KHZ800);


void setup() {
  strip.begin();
  strip.setBrightness(NEOPIXEL_BRIGHTNESS);
  strip.show(); // Initialize all pixels to 'off'
  pinMode(ONBOARD_BUTTON_PIN, INPUT);
  Serial.begin(9600);
}

int onboardButtonState = 0;  // current
int lastOnboardButtonState = 0;     // previous state of the button
int lightMode = 0; // 0: rainbow, 1: red circle, 2: red sweep
const int NUM_LIGHT_MODES = 3; // the total number of light states

void loop() {

  onboardButtonState = digitalRead(ONBOARD_BUTTON_PIN);

  if (onboardButtonState == HIGH ) { // button was pressed down
    lightMode = ( lightMode + 1 ) % NUM_LIGHT_MODES; // cycle through light states
  }

  if (lightMode == 0) { // rainbow mode
    uint16_t i, j;
    for(j=0; j<256; j++) { // 1 cycle of all colors on wheel
      for(i=0; i< strip.numPixels(); i++) {
       strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
      }
      // show the update
      strip.show();
 
      delay(RAINBOW_SPEED); // delay according to speed rainbow circulates around the circle
      
    }

  } else if (lightMode == 1) { // red circle mode
    uint16_t i, j;
    for(j=0; j<256; j++) { // 1 cycle of all colors on wheel
      for(i=0; i< strip.numPixels(); i++) {
       strip.setPixelColor(i, RedWheel(((i * 256 / strip.numPixels()) + j) & 255));
      }
      strip.show();
 
      delay(RED_RAINBOW_SPEED); // delay according to speed rainbow circulates around the circle
      
    }

  } else if (lightMode == 2) {
    uint16_t i, j;
    for(j=0; j<=1; j++) {
      for(i=0; i<strip.numPixels(); i++) {
        if (j == 0) {
          strip.setPixelColor(i, (0,177,255));
        } else {
          strip.setPixelColor(i, (0,0,0));
        }
        strip.show();
  
        delay(SWEEP_SPEED); // delay according to speed rainbow circulates around the circle
      }
    }

  } else {
    Serial.println("this should never happen");
  }


  lastOnboardButtonState = onboardButtonState;
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if (WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

uint32_t RedWheel(byte WheelPos) {
  if (WheelPos < 85) {
   return strip.Color(WheelPos * 3, 0, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, 0);
  } else {
   WheelPos -= 170;
   return strip.Color(0, 0, 0);
  }
}
