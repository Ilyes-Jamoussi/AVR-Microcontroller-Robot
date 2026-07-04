#ifndef RS232_H
#define RS232_H

#include <avr/io.h>

class RS232 {
public:
    RS232();  
    ~RS232() = default;  

    void transmit(char data); 
    void transmit(const char* data);
    void transmitFormatted(const char* format, ...);
    uint8_t receive(); 
  
private:
    void init();  

    static constexpr uint16_t UBRR_VALUE = 0xCF;
    static constexpr uint8_t BUFFER_SIZE = 128;

};

#endif 