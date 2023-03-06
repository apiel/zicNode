#ifndef ZIC_NODE_H_
#define ZIC_NODE_H_

#include <napi.h>

Napi::Value error(Napi::Env& env, const std::string& message)
{
    Napi::TypeError::New(env, message).ThrowAsJavaScriptException();
    return env.Undefined();
}

#endif