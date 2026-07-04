#include "utils.h"

namespace utils {

void delayMs(uint16_t delayMs) {
    if (delayMs > 0)
        for (uint16_t i = 0; i < delayMs; ++i)
            _delay_ms(1);
}

} 