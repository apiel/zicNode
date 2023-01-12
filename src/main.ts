import { exit } from 'process';
import { getAudoDeviceInfo, getBpm, isAudioRunning, setBpm, start } from './lib';

console.log('Zic node');
console.log('getAudoDeviceInfo', getAudoDeviceInfo());

console.log('Current Bpm', getBpm());
setBpm(90);
console.log('New Bpm', getBpm());

// Using synchronous Napi::Function
start(123);

console.log('next Bpm', getBpm());

setInterval(() => {
    if (!isAudioRunning()) {
        exit();
    }
    console.log('keep alive');
}, 1000);
