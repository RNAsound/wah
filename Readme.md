Wah~ (PD external)
====
Created by: Aren Akian

Last updated: Jul 30, 2024


## About
 - _Wah~_ is a Pure Data (PD) external written in C, built on top of the PD “vanilla” library

- _Wah~_ is a wah filter, similar to a wah-wah pedal.

- _Wah~_ has a filter, with variable resonance (Q). The user controls a wah frequency "center" via the inlet


## Installation
To use _Wah~_, you need to install the correct compiled binary for your system, in your PD externals folder. 

    - Mac (any 64 bit architecture): .pd_Darwin

    - Linux (any 64 bit architecture): .pd_linux

    - Windows (any 64 bit architecture): .dll

1. Find the location your PD externals folder
    - Open PD > Preferences > Path to see the default path to your externals folder 

2. Copy the _"wah~"_ folder containing the compiled binaries and the wah~-help.pd_ files into the externals folder

Typically, the default externals path is as follows:
        
### macOS:

| Scope | Path |
| ------ | ------- |
| Application-specific: |  /$PdPath/Contents/Resources/extra (this is inside the Pd Application (like Pd-0.49-1 in ~/Applications); right click it and choose "Show Package Contents", then navigate to "Resources/extra") |
| User-specific: |  ~/Library/Pd (/Users/user_name/Library/Pd) |
| Global: |  /Library/Pd

### Windows
| Scope | Path |
| ------ | ------- |
| Application-specific: | %ProgramFiles(x86)%\Pd\extra (for 64-bit OS and 32-bit Pd) or %ProgramFiles%\Pd\extra; this is inside the Pd Application (usually in C:\Program Files). This folder needs to be set to writeable.
| User-specific: | %AppData%\Pd (usually in C:\Users\user_name\AppData\Roaming\Pd).
| Global: | %CommonProgramFiles%\Pd (usually in C:\Program Files\Common Files\Pd).

### GNU/Linux

| Scope | Path |
| ------ | ------- |
| Application-specific: |  /usr/lib/pd/extra if installed via a package manager (apt-get) or /usr/local/lib/pd/extra if compiled by yourself.
| User-specific: | ~/.local/lib/pd/extra (preferred since version Pd-0.47-1) or ~/pd-externals (deprecated but still usable).
| Global: | /usr/local/lib/pd-externals.

In most cases, the “User-Specific” path is reccomended.
- If unsure, you can add a search path to your custom install location. 



How to Run
=======

1. Open a PD project
2. Inside the patcher, create a new object (⇧⌘ + 1 or ⇧Ctl + 1) 
    - type in "_wah~_ "


How to Use
=======
1. Follow the steps above to create a _wah~_ object. Connect the outlet to a _dac~_ or other outlet for further processing
2. Connect an audio signal to the first inlet
3. Then send frequency values (as number atoms) to the first inlet to set the frequency "center" of the _wah~_
    - modulate this value to for wah-wah like effects
4. Set the filter resonance to desired value (_0 - 0.9_)
5. For examples or further help, open “_wah~-help.pd_” included in this repo.

Inputs
===

### Inlet~ 1: Audio & Root Frequency
**Info:** Inlet 1 accepts an audio~ input, and frequency atom input, corresponding to the center frequency of the _wah~_ filter 

**Arguments:** 
- _Frequency~_ - the frequency of the root note
    - _Tip_: use an _mtof_ object to send frequency values into a _sig~_ object, then connect the _sig~_ outlet to the first inlet of _houscil~_ (like in the _houscil~-help.pd_ file)


***
### Inlet 2: Harmonic Amount 

**Info:** Inlet 2 accepts atom values, corresponding to the amount of 7th chord harmonics added to the root tone

**Arguments:**
- _harmonicAmt_ - The amount of harmonics to be added (0-100)


Troubleshooting
===
If the external does not run in PD, or causes any issues, you may need to _recompile_ the included source code for your system. I recommend using [pd-lib-builder](https://github.com/pure-data/pd-lib-builder)

Credits
===
- [PD Docs: Externals](https://msp.ucsd.edu/Pd_documentation/x4.htm)
- [pd-lib-builder](https://github.com/pure-data/pd-lib-builder)
- _Designing Audio Objects for Max/MSP and Pd_ by Eric Lyon














