#ifndef ZIC_MAIN_H
#define ZIC_MAIN_H

#include <RtAudio.h>
#include <napi.h>

#include <unistd.h>

#ifndef SAMPLE_RATE
#define SAMPLE_RATE 44100
#endif

#define FORMAT RTAUDIO_FLOAT32

unsigned int channels;

int audioCallback(void* outputBuffer, void* /*inputBuffer*/, unsigned int nBufferFrames,
    double /*streamTime*/, RtAudioStreamStatus status, void* data)
{
    unsigned int i, j;
    // extern unsigned int channels;
    // unsigned int channels = 2; // FIXME
    float* buffer = (float*)outputBuffer;
    double* lastValues = (double*)data;

    if (status)
        std::cout << "Stream underflow detected!" << std::endl;

    const float BASE_RATE = 0.005;
    double increment;
    for (j = 0; j < channels; j++) {
        increment = BASE_RATE * (j + 1 + (j * 0.1));
        for (i = 0; i < nBufferFrames; i++) {
            *buffer++ = (float)(lastValues[j] * 0.5);
            lastValues[j] += increment;
            if (lastValues[j] >= 1.0)
                lastValues[j] -= 2.0;
        }
    }
    return 0;
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
        unsigned int deviceCount = audio.getDeviceCount();
        if (deviceCount < 1) {
            SetError("No audio devices found");
            return;
        }

        if (deviceId > deviceCount - 1) {
            deviceId = audio.getDefaultOutputDevice();
            // std::cout << "get default device: " << deviceId << std::endl;
        }

        unsigned int bufferFrames = 512;
        RtAudio::StreamParameters oParams;
        oParams.deviceId = 0;

        // RtAudio::DeviceInfo rtInfo = audio.getDeviceInfo(oParams.deviceId);
        // std::cout << "rtInfo.name: " << rtInfo.name << std::endl;
        // oParams.nChannels = rtInfo.outputChannels;
        oParams.nChannels = 1; // Might want to use only one channel

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