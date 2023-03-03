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
var __exportStar = (this && this.__exportStar) || function(m, exports) {
    for (var p in m) if (p !== "default" && !Object.prototype.hasOwnProperty.call(exports, p)) __createBinding(exports, m, p);
};
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
exports.getWavetable = exports.setMasterVolume = exports.getMasterVolume = exports.setOnBeatCallback = exports.trackSetNumber = exports.trackSetString = exports.trackCc = exports.trackNoteOff = exports.trackNoteOn = exports.getAllSequencerStates = exports.getSequencerStates = exports.setSequencerState = exports.setPatch = exports.getPattern = exports.setPatternStep = exports.getPatternLength = exports.setPatternLength = exports.setBpm = exports.getBpm = exports.isAudioRunning = exports.stop = exports.start = exports.getAudioDeviceInfo = exports.ZIC_PATCH_MAX_CC = exports.ZIC_PATCH_STRING_LENGTH = exports.ZIC_PATCH_MAX_STRINGS = exports.ZIC_PATCH_MAX_FLOATS = exports.NOTE_END = exports.NOTE_START = exports.SAMPLE_RATE = exports.TRACK_COUNT = exports.MAX_VOICES_IN_PATTERN = exports.MAX_STEPS_IN_PATTERN = exports.PATCH_COUNT = exports.PATTERN_COUNT = exports.SynthDualOsc = exports.ZicSynth = exports.Kick23 = void 0;
const bindings_1 = __importDefault(require("bindings"));
const Kick23 = __importStar(require("./Tracks/kick23"));
exports.Kick23 = Kick23;
const ZicSynth = __importStar(require("./Tracks/zicSynth"));
exports.ZicSynth = ZicSynth;
const SynthDualOsc = __importStar(require("./Tracks/synthDualOsc"));
exports.SynthDualOsc = SynthDualOsc;
__exportStar(require("./Tracks/filter"), exports);
const zic = (0, bindings_1.default)('zic');
exports.PATTERN_COUNT = zic.PATTERN_COUNT;
exports.PATCH_COUNT = zic.PATCH_COUNT;
exports.MAX_STEPS_IN_PATTERN = zic.MAX_STEPS_IN_PATTERN;
exports.MAX_VOICES_IN_PATTERN = zic.MAX_VOICES_IN_PATTERN;
exports.TRACK_COUNT = zic.TRACK_COUNT;
exports.SAMPLE_RATE = zic.SAMPLE_RATE;
exports.NOTE_START = zic.NOTE_START;
exports.NOTE_END = zic.NOTE_END;
exports.ZIC_PATCH_MAX_FLOATS = zic.ZIC_PATCH_MAX_FLOATS;
exports.ZIC_PATCH_MAX_STRINGS = zic.ZIC_PATCH_MAX_STRINGS;
exports.ZIC_PATCH_STRING_LENGTH = zic.ZIC_PATCH_STRING_LENGTH;
exports.ZIC_PATCH_MAX_CC = zic.ZIC_PATCH_MAX_CC;
exports.getAudioDeviceInfo = zic.getAudioDeviceInfo;
exports.start = zic.start;
exports.stop = zic.stop;
exports.isAudioRunning = zic.isAudioRunning;
exports.getBpm = zic.getBpm;
exports.setBpm = zic.setBpm;
exports.setPatternLength = zic.setPatternLength;
exports.getPatternLength = zic.getPatternLength;
exports.setPatternStep = zic.setPatternStep;
exports.getPattern = zic.getPattern;
exports.setPatch = zic.setPatch;
exports.setSequencerState = zic.setSequencerState;
exports.getSequencerStates = zic.getSequencerStates;
exports.getAllSequencerStates = zic.getAllSequencerStates;
// trackIndex, note, velocity, (voice=0)
exports.trackNoteOn = zic.trackNoteOn;
exports.trackNoteOff = zic.trackNoteOff;
exports.trackCc = zic.trackCc;
exports.trackSetString = zic.trackSetString;
exports.trackSetNumber = zic.trackSetNumber;
exports.setOnBeatCallback = zic.setOnBeatCallback;
exports.getMasterVolume = zic.getMasterVolume;
exports.setMasterVolume = zic.setMasterVolume;
exports.getWavetable = zic.getWavetable;
