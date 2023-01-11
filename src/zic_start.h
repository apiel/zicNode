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

Napi::Function emit;

int counter = 0;

int audioCallback(void* outputBuffer, void* /*inputBuffer*/, unsigned int nBufferFrames,
    double /*streamTime*/, RtAudioStreamStatus status, void* data)
{
    Zic_Server::getInstance()->sample((float*)outputBuffer, nBufferFrames);
    return 0;
}

Napi::Number getCounter(const Napi::CallbackInfo& info)
{
    return Napi::Number::New(info.Env(), counter);
}

class ZicWorker : public Napi::AsyncWorker {
protected:
    RtAudio::StreamOptions options;
    unsigned int deviceId;

public:
    ZicWorker(Napi::Function& callback, unsigned int deviceId)
        : Napi::AsyncWorker(callback)
        , deviceId(deviceId)
    {
    }
    ~ZicWorker() { }

    void Execute()
    {
        RtAudio audio;

        unsigned int bufferFrames = APP_AUDIO_CHUNK;
        RtAudio::StreamParameters oParams;
        oParams.deviceId = deviceId;

        oParams.nChannels = APP_CHANNELS;

        channels = oParams.nChannels;

        double* data = (double*)calloc(oParams.nChannels, sizeof(double));
        try {
            audio.openStream(&oParams, NULL, FORMAT, SAMPLE_RATE, &bufferFrames, &audioCallback, (void*)data);
            audio.startStream();
        } catch (RtAudioError& e) {
            e.printMessage();
            goto cleanup;
        }

        while (audio.isStreamRunning() == true) {
            usleep(100000);
        }

    cleanup:
        if (audio.isStreamOpen()) {
            audio.closeStream();
        }
        free(data);
    }

    void OnOK()
    {
        Callback().Call({ Env().Undefined(), Napi::Number::New(Env(), 23) }); // 23 Just because I can
    }
};

#endif