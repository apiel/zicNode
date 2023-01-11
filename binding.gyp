{
  "targets": [
    {
      "target_name": "zic",
      "cflags!": [
        "-fno-exceptions",
        '<!@(pkg-config rtaudio --cflags --libs)',
        '<!@(pkg-config rtmidi --cflags --libs)',
        './libpd/libs/libpd.so -DPDINSTANCE'
      ],
      "cflags_cc!": [ 
        "-fno-exceptions",
        # '<!@(pkg-config rtaudio --cflags --libs)'
      ],
      "sources": [ "src/zic.cc" ],
      "include_dirs": [
        "zic",
        "<!@(node -p \"require('node-addon-api').include\")",
        '<!@(pkg-config rtaudio --cflags-only-I | sed s/-I//g)',
        '<!@(pkg-config rtmidi --cflags-only-I | sed s/-I//g)',
        './libpd/libpd_wrapper',
        './libpd/libpd_wrapper/util',
        './libpd/pure-data/src',
        './libpd/cpp'
      ],
      'libraries': [
          # '-lrtaudio'
          '<!@(pkg-config rtaudio --cflags --libs)',
          '<!@(pkg-config rtmidi --cflags --libs)'
      ],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],

    }
  ]
}
