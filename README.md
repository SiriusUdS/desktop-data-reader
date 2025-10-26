# Desktop Data Reader

## Table of Contents
- [Project Overview](#project-overview)
- [Prerequisites](#prerequisites)
- [Setup Instructions](#setup-instructions)
- [Building the Project](#building-the-project)
- [Running the Program](#running-the-program)
- [Troubleshooting](#troubleshooting)
- [License](#license)

## Project Overview
This repository is a C++20 project that extract data from an SD card and generates beep sounds at different frequencies and durations while doing it using SFML for audio playback.

## Prerequisites
- Visual Studio 2019 or newer (x64 configuration recommended)
- [SFML 2.6.2](https://www.sfml-dev.org/download.php) for Visual C++17 (64-bit)

## Setup Instructions
1. **Download SFML:**
   - Go to [SFML Download Page](https://www.sfml-dev.org/download.php)
   - Download the 64-bit Visual C++ version (e.g., "SFML 2.6.2 Visual C++ 17 (2022) - 64-bit"). Make sure to choose the 2.6.2 version since there is a problem with the loadFromSamples function in the 3.0.0 version with the new channel parameter.
   - Extract to a folder, e.g., `C:\SFML`

2. **Configure Visual Studio Project:**
   - Right-click your project > Properties.
   - Go to `Configuration Properties > VC++ Directories`:
     - **Include Directories:** Add `C:\SFML\include`
     - **Library Directories:** Add `C:\SFML\lib`
   - Go to `Configuration Properties > C/C++ > General`:
     - Add `C:\SFML\include` to `Additional Include Directories`.
   - Go to `Configuration Properties > Linker > Input > Additional Dependencies`:
     - Add:
       - `sfml-system.lib`
       - `sfml-window.lib`
       - `sfml-graphics.lib`
       - `sfml-audio.lib`
   - Remove any `-s` (static) libraries and do not define `SFML_STATIC`.

3. **Copy SFML DLLs:**
   - From `C:\SFML\bin`, copy these DLLs to your build output directory (e.g., `SDCerdExtrecter\x64\Debug`):
     - `sfml-system-3.dll`
     - `sfml-window-3.dll`
     - `sfml-graphics-3.dll`
     - `sfml-audio-3.dll`

## Building the Project
- Set your build configuration to `x64` and `Debug` or `Release`.
- Build the solution in Visual Studio (`Build > Build Solution`).
- The executable will be generated in the output directory (e.g., `x64\Debug`).

## Running the Program
- Run the executable from the output directory.
- Ensure all required SFML DLLs are present in the same folder as the `.exe`.

## Troubleshooting
- **DLL Not Found:** Make sure the correct SFML DLLs are in the output directory and match your build architecture (x64).
- **No .exe File:** Check for build errors in Visual Studio and ensure the output directory is correct.
- **Linker Errors:** Verify that you are using the dynamic libraries (no `-s` suffix) and have removed `SFML_STATIC` from your code.

## License
This project is provided as-is for educational purposes. See SFML's license for details on using the SFML library.
