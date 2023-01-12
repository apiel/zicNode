#ifndef ZIC_SERVER_AUDIO_H
#define ZIC_SERVER_AUDIO_H

#include <zic_file_linux.h> // Must be included on top

#include "./zic_server_def.h"

#include "./zic_server.h"

#include <RtAudio.h>
#include <stdlib.h>
#include <unistd.h> // usleep

#define FORMAT RTAUDIO_FLOAT32

// Zic_Server *global_zic_server = &Zic_Server::getInstance();
// Zic_State *global_zic_state = &Zic_State::getInstance();

int audioCallback(void* outputBuffer, void* /*inputBuffer*/, unsigned int nBufferFrames,
    double /*streamTime*/, RtAudioStreamStatus status, void* data)
{
    // zic_state.counter++;
    // Zic_State::getInstance().counter++;
    Zic_State::getInstance().tick((float*)outputBuffer, nBufferFrames);
    // global_zic_state->tick();

    Zic_Server::getInstance().sample((float*)outputBuffer, nBufferFrames);
    // extern Zic_Server *global_zic_server;
    // global_zic_server->sample((float*)outputBuffer, nBufferFrames);
    return 0;
}

class Zic_Server_Audio {
protected:
    RtAudio::StreamOptions options;
    RtAudio::StreamParameters audioParams;
    RtAudio audio;
    double* data;

    Zic_Server_Audio()
    {
    }

    ~Zic_Server_Audio()
    {
        if (audio.isStreamOpen()) {
            audio.closeStream();
        }
        free(data);
    }

public:
    static Zic_Server_Audio& getInstance()
    {
        static Zic_Server_Audio instance;
        return instance;
    }

    void initAudio(unsigned int deviceId)
    {
        unsigned int bufferFrames = APP_AUDIO_CHUNK;
        audioParams.deviceId = deviceId;
        audioParams.nChannels = APP_CHANNELS;
        data = (double*)calloc(audioParams.nChannels, sizeof(double));
        try {
            audio.openStream(&audioParams, NULL, FORMAT, SAMPLE_RATE, &bufferFrames, &audioCallback, (void*)data);
            audio.startStream();
        } catch (RtAudioError& e) {
            e.printMessage();
            return;
        }
    }

    void keepAlive()
    {
        while (audio.isStreamRunning() == true) {
            usleep(100000);
        }
    }
};

#endif