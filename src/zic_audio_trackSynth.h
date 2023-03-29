#ifndef ZIC_AUDIO_TRACK_SYNTH_H_
#define ZIC_AUDIO_TRACK_SYNTH_H_

#include "./zic_audio_synth_dualOsc.h"
#include "./zic_audio_track.h"

#include <zic_note.h>

class Zic_Audio_TrackSynth : public Zic_Audio_Track {
public:
    // TODO want to add some "effect" to the synth like bitcrusher, waveshaper, etc
    Zic_Audio_SynthDualOsc synth;

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

    void cc(uint8_t num, uint8_t val, uint8_t voice = 0) override
    {
        if (num == 5 || num == 21) {
            setFloat(Zic::NOTE_FREQ[val], num);
        } else if (num == 7) {
            setFloat(pow(val, 2), num);
        } else if (num == 8) {
            setFloat(val * 10, num);
        } else if (num == 9) {
            setFloat(val * 0.787402, num);
        } else if (num == 10) {
            setFloat(pow(val, 2), num);
        } else {
            setFloat(val / 127.0f, num);
        }
    }

    void setFloat(float val, uint16_t num = 0)
    {
        switch (num) {
        case 0:
            synth.updateVolume(val);
            break;
        case 1:
            synth.filter.setFrequency(val);
            break;
        case 2:
            synth.filter.setResonance(val);
            break;
        case 3:
            synth.filter.setFilterMode(val);
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
            synth.setModIntensity(Zic_Audio_SynthDualOsc::MOD_SRC_ENV, Zic_Audio_SynthDualOsc::MOD_TARGET_AMP, val);
            break;
        case 12:
            synth.setModIntensity(Zic_Audio_SynthDualOsc::MOD_SRC_ENV, Zic_Audio_SynthDualOsc::MOD_TARGET_PITCH, val);
            break;
        case 13:
            synth.setModIntensity(Zic_Audio_SynthDualOsc::MOD_SRC_ENV, Zic_Audio_SynthDualOsc::MOD_TARGET_MORPH, val);
            break;
        case 14:
            synth.setModIntensity(Zic_Audio_SynthDualOsc::MOD_SRC_ENV, Zic_Audio_SynthDualOsc::MOD_TARGET_CUTOFF, val);
            break;
        case 15:
            synth.setModIntensity(Zic_Audio_SynthDualOsc::MOD_SRC_ENV, Zic_Audio_SynthDualOsc::MOD_TARGET_RES, val);
            break;
        case 16:
            synth.setModIntensity(Zic_Audio_SynthDualOsc::MOD_SRC_ENV, Zic_Audio_SynthDualOsc::MOD_TARGET_AMP2, val);
            break;
        case 17:
            synth.setModIntensity(Zic_Audio_SynthDualOsc::MOD_SRC_ENV, Zic_Audio_SynthDualOsc::MOD_TARGET_PITCH2, val);
            break;
        case 18:
            synth.setModIntensity(Zic_Audio_SynthDualOsc::MOD_SRC_ENV, Zic_Audio_SynthDualOsc::MOD_TARGET_MORPH2, val);
            break;
        case 19:
            synth.setModIntensity(Zic_Audio_SynthDualOsc::MOD_SRC_ENV, Zic_Audio_SynthDualOsc::MOD_TARGET_MIX, val);
            break;
        case 20:
            synth.osc2.wavetable.morphPct(val);
            break;
        case 21:
            synth.osc2.setFrequency(val);
            break;
        case 22:
            synth.osc2.setAmplitude(val);
            break;
        case 23:
            synth.osc2FreqNoteOn = val != 0.0f;
            break;
        case 24:
            synth.updateMix(val);
            break;
        case 25:
            synth.setModIntensity(Zic_Audio_SynthDualOsc::MOD_SRC_OSC_2, Zic_Audio_SynthDualOsc::MOD_TARGET_AMP, val);
            break;
        case 26:
            synth.setModIntensity(Zic_Audio_SynthDualOsc::MOD_SRC_OSC_2, Zic_Audio_SynthDualOsc::MOD_TARGET_PITCH, val);
            break;
        case 27:
            synth.setModIntensity(Zic_Audio_SynthDualOsc::MOD_SRC_OSC_2, Zic_Audio_SynthDualOsc::MOD_TARGET_MORPH, val);
            break;
        case 28:
            synth.setModIntensity(Zic_Audio_SynthDualOsc::MOD_SRC_OSC_2, Zic_Audio_SynthDualOsc::MOD_TARGET_CUTOFF, val);
            break;
        case 29:
            synth.setModIntensity(Zic_Audio_SynthDualOsc::MOD_SRC_OSC_2, Zic_Audio_SynthDualOsc::MOD_TARGET_RES, val);
            break;
        case 30:
            synth.osc2Active = val != 0.0f;
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
            synth.osc2.open(path);
            break;
        }
    }
};

#endif