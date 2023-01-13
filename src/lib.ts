import bindings from 'bindings';

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

export const getAudoDeviceInfo: () => AudioDeviceInfo[] = bindings('zic').getAudoDeviceInfo;
export const start: (deviceId: number) => void = bindings('zic').start;
export const stop: () => void = bindings('zic').stop;
export const isAudioRunning: () => boolean = bindings('zic').isAudioRunning;
export const getBpm: () => number = bindings('zic').getBpm;
export const setBpm: (bpm: number) => void = bindings('zic').setBpm;
export const setPatternLength: (index: number, length: number) => void =
    bindings('zic').setPatternLength;
export const getPatternLength: (index: number) => number = bindings('zic').getPatternLength;
export const setPatternStep: (
    index: number,
    stepIndex: number,
    note: number,
    velocity: number,
    tie: boolean,
    voice?: number,
) => void = bindings('zic').setPatternStep;

interface PatternStep {
    note: number;
    velocity: number;
    tie: boolean;
}
export const getPattern: (index: number) => PatternStep[][] = bindings('zic').getPattern;
export const setSequencerState: (
    trackIndex: number,
    patternIndex: number,
    detune: number,
    playing: boolean,
    next?: boolean, // false for current playing pattern, true for up coming pattern (wait till the end of it)
) => void = bindings('zic').setSequencerState;
export const getSequencerStates: (trackIndex: number) => void = bindings('zic').getSequencerStates;

// trackIndex, note, velocity, (voice=0)
export const trackNoteOn: (
    trackIndex: number,
    note: number,
    velocity: number,
    voice?: number,
) => void = bindings('zic').trackNoteOn;
export const trackNoteOff: (trackIndex: number, note: number, voice?: number) => void =
    bindings('zic').trackNoteOff;
export const trackCc: (trackIndex: number, cc: number, value: number, voice?: number) => void =
    bindings('zic').trackCc;
export const trackSetPath: (
    trackIndex: number,
    path: string,
    pathId?: number, // Multiple path can be set for a track, this is the id of path to assign
    voice?: number,
) => void = bindings('zic').trackSetPath;
