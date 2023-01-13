#ifndef ZIC_SERVER_H
#define ZIC_SERVER_H

#include <stdlib.h>
#include <zic_seq_tempo.h>

#include "./zic_audio_tracks.h"
#include "./zic_midi.h"
#include "./zic_util.h"
#include "./zic_state.h"

class Zic_Server {
protected:
    Zic_Server()
    {
        zic_state_init();
        tracks = &Zic_Audio_Tracks::getInstance();
        Zic_Midi::getInstance();
    }

public:
    Zic_Seq_Tempo<> tempo;

    Zic_Audio_Tracks* tracks;

    static Zic_Server& getInstance()
    {
        static Zic_Server instance;
        return instance;
    }

    void sample(float* buf, int len)
    {
        // printf("sample time %ld\n", timeMs());
        // std::cout << "sample time " << timeMs() << std::endl;
        if (tempo.next(timeMs()))
        // if (tempo.next()) // Using sample rate instead of time
        {
            tracks->next();
        }
        tracks->sample(buf, len);
    }
};

#endif