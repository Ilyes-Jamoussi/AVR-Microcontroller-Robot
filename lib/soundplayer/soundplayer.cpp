#include "soundplayer.h"
#include "utils.h"

const uint8_t SoundPlayer::frequences[] = {
    141, 133, 125, 118, 111, 105, 99, 93, 88, 83, 78, 74, 70,
    66,  62,  58,  55,  52,  49,  46, 43, 41, 38, 36, 34, 32,
    30,  28,  27,  25,  24,  22,  21, 20, 18, 17, 16
    };

SoundPlayer::SoundPlayer() {
    init();
}

void SoundPlayer::init() { 
    DDRB |= (1 << PORTB2) | (1 << PORTB3); 
}

void SoundPlayer::pause() { 
    TCCR0A &= ~(1 << COM0A0); 
}

void SoundPlayer::pause(uint16_t delayMs) {
    pause();
    utils::delayMs(delayMs);
}

void SoundPlayer::playNote(uint8_t note) {
    pause();
    OCR0A  = frequences[note - MIN_NOTE]; 
    TCCR0A = (1 << WGM01) | (1 << COM0A0);
    TCCR0B = (1 << CS02);
}

void SoundPlayer::playNote(uint8_t note, uint16_t delayMs) {
    playNote(note);
    utils::delayMs(delayMs);
    pause();
}

void SoundPlayer::playLowestNote(uint16_t delayMs) {
    playNote(MIN_NOTE, delayMs);
}

void SoundPlayer::playHighestNote(uint16_t delayMs) {
    playNote(MAX_NOTE, delayMs);
}

