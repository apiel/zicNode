#ifndef ZIC_AUDIO_TRACKS_H_
#define ZIC_AUDIO_TRACKS_H_

#include "./zic_audio_track.h"
#include "./zic_audio_trackMidi.h"
#include "./zic_audio_trackSynth.h"
#include "./zic_audio_trackKick23.h"
#include "./zic_audio_track_def.h"

#include <stdlib.h>

#include <zic_seq_loop.h>
#include <zic_synth_file.h>

#include "./zic_state.h" // to be removed?

class Zic_Audio_Tracks {
protected:
    float mixerDivider = 0.0f;
    float volume = 1.0f;

    Zic_Audio_Tracks()
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
        // mixerDivider = 1.0f / TRACK_AUDIO_COUNT;
        setMasterVolume(volume);
        // APP_LOG("TRACK_AUDIO_COUNT: %d, TRACK_MIDI_COUNT %d, mixerDivider %f\n",
        //     TRACK_AUDIO_COUNT, TRACK_MIDI_COUNT, mixerDivider);
    }

public:
    uint8_t trackId = TRACK_1;

    uint8_t TRACK_AUDIO_COUNT = 0;
    uint8_t TRACK_MIDI_COUNT = 0;

    Zic_Audio_TrackKick23 track0;
    Zic_Audio_TrackSynth track1, track2, track3;

    Zic_Audio_TrackMidi track4, track5, track6, track7;

    Zic_Audio_Track* tracks[TRACK_COUNT] = { &track0, &track1, &track2, &track3,
        &track4, &track5, &track6, &track7 };

    static Zic_Audio_Tracks& getInstance()
    {
        static Zic_Audio_Tracks instance;
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
        // Set buf to 0 else we will sum up the samples
        // over the previous chunck
        for (int j = 0; j < len; j++) {
            buf[j] = 0.0f;
        }

        // float buffer[APP_AUDIO_CHUNK];
        float* buffer = new float[len];
        // Skip MIDI tracks, only return audio tracks samples
        for (uint8_t i = 0; i < TRACK_AUDIO_COUNT; i++) {
            tracks[i]->sample(buffer, len); // TODO could apply the mixerDivider directly there but might be difficult with PD
            for (int j = 0; j < len; j++) {
                buf[j] += buffer[j] * mixerDivider;
            }
        }
        delete[] buffer;
    }

    float getMasterVolume()
    {
        return volume;
    }

    void setMasterVolume(float _volume)
    {
        volume = range(_volume, 0.0f, 1.0f);
        mixerDivider = volume / TRACK_AUDIO_COUNT;
    }
};

#endif