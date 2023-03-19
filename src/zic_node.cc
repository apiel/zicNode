#include <RtAudio.h>
#include <napi.h>

#include "zic_node.h"
#include "zic_node_args.h"
#include "zic_node_midi.h"
#include "zic_node_pattern.h"
#include "zic_node_sequencer.h"
#include "zic_node_track.h"
#include "zic_server_audio.h"

Napi::ThreadSafeFunction tsfn;
Napi::Value setOnBeatCallback(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    if (info.Length() != 1) {
        return error(env, "Invalid number of arguments: setOnBeatCallback(callback: () => void)");
    }
    if (!info[0].IsFunction()) {
        return error(env, "callback must be a function.");
    }
    tsfn = Napi::ThreadSafeFunction::New(env, info[0].As<Napi::Function>(), "OnBeat", 0, 1);
    Zic_Server::getInstance().onBeat = []() -> void {
        tsfn.BlockingCall();
    };

    return env.Undefined();
}

Napi::Array getAudioDeviceInfo(const Napi::CallbackInfo& info)
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

Napi::Value setPatch(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    try {
        if (info.Length() < 1 || !info[0].IsObject()) {
            throw Napi::Error::New(env, "Invalid arguments: patch");
        }
        Napi::Object patch = info[0].As<Napi::Object>();
        uint32_t id = patch.Get("id").As<Napi::Number>().Uint32Value();
        if (id >= ZIC_PATCH_COUNT) {
            throw Napi::Error::New(env, "Invalid patch id");
        }
        Zic_Seq_Patch p;
        if (patch.Has("floats")) {
            Napi::Object floats = patch.Get("floats").As<Napi::Object>();
            Napi::Array names = floats.GetPropertyNames();
            for (uint32_t i = 0; i < names.Length(); i++) {
                std::string strIndex = names.Get(i).As<Napi::String>().Utf8Value();
                float value = floats.Get(strIndex).As<Napi::Number>().FloatValue();
                p.setFloat(std::stoi(strIndex), value);
            }
        }
        if (patch.Has("strings")) {
            Napi::Object strings = patch.Get("strings").As<Napi::Object>();
            Napi::Array names = strings.GetPropertyNames();
            for (uint32_t i = 0; i < names.Length(); i++) {
                std::string strIndex = names.Get(i).As<Napi::String>().Utf8Value();
                std::string value = strings.Get(strIndex).As<Napi::String>().Utf8Value();
                p.setString(std::stoi(strIndex), value.c_str());
            }
        }
        if (patch.Has("cc")) {
            Napi::Object cc = patch.Get("cc").As<Napi::Object>();
            Napi::Array names = cc.GetPropertyNames();
            for (uint32_t i = 0; i < names.Length(); i++) {
                std::string strIndex = names.Get(i).As<Napi::String>().Utf8Value();
                uint32_t value = cc.Get(strIndex).As<Napi::Number>().Uint32Value();
                p.setCc(std::stoi(strIndex), value);
            }
        }
        patches[id].set(p);
    } catch (const Napi::Error& e) {
        e.ThrowAsJavaScriptException();
    }
    return env.Undefined();
}

Napi::Value getMasterVolume(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    try {
        return Napi::Number::New(env, Zic_Audio_Tracks::getInstance().getMasterVolume());
    } catch (const Napi::Error& e) {
        e.ThrowAsJavaScriptException();
        return env.Undefined();
    }
}

Napi::Value setMasterVolume(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    try {
        if (info.Length() < 1 || !info[0].IsNumber()) {
            throw Napi::Error::New(env, "Invalid arguments: volume");
        }
        float volume = info[0].As<Napi::Number>().FloatValue();
        Zic_Audio_Tracks::getInstance().setMasterVolume(volume);
    } catch (const Napi::Error& e) {
        e.ThrowAsJavaScriptException();
    }
    return env.Undefined();
}

Napi::Value getWavetable(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    try {
        if (info.Length() < 1 || !info[0].IsString() || (info.Length() > 1 && !info[1].IsNumber())) {
            throw Napi::Error::New(env, "Invalid arguments: path morph");
        }
        std::string path = info[0].As<Napi::String>().Utf8Value();
        float morphPosition = 0;
        if (info.Length() > 1) {
            morphPosition = info[1].As<Napi::Number>().FloatValue();
        }

        Zic_Wavetable_File wavetable;
        wavetable.open(path.c_str());
        wavetable.morph(morphPosition);
        uint64_t count = wavetable.getSampleCount();

        Napi::Array data = Napi::Array::New(env, count);
        for (uint32_t i = 0; i < count; i++) {
            float value = wavetable.getValueAt(i);
            data.Set(i, Napi::Number::New(env, value));
        }
        wavetable.audioFile.close();

        Napi::Object result = Napi::Object::New(env);
        result.Set("data", data);
        result.Set("wavetableCount", Napi::Number::New(env, wavetable.audioFile.wavetableCount));
        result.Set("totalSampleCount", Napi::Number::New(env, wavetable.audioFile.sampleCount));
        result.Set("wavetableSampleCount",
            Napi::Number::New(env, wavetable.audioFile.sampleCount / wavetable.audioFile.wavetableCount));
        return result;
    } catch (const Napi::Error& e) {
        e.ThrowAsJavaScriptException();
    }
    return env.Undefined();
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set(Napi::String::New(env, "PATTERN_COUNT"), Napi::Number::New(env, ZIC_PATTERN_COUNT));
    exports.Set(Napi::String::New(env, "PATCH_COUNT"), Napi::Number::New(env, ZIC_PATCH_COUNT));
    exports.Set(Napi::String::New(env, "MAX_STEPS_IN_PATTERN"), Napi::Number::New(env, MAX_STEPS_IN_PATTERN));
    exports.Set(Napi::String::New(env, "MAX_VOICES_IN_PATTERN"), Napi::Number::New(env, MAX_VOICES_IN_PATTERN));
    exports.Set(Napi::String::New(env, "TRACK_COUNT"), Napi::Number::New(env, TRACK_COUNT));
    exports.Set(Napi::String::New(env, "SAMPLE_RATE"), Napi::Number::New(env, SAMPLE_RATE));
    exports.Set(Napi::String::New(env, "NOTE_START"), Napi::Number::New(env, Zic::_NOTE_START));
    exports.Set(Napi::String::New(env, "NOTE_END"), Napi::Number::New(env, Zic::_NOTE_END));
    exports.Set(Napi::String::New(env, "ZIC_PATCH_MAX_FLOATS"), Napi::Number::New(env, ZIC_PATCH_MAX_FLOATS));
    exports.Set(Napi::String::New(env, "ZIC_PATCH_MAX_STRINGS"), Napi::Number::New(env, ZIC_PATCH_MAX_STRINGS));
    exports.Set(Napi::String::New(env, "ZIC_PATCH_STRING_LENGTH"), Napi::Number::New(env, ZIC_PATCH_STRING_LENGTH));
    exports.Set(Napi::String::New(env, "ZIC_PATCH_MAX_CC"), Napi::Number::New(env, ZIC_PATCH_MAX_CC));

    exports.Set(Napi::String::New(env, "getAudioDeviceInfo"), Napi::Function::New(env, getAudioDeviceInfo));
    exports.Set(Napi::String::New(env, "getMidiDevices"), Napi::Function::New(env, getMidiDevices));
    exports.Set(Napi::String::New(env, "setMidiCallback"), Napi::Function::New(env, setMidiCallback));
    exports.Set(Napi::String::New(env, "subscribeMidiInput"), Napi::Function::New(env, subscribeMidiInput));
    exports.Set(Napi::String::New(env, "unsubscribeMidiInput"), Napi::Function::New(env, unsubscribeMidiInput));
    exports.Set(Napi::String::New(env, "closeMidiOutput"), Napi::Function::New(env, closeMidiOutput));
    exports.Set(Napi::String::New(env, "sendMidiMessage"), Napi::Function::New(env, sendMidiMessage));
    exports.Set(Napi::String::New(env, "start"), Napi::Function::New(env, start));
    exports.Set(Napi::String::New(env, "stop"), Napi::Function::New(env, stop));
    exports.Set(Napi::String::New(env, "isAudioRunning"), Napi::Function::New(env, isAudioRunning));
    exports.Set(Napi::String::New(env, "getBpm"), Napi::Function::New(env, getBpm));
    exports.Set(Napi::String::New(env, "setBpm"), Napi::Function::New(env, setBpm));
    exports.Set(Napi::String::New(env, "setPatch"), Napi::Function::New(env, setPatch));
    exports.Set(Napi::String::New(env, "setPatternLength"), Napi::Function::New(env, setPatternLength));
    exports.Set(Napi::String::New(env, "getPatternLength"), Napi::Function::New(env, getPatternLength));
    exports.Set(Napi::String::New(env, "setPatternStep"), Napi::Function::New(env, setPatternStep));
    exports.Set(Napi::String::New(env, "cleanPatternStep"), Napi::Function::New(env, cleanPatternStep));
    exports.Set(Napi::String::New(env, "getPattern"), Napi::Function::New(env, getPattern));
    exports.Set(Napi::String::New(env, "setSequencerState"), Napi::Function::New(env, setSequencerState));
    exports.Set(Napi::String::New(env, "getSequencerStates"), Napi::Function::New(env, getSequencerStates));
    exports.Set(Napi::String::New(env, "getAllSequencerStates"), Napi::Function::New(env, getAllSequencerStates));
    exports.Set(Napi::String::New(env, "trackNoteOn"), Napi::Function::New(env, trackNoteOn));
    exports.Set(Napi::String::New(env, "trackNoteOff"), Napi::Function::New(env, trackNoteOff));
    exports.Set(Napi::String::New(env, "trackCc"), Napi::Function::New(env, trackCc));
    exports.Set(Napi::String::New(env, "trackSetNumber"), Napi::Function::New(env, trackSetNumber));
    exports.Set(Napi::String::New(env, "trackSetString"), Napi::Function::New(env, trackSetString));
    exports.Set(Napi::String::New(env, "setOnBeatCallback"), Napi::Function::New(env, setOnBeatCallback));
    exports.Set(Napi::String::New(env, "getMasterVolume"), Napi::Function::New(env, getMasterVolume));
    exports.Set(Napi::String::New(env, "setMasterVolume"), Napi::Function::New(env, setMasterVolume));
    exports.Set(Napi::String::New(env, "getWavetable"), Napi::Function::New(env, getWavetable));
    return exports;
}

NODE_API_MODULE(zic, Init)
