#ifndef ZIC_AUDIO_TRACK_DUMB_H_
#define ZIC_AUDIO_TRACK_DUMB_H_

#include "./zic_audio_track.h"
#include "./zic_server_def.h"

#include "./zic_PdObject.h"
#include <PdBase.hpp>

class Zic_Audio_TrackDumb : public Zic_Audio_Track {
public:
    Zic_Audio_TrackDumb(uint8_t _id = 0, const char* _name = NULL)
        : Zic_Audio_Track(_id, _name)
    {
    }

    ~Zic_Audio_TrackDumb()
    {
    }

    void noteOn(uint8_t note, uint8_t velocity, uint8_t voice)
    {
    }

    void noteOff(uint8_t note, uint8_t voice)
    {
    }

    void sample(float* buf, int len)
    {
        for (int i = 0; i < len; i++) {
            buf[i] = 0.0;
        }
    }

    void cc(uint8_t num, uint8_t val, uint8_t voice) override
    {
    }
};

#endif