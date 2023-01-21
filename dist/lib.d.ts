export declare const PATTERN_COUNT: number;
export declare const MAX_STEPS_IN_PATTERN: number;
export declare const MAX_VOICES_IN_PATTERN: number;
export declare const TRACK_COUNT: number;
export declare const SAMPLE_RATE: number;
export declare const NOTE_START: number;
export declare const NOTE_END: number;
interface AudioDeviceInfo {
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
export declare const getAudoDeviceInfo: () => AudioDeviceInfo[];
export declare const start: (deviceId?: number) => void;
export declare const stop: () => void;
export declare const isAudioRunning: () => boolean;
export declare const getBpm: () => number;
export declare const setBpm: (bpm: number) => void;
export declare const setPatternLength: (index: number, length: number) => void;
export declare const getPatternLength: (index: number) => number;
export declare const setPatternStep: (index: number, stepIndex: number, note: number, velocity: number, tie: boolean, voice?: number) => void;
interface PatternStep {
    note: number;
    velocity: number;
    tie: boolean;
}
export declare const getPattern: (index: number) => PatternStep[][];
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
export declare enum PdPathIds {
    Patch = 0
}
export declare enum SynthPathIds {
    Osc = 0,
    Lfo1 = 1,
    Lfo2 = 2
}
export declare const trackSetString: (trackIndex: number, value: string, stringId?: number, voice?: number) => void;
export declare const trackSetNumber: (trackIndex: number, value: number, numberId?: number, voice?: number) => void;
export declare const setOnBeatCallback: (callback: () => void) => void;
export declare const getMasterVolume: () => number;
export declare const setMasterVolume: (volume: number) => void;
export declare const getWavetable: (filepath: string, morph?: number) => number[];
export {};
//# sourceMappingURL=lib.d.ts.map