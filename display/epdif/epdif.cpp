/**
 *  @filename   :   epdif.cpp
 *  @brief      :   Implements EPD interface functions
 *                  Users have to implement all the functions in epdif.cpp
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     August 10 2017
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "epdif.h"

#ifndef DISABLE_WIRING_PI
#include <wiringPi.h>
#include <wiringPiSPI.h>
#endif

EpdIf::EpdIf() {
};
EpdIf::~EpdIf() {
};

void EpdIf::DigitalWrite(int pin, int value) {
    #ifndef DISABLE_WIRING_PI
    digitalWrite(pin, value);
    #endif
}

int EpdIf::DigitalRead(int pin) {
    #ifndef DISABLE_WIRING_PI
    return digitalRead(pin);
    #else
    return 0;
    #endif
}

void EpdIf::DelayMs(unsigned int delaytime) {
    #ifndef DISABLE_WIRING_PI
    delay(delaytime);
    #endif
}

void EpdIf::SpiTransfer(unsigned char data) {
    #ifndef DISABLE_WIRING_PI
    wiringPiSPIDataRW(0, &data, 1);
    #endif
}

int EpdIf::IfInit(void) {
    #ifndef DISABLE_WIRING_PI
    if(wiringPiSetupGpio() < 0) {    // using Broadcom GPIO pin mapping
        return -1;
    }
    pinMode(RST_PIN, OUTPUT);
    pinMode(DC_PIN, OUTPUT);
    pinMode(BUSY_PIN, INPUT);
    wiringPiSPISetup(0, 2000000);
    return 0;
    #else
    return -1;
    #endif
}

