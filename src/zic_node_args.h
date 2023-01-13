#ifndef ZIC_NODE_ARGS_H_
#define ZIC_NODE_ARGS_H_

#include <napi.h>

#include "zic_server_audio.h"

uint32_t getArgsInRange(const Napi::CallbackInfo& info, uint32_t pos, const std::string& name, uint32_t min, uint32_t max)
{
    Napi::Env env = info.Env();
    if (!info[pos].IsNumber()) {
        throw Napi::Error::New(env, name + " must be a number");
    }
    uint32_t value = info[pos].As<Napi::Number>().Uint32Value();
    if (value < min || value > max) {
        throw Napi::Error::New(env, name + " out of range , min: " + std::to_string(min) + ", max: " + std::to_string(max));
    }
    return value;
}

uint32_t argPatternIndex(const Napi::CallbackInfo& info, uint32_t pos)
{
    return getArgsInRange(info, pos, "Pattern index", 0, ZIC_PATTERN_COUNT - 1);
}

uint32_t argStepIndex(const Napi::CallbackInfo& info, uint32_t pos)
{
    return getArgsInRange(info, pos, "Step index", 0, MAX_STEPS_IN_PATTERN - 1);
}

uint32_t argVoiceIndex(const Napi::CallbackInfo& info, uint32_t pos)
{
    return getArgsInRange(info, pos, "Voice index", 0, MAX_VOICES_IN_PATTERN - 1);
}

uint32_t argTrackIndex(const Napi::CallbackInfo& info, uint32_t pos)
{
    return getArgsInRange(info, pos, "Track index", 0, TRACK_COUNT - 1);
}

uint32_t argNote(const Napi::CallbackInfo& info, uint32_t pos)
{
    return getArgsInRange(info, pos, "Note", Zic::_NOTE_START, Zic::_NOTE_END);
}

#endif
