import { getAudoDeviceInfo, getBpm, safeStart, setBpm, start, promiseStart } from './lib';

console.log('Zic node');
console.log('getAudoDeviceInfo', getAudoDeviceInfo());

// // Bug most likely because of `start` being async
// console.log('Current Bpm', getBpm());
// setBpm(120);
// console.log('New Bpm', getBpm());

// // Using Napi::AsyncWorker
// async function main() {
//     const result = await promiseStart(123);
//     console.log('promise result', result);
// }
// main();

// // Using synchronous Napi::Function
// start(123);
// setInterval(() => {
//     // keep alive
// }, 1000);

// Using Napi::ThreadSafeFunction
safeStart((...values) => { console.log('Thread safe callback', values); }, 123);

// console.log('next Bpm', getBpm());

