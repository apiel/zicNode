{
  "targets": [
    {
      "target_name": "zic",
      "cflags!": [
        "-fno-exceptions",
        '<!@(pkg-config sndfile --cflags --libs)'
      ],
      "cflags_cc!": [ 
        "-fno-exceptions",
      ],
      "sources": [ "src/zic_node.cc" ],
      "include_dirs": [
        "zic",
        "<!@(node -p \"require('node-addon-api').include\")",
        'lib/rtaudio',
        'lib/rtmidi',
        '<!@(pkg-config sndfile --cflags-only-I | sed s/-I//g)'
      ],
      'libraries': [
          '-lrtaudio',
          '-lrtmidi',
          '<!@(pkg-config sndfile --cflags --libs)'
      ],
      'defines': [ 
        'NAPI_DISABLE_CPP_EXCEPTIONS', 
        'ZIC_USE_LIBSNDFILE=1',
        '__LINUX_ALSA__=1',
        '__UNIX_JACK__=1',
        '__LINUX_PULSE__=1',
        '_REENTRANT=1',
      ],
    }
  ]
}
