#ifndef ZIC_AUDIO_TRACK_MIDI_H_
#define ZIC_AUDIO_TRACK_MIDI_H_

#include "./zic_audio_track.h"

#include <RtMidi.h>
#include <zic_note.h>

class Zic_Audio_TrackMidi : public Zic_Audio_Track {
public:
    RtMidiOut* midiout = 0;
    uint8_t port = 0;

    Zic_Audio_TrackMidi(uint8_t _id = 0, const char* _name = NULL)
        : Zic_Audio_Track(_id, _name)
    {
    }

    ~Zic_Audio_TrackMidi()
    {
        delete midiout;
    }

    bool isAudioTrack() override
    {
        return false;
    }

    void noteOn(uint8_t note, uint8_t velocity, uint8_t voice)
    {
        // APP_LOG("MIDI note on %d %d %d\n", note, velocity, voice);
        std::vector<unsigned char> message;
        message.push_back(144); // + channel
        message.push_back(note);
        message.push_back(velocity);
        midiout->sendMessage(&message);
    }

    void noteOff(uint8_t note, uint8_t voice)
    {
        // APP_LOG("MIDI note off %d %d\n", note, voice);
        std::vector<unsigned char> message;
        message.push_back(128); // + channel
        message.push_back(note);
        message.push_back(0);
        midiout->sendMessage(&message);
    }

    void cc(uint8_t num, uint8_t val, uint8_t voice) override
    {
        // APP_LOG("MIDI cc %d %d %d\n", num, val, voice);
        std::vector<unsigned char> message;
        message.push_back(176); // + channel
        message.push_back(num);
        message.push_back(val);
        midiout->sendMessage(&message);
    }

    void sample(float* buf, int len)
    {
    }

    const char* getPatchDirectory() override
    {
        return "instruments/midi";
    }

    void setMidiOut(RtMidiOut* _midiout)
    {
        delete midiout;
        midiout = _midiout;
        setMidiOutPort(0);
    }

    void setMidiOutPort(int8_t direction)
    {
        int portCount = midiout->getPortCount();
        port = (port + direction + portCount) % portCount;
        midiout->closePort();
        midiout->openPort(port);
        printf("set MIDI out port %d\n", port);
    }
};

#endif