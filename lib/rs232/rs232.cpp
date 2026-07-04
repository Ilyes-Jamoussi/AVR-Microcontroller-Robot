#include "rs232.h"
#include <stdarg.h>
#include <stdio.h>

RS232::RS232() {
    init();
}

void RS232::transmit (char data) {
    while ( !( UCSR0A & (1<<UDRE0)) );    
    UDR0 = data;   

}

void RS232::transmit(const char* data) {
    while (*data != '\0') {
        transmit(*data); 
        data++;
    }
}

void RS232::transmitFormatted(const char* format, ...) {
    char buffer[BUFFER_SIZE];                         // Temporary buffer for the formatted string
    va_list args;

    va_start(args, format);                          // Initialize variable arguments
    vsnprintf(buffer, sizeof(buffer), format, args); // Format the string
    va_end(args);                                    // Clean up the variable argument list

    transmit(buffer); 
}

uint8_t RS232::receive() {
    while (!(UCSR0A & (1 << RXC0)));      
    return UDR0;   

}

void RS232::init() {
    UBRR0H = 0;
    UBRR0L = UBRR_VALUE;
    UCSR0A = 0 ;
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}