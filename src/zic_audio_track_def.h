#ifndef ZIC_AUDIO_TRACK_DEF_H_
#define ZIC_AUDIO_TRACK_DEF_H_

#include <stdint.h>

#define PATTERN_COUNT 255 // uint8 -> 2 hex

enum {
    TRACK_1,
    TRACK_2,
    TRACK_3,
    TRACK_4,
    MIDI_TRACK_1,
    MIDI_TRACK_2,
    MIDI_TRACK_3,
    MIDI_TRACK_4,
    TRACK_COUNT
};

enum {
    VOICE_1,
    VOICE_2,
    VOICE_3,
    VOICE_4,
    VOICE_COUNT
};

#endif