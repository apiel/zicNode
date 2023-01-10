import { getAudoDeviceInfo, start } from './lib';
import { EventEmitter } from 'events';

console.log('getAudoDeviceInfo', getAudoDeviceInfo());

const emitter = new EventEmitter();
emitter.on('data', (evt) => {
    console.log(evt);
});

start(emitter.emit.bind(emitter));

console.log('end');
