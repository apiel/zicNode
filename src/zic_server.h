#ifndef ZIC_SERVER_H
#define ZIC_SERVER_H

#include <stdlib.h>
#include <zic_seq_tempo.h>

#include "./zic_audio_tracks.h"
#include "./zic_midi.h"
#include "./zic_util.h"

class Zic_State {
public:
    static Zic_State& getInstance()
    {
        static Zic_State instance;
        return instance;
    }

    unsigned int counter = 0;

    void tick(float* buf, int len)
    {
        counter++;
    }

    unsigned int getCounter()
    {
        return counter;
    }
};

class Zic_Server {
protected:
    Zic_Server()
    {
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