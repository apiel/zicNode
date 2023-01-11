#ifndef ZIC_AUDIO_WAVETABLE_H_
#define ZIC_AUDIO_WAVETABLE_H_

#include <zic_wavetable_file.h>
#include "./zic_core_file.h"

#define ZIC_AUDIO_WAVETABLES_FOLDER "wavetables"

// TODO remove this, as file searching logic should be done in nodejs

class Zic_Audio_Wavetable : public Zic_Wavetable_File {
protected:
    void openCurrentFilename()
    {
        char filepath[270];
        snprintf(filepath, 270, "%s/%s", ZIC_AUDIO_WAVETABLES_FOLDER, filename);
        open(filepath);
        // printf("App_Wavetable::openCurrentFilename: %s\n", filepath);
    }

public:
    char filename[256] = "";

    Zic_Audio_Wavetable()
    {
        firstFile(filename, ZIC_AUDIO_WAVETABLES_FOLDER);
        openCurrentFilename();
    }

    Zic_Audio_Wavetable* setNext(int8_t direction = 0)
    {
        // TODO if direction > 1 then should jump to next letter
        direction = range(direction, -1, 1);
        nextFile(filename, ZIC_AUDIO_WAVETABLES_FOLDER, filename, direction);
        openCurrentFilename();
        return this;
    }
};

#endif