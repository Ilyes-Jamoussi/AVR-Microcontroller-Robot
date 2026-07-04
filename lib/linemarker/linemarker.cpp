#include "linemarker.h"
#include "constants.h"
#include <util/delay.h>

LineMarker::LineMarker() {
    initializePins();
}

void LineMarker::initializePins() {
    DDRA &= ~((1 << PA1) | (1 << PA2) | (1 << PA3) | (1 << PA4) | (1 << PA5));
}

bool LineMarker::isFarRightActive() const {
    return (PINA & (1 << PIN_5));
}

bool LineMarker::isRightActive() const {
    return (PINA & (1 << PIN_4));
}

bool LineMarker::isCenterActive() const {
    return (PINA & (1 << PIN_3));
}

bool LineMarker::isLeftActive() const {
    return (PINA & (1 << PIN_2));
}

bool LineMarker::isFarLeftActive() const {
    return (PINA & (1 << PIN_1));
}

bool LineMarker::isLineFound() const {
    return (isFarRightActive() || isRightActive() || isCenterActive() || isLeftActive() || isFarLeftActive());
}

bool LineMarker::areAllSensorsActive() const {
    return (isFarRightActive() && isRightActive() && isCenterActive() && isLeftActive() && isFarLeftActive());
}

bool LineMarker::isInLine(uint16_t delayMs) const {
    for (uint16_t i = 0; i < delayMs; i++) {
        if (isLineFound()) {
            return true; 
        }
        _delay_ms(CHECK_DELAY_MS);
    }
    return false;
} 

bool LineMarker::isAligned() const {
    if (isCenterActiveOnly()) {
        _delay_ms(100);
        return isCenterActiveOnly();
    }
    return false;
}

bool LineMarker::isCenterActiveOnly() const {
    return (!isFarLeftActive() && !isLeftActive() &&isCenterActive() && !isRightActive() && !isFarRightActive());
}