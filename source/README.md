BESSPIN-Voter-Registration Sources
===

This directory contains the source of the Voting app (BVRS) sources and some
helper scripts to get going with the external dependencies.

- `getdeps.sh` will fetch the dependencies into `ext`
- `builddeps.sh` will build each dependency and install it into `../build`
  - This file is configurable. Setting the environment variables `BVRS_RISCV=1`
    will build the dependencies for a RISCV target (the OS should be set as
    well, see the script for details)
- The actual BVRS app is in `src`. Once the dependencies are installed in `../build`, 
  build the app by running `make` (or `BVRS_RISCV=1 make`) in `src`.
