#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include <avr/io.h>
#include <util/delay.h>

class SoundPlayer {
public:
    SoundPlayer();
    ~SoundPlayer() = default;
    void pause(uint16_t delayMs);
    void playNote(uint8_t note, uint16_t delayMs);
    void playLowestNote(uint16_t delayMs);
    void playHighestNote(uint16_t delayMs);

private:
    void init();
    void pause();
    void playNote(uint8_t note);
    static constexpr uint8_t MIN_NOTE = 45;
    static constexpr uint8_t MAX_NOTE = 81;
    static constexpr uint8_t NOTES = MAX_NOTE - MIN_NOTE + 1;

    static const uint8_t frequences[NOTES];
};

#endif