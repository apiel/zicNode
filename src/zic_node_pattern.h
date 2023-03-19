#ifndef ZIC_NODE_PATTERN_H_
#define ZIC_NODE_PATTERN_H_

#include <napi.h>

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

Napi::Value cleanPatternStep(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    try {
        if (info.Length() < 2 || !info[0].IsNumber() || !info[1].IsNumber()
            || (info.Length() > 2 && !info[2].IsNumber())) {
            throw Napi::Error::New(env, "Invalid arguments: patternIndex, stepIndex, (voice=0)");
        }
        uint32_t patternIndex = argPatternIndex(info, 0);
        uint32_t stepIndex = argStepIndex(info, 1);
        uint32_t voice = 0;
        if (info.Length() > 2) {
            voice = argVoiceIndex(info, 2);
        }
        Zic_Seq_Step& step = patterns[patternIndex].steps[voice][stepIndex];
        step.note = 0;
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

#endif
