interface AudioDeviceInfo {
    probed: boolean;
    name: string;
    outputChannels: number;
    inputChannels: number;
    duplexChannels: number;
    isDefaultOutput: boolean;
    isDefaultInput: boolean;
    sampleRates: number[];
    preferredSampleRate: number;
    nativeFormats: number;
}
export declare const getAudoDeviceInfo: () => AudioDeviceInfo[];
export declare const asyncStart: (deviceId: number, cb: (err: any, result: any) => void) => void;
export declare const promiseStart: (arg1: number) => Promise<any>;
export declare const start: (deviceId: number) => void;
export declare const safeStart: (cb: () => void, deviceId: number) => void;
export declare const getBpm: () => number;
export declare const setBpm: (bpm: number) => void;
export declare const getCounter: () => number;
export {};
//# sourceMappingURL=lib.d.ts.map