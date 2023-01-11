#ifndef ZIC_SERVER_H
#define ZIC_SERVER_H

#include <stdlib.h>
#include <zic_seq_tempo.h>

#include "./zic_util.h"
// #include "./zic_midi.h"
// #include "./zic_tracks.h"

class Zic_Server {

protected:
    Zic_Seq_Tempo<> tempo;

    Zic_Server()
    {
        // tracks = App_Tracks::getInstance();

        // App_Midi::getInstance();
    }

public:
    // App_Tracks * tracks;

    static Zic_Server* instance;

    static Zic_Server* getInstance()
    {
        if (!instance) {
            instance = new Zic_Server();
        }
        return instance;
    }

    void sample(float* buf, int len)
    {
        if (tempo.next(timeMs()))
        // if (tempo.next()) // Using sample rate instead of time
        {
            // tracks->next();
        }
        // tracks->sample(buf, len);
    }
};

Zic_Server* Zic_Server::instance = NULL;

#endif