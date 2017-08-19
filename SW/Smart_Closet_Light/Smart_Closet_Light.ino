/* 
 *  Smart Closet Light
 *  
 *  Turns on the light when the closet is open.
 *  
 *  The light is 1m long strip of WS2812 RGB LED. 
 *  Motion detector is classic PIR with BISS0001 connected to
 *  pin PB2 and generates external interrupt.
 *  
 *  Code lously based on WS2812 Breakout Example by SparkFun.
 *
 *  Pero, August 2017
*/

#include <Adafruit_NeoPixel.h>
#include "WS2812_Definitions.h"
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define PIN 4
#define LED_COUNT 30
#define boja GRAY

Adafruit_NeoPixel leds = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
  leds.begin();  // Call this to start up the LED strip.
  clearLEDs();   // This function, defined below, turns all LEDs off...
  leds.show();   // ...but the LEDs don't actually update until you call this.

  MCUCR |= 3;           //The rising edge of INT0 generates an interrupt request
  MCUCR &= ~(3 << 3);   //Set "idle" sleeping mode
  MCUCR |= (1<<SE);     // Sleep enable;
  GIMSK |= (1 << INT0); // external interrupt enable
  sei();                // enable all interrupts

}

void loop()
{
  ;
}


void LEDShow(unsigned long color, boolean direction){

    if(direction){
        for( int i = (LED_COUNT-1)/2; i >= 0; i--){
          clearLEDs();
          leds.setPixelColor(i, color);
          leds.setPixelColor(LED_COUNT - i, color);
          leds.show();
          delay(15);    
      }
    }
    else{
        for( int i = 0; i < LED_COUNT/2; i++){
          clearLEDs();
          leds.setPixelColor(i, color);
          leds.setPixelColor(LED_COUNT - i, color);
          leds.show();
          delay(15);    
      }      
    }
}

void clearLEDs(){
// Sets all LEDs to off, but DOES NOT update the display;
// call leds.show() to actually turn them off after this.
  for (int i=0; i<LED_COUNT; i++)
  {
    leds.setPixelColor(i, 0);
  }
}

void setAllLEDs(unsigned long color){
  for (int i = 0; i<LED_COUNT; i++){
      leds.setPixelColor(i, color);
      leds.show();
  }
}

ISR(INT0_vect){
  
   // First parameter is the color, second is direction, third is ms between falls
   LEDShow(boja, 1);  
   LEDShow(boja, 0);  
   setAllLEDs(boja);
   delay(5000);   
   clearLEDs();   
   leds.show();   
   sleep_cpu();
}

