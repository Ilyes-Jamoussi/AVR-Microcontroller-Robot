#include "button.h"
#include <util/delay.h>

Button::Button() : clickCounter_(INITIAL_CLICK_COUNTER) {
    init();
}

bool Button::isPressed() { 
    if (isSignalActive()) {
        _delay_ms(DEBOUNCE_DELAY_MS);
        return isSignalActive();
    }
    return false;
}

bool Button::isClicked() { 
    if (isPressed()) {
        while (isPressed()) {}
        return true;
    }
    return false;
}

void Button::waitForClick() {
    while (!isClicked());
    clickCounter_++;
}

uint8_t Button::getClickCounter() const {
    return clickCounter_;
}

void Button::init() {
    DDRD &= ~(1 << DDD2);
}

bool Button::isSignalActive() {  
    return (PIND & (1 << PD2));
}

