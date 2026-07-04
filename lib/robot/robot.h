#ifndef ROBOT_H
#define ROBOT_H

#include <avr/io.h>
#include <util/delay.h>

#include "constants.h"
#include "memoire_24.h"
#include "rs232.h"
#include "button.h"
#include "led.h"
#include "motor.h"
#include "soundplayer.h"
#include "linemarker.h"
#include "sharp.h"
#include "path.h"

class Robot {
public:
    Robot();
    ~Robot() = default;

    void start();
   
private:
    static constexpr uint8_t MODE_ADDRESS = 0;
    static constexpr uint8_t REPORT_DATA_ADDRESS = 1;
    static constexpr uint8_t COURSE_DATA_ADDRESS = 5;

    static constexpr uint8_t NORMAL_SPEED = 100;
    static constexpr uint8_t REDUCED_SPEED = 88;
    static constexpr uint8_t ALIGN_SPEED = 80;

    static constexpr uint16_t LED_DISPLAY_MS = 2000; 
    static constexpr uint16_t QUARTER_TURN_DELAY_MS = 1500;
    static constexpr uint16_t TESTING_DELAY_MS = 3000;
    static constexpr uint16_t LINE_CROSS_DELAY_MS = 600;
    static constexpr uint16_t POSITIONNING_DELAY_MS = 950;
    static constexpr uint16_t TEST_POSITIONNING_DELAY_MS = 900;
    static constexpr uint16_t MIN_TURNING_DELAY_MS = 1200;
    static constexpr uint16_t ENDPOINT_RETURN_DELAY_MS = 2200;
    static constexpr uint16_t COURSE_LINE_CROSS_DELAY_MS = 1100;
    static constexpr uint16_t SOUND_END_POINT_DELAY_MS = 750;
    static constexpr uint16_t COURSE_START_DELAY_MS = 2000;
    static constexpr uint8_t COURSE_SOUND_DELAY_MS = 150;
    static constexpr uint8_t SOUND_PAUSE_DELAY_MS= 50;

    static constexpr uint8_t DATA_SIZE = 8;  
    static constexpr uint8_t MAX_BUTTON_CLICKS = 4;
    static constexpr uint8_t INITIAL_POST_COUNT = 0;

    void selectMode();
    void selectSearchEndpoint();
    void selectCrossCourse();
    void selectDisplayReport();

    void executeMode();   
    void executeSearchEndpoint();
    void executeCrossCourse();
    void executeDisplayReport();

    void searchEndpoint();
    void align();
    void traceLine();
    void turnOnLine();
    void turn(uint8_t speed, Orientation orientation);
    void checkForEndpoint();
    void playEndPointSound();

    void crossCourse();
    void getPostInfo();  
    uint8_t getLeftPost();
    uint8_t getRightPost();
    void goToPost();
    void traceLineUntil10cm();
    bool isAtLastPost();
    void playCrossCourseSound();

    void displayReport();

    Memoire24CXXX memory_;
    RS232 rs232_;
    Button button_;
    Led led_;
    Motor motor_;
    SoundPlayer soundPlayer_;
    LineMarker lineMarker_;
    Sharp sharp_;
    Path path_; 

    uint8_t memoryAddress_;
    uint8_t selectedMode_;
    uint8_t postCount_;
    bool isAtEndPoint_; 
};

#endif
