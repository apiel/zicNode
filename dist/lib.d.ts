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
export declare const start: (arg1: number) => Promise<any>;
export declare const getCounter: () => void;
export {};
//# sourceMappingURL=lib.d.ts.map