#ifndef ZIC_SERVER_AUDIO_H
#define ZIC_SERVER_AUDIO_H

#include <zic_file_linux.h> // Must be included on top

#include "./zic_server_def.h"

#include "./zic_server.h"

#include <RtAudio.h>
#include <stdlib.h>
#include <unistd.h> // usleep

#define FORMAT RTAUDIO_FLOAT32

int audioCallback(void* outputBuffer, void* /*inputBuffer*/, unsigned int nBufferFrames,
    double /*streamTime*/, RtAudioStreamStatus status, void* data)
{
    Zic_Server::getInstance().sample((float*)outputBuffer, nBufferFrames);
    return 0;
}

class Zic_Server_Audio {
protected:
    RtAudio::StreamOptions options;
    RtAudio::StreamParameters audioParams;

    Zic_Server_Audio()
    {
    }

    ~Zic_Server_Audio()
    {
        stopAudio();
    }

public:
    RtAudio audio;

    static Zic_Server_Audio& getInstance()
    {
        static Zic_Server_Audio instance;
        return instance;
    }

    void initAudio(unsigned int deviceId)
    {
        stopAudio();
        unsigned int bufferFrames = APP_AUDIO_CHUNK;
        audioParams.deviceId = deviceId;
        audioParams.nChannels = APP_CHANNELS;
        audio.showWarnings(false);
        try {
            audio.openStream(&audioParams, NULL, FORMAT, SAMPLE_RATE, &bufferFrames, &audioCallback);
            audio.startStream();
        } catch (RtAudioError& e) {
            e.printMessage();
            return;
        }
    }

    void stopAudio()
    {
        if (audio.isStreamOpen()) {
            audio.closeStream();
        }
    }
};

#endif