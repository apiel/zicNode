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
export {};
//# sourceMappingURL=lib.d.ts.map