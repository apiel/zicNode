"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.FilterNames = exports.FilterShortNames = exports.FilterFullNames = exports.FilterMode = void 0;
var FilterMode;
(function (FilterMode) {
    FilterMode[FilterMode["OFF"] = 0] = "OFF";
    FilterMode[FilterMode["LOWPASS_12"] = 1] = "LOWPASS_12";
    FilterMode[FilterMode["LOWPASS_24"] = 2] = "LOWPASS_24";
    FilterMode[FilterMode["HIGHPASS_12"] = 3] = "HIGHPASS_12";
    FilterMode[FilterMode["HIGHPASS_24"] = 4] = "HIGHPASS_24";
    FilterMode[FilterMode["BANDPASS_12"] = 5] = "BANDPASS_12";
    FilterMode[FilterMode["BANDPASS_24"] = 6] = "BANDPASS_24";
    FilterMode[FilterMode["LOWPASS_STATE_VARIABLE"] = 7] = "LOWPASS_STATE_VARIABLE";
    FilterMode[FilterMode["HIGHPASS_STATE_VARIABLE"] = 8] = "HIGHPASS_STATE_VARIABLE";
    FilterMode[FilterMode["BANDPASS_STATE_VARIABLE"] = 9] = "BANDPASS_STATE_VARIABLE";
    FilterMode[FilterMode["NOTCH_STATE_VARIABLE"] = 10] = "NOTCH_STATE_VARIABLE";
    FilterMode[FilterMode["COUNT"] = 11] = "COUNT";
})(FilterMode = exports.FilterMode || (exports.FilterMode = {}));
exports.FilterFullNames = [
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
exports.FilterShortNames = [
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
exports.FilterNames = [
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
