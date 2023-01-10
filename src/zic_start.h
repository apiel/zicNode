#ifndef ZIC_MAIN_H
#define ZIC_MAIN_H

#include <RtAudio.h>
#include <napi.h>

#include <unistd.h>

typedef float MY_TYPE;
#define FORMAT RTAUDIO_FLOAT32
#define SCALE 1.0
#define BASE_RATE 0.005
#define TIME   1.0

void errorCallback(RtAudioError::Type type, const std::string& errorText)
{
    // This example error handling function does exactly the same thing
    // as the embedded RtAudio::error() function.
    std::cout << "in errorCallback" << std::endl;
    if (type == RtAudioError::WARNING)
        std::cerr << '\n'
                  << errorText << "\n\n";
    else if (type != RtAudioError::WARNING)
        throw(RtAudioError(errorText, type));
}

int saw(void* outputBuffer, void* /*inputBuffer*/, unsigned int nBufferFrames,
    double /*streamTime*/, RtAudioStreamStatus status, void* data)
{
    unsigned int i, j;
    // extern unsigned int channels;
    unsigned int channels = 2; // FIXME
    MY_TYPE* buffer = (MY_TYPE*)outputBuffer;
    double* lastValues = (double*)data;

    if (status)
        std::cout << "Stream underflow detected!" << std::endl;

    double increment;
    for (j = 0; j < channels; j++) {
        increment = BASE_RATE * (j + 1 + (j * 0.1));
        for (i = 0; i < nBufferFrames; i++) {
            *buffer++ = (MY_TYPE)(lastValues[j] * SCALE * 0.5);
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
    int points;

public:
    ZicWorker(Napi::Function& callback, int points)
        : Napi::AsyncWorker(callback)
        , points(points)
    {
    }
    ~ZicWorker() { }

    void Execute()
    {
        RtAudio audio;
        if (audio.getDeviceCount() < 1) {
            SetError("No audio devices found");
            return;
        }

        // Set our stream parameters for output only.
        unsigned int bufferFrames = 512;
        RtAudio::StreamParameters oParams;
        oParams.deviceId = 0;
        if (oParams.deviceId == 0)
            oParams.deviceId = audio.getDefaultOutputDevice();

        oParams.nChannels = 2;
        // oParams.firstChannel = offset;

        options.flags = RTAUDIO_HOG_DEVICE;
        options.flags |= RTAUDIO_SCHEDULE_REALTIME;
#if !defined(USE_INTERLEAVED)
        options.flags |= RTAUDIO_NONINTERLEAVED;
#endif

        double* data = (double*)calloc(oParams.nChannels, sizeof(double));
        // unsigned int sampleRate;
        try {
            audio.openStream(&oParams, NULL, FORMAT, 44100, &bufferFrames, &saw, (void*)data, &options, &errorCallback);
            // here we could send the sampleRate to the main thread
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