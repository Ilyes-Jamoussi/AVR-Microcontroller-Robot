#ifndef BUTTON_H
#define BUTTON_H

#include <avr/io.h>

class Button {
public:
    Button();
    ~Button() = default;

    bool isPressed();
    bool isClicked();
    void waitForClick();
    uint8_t getClickCounter() const;

private:
    static constexpr uint8_t DEBOUNCE_DELAY_MS = 10;
    static constexpr uint8_t INITIAL_CLICK_COUNTER = 0;

    uint8_t clickCounter_; 

    void init();
    bool isSignalActive();
};

#endif 