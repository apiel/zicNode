/*
 * Copyright (c) 2012 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/libpd/libpd for documentation
 *
 * This file was adapted from the ofxPd openFrameworks addon example:
 * https://github.com/danomatika/ofxPd
 *
 */
#pragma once

#include "PdBase.hpp"

#include <iostream>

// custom receiver class
class Zic_PdObject : public pd::PdReceiver, public pd::PdMidiReceiver {

public:
    Zic_PdObject(int id)
        : id(id)
    {
    }

    int id; //< instance identifier

    // pd message receiver callbacks
    void print(const std::string& message)
    {
        std::cout << "pd print (" << id << "): " << message << std::endl;
    }

    void receiveBang(const std::string& dest)
    {
        std::cout << "pd (" << id << "): bang " << dest << std::endl;
    }
    void receiveFloat(const std::string& dest, float num)
    {
        std::cout << "pd (" << id << "): float " << dest << ": " << num << std::endl;
    }
    void receiveSymbol(const std::string& dest, const std::string& symbol)
    {
        std::cout << "pd(" << id << "): symbol " << dest << ": " << symbol << std::endl;
    }
    void receiveList(const std::string& dest, const pd::List& list)
    {
        std::cout << "pd(" << id << "): list " << dest << ": " << list.toString() << " " << list.types() << std::endl;
    }

    void receiveMessage(const std::string& dest, const std::string& msg, const pd::List& list)
    {
        std::cout << "pd(" << id << "): message " << dest << ": " << msg << " " << list.toString() << " " << list.types() << std::endl;
    }

    // pd midi receiver callbacks
    void receiveNoteOn(const int channel, const int pitch, const int velocity)
    {
        std::cout << "pd(" << id << ") MIDI: note on: " << channel << " " << pitch << " " << velocity << std::endl;
    }

    void receiveControlChange(const int channel, const int controller, const int value)
    {
        std::cout << "pd(" << id << ") MIDI: control change: " << channel << " " << controller << " " << value << std::endl;
    }

    void receiveProgramChange(const int channel, const int value)
    {
        std::cout << "pd(" << id << ") MIDI: program change: " << channel << " " << value << std::endl;
    }

    void receivePitchBend(const int channel, const int value)
    {
        std::cout << "pd(" << id << ") MIDI: pitch bend: " << channel << " " << value << std::endl;
    }

    void receiveAftertouch(const int channel, const int value)
    {
        std::cout << "pd(" << id << ") MIDI: aftertouch: " << channel << " " << value << std::endl;
    }

    void receivePolyAftertouch(const int channel, const int pitch, const int value)
    {
        std::cout << "pd(" << id << ") MIDI: poly aftertouch: " << channel << " " << pitch << " " << value << std::endl;
    }

    void receiveMidiByte(const int port, const int byte)
    {
        std::cout << "pd(" << id << ") MIDI: midi byte: " << port << " " << byte << std::endl;
    }
};
