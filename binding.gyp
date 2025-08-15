{
  "targets": [
    {
      "target_name": "g722",
      "sources": [ "src/g722_wrap.cc" ],

      "include_dirs": [
        "<!(node -p \"require('node-addon-api').include_dir\")",
        "<!@(pkg-config --cflags-only-I spandsp | sed -e 's/-I//g')"
      ],
      "dependencies": [
        "<!(node -p \"require('node-addon-api').gyp\")"
      ],
      "libraries": [
        "<!@(pkg-config --libs spandsp)"
      ],

      "cflags_cc": [ "-std=c++17" ],
      "defines": [ "NAPI_DISABLE_CPP_EXCEPTIONS" ],

      "xcode_settings": {
        "CLANG_CXX_LANGUAGE_STANDARD": "c++17",
        "GCC_ENABLE_CPP_EXCEPTIONS": "NO",
        "OTHER_LDFLAGS": [ "-Wl,-rpath,/opt/homebrew/lib" ]
      }
    }
  ]
}