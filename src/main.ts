import { getAudoDeviceInfo, start, getCounter } from './lib';

console.log('Zic node');
console.log('getAudoDeviceInfo', getAudoDeviceInfo());

async function main() {
    const result = await start(123);
    console.log('promise result', result);
}
main();

// setInterval(() => {
//     console.log('counter', getCounter());
// }, 1000);
