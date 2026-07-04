#include "motor.h"
#include <util/delay.h>


Motor::Motor(){ 
    init();
}

void Motor::move(uint8_t speed, Direction direction) {
    setDirection(direction);
    setSpeed(speed, speed);
}

void Motor::adjust(uint8_t speed, Orientation orientation, Direction direction) {
    setDirection(direction);
    switch (orientation) {
        case RIGHT:
        case RIGHT_TEST:
            setSpeed(speed, speed - SPEED_OFFSET);
            break;
        case LEFT: 
        case LEFT_TEST:
            setSpeed(speed - SPEED_OFFSET, speed);
            break;
    }

}

void Motor::turn(uint8_t speed, Orientation orientation, Direction direction) {
    setDirection(direction);
    switch (orientation) {
        case RIGHT:
        case RIGHT_TEST:
            setSpeed(MAX_SPEED, NO_SPEED);
            _delay_ms(BOOST_DELAY_MS);
            setSpeed(speed, NO_SPEED);
            break;
        case LEFT: 
        case LEFT_TEST:
            setSpeed(NO_SPEED, MAX_SPEED);
            _delay_ms(BOOST_DELAY_MS);
            setSpeed(NO_SPEED, speed);
            break;
    }
}

void Motor::pivot(uint8_t speed, Orientation orientation) {;
    switch (orientation) {
        case RIGHT:
        case RIGHT_TEST:
            PORTD &= ~(1 << leftDirection_);  
            PORTD |= (1 << rightDirection_);
            break;
        case LEFT: 
        case LEFT_TEST:
            PORTD |= (1 << leftDirection_);  
            PORTD &= ~(1 << rightDirection_); 
            break;
    }
    setSpeed(MAX_SPEED, MAX_SPEED);
    _delay_ms(BOOST_DELAY_MS);
    setSpeed(speed, speed);
}

void Motor::stop() {
    setSpeed(NO_SPEED, NO_SPEED);  
}

void Motor::stabilize() {
    stop();
    _delay_ms(STABILITY_DELAY_MS);
}

void Motor::init() {
    DDRD |= (1 << rightWheel_) | (1 << leftWheel_) | (1 << rightDirection_) | (1 << leftDirection_);
    TCNT2 = 0;
    TCCR2A |= (1 << COM2A1) | (1 << COM2B1) | (1 << WGM20);       
    TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);
    OCR2A = NO_SPEED;                    
    OCR2B = NO_SPEED; 
    }

void Motor::setDirection(Direction direction) {
    switch (direction) {
        case FORWARD:
            PORTD &= ~((1 << leftDirection_) | (1 << rightDirection_));
            break;
        case BACKWARD:
            PORTD |= (1 << leftDirection_) | (1 << rightDirection_);
            break;
    }
}

void Motor::setSpeed(uint8_t leftWeelSpeed, uint8_t rightWeelSpeed) {
    OCR2A = rightWeelSpeed;
    OCR2B = leftWeelSpeed;
}