"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
exports.setMasterVolume = exports.getMasterVolume = exports.setOnBeatCallback = exports.trackSetPath = exports.SynthPathIds = exports.PdPathIds = exports.trackCc = exports.trackNoteOff = exports.trackNoteOn = exports.getAllSequencerStates = exports.getSequencerStates = exports.setSequencerState = exports.getPattern = exports.setPatternStep = exports.getPatternLength = exports.setPatternLength = exports.setBpm = exports.getBpm = exports.isAudioRunning = exports.stop = exports.start = exports.getAudoDeviceInfo = exports.NOTE_END = exports.NOTE_START = exports.SAMPLE_RATE = exports.TRACK_COUNT = exports.MAX_VOICES_IN_PATTERN = exports.MAX_STEPS_IN_PATTERN = exports.PATTERN_COUNT = void 0;
const bindings_1 = __importDefault(require("bindings"));
const zic = (0, bindings_1.default)('zic');
exports.PATTERN_COUNT = zic.PATTERN_COUNT;
exports.MAX_STEPS_IN_PATTERN = zic.MAX_STEPS_IN_PATTERN;
exports.MAX_VOICES_IN_PATTERN = zic.MAX_VOICES_IN_PATTERN;
exports.TRACK_COUNT = zic.TRACK_COUNT;
exports.SAMPLE_RATE = zic.SAMPLE_RATE;
exports.NOTE_START = zic.NOTE_START;
exports.NOTE_END = zic.NOTE_END;
exports.getAudoDeviceInfo = zic.getAudoDeviceInfo;
exports.start = zic.start;
exports.stop = zic.stop;
exports.isAudioRunning = zic.isAudioRunning;
exports.getBpm = zic.getBpm;
exports.setBpm = zic.setBpm;
exports.setPatternLength = zic.setPatternLength;
exports.getPatternLength = zic.getPatternLength;
exports.setPatternStep = zic.setPatternStep;
exports.getPattern = zic.getPattern;
exports.setSequencerState = zic.setSequencerState;
exports.getSequencerStates = zic.getSequencerStates;
exports.getAllSequencerStates = zic.getAllSequencerStates;
// trackIndex, note, velocity, (voice=0)
exports.trackNoteOn = zic.trackNoteOn;
exports.trackNoteOff = zic.trackNoteOff;
exports.trackCc = zic.trackCc;
var PdPathIds;
(function (PdPathIds) {
    PdPathIds[PdPathIds["Patch"] = 0] = "Patch";
})(PdPathIds = exports.PdPathIds || (exports.PdPathIds = {}));
var SynthPathIds;
(function (SynthPathIds) {
    SynthPathIds[SynthPathIds["Osc"] = 0] = "Osc";
    SynthPathIds[SynthPathIds["Lfo1"] = 1] = "Lfo1";
    SynthPathIds[SynthPathIds["Lfo2"] = 2] = "Lfo2";
})(SynthPathIds = exports.SynthPathIds || (exports.SynthPathIds = {}));
exports.trackSetPath = zic.trackSetPath;
exports.setOnBeatCallback = zic.setOnBeatCallback;
exports.getMasterVolume = zic.getMasterVolume;
exports.setMasterVolume = zic.setMasterVolume;
