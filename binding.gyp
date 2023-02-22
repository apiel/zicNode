{
  "targets": [
    {
      "target_name": "zic",
      "cflags!": [
        "-fno-exceptions",
        '<!@(pkg-config rtaudio --cflags --libs)',
        '<!@(pkg-config rtmidi --cflags --libs)',
        '<!@(pkg-config sndfile --cflags --libs)'
      ],
      "cflags_cc!": [ 
        "-fno-exceptions",
        # '-DPDINSTANCE'
        # '<!@(pkg-config rtaudio --cflags --libs)'
      ],
      "sources": [ "src/zic_node.cc" ],
      "include_dirs": [
        "zic",
        "<!@(node -p \"require('node-addon-api').include\")",
        '<!@(pkg-config rtaudio --cflags-only-I | sed s/-I//g)',
        '<!@(pkg-config rtmidi --cflags-only-I | sed s/-I//g)',
        '<!@(pkg-config sndfile --cflags-only-I | sed s/-I//g)'
      ],
      'libraries': [
          # '-lrtaudio'
          '<!@(pkg-config rtaudio --cflags --libs)',
          '<!@(pkg-config rtmidi --cflags --libs)',
          '<!@(pkg-config sndfile --cflags --libs)'
      ],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS', 'ZIC_USE_LIBSNDFILE=1' ],

    }
  ]
}
