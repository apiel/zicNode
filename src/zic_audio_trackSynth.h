#ifndef ZIC_AUDIO_TRACK_SYNTH_H_
#define ZIC_AUDIO_TRACK_SYNTH_H_

#include "./zic_audio_synth.h"
#include "./zic_audio_track.h"

#include <zic_note.h>

class Zic_Audio_TrackSynth : public Zic_Audio_Track {
public:
    Zic_Audio_Synth synth;

    Zic_Audio_TrackSynth(uint8_t _id = 0, const char* _name = NULL)
        : Zic_Audio_Track(_id, _name)
    {
    }

    void noteOn(uint8_t note, uint8_t velocity, uint8_t voice)
    {
        // printf("[track %d] noteOn %d %d %d\n", id, note, velocity, voice);
        synth.noteOn(note, velocity);
    }

    void noteOff(uint8_t note, uint8_t voice)
    {
        synth.noteOff(note);
    }

    void sample(float* buf, int len)
    {
        for (int i = 0; i < len; i++) {
            buf[i] = synth.sample();
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
        if (num == 5) {
            setFloat(Zic::NOTE_FREQ[val], num);
        } else if (num == 7) {
            setFloat(pow(val, 2), num);
        } else if (num == 8) {
            setFloat(val * 10, num);
        } else if (num == 9) {
            setFloat(val * 0.787402, num);
        } else if (num == 10) {
            setFloat(pow(val, 2), num);
        } else if (num == 16) {
            // should there be a map table for LFO frequencies?
            setFloat(val / 127.0f * 20 + 0.01, num);
        } else {
            setFloat(val / 127.0f, num);
        }
    }

    void setFloat(float val, uint16_t num = 0)
    {
        switch (num) {
        case 1:
            synth.filter.setCutoff(val);
            break;
        case 2:
            synth.filter.setResonance(val);
            break;
        case 3:
            synth.filter.setFilterMode(val * Zic_Effect_Filter::FILTER_MODE_COUNT - 1);
            break;
        case 4:
            synth.osc.wavetable.morphPct(val);
            break;
        case 5:
            synth.osc.setFrequency(val);
            break;
        case 6:
            synth.osc.setAmplitude(val);
            break;
        case 7:
            synth.adsr.setAttack(val);
            break;
        case 8:
            synth.adsr.setDecay(val);
            break;
        case 9:
            synth.adsr.setSustain(val);
            break;
        case 10:
            synth.adsr.setRelease(val);
            break;
        case 11:
            synth.setModIntensity(Zic_Audio_Synth::MOD_SRC_ENV, Zic_Audio_Synth::MOD_TARGET_AMP, val);
            break;
        case 12:
            synth.setModIntensity(Zic_Audio_Synth::MOD_SRC_ENV, Zic_Audio_Synth::MOD_TARGET_PITCH, val);
            break;
        case 13:
            synth.setModIntensity(Zic_Audio_Synth::MOD_SRC_ENV, Zic_Audio_Synth::MOD_TARGET_MORPH, val);
            break;
        case 14:
            synth.setModIntensity(Zic_Audio_Synth::MOD_SRC_ENV, Zic_Audio_Synth::MOD_TARGET_CUTOFF, val);
            break;
        case 15:
            synth.setModIntensity(Zic_Audio_Synth::MOD_SRC_ENV, Zic_Audio_Synth::MOD_TARGET_RES, val);
            break;
        case 16:
            synth.lfo[0].setFrequency(val);
            break;
        }
    }

    void setString(const char* path, uint16_t num = 0)
    {
        switch (num) {
        case 0:
            synth.osc.open(path);
            break;
        case 1:
            synth.lfo[0].open(path);
            break;
        case 2:
            synth.lfo[1].open(path);
            break;
        }
    }
};

#endif