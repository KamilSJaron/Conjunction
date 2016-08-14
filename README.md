# Conjunction
the simulator of secondaty CONtact using Fisher's JUNCTION model of genome admixture. It is a direct descendant of forsim, simulator writen for my Master thesis.

## Build

The software was developed on OS X (Yossemite) and Linux (Kubuntu) and it has not been tested for windows.

### Quick

Open a terminal promt, go to folder `<path>/Conjunction/` and just write

```{bash}
make
```

The fully operational exectutible `conjunction` should be created and prepared for use.

### Safe / developer build

First of all, built a testing program by

```{bash}
make test
```

if program report, that all tests passed, build a simulator

```{bash}
make
```

executible is ready to use.

## Usage

The software reads a setting file (`setting.txt` if it is not specified by argument), where all parameters are expected to be found. The template is delivered with the software, therefore simple `conjunction` should perform a set of sumulations. For details of check setting file or wiki.

```{bash}
conjunction [--version] [--help] [setting_file.txt] > summary.out
```
