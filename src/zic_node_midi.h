#ifndef ZIC_NODE_MIDI_H_
#define ZIC_NODE_MIDI_H_

#include <RtMidi.h>
#include <napi.h>

#include "zic_node.h"

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

    devices.Set("input", inDevices);
    devices.Set("output", outDevices);

    return devices;
}

Napi::ThreadSafeFunction tsfnMidi;
void (*onMidi)(double deltatime, std::vector<unsigned char>* message, void* userData) = NULL;
void (*onMidiError)(RtMidiError::Type type, const std::string& errorText, void* userData) = NULL;

void midiCallback(double deltatime, std::vector<unsigned char>* message, void* userData)
{
    if (onMidi != NULL) {
        onMidi(deltatime, message, userData);
    }
}

void midiErrorCallback(RtMidiError::Type type, const std::string& errorText, void* userData)
{
    if (onMidiError != NULL) {
        onMidiError(type, errorText, userData);
    }
}

Napi::Value setMidiCallback(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    if (info.Length() < 1) {
        return error(env, "Invalid number of arguments: setMidiCallback(callback: () => void)");
    }
    if (!info[0].IsFunction()) {
        return error(env, "callback must be a function.");
    }
    tsfnMidi = Napi::ThreadSafeFunction::New(env, info[0].As<Napi::Function>(), "OnMidi", 0, 1);
    onMidi = [](double deltatime, std::vector<unsigned char>* message, void* userData) -> void {
        struct Data {
            double deltatime;
            std::vector<unsigned char>* message;
            uint32_t* port;
        };
        Data* data = new Data();
        data->deltatime = deltatime;
        data->message = message;
        data->port = (uint32_t*)userData;

        auto callback = [](Napi::Env env, Napi::Function jsCallback, Data* value) {
            Napi::Object obj = Napi::Object::New(env);
            obj.Set("deltatime", value->deltatime);
            obj.Set("port", *value->port);
            Napi::Array arr = Napi::Array::New(env);
            for (unsigned int i = 0; i < value->message->size(); i++) {
                arr.Set(i, value->message->at(i));
            }
            obj.Set("message", arr);
            jsCallback.Call({ obj });
            delete value;
        };
        tsfnMidi.BlockingCall(data, callback);
    };

    onMidiError = [](RtMidiError::Type type, const std::string& errorText, void* userData) -> void {
        struct  Data
        {
            RtMidiError::Type type;
            std::string errorText;
            uint32_t* port;
        };
        Data* data = new Data();
        data->type = type;
        data->errorText = errorText;
        data->port = (uint32_t*)userData;

        auto callback = [](Napi::Env env, Napi::Function jsCallback, Data* value) {
            Napi::Object obj = Napi::Object::New(env);
            // obj.Set("type", value->type);
            obj.Set("error", value->errorText);
            obj.Set("port", *value->port);
            jsCallback.Call({ obj });
            delete value;
        };
        tsfnMidi.BlockingCall(data, callback);
    };

    return env.Undefined();
}

struct MidiInput {
    RtMidiIn* midiin;
    uint32_t port;
};

std::vector<MidiInput*> midiInputs;

MidiInput* getMidiInput(uint32_t port)
{
    for (unsigned int i = 0; i < midiInputs.size(); i++) {
        if (midiInputs[i]->port == port) {
            return midiInputs[i];
        }
    }
    return NULL;
}

Napi::Value subscribeMidiInput(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    if (info.Length() < 1 || !info[0].IsNumber()) {
        return error(env, "Invalid arguments: listenMidi(inputPort: number, ignoreTypes?: {midiSysex: boolean, midiTime: boolean, midiSense: boolean})");
    }

    uint32_t port = info[0].As<Napi::Number>().Uint32Value();
    if (getMidiInput(port) != NULL) {
        // return error(env, "Already listening to this port.");
        return env.Undefined();
    }

    MidiInput* midiInput = new MidiInput();
    midiInputs.push_back(midiInput);
    midiInput->midiin = new RtMidiIn();
    midiInput->port = port;

    if (midiInput->port >= midiInput->midiin->getPortCount()) {
        return error(env, "Invalid port number.");
    }

    midiInput->midiin->openPort(midiInput->port);
    midiInput->midiin->setCallback(&midiCallback, &midiInput->port);
    midiInput->midiin->setErrorCallback(&midiErrorCallback, &midiInput->port);

    if (info.Length() > 1 && info[1].IsObject()) {
        Napi::Object ignoreTypes = info[1].As<Napi::Object>();
        midiInput->midiin->ignoreTypes(
            ignoreTypes.Get("midiSysex").As<Napi::Boolean>().Value(),
            ignoreTypes.Get("midiTime").As<Napi::Boolean>().Value(),
            ignoreTypes.Get("midiSense").As<Napi::Boolean>().Value());
    }

    return env.Undefined();
}

Napi::Value unsubscribeMidiInput(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    if (info.Length() < 1 || !info[0].IsNumber()) {
        return error(env, "Invalid arguments: closeMidi(inputPort: number)");
    }

    uint32_t port = info[0].As<Napi::Number>().Uint32Value();
    MidiInput* midiInput = getMidiInput(port);
    if (midiInput == NULL) {
        // return error(env, "Not listening to this port.");
        return env.Undefined();
    }

    midiInput->midiin->closePort();
    delete midiInput->midiin;
    midiInputs.erase(std::remove(midiInputs.begin(), midiInputs.end(), midiInput), midiInputs.end());
    delete midiInput;

    return env.Undefined();
}

struct MidiOutput {
    RtMidiOut* midiout;
    uint32_t port;
};

std::vector<MidiOutput*> midiOutputs;

MidiOutput* getMidiOutput(uint32_t port)
{
    for (unsigned int i = 0; i < midiOutputs.size(); i++) {
        if (midiOutputs[i]->port == port) {
            return midiOutputs[i];
        }
    }

    RtMidiOut* midiout = new RtMidiOut();
    if (port >= midiout->getPortCount()) {
        return NULL;
    }

    midiout->openPort(port);
    // midiout->setErrorCallback(&errorCallback, &port);

    MidiOutput* midiOutput = new MidiOutput();
    midiOutputs.push_back(midiOutput);
    midiOutput->midiout = midiout;
    midiOutput->port = port;

    return midiOutput;
}

Napi::Value closeMidiOutput(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    if (info.Length() < 1 || !info[0].IsNumber()) {
        return error(env, "Invalid arguments: closeMidiOutput(outputPort: number)");
    }

    uint32_t port = info[0].As<Napi::Number>().Uint32Value();
    MidiOutput* midiOutput = getMidiOutput(port);
    if (midiOutput == NULL) {
        return env.Undefined();
    }

    midiOutput->midiout->closePort();
    delete midiOutput->midiout;
    midiOutputs.erase(std::remove(midiOutputs.begin(), midiOutputs.end(), midiOutput), midiOutputs.end());
    delete midiOutput;

    return env.Undefined();
}

Napi::Value sendMidiMessage(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    if (info.Length() < 2 || !info[0].IsNumber() || !info[1].IsArray()) {
        return error(env, "Invalid arguments: sendMidiMessage(outputPort: number, message: number[])");
    }

    uint32_t port = info[0].As<Napi::Number>().Uint32Value();
    MidiOutput* midiOutput = getMidiOutput(port);
    if (midiOutput == NULL) {
        return env.Undefined();
    }

    Napi::Array arr = info[1].As<Napi::Array>();
    std::vector<unsigned char> message;
    for (unsigned int i = 0; i < arr.Length(); i++) {
        message.push_back(arr.Get(i).As<Napi::Number>().Uint32Value());
    }

    midiOutput->midiout->sendMessage(&message);

    return env.Undefined();
}

#endif
