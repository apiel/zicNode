"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const process_1 = require("process");
const lib_1 = require("./lib");
console.log('Zic node');
// export const PATTERN_COUNT: number = zic.PATTERN_COUNT;
// export const MAX_STEPS_IN_PATTERN: number = zic.MAX_STEPS_IN_PATTERN;
// export const MAX_VOICES_IN_PATTERN: number = zic.MAX_VOICES_IN_PATTERN;
// export const TRACK_COUNT: number = zic.TRACK_COUNT;
// export const SAMPLE_RATE: number = zic.SAMPLE_RATE;
// export const NOTE_START: number = zic.NOTE_START;
// export const NOTE_END: number = zic.NOTE_END;
console.log('DEFINE VAR', {
    PATTERN_COUNT: lib_1.PATTERN_COUNT,
    MAX_STEPS_IN_PATTERN: lib_1.MAX_STEPS_IN_PATTERN,
    MAX_VOICES_IN_PATTERN: lib_1.MAX_VOICES_IN_PATTERN,
    TRACK_COUNT: lib_1.TRACK_COUNT,
    SAMPLE_RATE: lib_1.SAMPLE_RATE,
    NOTE_START: lib_1.NOTE_START,
    NOTE_END: lib_1.NOTE_END,
});
console.log('getAudoDeviceInfo', (0, lib_1.getAudoDeviceInfo)());
console.log('Current Bpm', (0, lib_1.getBpm)());
(0, lib_1.setBpm)(90);
console.log('New Bpm', (0, lib_1.getBpm)());
(0, lib_1.start)(123);
console.log('next Bpm', (0, lib_1.getBpm)());
console.log('master volume', (0, lib_1.getMasterVolume)());
(0, lib_1.trackSetPath)(0, '/home/alex/Music/zicJs/zicNode/data/wavetables/bank.wav');
(0, lib_1.trackSetPath)(1, '/home/alex/Music/zicJs/zicNode/data/instruments/pd/01_synth');
(0, lib_1.trackSetPath)(2, '/home/alex/Music/zicJs/zicNode/data/wavetables/0_test.wav', lib_1.SynthPathIds.Osc);
(0, lib_1.trackSetPath)(2, '/home/alex/Music/zicJs/zicNode/data/wavetables/0_test.wav', lib_1.SynthPathIds.Lfo1);
(0, lib_1.trackSetPath)(2, '/home/alex/Music/zicJs/zicNode/data/wavetables/0_test.wav', lib_1.SynthPathIds.Lfo2);
(0, lib_1.setPatternLength)(0, 4);
console.log('pattern length', (0, lib_1.getPatternLength)(0));
(0, lib_1.setPatternStep)(0, 0, 60, 100, false);
(0, lib_1.setSequencerState)(0, 0, true);
(0, lib_1.setPatternLength)(1, 8);
(0, lib_1.setPatternStep)(1, 2, 70, 100, false);
(0, lib_1.setPatternStep)(1, 6, 80, 100, false);
// console.log('pattern', JSON.stringify(getPattern(1), null, 4));
(0, lib_1.setSequencerState)(1, 1, true, { dataId: 23 });
// console.log('Get sequencer state track 2', getSequencerStates(1));
(0, lib_1.setPatternLength)(2, 8);
(0, lib_1.setPatternStep)(2, 2, 75, 100, false);
(0, lib_1.setPatternStep)(2, 3, 76, 100, false);
(0, lib_1.setPatternStep)(2, 4, 77, 100, false);
// console.log('pattern', JSON.stringify(getPattern(1), null, 4));
(0, lib_1.setSequencerState)(2, 2, true, { dataId: 23 });
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
    if (!(0, lib_1.isAudioRunning)()) {
        (0, process_1.exit)();
    }
    console.log('keep alive');
}, 1000);
(0, lib_1.trackNoteOn)(0, 60, 100);
setTimeout(() => {
    (0, lib_1.trackNoteOff)(0, 60);
}, 1000);
