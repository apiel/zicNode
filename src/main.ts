import { getAudoDeviceInfo, start } from './lib';

console.log('getAudoDeviceInfo', getAudoDeviceInfo());

function done(err: any, result: any) {
    console.log('done', { err, result });
}
console.log('start', start(123, done));
console.log('end');
