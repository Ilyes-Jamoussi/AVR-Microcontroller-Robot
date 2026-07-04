/* Voltage output: the sensor output (Vo) varies with the distance to the object,
   typically from 0.25 V (at 80 cm) up to about 2.15 V (at 10 cm). */

#include "sharp.h"
#include <util/delay.h>


Sharp::Sharp(Pin pin) : pin_(pin), can_() {
    initialisation();
}

void Sharp::initialisation() {
    DDRA &= ~(1 << pin_);
}

uint8_t Sharp::getAnalogValue() {
    return (can_.lecture(pin_) >> BITSHIFT);//pin_0 de A
}

bool Sharp::isWithin10cm() {
    uint8_t analogValue = getAnalogValue();
    return analogValue >= ANALOG_VALUE_10CM;
}

bool Sharp::isWithin80cm() {
    uint8_t analogValue = getAnalogValue();
    return analogValue >= ANALOG_VALUE_80CM;
}

bool Sharp::isWithin80cm(uint16_t delayMs) {
    for (uint16_t i = 0; i < delayMs; i++) {
        if (isWithin80cm()) {
            return true;
        }
        _delay_ms(CHECK_DELAY_MS);
    }
    return false;
}

