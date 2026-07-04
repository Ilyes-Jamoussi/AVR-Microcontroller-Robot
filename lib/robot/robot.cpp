#include "robot.h"

Robot::Robot()
    : memory_(),
      rs232_(),
      button_(),
      led_(C, PIN_6, PIN_7),
      motor_(),
      soundPlayer_(),
      lineMarker_(),
      sharp_(PIN_0),
      path_(), 
      memoryAddress_(MODE_ADDRESS),
      selectedMode_(NONE),
      postCount_(INITIAL_POST_COUNT),
      isAtEndPoint_(false) {} 

void Robot::start() {
    selectMode();
    executeMode();
}

void Robot::selectMode() {
    memory_.lecture(memoryAddress_, &selectedMode_);

    if (selectedMode_ == NONE) 
        while (true) {
            selectSearchEndpoint();
            selectCrossCourse();
            selectDisplayReport();
        } 
}

void Robot::selectSearchEndpoint() {
    memory_.ecriture(memoryAddress_, SEARCH_ENDPOINT);
    led_.showGreen(LED_DISPLAY_MS);
}

void Robot::selectCrossCourse() {
    memory_.ecriture(memoryAddress_, CROSS_COURSE);
    led_.showRed(LED_DISPLAY_MS);
}

void Robot::selectDisplayReport() {
    memory_.ecriture(memoryAddress_, DISPLAY_REPORT);
    led_.showAmber(LED_DISPLAY_MS);
}

void Robot::executeMode() {
    memory_.lecture(memoryAddress_, &selectedMode_);

    switch (selectedMode_) {
        case SEARCH_ENDPOINT:
            executeSearchEndpoint();
            break;
        case CROSS_COURSE:
            executeCrossCourse();
            break;
        case DISPLAY_REPORT:
            executeDisplayReport();
            break;   
    }
}

void Robot::executeSearchEndpoint() {
    memory_.ecriture(memoryAddress_, NONE);
    led_.blinkGreen(LED_DISPLAY_MS);
    
    searchEndpoint();
}

void Robot::executeCrossCourse() {
    memory_.ecriture(memoryAddress_, NONE);
    led_.blinkRed(LED_DISPLAY_MS);

    crossCourse();
}

void Robot::executeDisplayReport() {
    memory_.ecriture(memoryAddress_, NONE);
    led_.blinkAmber(LED_DISPLAY_MS);

    displayReport();
}

void Robot::searchEndpoint() {
    while(!isAtEndPoint_) {
    align();
    traceLine();
    turnOnLine();
    }
}

void Robot::align() {
    while (!lineMarker_.isCenterActiveOnly() && lineMarker_.isLineFound()) {
        if(lineMarker_.isFarLeftActive() || lineMarker_.isLeftActive()) {
            motor_.adjust(ALIGN_SPEED, LEFT);
            while(lineMarker_.isFarLeftActive() || lineMarker_.isLeftActive());
        }
        else if (lineMarker_.isFarRightActive() || lineMarker_.isRightActive()) { 
            motor_.adjust(ALIGN_SPEED, RIGHT);
            while(lineMarker_.isFarRightActive() || lineMarker_.isRightActive());
        }
    }
}

void Robot::traceLine() {
    path_.addLetter(FORWARD);

    while (!lineMarker_.isFarLeftActive() && !lineMarker_.isFarRightActive() && lineMarker_.isLineFound()) {
        if(lineMarker_.isLeftActive()) {
            motor_.adjust(NORMAL_SPEED, LEFT);
            while(lineMarker_.isLeftActive() && !lineMarker_.isFarLeftActive() && !lineMarker_.isFarRightActive());
        }
        else if (lineMarker_.isRightActive()) { 
            motor_.adjust(NORMAL_SPEED, RIGHT);
            while(lineMarker_.isRightActive() && !lineMarker_.isFarLeftActive() && !lineMarker_.isFarRightActive());
            }
        else if (lineMarker_.isCenterActive()) { 
            motor_.move(NORMAL_SPEED);
        }
    }
    motor_.move(NORMAL_SPEED);
}

void Robot::turnOnLine() {

    if (lineMarker_.isFarLeftActive()) {
        _delay_ms(LINE_CROSS_DELAY_MS);
        if (lineMarker_.isLineFound()) {
            _delay_ms(POSITIONNING_DELAY_MS);
            turn(NORMAL_SPEED, LEFT);
            if (!lineMarker_.isLineFound()) {
            motor_.stabilize();
            motor_.pivot(REDUCED_SPEED, RIGHT);
            while (!lineMarker_.isLineFound());
            }
        }
        else checkForEndpoint();             
    }
    else if (lineMarker_.isFarRightActive()) {
        _delay_ms(LINE_CROSS_DELAY_MS);
        if (lineMarker_.isLineFound()){
            _delay_ms(POSITIONNING_DELAY_MS);
            turn(NORMAL_SPEED, RIGHT);
        }
        else checkForEndpoint();            
    }
}

void Robot::turn(uint8_t speed, Orientation orientation) {
    path_.addLetter(orientation);
    motor_.stabilize();

    if (orientation == LEFT_TEST || orientation == RIGHT_TEST) {
        motor_.turn(speed, orientation);
        lineMarker_.isInLine(TESTING_DELAY_MS);
        if (!lineMarker_.isLineFound()) {
            motor_.stabilize();
            motor_.turn(speed, orientation, BACKWARD);
            lineMarker_.isInLine(TESTING_DELAY_MS);
        } 
    } 
    else {
        motor_.pivot(speed, orientation);
        _delay_ms(MIN_TURNING_DELAY_MS);
        lineMarker_.isInLine(QUARTER_TURN_DELAY_MS);
    }    
    motor_.stabilize();
}

void Robot::checkForEndpoint() {
    _delay_ms(TEST_POSITIONNING_DELAY_MS);
    turn(NORMAL_SPEED, LEFT_TEST); 
    if (lineMarker_.isLineFound()) return;
    
    turn(NORMAL_SPEED, RIGHT_TEST);
    if (lineMarker_.isLineFound()) return;

    isAtEndPoint_ = true;
    const uint8_t* pathMetadata = path_.compareEndPoints();
    const uint8_t SIZE = 4;
    memory_.ecriture(REPORT_DATA_ADDRESS, const_cast<uint8_t*>(pathMetadata), SIZE);

     motor_.move(NORMAL_SPEED, BACKWARD);
    _delay_ms(ENDPOINT_RETURN_DELAY_MS);
    motor_.stabilize();
    playEndPointSound();

    const uint8_t END_POINT_INDEX = 3;
    switch (pathMetadata[END_POINT_INDEX]) {
        case POINT_C:
            led_.showGreen(LED_DISPLAY_MS);
            break;        
        case POINT_D:
            led_.showRed(LED_DISPLAY_MS);
            break;
        case POINT_E:
            led_.showAmber(LED_DISPLAY_MS);
            break;
    }
}

void Robot::playEndPointSound(){
    uint8_t HIGH_SOUNDS = 2;
    for (uint8_t i = 0; i < HIGH_SOUNDS; i++) {
    soundPlayer_.playHighestNote(SOUND_END_POINT_DELAY_MS);
    soundPlayer_.pause(SOUND_PAUSE_DELAY_MS);
    }
}

void Robot::crossCourse() {
    button_.waitForClick();
    _delay_ms(COURSE_START_DELAY_MS);
    memoryAddress_ = COURSE_DATA_ADDRESS;
    traceLine();
    getPostInfo();
    goToPost();
    while (!isAtLastPost()) {
        _delay_ms(COURSE_START_DELAY_MS);
        traceLine();
        turnOnLine();
        align();
        traceLine();
        getPostInfo();
        goToPost();
    }
}

void Robot::getPostInfo() {
    uint8_t post = (lineMarker_.isFarLeftActive()) ? getLeftPost() : getRightPost();
    memory_.ecriture(memoryAddress_++, post);
}

uint8_t Robot::getLeftPost() {
    switch (button_.getClickCounter()) {
        case 1: return POINT_C;
        case 2: return POST_1;
        case 3: return POST_3;
        case 4: return POST_5;
    }
    return POST_NONE;
}

uint8_t Robot::getRightPost() {
    switch (button_.getClickCounter()) {
        case 1: return POINT_D;
        case 2: return POST_2;
        case 3: return POST_4;
        case 4: return POST_6;
    }
    return POST_NONE;
}

void Robot::goToPost() {
    if (lineMarker_.isFarLeftActive()) {
        _delay_ms(COURSE_LINE_CROSS_DELAY_MS);
        align();
        motor_.stabilize();
        if (sharp_.isWithin80cm()) traceLineUntil10cm();
        else {
            turn(NORMAL_SPEED, LEFT);
 
            
            align();
            traceLineUntil10cm();
        }
    }
    else {
        _delay_ms(COURSE_LINE_CROSS_DELAY_MS);
        align();
        motor_.stabilize();
        if (sharp_.isWithin80cm()) traceLineUntil10cm();
        else {
            turn(NORMAL_SPEED, RIGHT);
            align();
            traceLineUntil10cm();
        }
    }
}

void Robot::traceLineUntil10cm() {
    while (!sharp_.isWithin10cm()) {
        if(lineMarker_.isLeftActive()) {
            motor_.adjust(NORMAL_SPEED, LEFT);
            while(lineMarker_.isLeftActive() && !sharp_.isWithin10cm());
        }
        else if (lineMarker_.isRightActive()) { 
            motor_.adjust(NORMAL_SPEED, RIGHT);
            while(lineMarker_.isRightActive() && !sharp_.isWithin10cm());
            }
        else if (lineMarker_.isCenterActive()) { 
            motor_.move(NORMAL_SPEED, FORWARD);
        }
    }
    motor_.stop();
}

bool Robot::isAtLastPost() {
    button_.waitForClick();
    if (button_.getClickCounter() == MAX_BUTTON_CLICKS) {
        _delay_ms(COURSE_START_DELAY_MS);
        traceLine();
        turnOnLine();
        align();
        traceLine();
        getPostInfo();
        turnOnLine();
        align();
        traceLine();
        motor_.stabilize();
        playEndPointSound();
        return true;
    }
    return false;
}

void Robot::playCrossCourseSound(){
    uint8_t LOW_SOUNDS = 8;
    for (uint8_t i = 0; i < LOW_SOUNDS; i++) {
        soundPlayer_.playLowestNote(COURSE_SOUND_DELAY_MS);
        soundPlayer_.pause(SOUND_PAUSE_DELAY_MS);
    }
}

void Robot::displayReport() {
    const uint8_t DATA_SIZE = 8;          
    uint8_t reportData[DATA_SIZE];

    memory_.lecture(REPORT_DATA_ADDRESS, reportData, DATA_SIZE);
    uint8_t index = 0;

    rs232_.transmit("Endpoint identification\n");
    rs232_.transmit("-----------------------\n");
    rs232_.transmitFormatted("Start point: %c\n", reportData[index++]);
    uint8_t firstCardinal = reportData[index++];
    uint8_t secondCardinal = reportData[index++];
    rs232_.transmitFormatted("Start orientation: %c%c\n", firstCardinal, secondCardinal);
    rs232_.transmitFormatted("Endpoint found: %c\n", reportData[index++]);
    rs232_.transmit("\n");

    rs232_.transmit("Course traversal\n");
    rs232_.transmit("----------------\n");
    rs232_.transmitFormatted("Start point: %c\n", reportData[index++]);
    rs232_.transmitFormatted("Post 1 location: %c\n", reportData[index++]);
    rs232_.transmitFormatted("Post 2 location: %c\n", reportData[index++]);
    rs232_.transmitFormatted("Post 3 location: %c\n", reportData[index]);
    rs232_.transmit("\n");

    rs232_.transmit("End of report\n");
}










