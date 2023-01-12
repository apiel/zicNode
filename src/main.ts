import { exit } from 'process';
import { getAudoDeviceInfo, getBpm, getPatternLength, isAudioRunning, setBpm, setPatternLength, start } from './lib';

console.log('Zic node');
console.log('getAudoDeviceInfo', getAudoDeviceInfo());

console.log('Current Bpm', getBpm());
setBpm(90);
console.log('New Bpm', getBpm());

// Using synchronous Napi::Function
start(123);

console.log('next Bpm', getBpm());

setPatternLength(0, 16);
console.log('pattern length', getPatternLength(0));
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
