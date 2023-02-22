{
  "targets": [
    {
      "target_name": "zic",
      "cflags!": [
        "-fno-exceptions",
      ],
      "cflags_cc!": [ 
        "-fno-exceptions",
      ],
      "sources": [
        "src/zic_node.cc",
        "lib/rtaudio/RtAudio.cpp",
        "lib/rtmidi/RtMidi.cpp",
      ],
      "include_dirs": [
        "zic",
        "<!@(node -p \"require('node-addon-api').include\")",
        'lib/rtaudio',
        'lib/rtmidi',
        'lib/libsndfile/include',
      ],
      'libraries': [
          # '-lrtaudio',
          # '-lrtmidi',
          '-lasound -lpthread',
          '-lsndfile',
      ],
      'defines': [ 
        'NAPI_DISABLE_CPP_EXCEPTIONS', 
        'ZIC_USE_LIBSNDFILE=1',
        '__LINUX_ALSA__=1',
        #'__UNIX_JACK__=1',
        #'__LINUX_PULSE__=1',
        '_REENTRANT=1',
      ],
    }
  ]
}
