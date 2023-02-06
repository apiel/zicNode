export enum FilterMode {
    OFF,
    LOWPASS_12,
    LOWPASS_24,
    HIGHPASS_12,
    HIGHPASS_24,
    BANDPASS_12,
    BANDPASS_24,
    LOWPASS_STATE_VARIABLE,
    HIGHPASS_STATE_VARIABLE,
    BANDPASS_STATE_VARIABLE,
    NOTCH_STATE_VARIABLE,
    COUNT,
}

export const FilterFullNames = [
    "Off",
    "Lowpass 12dB",
    "Lowpass 24dB",
    "Highpass 12dB",
    "Highpass 24dB",
    "Bandpass 12dB",
    "Bandpass 24dB",
    "Lowpass State Variable",
    "Highpass State Variable",
    "Bandpass State Variable",
    "Notch State Variable",
];

export const FilterShortNames = [
    "Off",
    "LP12",
    "LP24",
    "HP12",
    "HP24",
    "BP12",
    "BP24",
    "LP SV",
    "HP SV",
    "BP SV",
    "Notch SV",
];

export const FilterNames = [
    "Off",
    "Lowpass 12dB",
    "Lowpass 24dB",
    "Highpass 12dB",
    "Highpass 24dB",
    "Bandpass 12dB",
    "Bandpass 24dB",
    "Lowpass SV",
    "Highpass SV",
    "Bandpass SV",
    "Notch SV",
];
