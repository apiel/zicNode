#ifndef ZIC_AUDIO_TRACK_KICK23_H_
#define ZIC_AUDIO_TRACK_KICK23_H_

#include <zic_drum_kick23.h>

#include "./zic_audio_track.h"

// #include <zic_note.h>

class Zic_Audio_TrackKick23 : public Zic_Audio_Track {
public:
    Zic_Drum_Kick23 kick;

    Zic_Audio_TrackKick23(uint8_t _id = 0, const char* _name = NULL)
        : Zic_Audio_Track(_id, _name)
    {
    }

    void noteOn(uint8_t note, uint8_t velocity, uint8_t voice)
    {
        kick.noteOn();
    }

    void noteOff(uint8_t note, uint8_t voice)
    {
    }

    void sample(float* buf, int len)
    {
        for (int i = 0; i < len; i++) {
            buf[i] = kick.sample();
#if APP_CHANNELS == 2
            // TODO
            // or not ?? if sample is mono, then it's ok
            // but if it's stereo, then it's not :p
            buf[i + 1] = buf[i];
            i++;
#endif
        }
    }

    void cc(uint8_t num, uint8_t val, uint8_t voice) override
    {
        // if (num == 1) {
        //     synth.filter.setCutoff(val / 127.0f);
        // } else if (num == 2) {
        //     synth.filter.setResonance(val / 127.0f);
        // } else if (num == 3) {
        //     synth.filter.setFilterMode(val / 127.0f * Zic_Effect_Filter::FILTER_MODE_COUNT - 1);
        // } else if (num == 4) {
        //     synth.osc.morphPct(val / 127.0f);
        // } else if (num == 5) {
        //     synth.osc.setFrequency(Zic::NOTE_FREQ[val]);
        // } else if (num == 6) {
        //     synth.osc.setAmplitude(val / 127.0f);
        // } else if (num == 7) {
        //     synth.adsr.setAttack(pow(val, 2));
        // } else if (num == 8) {
        //     synth.adsr.setDecay(val * 10);
        // }
    }

    void setPath(const char* path, uint8_t num = 0, uint8_t voice = 0)
    {
        switch (num) {
        case 0:
            kick.open(path);
            break;
        }
    }
};

#endif