#ifndef LINEMARKER_H
#define LINEMARKER_H

#include "constants.h"
#include <avr/io.h>

class LineMarker {
public:

    LineMarker();
    ~LineMarker() = default;

    bool isFarRightActive() const;  
    bool isRightActive() const;     
    bool isCenterActive() const;     
    bool isLeftActive() const;      
    bool isFarLeftActive() const;    
    bool isLineFound() const; 
    bool areAllSensorsActive() const; 
    bool isCenterActiveOnly() const;
    bool isInLine(uint16_t delayMS) const;
    bool isAligned() const;      
    

private:
    static constexpr uint8_t CHECK_DELAY_MS = 1;

    void initializePins();          
};

#endif