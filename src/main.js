var addon = require('bindings')('zic');

console.log(addon.getAudoDeviceInfo());

const EventEmitter = require('events').EventEmitter;

const emitter = new EventEmitter();
emitter.on('data', (evt) => {
    console.log(evt);
});

emitter.on('start', () => {
    console.log('### START ...');
});

emitter.on('end', () => {
    console.log('### END ###');
});

addon.start(emitter.emit.bind(emitter));
