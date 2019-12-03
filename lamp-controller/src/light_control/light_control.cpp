#include "light_control.h"

#define LED_PIN    12
#define LED_COUNT  36

unsigned long timer = 0;
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void LightControl::init() {
    strip.begin();
    strip.show();
    strip.setBrightness(50);
}

void LightControl::setMode(LightMode mode) {
    this->mode = mode;
}

void LightControl::step() {
    switch (LightControl::mode)
    {
        case SOLID:
            for(int i=0; i<strip.numPixels(); i++) {
                strip.setPixelColor(i, strip.Color(this->red, this->green, this->blue));
            }
            strip.show();
            break;
        case BLINK:
            stepBlink();
            break;
        case RAINBOW:
            stepRainbow();
            break;
        case OFF:
        default:
            strip.clear();
            strip.show();
            break;
    }
}

void LightControl::setRGB(int r, int g, int b) {
    this->red = r;
    this->green = g;
    this->blue = b;
}

void LightControl::stepRainbow() {
    if(millis()-timer > 10) {
        timer = millis();
        firstPixelHue += 256;
        if(firstPixelHue > 5*65536) {
            firstPixelHue = 0;
        }

        for(int i=0; i<strip.numPixels(); i++) {
            int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
            strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
        }
        strip.show();
    }
}

void LightControl::stepBlink() {
    if(millis()-timer > BLINK_DELAY) {
        timer = millis();
        lightOn=!lightOn;
        if(lightOn) {
            for(int i=0; i<strip.numPixels(); i++) {
                strip.setPixelColor(i, strip.Color(this->red, this->green, this->blue));
            }
            strip.show();
        } else {
            strip.clear();
            strip.show();
        }
    }
}