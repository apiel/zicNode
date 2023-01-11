import { getAudoDeviceInfo, start, getCounter } from './lib';
import { EventEmitter } from 'events';

console.log('Zic node');
console.log('getAudoDeviceInfo', getAudoDeviceInfo());

async function main() {
    const result = await start(123);
    console.log('promise result', result);
}
main();

const emitter = new EventEmitter();
emitter.on('data', (evt) => {
    console.log(evt);
});


// setEmitter(emitter.emit.bind(emitter));

setInterval(() => {
    console.log('counter', getCounter());
}, 1000);
