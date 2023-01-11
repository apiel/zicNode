#ifndef ZIC_SERVER_DEF_H_
#define ZIC_SERVER_DEF_H_

// TODO rename to ZIC_AUDIO_CHUNK
#define APP_AUDIO_CHUNK 512
// #define APP_AUDIO_CHUNK 2024

// TODO rename to ZIC_AUDIO_CHANNELS
#define APP_CHANNELS 1 // Might want to use only one channel
// #define APP_CHANNELS 2

// TODO rename to ZIC_AUDIO_SAMPLE_RATE
#ifndef SAMPLE_RATE
#define SAMPLE_RATE 44100
#endif

#ifndef APP_LOG
#define APP_LOG(...) printf(__VA_ARGS__)
#endif

#endif
