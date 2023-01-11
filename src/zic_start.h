#ifndef ZIC_MAIN_H
#define ZIC_MAIN_H

#include <zic_file_linux.h> // Must be included on top

#include "./zic_server_def.h"

#include <RtAudio.h>
#include <napi.h>

#include <unistd.h> // usleep

#include "zic_server.h"

#define FORMAT RTAUDIO_FLOAT32

unsigned int channels;

int audioCallback(void* outputBuffer, void* /*inputBuffer*/, unsigned int nBufferFrames,
    double /*streamTime*/, RtAudioStreamStatus status, void* data)
{
    Zic_Server::getInstance()->sample((float*)outputBuffer, nBufferFrames);
    return 0;
}

class ZicWorker : public Napi::AsyncWorker {
protected:
    RtAudio::StreamOptions options;
    unsigned int deviceId;
    RtAudio audio;
    double* data;

public:
    ZicWorker(Napi::Function& callback, unsigned int deviceId)
        : Napi::AsyncWorker(callback)
        , deviceId(deviceId)
    {
    }
    ~ZicWorker()
    {
        if (audio.isStreamOpen()) {
            audio.closeStream();
        }
        free(data);
    }

    void Execute()
    {

        unsigned int bufferFrames = APP_AUDIO_CHUNK;
        RtAudio::StreamParameters oParams;
        oParams.deviceId = deviceId;

        oParams.nChannels = APP_CHANNELS;

        channels = oParams.nChannels;

        data = (double*)calloc(oParams.nChannels, sizeof(double));
        try {
            audio.openStream(&oParams, NULL, FORMAT, SAMPLE_RATE, &bufferFrames, &audioCallback, (void*)data);
            audio.startStream();
        } catch (RtAudioError& e) {
            e.printMessage();
            return;
        }

        while (audio.isStreamRunning() == true) {
            usleep(100000);
        }
    }

    void OnOK()
    {
        Callback().Call({ Env().Undefined(), Napi::Number::New(Env(), 23) }); // 23 Just because I can
    }
};

#endif