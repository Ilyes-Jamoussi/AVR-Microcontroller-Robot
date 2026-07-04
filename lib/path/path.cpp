#include "path.h"
#include "rs232.h"
#include <util/delay.h>
#include <string.h>

const uint8_t Path::endPointPaths[END_POINT_PATHS][PATH_LENGTH] = {
    {FORWARD, LEFT_TEST, RIGHT_TEST, FORWARD, LEFT, FORWARD, LEFT_TEST, RIGHT_TEST, FORWARD, LEFT, FORWARD, LEFT_TEST, RIGHT_TEST},
    {FORWARD, LEFT_TEST, FORWARD, RIGHT, FORWARD, LEFT_TEST, RIGHT_TEST},
    {FORWARD, LEFT_TEST, FORWARD, RIGHT, FORWARD, LEFT, FORWARD, RIGHT, FORWARD, LEFT_TEST, RIGHT_TEST},
    {FORWARD, RIGHT, FORWARD, LEFT, FORWARD, LEFT_TEST, RIGHT_TEST},

    {FORWARD, LEFT_TEST, RIGHT_TEST, FORWARD, LEFT, FORWARD, LEFT_TEST, RIGHT_TEST},
    {FORWARD, LEFT, FORWARD, RIGHT, FORWARD, LEFT_TEST, FORWARD, RIGHT, FORWARD, LEFT_TEST, RIGHT_TEST},
    {FORWARD, LEFT, FORWARD, RIGHT, FORWARD, LEFT_TEST, RIGHT_TEST},
    {FORWARD, RIGHT, FORWARD, LEFT, FORWARD, RIGHT, FORWARD, LEFT, FORWARD},
};

const uint8_t Path::endPointData[END_POINT_PATHS][END_POINT_DATA_SIZE] = {
    {POINT_A, NORTH, EAST, POINT_E},
    {POINT_A, NORTH, WEST, POINT_C},
    {POINT_A, SOUTH, EAST, POINT_E},
    {POINT_A, SOUTH, WEST, POINT_D},

    {POINT_B, NORTH, EAST, POINT_E},
    {POINT_B, NORTH, WEST, POINT_C},
    {POINT_B, SOUTH, EAST, POINT_E},
    {POINT_B, SOUTH, WEST, POINT_D},
};

Path::Path() : currentLength_(INITIAL_PATH_LENGTH) {
    memset(path_, DEFAULT_VALUE, PATH_LENGTH); 
}

void Path::addLetter(uint8_t letter) {
    if (currentLength_ < PATH_LENGTH) path_[currentLength_++] = letter;
    RS232 rs232;
    for (uint8_t j = 0; j < currentLength_; ++j) {
        rs232.transmit(path_[j]);
        _delay_ms(30);
    }
    rs232.transmit('\n');

}

void Path::fillRemaining() {
    for (uint8_t i = currentLength_; i < PATH_LENGTH; ++i) {
        path_[i] = FILLER; 
    }
}

const uint8_t* Path::compareEndPoints() {
    for (uint8_t i = 0; i < END_POINT_PATHS; ++i) {
        if (memcmp(path_, endPointPaths[i], PATH_LENGTH) == 0) {
            return endPointData[i]; 
        }
    }
    return nullptr; 
}
