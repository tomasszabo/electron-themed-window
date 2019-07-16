{
  "targets": [
    {
      "target_name": "addon",
      "sources": [ "src/index.cc" ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")",
      ],
      "conditions": [
        [
          'OS=="mac"', {
            "sources": [ "src/mac.mm" ]
          },
        ],
        [
          'OS=="win"', {
            "sources": [ "src/win.cc" ],
            "msvs_settings": {
              "VCCLCompilerTool": {
                "AdditionalOptions": [ '-std:c++17' ]
              }
            }
          }
        ],
        [
          'OS!="mac" and OS!="win"', {
            "sources": [ "src/default.cc" ]
          }
        ]
      ]
    }
  ]
}
