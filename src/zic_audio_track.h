#ifndef ZIC_AUDIO_TRACK_H_
#define ZIC_AUDIO_TRACK_H_

#include "./zic_audio_track_def.h"
#include "./zic_state.h"

#include <zic_seq_loop.h>
#include <zic_seq_patch.h>

class Zic_Audio_Track {
protected:
    Zic_Seq_Step* stepOff[MAX_VOICES_IN_PATTERN];

public:
    uint8_t id = 0;
    uint16_t lastPatchUpdatedId = -1; // Set to biggest number to make sure it's not loaded
    const char* name;

    Zic_Seq_Loop looper;

    Zic_Audio_Track(uint8_t _id = 0, const char* _name = NULL)
        : name(_name)
    {
        id = _id;
    }

    // TODO might want to implement more midi interface (but is it really needed?)
    // or make more generic varaible setter like setString, setInt, setFloat, setBool
    virtual void noteOn(uint8_t note, uint8_t velocity, uint8_t voice) = 0;
    virtual void noteOff(uint8_t note, uint8_t voice) { }
    virtual void cc(uint8_t num, uint8_t val, uint8_t voice = 0) { }
    virtual void setFloat(float val, uint16_t num = 0) { }
    virtual void setString(const char* path, uint16_t num = 0) { }
    virtual void sample(float* buf, int len) = 0;

    virtual void loadPatch(uint16_t patchId)
    {
        Zic_Seq_Patch& patch = patches[patchId];

        if (patch.updateId == lastPatchUpdatedId) {
            return;
        }
        lastPatchUpdatedId = patch.updateId;

        for (uint16_t i = 0; i < patch.floatsCount; i++) {
            setFloat(patch.floats[i], i);
        }
        for (uint16_t i = 0; i < patch.stringsCount; i++) {
            setString(patch.strings[i], i);
        }
        for (uint16_t i = 0; i < patch.ccCount; i++) {
            // Keep it simple for the moment and not have voice/channel for cc
            cc(i, patch.cc[i]);
        }
    }

    virtual bool isOn()
    {
        return false;
    }

    virtual bool isAudioTrack()
    {
        return true;
    }

    void next()
    {
        // TODO need to handle tie
        for (uint8_t i = 0; i < MAX_VOICES_IN_PATTERN; i++) {
            if (stepOff[i] && !stepOff[i]->tie) {
                // printf("note off %d\n", stepOff[i]->note);
                noteOff(stepOff[i]->note, i);
                stepOff[i] = NULL;
            }
            if (looper.state.playing && looper.stepOn != 255) {
                // printf("stepOn\n");
                Zic_Seq_Step* step = &looper.state.pattern->steps[i][looper.stepOn];
                if (step->note > 0) {
                    printf("note on %d\n", step->note);
                    loadPatch(step->patchId);
                    noteOn(step->note, step->velocity, i);
                    stepOff[i] = step;
                }
            }
        }
    }
};

#endif