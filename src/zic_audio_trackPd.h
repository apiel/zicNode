#ifndef ZIC_AUDIO_TRACK_PD_H_
#define ZIC_AUDIO_TRACK_PD_H_

#include "./zic_audio_track.h"
#include "./zic_server_def.h"

#include "./zic_PdObject.h"
#include <PdBase.hpp>

class Zic_Audio_TrackPd : public Zic_Audio_Track {
protected:
    Zic_PdObject pdObject;

public:
    pd::PdBase pd;
    pd::Patch patch;

    Zic_Audio_TrackPd(uint8_t _id = 0, const char* _name = NULL)
        : Zic_Audio_Track(_id, _name)
        , pdObject(_id)
    {
        if (!pd.init(0, APP_CHANNELS, SAMPLE_RATE)) {
            APP_LOG("Could not init pd\n");
        }
        pd.setReceiver(&pdObject);
        pd.setMidiReceiver(&pdObject);
    }

    ~Zic_Audio_TrackPd()
    {
        pd.closePatch(patch);
        pd.clear();
    }

    void noteOn(uint8_t note, uint8_t velocity, uint8_t voice)
    {
        // printf("[track %d] noteOn %d %d %d\n", id, note, velocity, voice);
        pd.sendNoteOn(voice + 1, note, velocity);
    }

    void noteOff(uint8_t note, uint8_t voice)
    {
        pd.sendNoteOn(voice + 1, note, 0);
    }

    void sample(float* buf, int len)
    {
        int ticks = len / libpd_blocksize();
        pd.processFloat(ticks, NULL, buf);
    }

    void cc(uint8_t num, uint8_t val, uint8_t voice) override
    {
        pd.sendControlChange(voice + 1, num, val);
    }

    void setString(const char* path, uint8_t num = 0, uint8_t voice = 0) override
    {
        switch (num) {
        case 0:
            // TODO skip if different... valid if last one
            // or if changing of row
            if (patch.isValid()) {
                pd.closePatch(patch);
            }
            pd.computeAudio(true);
            patch = pd.openPatch("main.pd", path);
            // FIXME if patch does not exist then crash!!
            break;
        }
    }
};

#endif