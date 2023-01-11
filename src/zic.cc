#include <RtAudio.h>
#include <napi.h>

#include "zic_server_audio.h"

Napi::Array getAudoDeviceInfo(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

    RtAudio audio;
    unsigned int deviceCount = audio.getDeviceCount();

    Napi::Array devices = Napi::Array::New(env);
    RtAudio::DeviceInfo rtInfo;
    for (unsigned int i = 0; i < deviceCount; i++) {
        rtInfo = audio.getDeviceInfo(i);
        if (rtInfo.probed == true) {
            Napi::Object device = Napi::Object::New(env);
            device.Set("probed", rtInfo.probed);
            device.Set("name", rtInfo.name);
            device.Set("outputChannels", rtInfo.outputChannels);
            device.Set("inputChannels", rtInfo.inputChannels);
            device.Set("duplexChannels", rtInfo.duplexChannels);
            device.Set("isDefaultOutput", rtInfo.isDefaultOutput);
            device.Set("isDefaultInput", rtInfo.isDefaultInput);

            Napi::Array sampleRates = Napi::Array::New(env);
            unsigned int vecSize = rtInfo.sampleRates.size();
            for (unsigned int i = 0; i < vecSize; i++) {
                sampleRates.Set(i, rtInfo.sampleRates[i]);
            }
            device.Set("sampleRates", sampleRates);

            device.Set("preferredSampleRate", rtInfo.preferredSampleRate);
            device.Set("nativeFormats", rtInfo.nativeFormats);

            devices.Set(i, device);
        }
    }

    return devices;
}

// class ZicWorker : public Napi::AsyncWorker {
// protected:
//     unsigned int deviceId;

// public:
//     ZicWorker(Napi::Function& callback, unsigned int deviceId)
//         : Napi::AsyncWorker(callback)
//         , deviceId(deviceId)
//     {
//     }

//     void Execute()
//     {
//         Zic_Server_Audio::getInstance()->initAudio(deviceId);
//     }

//     void OnOK()
//     {
//         Callback().Call({ Env().Undefined(), Napi::Number::New(Env(), 23) }); // 23 Just because I can
//     }
// };

// Napi::Value asyncStart(const Napi::CallbackInfo& info)
// {
//     RtAudio audio;

//     unsigned int deviceCount = audio.getDeviceCount();
//     if (deviceCount < 1) {
//         throw Napi::Error::New(info.Env(), "No audio devices found");
//         return info.Env().Undefined();
//     }

//     unsigned int deviceId = 0;
//     if (info.Length() > 0 && info[1].IsNumber()) {
//         deviceId = info[0].As<Napi::Number>().Uint32Value();
//         if (deviceId > deviceCount - 1) {
//             deviceId = audio.getDefaultOutputDevice();
//         }
//     } else {
//         deviceId = audio.getDefaultOutputDevice();
//     }

//     Napi::Function callback = info[1].As<Napi::Function>();
//     ZicWorker* worker = new ZicWorker(callback, deviceId);
//     worker->Queue();
//     return info.Env().Undefined();
// }

Napi::Value start(const Napi::CallbackInfo& info)
{
    RtAudio audio;

    unsigned int deviceCount = audio.getDeviceCount();
    if (deviceCount < 1) {
        throw Napi::Error::New(info.Env(), "No audio devices found");
        return info.Env().Undefined();
    }

    unsigned int deviceId = 0;
    if (info.Length() > 0 && info[1].IsNumber()) {
        deviceId = info[0].As<Napi::Number>().Uint32Value();
        if (deviceId > deviceCount - 1) {
            deviceId = audio.getDefaultOutputDevice();
        }
    } else {
        deviceId = audio.getDefaultOutputDevice();
    }

    Zic_Server_Audio::getInstance()->initAudio(deviceId);
    return info.Env().Undefined();
}

Napi::Number getBpm(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    return Napi::Number::New(env, Zic_Server::getInstance()->tempo.getBpm());
}

Napi::Value setBpm(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    if (info.Length() < 1 || !info[0].IsNumber()) {
        throw Napi::Error::New(env, "Missing bpm arguments");
    }
    Zic_Server::getInstance()->tempo.set(info[0].As<Napi::Number>().Uint32Value());
    return env.Undefined();
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set(Napi::String::New(env, "getAudoDeviceInfo"), Napi::Function::New(env, getAudoDeviceInfo));
    // exports.Set(Napi::String::New(env, "start"), Napi::Function::New(env, asyncStart));
    exports.Set(Napi::String::New(env, "start"), Napi::Function::New(env, start));
    exports.Set(Napi::String::New(env, "getBpm"), Napi::Function::New(env, getBpm));
    exports.Set(Napi::String::New(env, "setBpm"), Napi::Function::New(env, setBpm));
    return exports;
}

NODE_API_MODULE(zic, Init)
