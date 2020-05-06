SSITH-FETT-Voting
===

This repository contains the voting-related application - a _voter
registration system_ - of the SSITH FETT bug bounty exercise, which is
also the voting-related application that will later be adapted to
demonstrations at Black Hat and the ERI Summit.

The high-level requirements for the BESSPIN Voter Registration System are
in [requirements.md](requirements.md). The system design is in 
the [design](design) directory.

Building
===

1. Fetch dependencies: `./sources/getdeps.sh`
2. Build dependencies 
    -  for host: `./sources/builddeps.sh` 
    -  for riscv target: `BVRS_RISCV=1 ./sources/builddeps.sh`
3. Build BVRS CGI Handler: 
    - for host: `cd sources/src && make`
    - for host: `cd sources/src && BVRS_RISCV=1 make`
