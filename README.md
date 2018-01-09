
![logo](logo/Conjunction.png)

the simulator of secondaty CONtact using Fisher's JUNCTION model of genome admixture. It is a direct descendant of forsim, simulator writen for my [Master thesis](http://is.muni.cz/th/376090/prif_m/thesis_jaron_zadani.pdf).

## Build

The software was developed and tested on OS X and Linux and it has not been tested for windows.

#### Quick

Open a terminal, download repository

```
git clone https://github.com/KamilSJaron/Conjunction
```

enter the folder and build Conjunction by

```
cd Conjunction && make
```

The fully operational executable `conjunction` should be created and prepared for use. You can install `conjunction` to by

```
make install
```

which will copy the binary to `/usr/local/bin`. If you wist to install `conjunction` to an alternative place you can type

```
make install INSTAL_PREFIX=/home/slim/
```

and it will be installed in provided path/bin. In my example `/home/slim/bin`.

#### Testing

Travis build & unit test of `master` branch : [![master_build](https://travis-ci.org/KamilSJaron/Conjunction.svg?branch=master)](https://travis-ci.org/KamilSJaron/Conjunction/branches)
Travis build & unit test of `dev` branch : [![dev_test](https://travis-ci.org/KamilSJaron/Conjunction.svg?branch=dev)](https://travis-ci.org/KamilSJaron/Conjunction/branches)

Several unit tests are implemented in separated binary. To execute the unit test on your computer

```
make test
```

will compile testing binary, execute tests and delete testing binary if tests passed.

## Usage

The software reads a setting file (`setting.txt` by default), the specification of the simulation is read from there. The template is delivered with the software, therefore simple `conjunction` should perform a set of sumulations. For details of check [setting file](setting.txt) or wikipage [setting](https://github.com/KamilSJaron/Conjunction/wiki/setting).

```{bash}
conjunction [--version] [--help] [setting.txt] 1> summary.out 2> simulation_log.err
```

The log of the simulation is streamed to standard error stream. The basic output of simulation is streamed to standard output stream. If other type of output is desired it has to be set in setting, details can be found at corresponding wikipage describing [output](https://github.com/KamilSJaron/Conjunction/wiki/output).

## Mechanisms of simulations

The simulator is restricted to secondary contact hybrid zones where reduced fitness of hybrids is caused purely by genetic background. The representation of individuals currently does not allow new mutations in simulation. The only information for every loci is if it originated in population on one or the other side of the hybrid zone (population A or B). All details can be found in [wiki](https://github.com/KamilSJaron/Conjunction/wiki#principles).

If there is anything unclear or you encounter a bug, please open an issue!
