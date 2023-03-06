#ifndef ZIC_NODE_SEQUENCER_H_
#define ZIC_NODE_SEQUENCER_H_

#include <napi.h>

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

#endif
