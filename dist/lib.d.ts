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
export declare const asyncStart: (n: number, cb: (err: any, result: any) => void) => void;
export declare const start: (arg1: number) => Promise<any>;
export {};
//# sourceMappingURL=lib.d.ts.map