{
  "targets": [
    {
      "target_name": "zic",
      "cflags!": [
        "-fno-exceptions",
        '<!@(pkg-config sndfile --cflags --libs)',
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
        '<!@(pkg-config sndfile --cflags-only-I | sed s/-I//g)',
      ],
      'libraries': [
          '-lasound -lpthread',
          '<!@(pkg-config sndfile --cflags --libs)',
      ],
      'defines': [ 
        'NAPI_DISABLE_CPP_EXCEPTIONS', 
        'ZIC_USE_LIBSNDFILE=1',
        '__LINUX_ALSA__=1',
        '_REENTRANT=1',
      ],
    }
  ]
}
