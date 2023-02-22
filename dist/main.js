"use strict";
var __createBinding = (this && this.__createBinding) || (Object.create ? (function(o, m, k, k2) {
    if (k2 === undefined) k2 = k;
    var desc = Object.getOwnPropertyDescriptor(m, k);
    if (!desc || ("get" in desc ? !m.__esModule : desc.writable || desc.configurable)) {
      desc = { enumerable: true, get: function() { return m[k]; } };
    }
    Object.defineProperty(o, k2, desc);
}) : (function(o, m, k, k2) {
    if (k2 === undefined) k2 = k;
    o[k2] = m[k];
}));
var __setModuleDefault = (this && this.__setModuleDefault) || (Object.create ? (function(o, v) {
    Object.defineProperty(o, "default", { enumerable: true, value: v });
}) : function(o, v) {
    o["default"] = v;
});
var __importStar = (this && this.__importStar) || function (mod) {
    if (mod && mod.__esModule) return mod;
    var result = {};
    if (mod != null) for (var k in mod) if (k !== "default" && Object.prototype.hasOwnProperty.call(mod, k)) __createBinding(result, mod, k);
    __setModuleDefault(result, mod);
    return result;
};
Object.defineProperty(exports, "__esModule", { value: true });
const process_1 = require("process");
const lib_1 = require("./lib");
const SynthDualOsc = __importStar(require("./Tracks/synthDualOsc"));
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
// console.log('getAudoDeviceInfo', getAudoDeviceInfo());
console.log('Current Bpm', (0, lib_1.getBpm)());
(0, lib_1.setBpm)(90);
console.log('New Bpm', (0, lib_1.getBpm)());
(0, lib_1.start)(123);
console.log('next Bpm', (0, lib_1.getBpm)());
console.log('master volume', (0, lib_1.getMasterVolume)());
(0, lib_1.trackSetString)(0, '/home/alex/Music/zicJs/zicNode/data/wavetables/0_test.wav');
(0, lib_1.trackSetString)(2, '/home/alex/Music/zicJs/zicNode/data/wavetables/0_test.wav', SynthDualOsc.StringId.oscWavetable);
(0, lib_1.trackSetString)(2, '/home/alex/Music/zicJs/zicNode/data/wavetables/0_test.wav', SynthDualOsc.StringId.osc2Wavetable);
// Kick
(0, lib_1.setPatternLength)(0, 4);
console.log('pattern length', (0, lib_1.getPatternLength)(0));
(0, lib_1.setPatternStep)(0, 0, 60, 100, false);
(0, lib_1.setSequencerState)(0, 0, true);
// PD
(0, lib_1.setPatternLength)(1, 8);
(0, lib_1.setPatternStep)(1, 2, 70, 100, false);
(0, lib_1.setPatternStep)(1, 6, 80, 100, false);
// console.log('pattern', JSON.stringify(getPattern(1), null, 4));
(0, lib_1.setSequencerState)(1, 1, true, { dataId: 23 });
// console.log('Get sequencer state track 2', getSequencerStates(1));
// Dual osc synth
(0, lib_1.setPatternLength)(2, 8);
(0, lib_1.setPatternStep)(2, 2, 75, 100, false);
(0, lib_1.setPatternStep)(2, 3, 76, 100, false);
(0, lib_1.setPatternStep)(2, 4, 77, 100, false);
// console.log('pattern', JSON.stringify(getPattern(1), null, 4));
(0, lib_1.setSequencerState)(2, 2, true, { dataId: 23 });
console.log('wavetable', (0, lib_1.getWavetable)('/home/alex/Music/zicJs/zicNode/data/wavetables/0_test.wav'));
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
// trackNoteOn(0, 60, 100);
// setTimeout(() => {
//     trackNoteOff(0, 60);
// }, 1000);
