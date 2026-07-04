#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <avr/io.h>

namespace constants {

    enum Port : uint8_t {
        A,
        B,
        C,
        D
    };

    enum Pin : uint8_t {
        PIN_0,
        PIN_1,
        PIN_2,
        PIN_3,
        PIN_4,
        PIN_5,
        PIN_6,
        PIN_7
    };

    enum Direction : uint8_t {
        FORWARD = 'F',
        BACKWARD= 'B'
    };

    enum Orientation : uint8_t {
        LEFT = 'L',
        RIGHT = 'R',
        LEFT_TEST = 'l',
        RIGHT_TEST = 'r'
    };

    enum Cardinal : uint8_t {
        NORTH = 'N',
        EAST = 'E',
        SOUTH = 'S',
        WEST = 'O'
    };

    enum Mode : uint8_t {
        NONE,
        SEARCH_ENDPOINT,  
        CROSS_COURSE,     
        DISPLAY_REPORT    
    };

    enum Point : uint8_t {
        POINT_A = 'A',
        POINT_B = 'B',
        POINT_C = 'C',
        POINT_D = 'D',
        POINT_E = 'E'
    };

    enum Post : uint8_t {
        POST_NONE = '\0',
        POST_1 = '1',
        POST_2 = '2',
        POST_3 = '3',
        POST_4 = '4',
        POST_5 = '5',
        POST_6 = '6'
    };


}; 

using namespace constants;

#endif