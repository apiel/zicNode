import { getAudoDeviceInfo, start } from './lib';

console.log('getAudoDeviceInfo', getAudoDeviceInfo());

async function main() {
    const result = await start(123);
    console.log('promise result', result);
}
main();
console.log('end');
