#include <napi.h>
#include <RtAudio.h>

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
            // device.Set("sampleRates", rtInfo.sampleRates);
            device.Set("preferredSampleRate", rtInfo.preferredSampleRate);
            // device.Set("nativeFormats", rtInfo.nativeFormats);

            devices.Set(i, device);
        }
    }

    return devices;
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set(Napi::String::New(env, "getAudoDeviceInfo"), Napi::Function::New(env, getAudoDeviceInfo));
    return exports;
}

NODE_API_MODULE(hello, Init)

// typedef struct RtAudio::DeviceInfo {
//   bool probed;                           // true if the device capabilities were successfully probed.
//   std::string name;                      // Character string device identifier.
//   unsigned int outputChannels;           // Maximum output channels supported by device.
//   unsigned int inputChannels;            // Maximum input channels supported by device.
//   unsigned int duplexChannels;           // Maximum simultaneous input/output channels supported by device.
//   bool isDefaultOutput;                  // true if this is the default output device.
//   bool isDefaultInput;                   // true if this is the default input device.
//   std::vector<unsigned int> sampleRates; // Supported sample rates.
//   unsigned int preferredSampleRate;      // Preferred sample rate, e.g. for WASAPI the system sample rate.
//   RtAudioFormat nativeFormats;           // Bit mask of supported data formats.
// };
