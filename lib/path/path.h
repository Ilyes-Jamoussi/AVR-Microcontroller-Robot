#ifndef PATH_H
#define PATH_H

#include <avr/io.h>
#include "constants.h"

class Path {
public:
    Path();
    ~Path() = default;

    void addLetter(uint8_t letter);
    void fillRemaining();
    const uint8_t* compareEndPoints();
    

private:
    static constexpr uint8_t INITIAL_PATH_LENGTH = 0;
    static constexpr uint8_t END_POINT_PATHS = 8;       
    static constexpr uint8_t PATH_LENGTH = 15;          
    static constexpr uint8_t END_POINT_DATA_SIZE = 4;         
    static constexpr uint8_t DEFAULT_VALUE = 0;
    static constexpr uint8_t FILLER = '-';

    static const uint8_t endPointPaths[END_POINT_PATHS][PATH_LENGTH];
    static const uint8_t endPointData[END_POINT_PATHS][END_POINT_DATA_SIZE];

    uint8_t path_[PATH_LENGTH];
    uint8_t currentLength_;
};

#endif