import * as Kick23 from './Tracks/kick23';
import * as ZicSynth from './Tracks/zicSynth';
import * as SynthDualOsc from './Tracks/synthDualOsc';
export * from './Tracks/filter';
export { Kick23, ZicSynth, SynthDualOsc };
export declare const PATTERN_COUNT: number;
export declare const PATCH_COUNT: number;
export declare const MAX_STEPS_IN_PATTERN: number;
export declare const MAX_VOICES_IN_PATTERN: number;
export declare const TRACK_COUNT: number;
export declare const SAMPLE_RATE: number;
export declare const NOTE_START: number;
export declare const NOTE_END: number;
export declare const ZIC_PATCH_MAX_FLOATS: number;
export declare const ZIC_PATCH_MAX_STRINGS: number;
export declare const ZIC_PATCH_STRING_LENGTH: number;
export declare const ZIC_PATCH_MAX_CC: number;
export interface AudioDeviceInfo {
    probed: boolean;
    name: string;
    outputChannels: number;
    inputChannels: number;
    duplexChannels: number;
    isDefaultOutput: boolean;
    isDefaultInput: boolean;
    sampleRates: number[];
    preferredSampleRate: number;
    nativeFormats: number;
}
export declare const getAudioDeviceInfo: () => AudioDeviceInfo[];
export interface MidiDeviceInfo {
    name: string;
    port: number;
}
export interface MidiDevices {
    input: MidiDeviceInfo[];
    output: MidiDeviceInfo[];
}
export declare const getMidiDevices: () => MidiDevices;
export interface MidiMessage {
    port: number;
    deltatime: number;
    message: number[];
}
export interface MidiError {
    port: number;
    error: string;
}
export declare const setMidiCallback: (callback: (data: MidiMessage | MidiError) => void) => void;
export declare const subscribeMidiInput: (inputPort: number, ignoreTypes?: {
    midiSysex: boolean;
    midiTime: boolean;
    midiSense: boolean;
}) => void;
export declare const unsubscribeMidiInput: (inputPort: number) => void;
export declare const closeMidiOutput: (outputPort: number) => void;
export declare const sendMidiMessage: (outputPort: number, message: number[]) => void;
export declare const start: (deviceId?: number) => void;
export declare const stop: () => void;
export declare const isAudioRunning: () => boolean;
export declare const getBpm: () => number;
export declare const setBpm: (bpm: number) => void;
export declare const setPatternLength: (index: number, length: number) => void;
export declare const getPatternLength: (index: number) => number;
export declare const setPatternStep: (index: number, stepIndex: number, note: number, velocity: number, tie: boolean, patchId: number, voice?: number) => void;
export interface PatternStep {
    note: number;
    velocity: number;
    tie: boolean;
}
export declare const getPattern: (index: number) => PatternStep[][];
export interface Patch {
    strings: {
        [key: number]: string;
    };
    floats: {
        [key: number]: number;
    };
    cc: {
        [key: number]: number;
    };
    id: number;
}
export declare const setPatch: (patch: Patch) => void;
export declare const setSequencerState: (trackIndex: number, patternIndex: number, playing: boolean, options?: {
    detune?: number;
    next?: boolean;
    dataId?: number;
}) => void;
export interface SequencerState {
    patternIndex: number;
    detune: number;
    playing: boolean;
    dataId: number;
}
export interface SequencerStates {
    current: SequencerState;
    next: SequencerState;
    currentStep: number;
}
export declare const getSequencerStates: (trackIndex: number) => SequencerStates;
export declare const getAllSequencerStates: () => SequencerStates[];
export declare const trackNoteOn: (trackIndex: number, note: number, velocity: number, voice?: number) => void;
export declare const trackNoteOff: (trackIndex: number, note: number, voice?: number) => void;
export declare const trackCc: (trackIndex: number, cc: number, value: number, voice?: number) => void;
export declare const trackSetString: (trackIndex: number, value: string, stringId?: number) => void;
export declare const trackSetNumber: (trackIndex: number, value: number, numberId?: number) => void;
export declare const setOnBeatCallback: (callback: () => void) => void;
export declare const getMasterVolume: () => number;
export declare const setMasterVolume: (volume: number) => void;
export interface Wavetable {
    data: number[];
    wavetableCount: number;
    wavetableSampleCount: number;
    totalSampleCount: number;
}
export declare const getWavetable: (filepath: string, morph?: number) => Wavetable;
//# sourceMappingURL=lib.d.ts.map