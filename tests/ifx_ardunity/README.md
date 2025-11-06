# IFX Ardunity - Unity Testing Framework for Arduino

## Overview

IFX Ardunity provides a simple adapter layer to run Unity C testing framework in Arduino environments. It handles the Arduino-specific setup and allows you to write and run unit tests using Arduino CLI.

## Features

- Unity C testing framework integration
- Arduino Serial output for test results
- Simple build process with Arduino CLI


## Files

```
ifx_ardunity/
├── README.md              # This documentation
├── test_includes.h        # Includes
├── unity_ifx.h           # IFX macros  
├── unity_config.h        # Unity configuration
├── template/
│   ├── test_main.ino     # Template test runner
│   └── Makefile.lib      # Template Makefile
├── cpp_layer/
│   ├── unity_ifx.cpp     # Arduino Serial output
│   └── test_arduino_includes.hpp # Arduino test headers
└── Unity/                # Unity framework submodule
```