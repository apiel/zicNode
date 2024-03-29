#ifndef ZIC_AUDIO_TRACK_KICK23_H_
#define ZIC_AUDIO_TRACK_KICK23_H_

#include <zic_drum_kick23.h>
#include <zic_effect_filter.h>

#include "./zic_audio_track.h"

// #include <zic_note.h>

class Zic_Audio_TrackKick23 : public Zic_Audio_Track {
public:
    Zic_Drum_Kick23 kick;
    Zic_Effect_Filter filter;

    Zic_Audio_TrackKick23(uint8_t _id = 0, const char* _name = NULL)
        : Zic_Audio_Track(_id, _name)
    {
        filter.setFilterMode(filter.FILTER_MODE_LOWPASS_12);
    }

    void noteOn(uint8_t note, uint8_t velocity, uint8_t voice)
    {
        kick.noteOn();
    }

    void sample(float* buf, int len)
    {
        for (int i = 0; i < len; i++) {
            buf[i] = filter.next(kick.sample());
#if APP_CHANNELS == 2
            // TODO
            // or not ?? if sample is mono, then it's ok
            // but if it's stereo, then it's not :p
            buf[i + 1] = buf[i];
            i++;
#endif
        }
    }

    void cc(uint8_t num, uint8_t val, uint8_t voice = 0) override
    {
        if (num == 2) {
            // kick duration
            setFloat(val * val / 3, num); // FIXME
        } else if (num == 3) {
            // kick frequency
            setFloat(Zic::NOTE_FREQ[val], num); // FIXME to high
        } else if (num == 16) {
            setFloat(val, num);
        } else if (num == 18) {
            setFloat(val / 127.0f * 100, num);
        } else if (num == 19) {
            setFloat(val, num);
        } else {
            setFloat(val / 127.0f, num);
        }
    }

    void setFloat(float val, uint16_t num = 0)
    {
        switch (num) {
        case 0:
            kick.updateVolume(val);
            break;
        case 1:
            kick.wavetable.morph(val);
            break;
        case 2:
            kick.updateDuration(val);
            break;
        case 3:
            kick.updateFrequency(val);
            break;
        case 4:
            kick.envelopAmp[2][0] = val;
            break;
        case 5:
            kick.envelopAmp[2][1] = val;
            break;
        case 6:
            kick.envelopAmp[3][0] = val;
            break;
        case 7:
            kick.envelopAmp[3][1] = val;
            break;
        case 8:
            kick.envelopAmp[4][0] = val;
            break;
        case 9:
            kick.envelopAmp[4][1] = val;
            break;
        case 10:
            kick.envelopAmp[5][0] = val;
            break;
        case 11:
            kick.envelopAmp[5][1] = val;
            break;
        case 12:
            kick.envelopFreq[2][0] = val;
            break;
        case 13:
            kick.envelopFreq[2][1] = val;
            break;
        case 14:
            kick.envelopFreq[3][0] = val;
            break;
        case 15:
            kick.envelopFreq[3][1] = val;
            break;
        case 16:
            kick.envelopFreq[4][0] = val;
            break;
        case 17:
            kick.envelopFreq[4][1] = val;
            break;
        case 18:
            kick.envelopFreq[5][0] = val;
            break;
        case 19:
            kick.envelopFreq[5][1] = val;
            break;
        case 20:
            filter.setFrequency(val);
            break;
        case 21:
            filter.setResonance(val);
            break;
        case 22:
            kick.distortion.setDistortion(val);
            break;
        case 23:
            kick.distortion.setDistortionRange(val);
            break;
        }
    }

    void setString(const char* path, uint16_t num = 0)
    {
        switch (num) {
        case 0:
            kick.wavetable.open(path);
            break;
        }
    }
};

#endif