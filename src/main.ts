import { getAudoDeviceInfo, getBpm, setBpm, start } from './lib';

console.log('Zic node');
console.log('getAudoDeviceInfo', getAudoDeviceInfo());

// // Bug most likely because of `start` being async
// console.log('Current Bpm', getBpm());
// setBpm(120);
// console.log('New Bpm', getBpm());

async function main() {
    const result = await start(123);
    console.log('promise result', result);
}
main();

