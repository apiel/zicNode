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
} from './lib';

console.log('Zic node');
console.log('getAudoDeviceInfo', getAudoDeviceInfo());

console.log('Current Bpm', getBpm());
setBpm(90);
console.log('New Bpm', getBpm());

start(123);

console.log('next Bpm', getBpm());

setPatternLength(0, 4);
console.log('pattern length', getPatternLength(0));
setPatternStep(0, 0, 60, 100, false);
setSequencerState(0, 0, 0, true);

setPatternLength(1, 8);
setPatternStep(1, 2, 70, 100, false);
setPatternStep(1, 6, 80, 100, false);
console.log('pattern', JSON.stringify(getPattern(1), null, 4));
setSequencerState(1, 1, 0, true);
console.log('Get sequencer state track 2', getSequencerStates(1));

try {
    console.log('pattern length', getPatternLength(2000));
} catch (error) {
    console.log('error', error);
}

setInterval(() => {
    if (!isAudioRunning()) {
        exit();
    }
    console.log('keep alive');
}, 1000);

trackNoteOn(0, 60, 100);
setTimeout(() => {
    trackNoteOff(0, 60);
}, 1000);
