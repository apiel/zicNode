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

Napi::Object getMidiDevices(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

    RtMidiIn midiIn;
    RtMidiOut midiOut;

    Napi::Object devices = Napi::Object::New(env);
    Napi::Array inDevices = Napi::Array::New(env);
    Napi::Array outDevices = Napi::Array::New(env);

    unsigned int inDeviceCount = midiIn.getPortCount();
    unsigned int outDeviceCount = midiOut.getPortCount();

    for (unsigned int i = 0; i < inDeviceCount; i++) {
        Napi::Object device = Napi::Object::New(env);
        device.Set("port", i);
        device.Set("name", midiIn.getPortName(i));
        inDevices.Set(i, device);
    }
    for (unsigned int i = 0; i < outDeviceCount; i++) {
        Napi::Object device = Napi::Object::New(env);
        device.Set("port", i);
        device.Set("name", midiOut.getPortName(i));
        outDevices.Set(i, device);
    }

    devices.Set("in", inDevices);
    devices.Set("out", outDevices);

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
        if (info.Length() < 6 || !info[0].IsNumber() || !info[1].IsNumber() || !info[2].IsNumber()
            || !info[3].IsNumber() || !info[4].IsBoolean() || !info[5].IsNumber() 
            || (info.Length() > 6 && !info[6].IsNumber())) {
            throw Napi::Error::New(env, "Invalid arguments: patternIndex, stepIndex, note, velocity, tie, (voice=0)");
        }
        uint32_t patternIndex = argPatternIndex(info, 0);
        uint32_t stepIndex = argStepIndex(info, 1);
        uint32_t note = argNote(info, 2);
        uint32_t velocity = info[3].As<Napi::Number>().Uint32Value();
        bool tie = info[4].As<Napi::Boolean>().Value();
        uint32_t patchId = info[5].As<Napi::Number>().Uint32Value();
        uint32_t voice = 0;
        if (info.Length() > 6) {
            voice = argVoiceIndex(info, 6);
        }
        Zic_Seq_Step& step = patterns[patternIndex].steps[voice][stepIndex];
        step.note = note;
        step.velocity = velocity;
        step.tie = tie;
        step.patchId = patchId;
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

Napi::Value setSequencerState(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    try {
        if (info.Length() < 3 || !info[0].IsNumber() || !info[1].IsNumber() || !info[2].IsBoolean()) {
            throw Napi::Error::New(env, "Invalid arguments: trackIndex, patternIndex, playing, options?: { next=true, dataId=0, detune=0 }");
        }
        uint32_t trackIndex = argTrackIndex(info, 0);
        uint32_t patternIndex = argPatternIndex(info, 1);
        bool playing = info[2].As<Napi::Boolean>().Value();
        uint32_t dataId = 0;
        uint32_t detune = 0;
        bool next = true;

        Zic_Seq_Loop& looper = Zic_Audio_Tracks::getInstance().tracks[trackIndex]->looper;
        Zic_Seq_LoopState& state = next ? looper.nextState : looper.state;

        if (info.Length() > 3) {
            Napi::Object options = info[3].As<Napi::Object>();
            if (options.Has("next")) {
                next = options.Get("next").As<Napi::Boolean>().Value();
            }
            if (options.Has("dataId")) {
                dataId = options.Get("dataId").As<Napi::Number>().Uint32Value();
            }
            if (options.Has("detune")) {
                detune = options.Get("detune").As<Napi::Number>().Uint32Value();
            }
        }
        state.pattern = &patterns[patternIndex];
        state.detune = detune;
        state.playing = playing;
        state.dataId = dataId;
    } catch (const Napi::Error& e) {
        e.ThrowAsJavaScriptException();
    }
    return env.Undefined();
}

Napi::Object __getSequencerStates(Napi::Env& env, uint32_t trackIndex)
{
    Napi::Object states = Napi::Object::New(env);
    Zic_Seq_Loop& looper = Zic_Audio_Tracks::getInstance().tracks[trackIndex]->looper;
    Napi::Object state = Napi::Object::New(env);
    state.Set("patternIndex", looper.state.pattern ? Napi::Number::New(env, looper.state.pattern->id) : env.Null());
    state.Set("detune", Napi::Number::New(env, looper.state.detune));
    state.Set("playing", Napi::Boolean::New(env, looper.state.playing));
    state.Set("dataId", Napi::Number::New(env, looper.state.dataId));
    states.Set("current", state);
    state = Napi::Object::New(env);
    state.Set("patternIndex", looper.nextState.pattern ? Napi::Number::New(env, looper.nextState.pattern->id) : env.Null());
    state.Set("detune", Napi::Number::New(env, looper.nextState.detune));
    state.Set("playing", Napi::Boolean::New(env, looper.nextState.playing));
    state.Set("dataId", Napi::Number::New(env, looper.nextState.dataId));
    states.Set("next", state);
    states.Set("currentStep", Napi::Number::New(env, looper.currentStep));
    return states;
}

Napi::Value getSequencerStates(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    try {
        if (info.Length() < 1) {
            throw Napi::Error::New(env, "Missing trackIndex argument.");
        }
        uint32_t trackIndex = argTrackIndex(info, 0);
        return __getSequencerStates(env, trackIndex);
    } catch (const Napi::Error& e) {
        e.ThrowAsJavaScriptException();
        return env.Undefined();
    }
}

Napi::Value getAllSequencerStates(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    try {
        Napi::Array states = Napi::Array::New(env);
        for (uint32_t i = 0; i < TRACK_COUNT; i++) {
            states.Set(i, __getSequencerStates(env, i));
        }
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

Napi::Value trackSetString(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    try {
        if (info.Length() < 2 || !info[0].IsNumber() || !info[1].IsString()
            || (info.Length() > 2 && !info[2].IsNumber())) {
            throw Napi::Error::New(env, "Invalid arguments: trackIndex, strValue, (stringId=0)");
        }
        uint32_t trackIndex = argTrackIndex(info, 0);
        std::string strValue = info[1].As<Napi::String>().Utf8Value();
        uint32_t stringId = 0;
        if (info.Length() > 2) {
            stringId = info[2].As<Napi::Number>().Uint32Value();
        }
        Zic_Audio_Tracks::getInstance().tracks[trackIndex]->setString(strValue.c_str(), stringId);
    } catch (const Napi::Error& e) {
        e.ThrowAsJavaScriptException();
    }
    return env.Undefined();
}

Napi::Value trackSetNumber(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    try {
        if (info.Length() < 2 || !info[0].IsNumber() || !info[1].IsNumber()
            || (info.Length() > 2 && !info[2].IsNumber())) {
            throw Napi::Error::New(env, "Invalid arguments: trackIndex, value, (floatId=0)");
        }
        uint32_t trackIndex = argTrackIndex(info, 0);
        float value = info[1].As<Napi::Number>().FloatValue();
        uint32_t floatId = 0;
        if (info.Length() > 2) {
            floatId = info[2].As<Napi::Number>().Uint32Value();
        }
        Zic_Audio_Tracks::getInstance().tracks[trackIndex]->setFloat(value, floatId);
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
    exports.Set(Napi::String::New(env, "start"), Napi::Function::New(env, start));
    exports.Set(Napi::String::New(env, "stop"), Napi::Function::New(env, stop));
    exports.Set(Napi::String::New(env, "isAudioRunning"), Napi::Function::New(env, isAudioRunning));
    exports.Set(Napi::String::New(env, "getBpm"), Napi::Function::New(env, getBpm));
    exports.Set(Napi::String::New(env, "setBpm"), Napi::Function::New(env, setBpm));
    exports.Set(Napi::String::New(env, "setPatch"), Napi::Function::New(env, setPatch));
    exports.Set(Napi::String::New(env, "setPatternLength"), Napi::Function::New(env, setPatternLength));
    exports.Set(Napi::String::New(env, "getPatternLength"), Napi::Function::New(env, getPatternLength));
    exports.Set(Napi::String::New(env, "setPatternStep"), Napi::Function::New(env, setPatternStep));
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
