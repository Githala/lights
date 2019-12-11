#include "light_control.h"

#define LED_PIN    12
#define LED_COUNT  36

unsigned long timer1 = 0;
unsigned long timer2 = 0;
CRGB leds[LED_COUNT];
// Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void LightControl::init() {
    FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, LED_COUNT);
}

void LightControl::setMode(LightMode mode) {
    this->mode = mode;
}

void LightControl::step() {
    if(millis()-timer1 > 10) {
        timer1 = millis();
        switch (LightControl::mode)
        {
            case SOLID:
                for(int i=0; i<LED_COUNT; i++) {
                    leds[i] = CRGB(this->red, this->green, this->blue);
                }
                FastLED.show();
                break;
            case BLINK:
                stepBlink();
                break;
            case RAINBOW:
                stepRainbow();
                break;
            case OFF:
            default:
                for(int i=0; i<LED_COUNT; i++) {
                    leds[i] = CRGB::Black;
                }
                FastLED.show();
                break;
        }
    }
}

void LightControl::setRGB(int r, int g, int b) {
    this->red = r;
    this->green = g;
    this->blue = b;
}

void LightControl::stepRainbow() {    
    firstPixelHSV += 1;
    if(firstPixelHSV > 256) {
        firstPixelHSV = 0;
    }

    for(int i=0; i<LED_COUNT; i++) {
        int pixelHue = firstPixelHSV + (i * 255 / LED_COUNT);
        leds[i].setHue(pixelHue);
    }
    FastLED.show();
}

void LightControl::stepBlink() {
    if(millis()-timer2 > BLINK_DELAY) {
        timer2 = millis();
        lightOn=!lightOn;
        if(lightOn) {
            for(int i=0; i<LED_COUNT; i++) {
                leds[i] = CRGB(this->red, this->green, this->blue);
            }
            FastLED.show();
        } else {
            for(int i=0; i<LED_COUNT; i++) {
                leds[i] = CRGB::Black;
            }
            FastLED.show();
        }
    }
}