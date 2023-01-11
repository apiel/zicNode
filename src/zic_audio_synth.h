#ifndef ZIC_AUDIO_SYNTH_H_
#define ZIC_AUDIO_SYNTH_H_

#include <zic_effect_filter.h>
#include <zic_mod_adsr.h>

#include "./zic_audio_wavetable.h"

#define ZIC_AUDIO_SYNTH_LFO_COUNT 2

// Should modulation be activated/deactivated if not used...

class Zic_Audio_Synth {
public:
    enum {
        MOD_TARGET_CUTOFF = 0,
        MOD_TARGET_RES,
        MOD_TARGET_PITCH,
        MOD_TARGET_AMP,
        MOD_TARGET_MORPH,
        MOD_TARGET_COUNT
    };

    const char* modTargetName[MOD_TARGET_COUNT] = {
        "Cutoff",
        "Resonance",
        "Pitch",
        "Amp",
        "Morph",
    };

    enum {
        MOD_SRC_ENV = 0,
        MOD_SRC_LFO_1,
        MOD_SRC_LFO_2,
        MOD_SRC_COUNT
    };

protected:
    float modValue[MOD_TARGET_COUNT];
    float modIntensity[MOD_TARGET_COUNT][MOD_SRC_COUNT];
    float modSumIntensity[MOD_TARGET_COUNT] = { 0.0, 0.0, 0.0, 0.0, 0.0 };

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

public:
    Zic_Audio_Wavetable osc;
    Zic_Audio_Wavetable lfo[ZIC_AUDIO_SYNTH_LFO_COUNT];
    Zic_Effect_Filter filter;
    Zic_Mod_Adsr adsr;

    Zic_Audio_Synth()
    {
        setModIntensity(MOD_SRC_LFO_1, MOD_TARGET_RES, 1.0);
        lfo[0].setFrequency(0.5);
    }

    void setModIntensity(uint8_t src, uint8_t target, float intensity)
    {
        modIntensity[target][src] = intensity;
        updateModIntensity();
    }

    float sample()
    {
        resetModValue();

        // Use the first envelope to modulate the signal output
        // and this cannot be changed, however, modulation can
        // still be use with other targets
        float envOut = adsr.next();

        updateModValue(MOD_SRC_ENV, envOut);
        updateModValue(MOD_SRC_LFO_1, lfo[0].next());
        updateModValue(MOD_SRC_LFO_2, lfo[1].next());

        return filter.next(
                   osc.next(
                       modValue[MOD_TARGET_AMP] * modSumIntensity[MOD_TARGET_AMP],
                       modValue[MOD_TARGET_PITCH] * modSumIntensity[MOD_TARGET_PITCH],
                       modValue[MOD_TARGET_MORPH] * modSumIntensity[MOD_TARGET_MORPH]),
                   modValue[MOD_TARGET_CUTOFF] * modSumIntensity[MOD_TARGET_CUTOFF],
                   modValue[MOD_TARGET_RES] * modSumIntensity[MOD_TARGET_RES])
            * envOut;
    }

    void noteOn(uint8_t note, uint8_t velocity)
    {
        osc.setFrequency(Zic::NOTE_FREQ[note]);
        adsr.on();
    }

    void noteOff(uint8_t note)
    {
        adsr.off();
    }
};

#endif