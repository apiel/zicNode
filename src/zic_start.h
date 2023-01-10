#ifndef ZIC_MAIN_H
#define ZIC_MAIN_H

#include <RtAudio.h>
#include <napi.h>

#include <unistd.h> // usleep

#ifndef SAMPLE_RATE
#define SAMPLE_RATE 44100
#endif

#define FORMAT RTAUDIO_FLOAT32

unsigned int channels;

int audioCallback(void* outputBuffer, void* /*inputBuffer*/, unsigned int nBufferFrames,
    double /*streamTime*/, RtAudioStreamStatus status, void* data)
{
    unsigned int i, j;
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

Napi::Value syncStart(const Napi::CallbackInfo& info)
{
    RtAudio audio;
    unsigned int deviceCount = audio.getDeviceCount();
    if (deviceCount < 1) {
        // SetError("No audio devices found");
        throw Napi::Error::New(info.Env(), "No audio devices found");
        return info.Env().Undefined();
    }

    RtAudio::StreamOptions options;

    Napi::Env env = info.Env();
    Napi::Function emit = info[0].As<Napi::Function>();

    unsigned int deviceId = 0;
    if (info.Length() > 1 && info[1].IsNumber()) {
        deviceId = info[1].As<Napi::Number>().Uint32Value();
        if (deviceId > deviceCount - 1) {
            deviceId = audio.getDefaultOutputDevice();
        }
    } else {
        deviceId = audio.getDefaultOutputDevice();
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
        std::cout << "isStreamRunning" << std::endl;
        emit.Call({ Napi::String::New(env, "data"), Napi::String::New(env, "data ...") });
        usleep(1000000);
    }

    cleanup:
        if (audio.isStreamOpen()) {
            audio.closeStream();
        }
        free(data);

    return info.Env().Undefined();
}

#endif