#include <RtAudio.h>
#include <napi.h>

#include "zic_server_audio.h"

Napi::Value error(Napi::Env& env, const std::string& message)
{
    Napi::TypeError::New(env, message).ThrowAsJavaScriptException();
    return env.Undefined();
}

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

Napi::Number getBpm(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    return Napi::Number::New(env, Zic_Server::getInstance().tempo.getBpm());
}

Napi::Value setBpm(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    if (info.Length() < 1 || !info[0].IsNumber()) {
        return error(env, "Missing bpm arguments");
    }
    Zic_Server::getInstance().tempo.set(info[0].As<Napi::Number>().Uint32Value());
    return env.Undefined();
}

// Might want to be able to send events from server
// https://github.com/NickNaso/addon-event-emitter/blob/master/00/index.js
//
// or we might need to create a thread safe function and use callbacks
// https://github.com/nodejs/node-addon-api/blob/main/doc/threadsafe_function.md
// maybe we creating a keepAlive function where tsfn.BlockingCall( value, callback );
// would be called from anywhere...
Napi::Value start(const Napi::CallbackInfo& info)
{
    RtAudio audio;
    Napi::Env env = info.Env();

    unsigned int deviceCount = audio.getDeviceCount();
    if (deviceCount < 1) {
        return error(env, "No audio devices found");
    }

    unsigned int deviceId = 0;
    if (info.Length() > 0 && info[0].IsNumber()) {
        deviceId = info[0].As<Napi::Number>().Uint32Value();
        if (deviceId > deviceCount - 1) {
            deviceId = audio.getDefaultOutputDevice();
        }
    } else {
        deviceId = audio.getDefaultOutputDevice();
    }

    Zic_Server_Audio::getInstance().initAudio(deviceId);
    return env.Undefined();
}

Napi::Boolean isAudioRunning(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    return Napi::Boolean::New(env, Zic_Server_Audio::getInstance().audio.isStreamRunning());
}

Napi::Value stop(const Napi::CallbackInfo& info)
{
    Zic_Server_Audio::getInstance().stopAudio();
    return info.Env().Undefined();
}

Napi::Value setPatternLength(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    if (info.Length() < 2 || !info[0].IsNumber() || !info[1].IsNumber()) {
        return error(env, "Invalid arguments: patternIndex, length");
    }
    uint32_t patternIndex = info[0].As<Napi::Number>().Uint32Value();
    uint32_t patternLength = info[1].As<Napi::Number>().Uint32Value();
    if (patternIndex > ZIC_PATTERN_COUNT - 1) {
        return error(env, "Pattern index out of range");
    }
    patterns[patternIndex].stepCount = patternLength;
    return env.Undefined();
}

Napi::Value getPatternLength(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    if (info.Length() < 1 || !info[0].IsNumber()) {
        return error(env, "Missing pattern index arguments");
    }
    uint32_t patternIndex = info[0].As<Napi::Number>().Uint32Value();
    if (patternIndex > ZIC_PATTERN_COUNT - 1) {
        return error(env, "Pattern index out of range, max: " + std::to_string(ZIC_PATTERN_COUNT - 1));
    }
    return Napi::Number::New(env, patterns[patternIndex].stepCount);
}

Napi::Value setPatternStep(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    if (info.Length() < 5 || !info[0].IsNumber() || !info[1].IsNumber() || !info[2].IsNumber()
        || !info[3].IsNumber() || !info[4].IsBoolean() || (info.Length() > 5 && !info[5].IsNumber())) {
        return error(env, "Invalid arguments: patternIndex, stepIndex, note, velocity, tie, (voice=0)");
    }
    uint32_t patternIndex = info[0].As<Napi::Number>().Uint32Value();
    uint32_t stepIndex = info[1].As<Napi::Number>().Uint32Value();
    uint32_t note = info[2].As<Napi::Number>().Uint32Value();
    uint32_t velocity = info[3].As<Napi::Number>().Uint32Value();
    bool tie = info[4].As<Napi::Boolean>().Value();
    uint32_t voice = 0;
    if (info.Length() > 5) {
        voice = info[5].As<Napi::Number>().Uint32Value();
    }
    if (patternIndex > ZIC_PATTERN_COUNT - 1) {
        return error(env, "Pattern index out of range, max: " + std::to_string(ZIC_PATTERN_COUNT - 1));
    }
    if (stepIndex > MAX_STEPS_IN_PATTERN - 1) {
        return error(env, "Step index out of range, max: " + std::to_string(MAX_STEPS_IN_PATTERN - 1));
    }
    if (note > Zic::_NOTE_END || note < Zic::_NOTE_START) {
        return error(env, "Note out of range (" + std::to_string(Zic::_NOTE_START) + "-" + std::to_string(Zic::_NOTE_END) + ")");
    }

    Zic_Seq_Step& step = patterns[patternIndex].steps[voice][stepIndex];
    step.note = note;
    step.velocity = velocity;
    step.tie = tie;
    
    return env.Undefined();
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set(Napi::String::New(env, "getAudoDeviceInfo"), Napi::Function::New(env, getAudoDeviceInfo));
    exports.Set(Napi::String::New(env, "start"), Napi::Function::New(env, start));
    exports.Set(Napi::String::New(env, "stop"), Napi::Function::New(env, stop));
    exports.Set(Napi::String::New(env, "isAudioRunning"), Napi::Function::New(env, isAudioRunning));
    exports.Set(Napi::String::New(env, "getBpm"), Napi::Function::New(env, getBpm));
    exports.Set(Napi::String::New(env, "setBpm"), Napi::Function::New(env, setBpm));
    exports.Set(Napi::String::New(env, "setPatternLength"), Napi::Function::New(env, setPatternLength));
    exports.Set(Napi::String::New(env, "getPatternLength"), Napi::Function::New(env, getPatternLength));
    exports.Set(Napi::String::New(env, "setPatternStep"), Napi::Function::New(env, setPatternStep));
    return exports;
}

NODE_API_MODULE(zic, Init)
