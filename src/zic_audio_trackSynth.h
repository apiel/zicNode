#ifndef ZIC_AUDIO_TRACK_SYNTH_H_
#define ZIC_AUDIO_TRACK_SYNTH_H_

#include "./zic_audio_track.h"
#include "./zic_audio_synth.h"

#include <zic_note.h>

class Zic_Audio_TrackSynth : public Zic_Audio_Track {
public:
    Zic_Audio_Synth synth;

    Zic_Audio_TrackSynth(uint8_t _id = 0, const char * _name = NULL)
        : Zic_Audio_Track(_id, _name)
    {
    }

    void noteOn(uint8_t note, uint8_t velocity, uint8_t voice)
    {
        synth.noteOn(note, velocity);
    }

    void noteOff(uint8_t note, uint8_t voice)
    {
        synth.noteOff(note);
    }

    void sample(float* buf, int len)
    {
        int count = len / 4;
        for (int i = 0; i < count; i++) {
            buf[i] = synth.sample();
#if APP_CHANNELS == 2
            // TODO
            // FIXME
            // or not ?? if sample is mono, then it's ok
            // but if it's stereo, then it's not :p
            buf[i + 1] = buf[i];
            i++;
#endif
        }
    }

    void cc(uint8_t num, uint8_t val, uint8_t voice) override
    {
        if (num == 1) {
            synth.filter.setCutoff(val / 127.0f);
        } else if (num == 2) {
            synth.filter.setResonance(val / 127.0f);
        } else if (num == 3) {
            synth.filter.setFilterMode(val / 127.0f * Zic_Effect_Filter::FILTER_MODE_COUNT - 1);
        } else if (num == 4) {
            synth.osc.morphPct(val / 127.0f);
        } else if (num == 5) {
            synth.osc.setFrequency(Zic::NOTE_FREQ[val]);
        } else if (num == 6) {
            synth.osc.setAmplitude(val / 127.0f);
        } else if (num == 7) {
            synth.adsr.setAttack(pow(val, 2));
        } else if (num == 8) {
            synth.adsr.setDecay(val * 10);
        } else if (num == 9) {
            synth.adsr.setSustain(val * 0.787402);
        } else if (num == 10) {
            synth.adsr.setRelease(pow(val, 2));
        } else if (num == 11) {
            synth.setModIntensity(Zic_Audio_Synth::MOD_SRC_ENV, Zic_Audio_Synth::MOD_TARGET_AMP, val / 127.0f);
        } else if (num == 12) {
            synth.setModIntensity(Zic_Audio_Synth::MOD_SRC_ENV, Zic_Audio_Synth::MOD_TARGET_PITCH, val / 127.0f);
        } else if (num == 13) {
            synth.setModIntensity(Zic_Audio_Synth::MOD_SRC_ENV, Zic_Audio_Synth::MOD_TARGET_MORPH, val / 127.0f);
        } else if (num == 14) {
            synth.setModIntensity(Zic_Audio_Synth::MOD_SRC_ENV, Zic_Audio_Synth::MOD_TARGET_CUTOFF, val / 127.0f);
        } else if (num == 15) {
            synth.setModIntensity(Zic_Audio_Synth::MOD_SRC_ENV, Zic_Audio_Synth::MOD_TARGET_RES, val / 127.0f);
        } else if (num == 16) {
            // should there be a map table for LFO frequencies?
            synth.lfo[0].setFrequency(val / 127.0f * 20 + 0.01);
        } else if (num == 17) {

        } else if (num == 18) {
        }
    }

    const char * getPatchDirectory() override
    {
        return "instruments/zic";
    }
};

#endif