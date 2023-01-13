#ifndef ZIC_NODE_ARGS_H_
#define ZIC_NODE_ARGS_H_

#include <napi.h>

#include "zic_server_audio.h"

uint32_t getPatternIndex(const Napi::CallbackInfo& info, uint32_t pos)
{
    Napi::Env env = info.Env();
    if (!info[pos].IsNumber()) {
        throw Napi::Error::New(env, "Pattern index must be a number");
    }
    uint32_t patternIndex = info[pos].As<Napi::Number>().Uint32Value();
    if (patternIndex > ZIC_PATTERN_COUNT - 1) {
        throw Napi::Error::New(env, "Pattern index out of range, max: " + std::to_string(ZIC_PATTERN_COUNT - 1));
    }
    return patternIndex;
}

uint32_t getStepIndex(const Napi::CallbackInfo& info, uint32_t pos)
{
    Napi::Env env = info.Env();
    if (!info[pos].IsNumber()) {
        throw Napi::Error::New(env, "Step index must be a number");
    }
    uint32_t stepIndex = info[pos].As<Napi::Number>().Uint32Value();
    if (stepIndex > MAX_STEPS_IN_PATTERN - 1) {
        throw Napi::Error::New(env, "Step index out of range, max: " + std::to_string(MAX_STEPS_IN_PATTERN - 1));
    }
    return stepIndex;
}

uint32_t getVoiceIndex(const Napi::CallbackInfo& info, uint32_t pos)
{
    Napi::Env env = info.Env();
    if (!info[pos].IsNumber()) {
        throw Napi::Error::New(env, "Voice index must be a number");
    }
    uint32_t voiceIndex = info[pos].As<Napi::Number>().Uint32Value();
    if (voiceIndex > MAX_VOICES_IN_PATTERN - 1) {
        throw Napi::Error::New(env, "Voice index out of range, max: " + std::to_string(MAX_VOICES_IN_PATTERN - 1));
    }
    return voiceIndex;
}

uint32_t getNote(const Napi::CallbackInfo& info, uint32_t pos)
{
    Napi::Env env = info.Env();
    if (!info[pos].IsNumber()) {
        throw Napi::Error::New(env, "Note must be a number");
    }
    uint32_t note = info[pos].As<Napi::Number>().Uint32Value();
    if (note > Zic::_NOTE_END || note < Zic::_NOTE_START) {
        throw Napi::Error::New(env, "Note out of range (" + std::to_string(Zic::_NOTE_START) + "-" + std::to_string(Zic::_NOTE_END) + ")");
    }
    return note;
}

#endif
