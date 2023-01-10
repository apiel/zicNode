#ifndef ZIC_MAIN_H
#define ZIC_MAIN_H

#include <RtAudio.h>
#include <napi.h>

void zic_start(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    RtAudio audio;
    if (audio.getDeviceCount() < 1) {
        Napi::Error::New(env, "No audio devices found").ThrowAsJavaScriptException();
        return;
    }

    // while (1) { }
}

class ZicWorker : public Napi::AsyncWorker {
public:
    ZicWorker(Napi::Function& callback, int points)
        : Napi::AsyncWorker(callback)
        , points(points)
        , estimate(0)
    {
    }
    ~ZicWorker() { }

    void Execute() { estimate = 1; }

    void OnOK()
    {
        Callback().Call({ Env().Undefined(), Napi::Number::New(Env(), estimate) });
    }

    void OnError(const Napi::Error& e)
    {
        Callback().Call({ e.Value(), Env().Undefined() });
    }

private:
    int points;
    double estimate;
};

#endif