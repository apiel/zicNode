#include <napi.h>

#include <RtAudio.h>
#include <iostream>

Napi::String getAudoDeviceInfo(const Napi::CallbackInfo& info)
{
    RtAudio audio;
    // Determine the number of devices available
    unsigned int devices = audio.getDeviceCount();
    // // Scan through devices for various capabilities
    RtAudio::DeviceInfo rtInfo;
    for (unsigned int i = 0; i < devices; i++) {
        rtInfo = audio.getDeviceInfo(i);
        if (rtInfo.probed == true) {
            // Print, for example, the maximum number of output channels for each device
            std::cout << "device = " << i;
            std::cout << ": name = " << rtInfo.name << " ";
            std::cout << ": maximum output channels = " << rtInfo.outputChannels << "\n";
        }
    }

    Napi::Env env = info.Env();
    return Napi::String::New(env, "world 2");
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set(Napi::String::New(env, "getAudoDeviceInfo"), Napi::Function::New(env, getAudoDeviceInfo));
    return exports;
}

NODE_API_MODULE(hello, Init)
