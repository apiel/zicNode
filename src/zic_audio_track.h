#ifndef ZIC_AUDIO_TRACK_H_
#define ZIC_AUDIO_TRACK_H_

#include "./zic_audio_track_def.h"

// #include <app_core_file.h>
#include <zic_seq_loopMaster.h>

class Zic_Audio_Track {
protected:
    Zic_Seq_Step* stepOff[VOICE_COUNT];
    char statePath[50];

    Zic_Seq_PatternComponent* components[TRACK_COUNT] = {
        // &state[0].component,
        // &state[1].component,
        // &state[2].component,
        // &state[3].component
    };

    // void loadState()
    // {
    //     APP_LOG("Load state %s\n", statePath);

    //     Zic_File file(statePath, "r");
    //     if (!file.isOpen()) {
    //         APP_LOG("Could not open file %s\n", statePath);
    //         return;
    //     }

    //     char buffer[APP_STATE_BUFFER];
    //     uint8_t idx = 0;
    //     while (file.read(buffer, APP_STATE_BUFFER) > 0) {
    //         if (buffer[0] == ' ') {
    //             idx++;
    //             continue;
    //         }
    //         state[idx].load(buffer);
    //     }

    //     file.close();
    // }

public:
    uint8_t id = 0;
    const char * name;

    // App_State_Track state[TRACK_COUNT];
    Zic_Seq_LoopMaster looper;

    Zic_Audio_Track(uint8_t _id = 0, const char * _name = NULL)
        : name(_name)
        , looper(components[0], TRACK_COUNT)
    {
        id = _id;
        sprintf(statePath, "projects/current/track_%d.zic", id);
        // loadState();
    }

    virtual const char * getPatchDirectory() = 0;
    virtual void noteOn(uint8_t note, uint8_t velocity, uint8_t voice) = 0;
    virtual void noteOff(uint8_t note, uint8_t voice) = 0;
    virtual void cc(uint8_t num, uint8_t val, uint8_t voice) { }
    virtual void sample(float* buf, int len) = 0;

    virtual void loadPatch() { }

    virtual bool isOn()
    {
        return false;
    }

    virtual bool isAudioTrack()
    {
        return true;
    }

    // void saveState()
    // {
    //     APP_LOG("Save state %s\n", statePath);

    //     Zic_File file(statePath, "w");
    //     if (!file.isOpen()) {
    //         APP_LOG("Could not open file %s\n", statePath);
    //         return;
    //     }

    //     for (uint8_t i = 0; i < TRACK_COUNT; i++) {
    //         state[i].save(&file);
    //     }

    //     file.close();
    // }

    bool isCurrentState(uint8_t pos)
    {
        return looper.getCurrentComponent() == pos;
    }

    void next()
    {
        looper.next();

        // TODO need to handle tie
        for (uint8_t i = 0; i < VOICE_COUNT; i++) {
            if (stepOff[i] && !stepOff[i]->tie) {
                // printf("note off %d\n", stepOff[i]->note);
                noteOff(stepOff[i]->note, i);
                stepOff[i] = NULL;
            }
            if (looper.state.playing && looper.stepOn != 255) {
                Zic_Seq_Step* step = &looper.state.pattern->steps[i][looper.stepOn];
                if (step->note > 0) {
                    noteOn(step->note, step->velocity, i);
                    stepOff[i] = step;
                }
            }
        }
    }
};

#endif