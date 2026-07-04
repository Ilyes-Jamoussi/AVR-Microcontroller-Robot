#ifndef SHARP_H
#define SHARP_H

#include <avr/io.h>
#include "can.h"
#include "constants.h"



class Sharp
{
public:
    Sharp(Pin pin);
    ~Sharp() = default;

    bool isWithin10cm();
    bool isWithin80cm();
    bool isWithin80cm(uint16_t delayMs);


private:
    Pin pin_;
    can can_;

    static constexpr uint8_t BITSHIFT = 2;
    static constexpr uint8_t ANALOG_VALUE_10CM = 110;
    static constexpr uint8_t ANALOG_VALUE_80CM = 20;
    static constexpr uint8_t CHECK_DELAY_MS = 1;


    void initialisation();
    uint8_t getAnalogValue();


};

#endif
