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
    -  for riscv target: `BVRS_OS=$OS BVRS_RISCV=1 ./sources/builddeps.sh`
       (`$OS` can be `linux-gnu`, `freebsd12.1`, etc.)
3. Build BVRS CGI Handler: 
    - for host: `cd sources/src && make`
    - for host: `cd sources/src && BVRS_RISCV=1 make`

"Deploying" to Target
===

Deploying to FETT Target amounts to copying the right files to the right places
in the FETT-TARGET repo. The script `deploy.sh` aims to make this a bit easier.

```
./deploy.sh bin-path common-path
```
copies binaries to bin-path and shared files to common-path (organized as expected by FETT-Target).

Assuming that `SSITH-FETT-Target` is checked out at `$FETT`, (and after building
the dependencies + bvrs app), one just needs to run (assuming built for debian):

``` sh
./deploy.sh $FETT/SSITH-FETT-Binaries/GFE/appsBinaries/voting/debian $FETT/build/voting/common
```


