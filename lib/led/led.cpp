#include "led.h"
#include "utils.h"

Led::Led(Port port, Pin pin1, Pin pin2)
    : port_(port), pin1_(pin1), pin2_(pin2) {
    configurePort(port_);

    *ddr_ |= (1 << pin1_) | (1 << pin2_);
}

void Led::showGreen(uint16_t delayMs) {
    showGreen();
    utils::delayMs(delayMs);
    showOff();
    
}

void Led::showRed(uint16_t delayMs) {
    showRed();
    utils::delayMs(delayMs);
    showOff();
} 

void Led::showAmber(uint16_t delayMs) {
    while (delayMs--) {
        showGreen();
        _delay_us(500);
        showRed();
        _delay_us(500);
    }
    showOff();
}
 
void Led::showOff(uint16_t delayMs) {
    showOff();
    utils::delayMs(delayMs);
}

void Led::blinkGreen(uint16_t delayMs) {
    while (delayMs) {
        showGreen(125);
        showOff(125);
        delayMs -= 250;
    }
}

void Led::blinkRed(uint16_t delayMs) {
    while (delayMs) {
        showRed(125);
        showOff(125);
        delayMs -= 250;
    }
}

void Led::blinkAmber(uint16_t delayMs) {
    while (delayMs) {
        showAmber(125);
        showOff(125);
        delayMs -= 250;
    }
}


void Led::configurePort(Port port){
        switch (port) {
    case A:
        ddr_ = &DDRA;
        por_ = &PORTA;
        break;
    case B:
        ddr_ = &DDRB;
        por_ = &PORTB;
        break;
    case C:
        ddr_ = &DDRC;
        por_ = &PORTC;
        break;
    case D:
        ddr_ = &DDRD;
        por_ = &PORTD;
        break;
    }
}

void Led::showGreen() {
    *por_ &= ~(1 << pin1_);
    *por_ |= (1 << pin2_);
}

void Led::showRed() {
    *por_ |= (1 << pin1_);
    *por_ &= ~(1 << pin2_);
}

void Led::showOff() {
    *por_ &= ~((1 << pin1_) | (1 << pin2_));
}

