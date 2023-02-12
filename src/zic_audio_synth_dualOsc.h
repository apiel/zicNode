#ifndef ZIC_AUDIO_SYNTH_DUAL_OSC_H_
#define ZIC_AUDIO_SYNTH_DUAL_OSC_H_

#include <zic_effect_filter.h>
#include <zic_mod_adsr.h>
#include <zic_osc_wavetable.h>

class Zic_Audio_SynthDualOsc {
public:
    enum {
        MOD_TARGET_CUTOFF = 0,
        MOD_TARGET_RES,
        MOD_TARGET_PITCH,
        MOD_TARGET_AMP,
        MOD_TARGET_MORPH,
        MOD_TARGET_PITCH2,
        MOD_TARGET_AMP2,
        MOD_TARGET_MORPH2,
        MOD_TARGET_MIX,
        MOD_TARGET_COUNT
    };

    enum {
        MOD_SRC_ENV = 0,
        MOD_SRC_OSC_2,
        MOD_SRC_IN,
        MOD_SRC_COUNT
    };

protected:
    float modValue[MOD_TARGET_COUNT];
    float modIntensity[MOD_TARGET_COUNT][MOD_SRC_COUNT];
    float modSumIntensity[MOD_TARGET_COUNT] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

    void updateModIntensity()
    {
        for (uint8_t i = 0; i < MOD_TARGET_COUNT; i++) {
            modSumIntensity[i] = 0.0;
            for (uint8_t j = 0; j < MOD_SRC_COUNT; j++) {
                modSumIntensity[i] += modIntensity[i][j];
            }
            if (modSumIntensity[i] > 0.0) {
                modSumIntensity[i] = 1.0 / modSumIntensity[i];
            }
        }
    }

    void updateModValue(uint8_t src, float value)
    {
        for (uint8_t i = 0; i < MOD_TARGET_COUNT; i++) {
            modValue[i] += value * modIntensity[i][src];
        }
    }

    void resetModValue()
    {
        for (uint8_t i = 0; i < MOD_TARGET_COUNT; i++) {
            modValue[i] = 0.0;
        }
    }

    float _sample()
    {
        float osc2Out = osc2.next(
            modValue[MOD_TARGET_AMP2] * modSumIntensity[MOD_TARGET_AMP2],
            modValue[MOD_TARGET_PITCH2] * modSumIntensity[MOD_TARGET_PITCH2],
            modValue[MOD_TARGET_MORPH2] * modSumIntensity[MOD_TARGET_MORPH2]);

        updateModValue(MOD_SRC_OSC_2, osc2Out);

        float osc1Out = osc.next(
            modValue[MOD_TARGET_AMP] * modSumIntensity[MOD_TARGET_AMP],
            modValue[MOD_TARGET_PITCH] * modSumIntensity[MOD_TARGET_PITCH],
            modValue[MOD_TARGET_MORPH] * modSumIntensity[MOD_TARGET_MORPH]);

        float modulatedMix = mix + modValue[MOD_TARGET_MIX] * modSumIntensity[MOD_TARGET_MIX];
        float mixedOsc = osc1Out * (1.0f - modulatedMix) + osc2Out * modulatedMix;

        return filter.next(
            mixedOsc,
            modValue[MOD_TARGET_CUTOFF] * modSumIntensity[MOD_TARGET_CUTOFF],
            modValue[MOD_TARGET_RES] * modSumIntensity[MOD_TARGET_RES]);
    }

public:
    Zic_Osc_Wavetable osc;
    Zic_Osc_Wavetable osc2;
    Zic_Effect_Filter filter;
    Zic_Mod_Adsr adsr;
    float volume = 1.0f;
    float mix = 0.0f;
    bool osc2FreqNoteOn = false;

    void setModIntensity(uint8_t src, uint8_t target, float intensity)
    {
        modIntensity[target][src] = intensity;
        updateModIntensity();
    }

    // Might be used to record wavetable...
    // float sample(float modIn)
    // {
    //     resetModValue();
    //     updateModValue(MOD_SRC_IN, modIn);
    //     return _sample();
    // }

    float sample()
    {
        resetModValue();
        float envOut = adsr.next();
        updateModValue(MOD_SRC_ENV, envOut);
        return _sample() * envOut * volume;
    }

    void noteOn(uint8_t note, uint8_t velocity)
    {
        osc.setFrequency(Zic::NOTE_FREQ[note]);
        if (osc2FreqNoteOn) {
            osc2.setFrequency(Zic::NOTE_FREQ[note]);
        }
        adsr.on();
    }

    void noteOff(uint8_t note)
    {
        adsr.off();
    }

    void updateVolume(float _volume)
    {
        volume = _volume;
        if (volume < 0.0f) {
            volume = 0.0f;
        } else if (volume > 1.0f) {
            volume = 1.0f;
        }
    }

    void updateMix(float _mix)
    {
        mix = _mix;
        if (mix < 0.0f) {
            mix = 0.0f;
        } else if (mix > 1.0f) {
            mix = 1.0f;
        }
    }
};

#endif