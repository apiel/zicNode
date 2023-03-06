#ifndef ZIC_NODE_TRACK_H_
#define ZIC_NODE_TRACK_H_

#include <napi.h>

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

#endif
