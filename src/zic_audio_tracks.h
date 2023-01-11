#ifndef ZIC_AUDIO_TRACKS_H_
#define ZIC_AUDIO_TRACKS_H_

#include "./zic_audio_track.h"
#include "./zic_audio_trackMidi.h"
#include "./zic_audio_trackPd.h"
#include "./zic_audio_trackSynth.h"
#include "./zic_audio_track_def.h"

#include <stdlib.h>

#include <wavetables/wavetable_Bank.h>
#include <zic_seq_loop.h>
#include <zic_synth_file.h>

class App_Tracks {
protected:
    float mixerDivider = 0.0f;

    App_Tracks()
        : track0(TRACK_1, "Track1")
        , track1(TRACK_2, "Track2")
        , track2(TRACK_3, "Track3")
        , track3(TRACK_4, "Track4")
        , track4(MIDI_TRACK_1, "Midi_1")
        , track5(MIDI_TRACK_2, "Midi_2")
        , track6(MIDI_TRACK_3, "Midi_3")
        , track7(MIDI_TRACK_4, "Midi_4")
    {
        for (uint8_t t = 0; t < TRACK_COUNT; t++) {
            if (tracks[t]->isAudioTrack()) {
                TRACK_AUDIO_COUNT++;
            } else {
                TRACK_MIDI_COUNT++;
            }
        }
        mixerDivider = 1.0f / TRACK_AUDIO_COUNT;
    }

public:
    uint8_t trackId = TRACK_1;

    uint8_t TRACK_AUDIO_COUNT = 0;
    uint8_t TRACK_MIDI_COUNT = 0;

#if ZIC_TRACK_LAYOUT == 1
    Zic_Audio_TrackPd track0;
    Zic_Audio_TrackSynth track1, track2, track3;
#else
    Zic_Audio_TrackPd track0, track1;
    Zic_Audio_TrackSynth track2, track3;
#endif
    Zic_Audio_TrackMidi track4, track5, track6, track7;

    Zic_Audio_Track* tracks[TRACK_COUNT] = { &track0, &track1, &track2, &track3,
        &track4, &track5, &track6, &track7 };

    static App_Tracks* instance;

    static App_Tracks* getInstance()
    {
        if (!instance) {
            instance = new App_Tracks();
        }
        return instance;
    }

    void next()
    {
        for (uint8_t t = 0; t < TRACK_COUNT; t++) {
            tracks[t]->next();
        }
    }

    void sample(float* buf, int len)
    {
        // tracks[0]->sample(buf, len);

        for (int j = 0; j < len; j++) {
            buf[j] = 0.0f;
        }

        // NOTE should dynamic data allocation?
        // float buffer[APP_AUDIO_CHUNK];
        float* buffer = new float[len];
        // Skip MIDI tracks, only return audio tracks samples
        for (uint8_t i = 0; i < TRACK_AUDIO_COUNT; i++) {
            tracks[i]->sample(buffer, len);
            for (int j = 0; j < len; j++) {
                buf[j] += buffer[j] * mixerDivider;
            }
        }
        delete[] buffer;
    }

    void togglePlay()
    {
        for (uint8_t i = 0; i < TRACK_COUNT; i++) {
            tracks[i]->looper.togglePlay();
        }
    }

    void reset()
    {
        for (uint8_t i = 0; i < TRACK_COUNT; i++) {
            tracks[i]->looper.reset();
        }
    }

    bool isPlaying()
    {
        return tracks[0]->looper.isPlaying();
    }

    // void cc(uint8_t num, uint8_t val, uint8_t voice)
    // {
    //     // TODO should we select the right track?
    //     for (uint8_t i = 0; i < TRACK_COUNT; i++) {
    //         tracks[i]->cc(num, val, voice);
    //     }
    // }

    void quit()
    {
        // for (uint8_t i = 0; i < TRACK_COUNT; i++) {
        //     tracks[i]->saveState();
        // }
    }
};

App_Tracks* App_Tracks::instance = NULL;

#endif