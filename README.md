# forsim
C++ software for forward simualtion

## Build

The software was developed on OS X (Yossemite) and Linux (Kubuntu) and it has not been tested for windows.

### Quick

Open a terminal promt, go to folder <path>/forsim/ and just write

$make

The fully operational exectutible "forsim" should be created and prepared for use.

### Safe / developer build

First of all, built a testing program by

$make test

and execture test

$./forsim_test

if program report, that all tests passed, clean this built without optimization

$make clean

and make optimized executible

$make

executible is ready to use.

## Usage

The software does not take any arguments, it reads the 'setting.txt' file, where all parameters are expected to be found. So, after compilation, user needs only change options in setting file before simulation.

