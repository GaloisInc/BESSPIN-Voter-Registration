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
    - for riscv target: `cd sources/src && BVRS_RISCV=1 make`

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

SSL
===
The cert and key must be installed according to voting.conf. 
A [development certificate](source/conf/ssl) is provided for convience and
is generated for `localhost`. No other DNS alias for it to work 
properly.  In additon you need to install the FETT root CA in your 
browsers root CA store. Located in SSITH-FETT-Target/apps/ssl/fettCA.pem

Exploiting
===
This app contains an exploitable cookie authorization via memory overflow
as well as a script for launching the attack.  For this to work there must 
have been an existing official session still active in the database.

```
./attack.sh <bvrs-server>
```
This script will print a full list of voter information to the console as
an unothorized offiical.
