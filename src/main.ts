import { getAudoDeviceInfo, getBpm, setBpm, start } from './lib';

console.log('Zic node');
console.log('getAudoDeviceInfo', getAudoDeviceInfo());

// // Bug most likely because of `start` being async
// console.log('Current Bpm', getBpm());
// setBpm(120);
// console.log('New Bpm', getBpm());

// https://stackoverflow.com/questions/58960713/how-to-use-napi-threadsafe-function-for-nodejs-native-addon
// https://github.com/nodejs/node-addon-api/blob/main/doc/threadsafe_function.md

// async function main() {
//     const result = await start(123);
//     console.log('promise result', result);
// }
// main();

start(123);

setInterval(() => {
    // keep alive
}, 1000);
