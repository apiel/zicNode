#ifndef ZIC_AUDIO_WAVETABLE_H_
#define ZIC_AUDIO_WAVETABLE_H_

#include <zic_wavetable_file.h>
// #include "./zic_core_file.h"

#define ZIC_AUDIO_WAVETABLES_FOLDER "wavetables"

// TODO remove this, as file searching logic should be done in nodejs

class Zic_Audio_Wavetable : public Zic_Wavetable_File {
public:
    Zic_Audio_Wavetable()
    {
        open("/home/alex/Music/zicJs/zicNode/data/wavetables/ADDITIVE.WAV");
    }
};

#endif