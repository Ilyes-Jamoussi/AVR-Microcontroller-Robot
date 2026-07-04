#ifndef MOTOR_H
#define MOTOR_H

#include "constants.h"
#include <avr/io.h>

class Motor {
  public:
    Motor();
    ~Motor() = default;

    void move(uint8_t speed, Direction direction = FORWARD);
    void adjust(uint8_t speed, Orientation orientation, Direction direction = FORWARD);
    void turn(uint8_t speed, Orientation Orientation, Direction direction = FORWARD);
    void pivot(uint8_t speed, Orientation orientation);
    void stop();
    void stabilize();

  private:

    static constexpr uint8_t SPEED_OFFSET = 20;
    static constexpr uint8_t NO_SPEED = 0;
    static constexpr uint8_t MAX_SPEED = 255;
    static constexpr uint8_t BOOST_DELAY_MS = 50;
    static constexpr uint16_t STABILITY_DELAY_MS = 500;

    const Pin rightWheel_ = PIN_7;
    const Pin rightDirection_ = PIN_5;
    const Pin leftWheel_ = PIN_6;
    const Pin leftDirection_ = PIN_4;

    void init();
    void setDirection(Direction direction);
    void setSpeed(uint8_t valueA, uint8_t valueB);

};

#endif