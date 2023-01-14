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
export declare const start: (deviceId: number) => void;
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
export declare const setSequencerState: (trackIndex: number, patternIndex: number, detune: number, playing: boolean, next?: boolean) => void;
export declare const getSequencerStates: (trackIndex: number) => void;
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
export declare const trackSetPath: (trackIndex: number, path: string, pathId?: number, // Multiple path can be set for a track, this is the id of path to assign
voice?: number) => void;
export {};
//# sourceMappingURL=lib.d.ts.map