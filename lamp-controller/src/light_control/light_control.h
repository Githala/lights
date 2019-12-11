#include <Arduino.h>
#include <FastLED.h>
#include "light_mode.h"

#define ON_SIGNAL 0x0
#define OFF_SIGNAL 0x1
#define BLINK_DELAY 1000

class LightControl {

    LightMode mode;
    boolean lightOn=false;

    int firstPixelHSV = 0;

    int red = 0, green = 0, blue = 0;

    public:
        void init();
        void setMode(LightMode);
        void step();
        void setRGB(int r, int g, int b);

    private:
        void stepBlink();
        void stepRainbow();
};