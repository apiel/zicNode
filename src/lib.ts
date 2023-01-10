import bindings from 'bindings';

interface AudioDeviceInfo {
    probed: boolean;             // true if the device capabilities were successfully probed.
    name: string;                // Character string device identifier.
    outputChannels: number;      // Maximum output channels supported by device.
    inputChannels: number;       // Maximum input channels supported by device.
    duplexChannels: number;      // Maximum simultaneous input/output channels supported by device.
    isDefaultOutput: boolean;    // true if this is the default output device.
    isDefaultInput: boolean;     // true if this is the default input device.
    sampleRates: number[];       // Supported sample rates.
    preferredSampleRate: number; // Preferred sample rate, e.g. for WASAPI the system sample rate.
    nativeFormats: number;       // Bit mask of supported data formats.
}

export const getAudoDeviceInfo: () => AudioDeviceInfo[] = bindings('zic').getAudoDeviceInfo;
export const start: (n: number, cb: (err: any, result: any) => void) => void = bindings('zic').start;
