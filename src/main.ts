import { exit } from 'process';
import {
    getAudoDeviceInfo,
    getBpm,
    getPattern,
    getPatternLength,
    getSequencerStates,
    isAudioRunning,
    setBpm,
    setPatternLength,
    setPatternStep,
    setSequencerState,
    start,
    trackNoteOff,
    trackNoteOn,
    trackSetString,
    PATTERN_COUNT,
    MAX_STEPS_IN_PATTERN,
    MAX_VOICES_IN_PATTERN,
    TRACK_COUNT,
    SAMPLE_RATE,
    NOTE_START,
    NOTE_END,
    setOnBeatCallback,
    getMasterVolume,
    getWavetable,
} from './lib';

import * as SynthDualOsc from './Tracks/synthDualOsc';

console.log('Zic node');

// export const PATTERN_COUNT: number = zic.PATTERN_COUNT;
// export const MAX_STEPS_IN_PATTERN: number = zic.MAX_STEPS_IN_PATTERN;
// export const MAX_VOICES_IN_PATTERN: number = zic.MAX_VOICES_IN_PATTERN;
// export const TRACK_COUNT: number = zic.TRACK_COUNT;
// export const SAMPLE_RATE: number = zic.SAMPLE_RATE;
// export const NOTE_START: number = zic.NOTE_START;
// export const NOTE_END: number = zic.NOTE_END;

console.log('DEFINE VAR', {
    PATTERN_COUNT,
    MAX_STEPS_IN_PATTERN,
    MAX_VOICES_IN_PATTERN,
    TRACK_COUNT,
    SAMPLE_RATE,
    NOTE_START,
    NOTE_END,
});

// console.log('getAudoDeviceInfo', getAudoDeviceInfo());

console.log('Current Bpm', getBpm());
setBpm(90);
console.log('New Bpm', getBpm());

start(123);

console.log('next Bpm', getBpm());

console.log('master volume', getMasterVolume());

trackSetString(0, '/home/alex/Music/zicJs/zicNode/data/wavetables/0_test.wav');
trackSetString(2, '/home/alex/Music/zicJs/zicNode/data/wavetables/0_test.wav', SynthDualOsc.StringId.oscWavetable);
trackSetString(2, '/home/alex/Music/zicJs/zicNode/data/wavetables/0_test.wav', SynthDualOsc.StringId.osc2Wavetable);

// Kick
setPatternLength(0, 4);
console.log('pattern length', getPatternLength(0));
setPatternStep(0, 0, 60, 100, false);
setSequencerState(0, 0, true);

// PD
setPatternLength(1, 8);
setPatternStep(1, 2, 70, 100, false);
setPatternStep(1, 6, 80, 100, false);
// console.log('pattern', JSON.stringify(getPattern(1), null, 4));
setSequencerState(1, 1, true, { dataId: 23 });
// console.log('Get sequencer state track 2', getSequencerStates(1));

// Dual osc synth
setPatternLength(2, 8);
setPatternStep(2, 2, 75, 100, false);
setPatternStep(2, 3, 76, 100, false);
setPatternStep(2, 4, 77, 100, false);
// console.log('pattern', JSON.stringify(getPattern(1), null, 4));
setSequencerState(2, 2, true, { dataId: 23 });

console.log('wavetable', getWavetable('/home/alex/Music/zicJs/zicNode/data/wavetables/0_test.wav'));

// setOnBeatCallback(() => {
//     console.log('new beat from C++', getSequencerStates(0));
// });

// Test error handling
// try {
//     console.log('pattern length', getPatternLength(2000));
// } catch (error) {
//     console.log('error', error);
// }

setInterval(() => {
    if (!isAudioRunning()) {
        exit();
    }
    console.log('keep alive');
}, 1000);

// trackNoteOn(0, 60, 100);
// setTimeout(() => {
//     trackNoteOff(0, 60);
// }, 1000);
