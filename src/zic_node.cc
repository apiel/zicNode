#include <RtAudio.h>
#include <napi.h>

#include "zic_node_args.h"
#include "zic_server_audio.h"

Napi::Value error(Napi::Env& env, const std::string& message)
{
    Napi::TypeError::New(env, message).ThrowAsJavaScriptException();
    return env.Undefined();
}

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
    try {
        if (info.Length() < 2 || !info[0].IsNumber() || !info[1].IsNumber()) {
            throw Napi::Error::New(env, "Invalid arguments: patternIndex, length");
        }
        uint32_t patternIndex = argPatternIndex(info, 0);
        uint32_t patternLength = info[1].As<Napi::Number>().Uint32Value();
        patterns[patternIndex].stepCount = patternLength;
        return env.Undefined();
    } catch (const Napi::Error& e) {
        e.ThrowAsJavaScriptException();
        return env.Undefined();
    }
}

Napi::Value getPatternLength(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    try {
        if (info.Length() < 1) {
            throw Napi::Error::New(env, "Missing pattern index arguments");
        }
        uint32_t patternIndex = argPatternIndex(info, 0);
        return Napi::Number::New(env, patterns[patternIndex].stepCount);
    } catch (const Napi::Error& e) {
        e.ThrowAsJavaScriptException();
        return env.Undefined();
    }
}

Napi::Value setPatternStep(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    try {
        if (info.Length() < 5 || !info[0].IsNumber() || !info[1].IsNumber() || !info[2].IsNumber()
            || !info[3].IsNumber() || !info[4].IsBoolean() || (info.Length() > 5 && !info[5].IsNumber())) {
            throw Napi::Error::New(env, "Invalid arguments: patternIndex, stepIndex, note, velocity, tie, (voice=0)");
        }
        uint32_t patternIndex = argPatternIndex(info, 0);
        uint32_t stepIndex = argStepIndex(info, 1);
        uint32_t note = argNote(info, 2);
        uint32_t velocity = info[3].As<Napi::Number>().Uint32Value();
        bool tie = info[4].As<Napi::Boolean>().Value();
        uint32_t voice = 0;
        if (info.Length() > 5) {
            voice = argVoiceIndex(info, 5);
        }
        Zic_Seq_Step& step = patterns[patternIndex].steps[voice][stepIndex];
        step.note = note;
        step.velocity = velocity;
        step.tie = tie;
    } catch (const Napi::Error& e) {
        e.ThrowAsJavaScriptException();
    }
    return env.Undefined();
}

Napi::Value getPattern(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    try {
        uint32_t patternIndex = argPatternIndex(info, 0);
        Napi::Object obj = Napi::Object::New(env);
        obj.Set("stepCount", Napi::Number::New(env, patterns[patternIndex].stepCount));
        Napi::Array steps = Napi::Array::New(env);
        for (uint32_t i = 0; i < patterns[patternIndex].stepCount; i++) {
            Napi::Array voices = Napi::Array::New(env);
            for (uint32_t j = 0; j < MAX_VOICES_IN_PATTERN; j++) {
                Napi::Object voice = Napi::Object::New(env);
                voice.Set("note", Napi::Number::New(env, patterns[patternIndex].steps[j][i].note));
                voice.Set("velocity", Napi::Number::New(env, patterns[patternIndex].steps[j][i].velocity));
                voice.Set("tie", Napi::Boolean::New(env, patterns[patternIndex].steps[j][i].tie));
                voices.Set(j, voice);
            }
            steps.Set(i, voices);
        }
        obj.Set("steps", steps);
        return obj;
    } catch (const Napi::Error& e) {
        e.ThrowAsJavaScriptException();
        return env.Undefined();
    }
    if (info.Length() < 1 || !info[0].IsNumber()) {
        throw Napi::Error::New(env, "Missing pattern index arguments");
    }
}

Napi::Value setSequencerState(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    try {
        if (info.Length() < 4 || !info[0].IsNumber() || !info[1].IsNumber() || !info[2].IsNumber()
            || !info[3].IsBoolean() || (info.Length() > 4 && !info[4].IsBoolean())) {
            throw Napi::Error::New(env, "Invalid arguments: trackIndex, patternIndex, detune, playing, (next=true)");
        }
        uint32_t trackIndex = argTrackIndex(info, 0);
        uint32_t patternIndex = argPatternIndex(info, 1);
        uint32_t detune = info[2].As<Napi::Number>().Uint32Value();
        bool playing = info[3].As<Napi::Boolean>().Value();
        bool next = true;
        if (info.Length() > 4) {
            next = info[4].As<Napi::Boolean>().Value();
        }
        Zic_Seq_Loop& looper = Zic_Audio_Tracks::getInstance().tracks[trackIndex]->looper;
        Zic_Seq_LoopState& state = next ? looper.nextState : looper.state;
        state.pattern = &patterns[patternIndex];
        state.detune = detune;
        state.playing = playing;
    } catch (const Napi::Error& e) {
        e.ThrowAsJavaScriptException();
    }
    return env.Undefined();
}

Napi::Value getSequencerStates(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    try {
        if (info.Length() < 1) {
            throw Napi::Error::New(env, "Missing trackIndex argument.");
        }
        uint32_t trackIndex = argTrackIndex(info, 0);

        Napi::Object states = Napi::Object::New(env);
        Zic_Seq_Loop& looper = Zic_Audio_Tracks::getInstance().tracks[trackIndex]->looper;
        Napi::Object state = Napi::Object::New(env);
        state.Set("patternIndex", looper.state.pattern ? Napi::Number::New(env, looper.state.pattern->id) : env.Null());
        state.Set("detune", Napi::Number::New(env, looper.state.detune));
        state.Set("playing", Napi::Boolean::New(env, looper.state.playing));
        states.Set("current", state);
        state = Napi::Object::New(env);
        state.Set("patternIndex", looper.nextState.pattern ? Napi::Number::New(env, looper.nextState.pattern->id) : env.Null());
        state.Set("detune", Napi::Number::New(env, looper.nextState.detune));
        state.Set("playing", Napi::Boolean::New(env, looper.nextState.playing));
        states.Set("next", state);
        return states;
    } catch (const Napi::Error& e) {
        e.ThrowAsJavaScriptException();
        return env.Undefined();
    }
}

Napi::Value trackNoteOn(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    try {
        if (info.Length() < 3 || !info[0].IsNumber() || !info[1].IsNumber()
            || !info[2].IsNumber() || (info.Length() > 3 && !info[3].IsNumber())) {
            throw Napi::Error::New(env, "Invalid arguments: trackIndex, note, velocity, (voice=0)");
        }
        uint32_t trackIndex = argTrackIndex(info, 0);
        uint32_t note = argNote(info, 1);
        uint32_t velocity = info[2].As<Napi::Number>().Uint32Value();
        uint32_t voice = 0;
        if (info.Length() > 3) {
            voice = argVoiceIndex(info, 3);
        }
        Zic_Audio_Tracks::getInstance().tracks[trackIndex]->noteOn(note, velocity, voice);
    } catch (const Napi::Error& e) {
        e.ThrowAsJavaScriptException();
    }
    return env.Undefined();
}

Napi::Value trackNoteOff(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    try {
        if (info.Length() < 2 || !info[0].IsNumber() || !info[1].IsNumber()
            || (info.Length() > 2 && !info[2].IsNumber())) {
            throw Napi::Error::New(env, "Invalid arguments: trackIndex, note, (voice=0)");
        }
        uint32_t trackIndex = argTrackIndex(info, 0);
        uint32_t note = argNote(info, 1);
        uint32_t voice = 0;
        if (info.Length() > 2) {
            voice = argVoiceIndex(info, 2);
        }
        Zic_Audio_Tracks::getInstance().tracks[trackIndex]->noteOff(note, voice);
    } catch (const Napi::Error& e) {
        e.ThrowAsJavaScriptException();
    }
    return env.Undefined();
}

Napi::Value trackCc(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    try {
        if (info.Length() < 3 || !info[0].IsNumber() || !info[1].IsNumber()
            || !info[2].IsNumber() || (info.Length() > 3 && !info[3].IsNumber())) {
            throw Napi::Error::New(env, "Invalid arguments: trackIndex, cc, value, (voice=0)");
        }
        uint32_t trackIndex = argTrackIndex(info, 0);
        uint32_t cc = info[1].As<Napi::Number>().Uint32Value();
        uint32_t value = info[2].As<Napi::Number>().Uint32Value();
        uint32_t voice = 0;
        if (info.Length() > 3) {
            voice = argVoiceIndex(info, 3);
        }
        Zic_Audio_Tracks::getInstance().tracks[trackIndex]->cc(cc, value, voice);
    } catch (const Napi::Error& e) {
        e.ThrowAsJavaScriptException();
    }
    return env.Undefined();
}

Napi::Value trackSetPath(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    try {
        if (info.Length() < 2 || !info[0].IsNumber() || !info[1].IsString()
            || (info.Length() > 2 && !info[2].IsNumber()) || (info.Length() > 3 && !info[3].IsNumber())) {
            throw Napi::Error::New(env, "Invalid arguments: trackIndex, path, (pathId=0), (voice=0)");
        }
        uint32_t trackIndex = argTrackIndex(info, 0);
        std::string path = info[1].As<Napi::String>().Utf8Value();
        uint32_t pathId = 0;
        if (info.Length() > 2) {
            pathId = info[2].As<Napi::Number>().Uint32Value();
        }
        uint32_t voice = 0;
        if (info.Length() > 3) {
            voice = argVoiceIndex(info, 3);
        }
        Zic_Audio_Tracks::getInstance().tracks[trackIndex]->setPath(path.c_str(), pathId, voice);
    } catch (const Napi::Error& e) {
        e.ThrowAsJavaScriptException();
    }
    return env.Undefined();
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set(Napi::String::New(env, "PATTERN_COUNT"), Napi::Number::New(env, ZIC_PATTERN_COUNT));
    exports.Set(Napi::String::New(env, "MAX_STEPS_IN_PATTERN"), Napi::Number::New(env, MAX_STEPS_IN_PATTERN));
    exports.Set(Napi::String::New(env, "MAX_VOICES_IN_PATTERN"), Napi::Number::New(env, MAX_VOICES_IN_PATTERN));
    exports.Set(Napi::String::New(env, "TRACK_COUNT"), Napi::Number::New(env, TRACK_COUNT));
    exports.Set(Napi::String::New(env, "SAMPLE_RATE"), Napi::Number::New(env, SAMPLE_RATE));
    exports.Set(Napi::String::New(env, "NOTE_START"), Napi::Number::New(env, Zic::_NOTE_START));
    exports.Set(Napi::String::New(env, "NOTE_END"), Napi::Number::New(env, Zic::_NOTE_END));

    exports.Set(Napi::String::New(env, "getAudoDeviceInfo"), Napi::Function::New(env, getAudoDeviceInfo));
    exports.Set(Napi::String::New(env, "start"), Napi::Function::New(env, start));
    exports.Set(Napi::String::New(env, "stop"), Napi::Function::New(env, stop));
    exports.Set(Napi::String::New(env, "isAudioRunning"), Napi::Function::New(env, isAudioRunning));
    exports.Set(Napi::String::New(env, "getBpm"), Napi::Function::New(env, getBpm));
    exports.Set(Napi::String::New(env, "setBpm"), Napi::Function::New(env, setBpm));
    exports.Set(Napi::String::New(env, "setPatternLength"), Napi::Function::New(env, setPatternLength));
    exports.Set(Napi::String::New(env, "getPatternLength"), Napi::Function::New(env, getPatternLength));
    exports.Set(Napi::String::New(env, "setPatternStep"), Napi::Function::New(env, setPatternStep));
    exports.Set(Napi::String::New(env, "getPattern"), Napi::Function::New(env, getPattern));
    exports.Set(Napi::String::New(env, "setSequencerState"), Napi::Function::New(env, setSequencerState));
    exports.Set(Napi::String::New(env, "getSequencerStates"), Napi::Function::New(env, getSequencerStates));
    exports.Set(Napi::String::New(env, "trackNoteOn"), Napi::Function::New(env, trackNoteOn));
    exports.Set(Napi::String::New(env, "trackNoteOff"), Napi::Function::New(env, trackNoteOff));
    exports.Set(Napi::String::New(env, "trackCc"), Napi::Function::New(env, trackCc));
    exports.Set(Napi::String::New(env, "trackSetPath"), Napi::Function::New(env, trackSetPath));
    exports.Set(Napi::String::New(env, "setOnBeatCallback"), Napi::Function::New(env, setOnBeatCallback));
    return exports;
}

NODE_API_MODULE(zic, Init)
