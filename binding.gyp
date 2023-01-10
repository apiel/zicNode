{
  "targets": [
    {
      "target_name": "zic",
      "cflags!": [
        "-fno-exceptions",
        '<!@(pkg-config rtaudio --cflags --libs)'
      ],
      "cflags_cc!": [ 
        "-fno-exceptions",
        # '<!@(pkg-config rtaudio --cflags --libs)'
      ],
      "sources": [ "src/zic.cc" ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        '<!@(pkg-config rtaudio --cflags-only-I | sed s/-I//g)'
      ],
      'libraries': [
          # '-lrtaudio'
          '<!@(pkg-config rtaudio --cflags --libs)'
      ],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],

    }
  ]
}
