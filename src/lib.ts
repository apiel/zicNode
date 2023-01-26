import bindings from 'bindings';
import * as Kick23 from './Tracks/kick23';

export { Kick23 };

const zic = bindings('zic');

export const PATTERN_COUNT: number = zic.PATTERN_COUNT;
export const MAX_STEPS_IN_PATTERN: number = zic.MAX_STEPS_IN_PATTERN;
export const MAX_VOICES_IN_PATTERN: number = zic.MAX_VOICES_IN_PATTERN;
export const TRACK_COUNT: number = zic.TRACK_COUNT;
export const SAMPLE_RATE: number = zic.SAMPLE_RATE;
export const NOTE_START: number = zic.NOTE_START;
export const NOTE_END: number = zic.NOTE_END;

export const ZIC_PATCH_MAX_FLOATS: number = zic.ZIC_PATCH_MAX_FLOATS;
export const ZIC_PATCH_MAX_STRINGS: number = zic.ZIC_PATCH_MAX_STRINGS;
export const ZIC_PATCH_STRING_LENGTH: number = zic.ZIC_PATCH_STRING_LENGTH;
export const ZIC_PATCH_MAX_CC: number = zic.ZIC_PATCH_MAX_CC;

interface AudioDeviceInfo {
    probed: boolean; // true if the device capabilities were successfully probed.
    name: string; // Character string device identifier.
    outputChannels: number; // Maximum output channels supported by device.
    inputChannels: number; // Maximum input channels supported by device.
    duplexChannels: number; // Maximum simultaneous input/output channels supported by device.
    isDefaultOutput: boolean; // true if this is the default output device.
    isDefaultInput: boolean; // true if this is the default input device.
    sampleRates: number[]; // Supported sample rates.
    preferredSampleRate: number; // Preferred sample rate, e.g. for WASAPI the system sample rate.
    nativeFormats: number; // Bit mask of supported data formats.
}

export const getAudoDeviceInfo: () => AudioDeviceInfo[] = zic.getAudoDeviceInfo;
export const start: (deviceId?: number) => void = zic.start;
export const stop: () => void = zic.stop;
export const isAudioRunning: () => boolean = zic.isAudioRunning;
export const getBpm: () => number = zic.getBpm;
export const setBpm: (bpm: number) => void = zic.setBpm;
export const setPatternLength: (index: number, length: number) => void = zic.setPatternLength;
export const getPatternLength: (index: number) => number = zic.getPatternLength;
export const setPatternStep: (
    index: number,
    stepIndex: number,
    note: number,
    velocity: number,
    tie: boolean,
    voice?: number,
) => void = zic.setPatternStep;

interface PatternStep {
    note: number;
    velocity: number;
    tie: boolean;
}
export const getPattern: (index: number) => PatternStep[][] = zic.getPattern;

export interface SequencerPatch {
    strings: { [key: number]: string };
    floats: { [key: number]: number };
    cc: { [key: number]: number };
    id: number;
}

export const setSequencerState: (
    trackIndex: number,
    patternIndex: number,
    playing: boolean,
    options?: {
        detune?: number;
        next?: boolean; // false for current playing pattern, true for up coming pattern (wait till the end of it)
        dataId?: number; // Used to put arbitrary data reference in the sequencer state
        patch?: SequencerPatch;
    },
) => void = zic.setSequencerState;

export interface SequencerState {
    patternIndex: number;
    detune: number;
    playing: boolean;
    dataId: number; // Used to put arbitrary data reference in the sequencer state
    patch: SequencerPatch;
}

export interface SequencerStates {
    current: SequencerState;
    next: SequencerState;
    currentStep: number;
}
export const getSequencerStates: (trackIndex: number) => SequencerStates = zic.getSequencerStates;
export const getAllSequencerStates: () => SequencerStates[] = zic.getAllSequencerStates;

// trackIndex, note, velocity, (voice=0)
export const trackNoteOn: (
    trackIndex: number,
    note: number,
    velocity: number,
    voice?: number,
) => void = zic.trackNoteOn;
export const trackNoteOff: (trackIndex: number, note: number, voice?: number) => void =
    zic.trackNoteOff;
export const trackCc: (trackIndex: number, cc: number, value: number, voice?: number) => void =
    zic.trackCc;

export enum PdPathIds {
    Patch = 0,
}

export enum SynthPathIds {
    Osc = 0,
    Lfo1 = 1,
    Lfo2 = 2,
}

export const trackSetString: (trackIndex: number, value: string, stringId?: number) => void =
    zic.trackSetString;

export const trackSetNumber: (trackIndex: number, value: number, numberId?: number) => void =
    zic.trackSetNumber;

export const setOnBeatCallback: (callback: () => void) => void = zic.setOnBeatCallback;

export const getMasterVolume: () => number = zic.getMasterVolume;
export const setMasterVolume: (volume: number) => void = zic.setMasterVolume;

export interface Wavetable {
    data: number[];
    wavetableCount: number;
    wavetableSampleCount: number;
    totalSampleCount: number;
}
export const getWavetable: (filepath: string, morph?: number) => Wavetable = zic.getWavetable;
