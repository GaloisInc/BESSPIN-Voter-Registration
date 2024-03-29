BESSPIN Voting Registration System Requirements
===

The following are the mandatory and secondary requirements imposed upon
the BESSPIN Voting Registration System.

Mandatory Requirements
==

* must have a web interface that allows new voter registration, checking
of existing registration status for non-confidential voters given public 
information (voter name, date of birth), and updating of existing 
registration for non-confidential voters given voter personal identifying
information (PII).

* must have a web interface that allows election officials to perform
maintenance tasks on the registration database (e.g. update registration
status based on whether the voter voted in the last election, list
registered voters matching sets of parameters, display identification
document/signature for a registered voter, etc.)

* must store, for each voter: full name, address, date of birth,
identification information (e.g., digitized signature, copy of
identification document, etc.), registration status (e.g., active,
inactive), time of original registration, and time of last update to
registration

* must enable voter information to be flagged as confidential (so it is
not revealed to, and not changeable by, anyone except properly
authorized election officials)

* must prevent duplicate voter registrations

* must log all transactions in a tamper-evident fashion

* must be completely open source, including all server and OS software
underlying the system

* must run on both Linux and FreeBSD, on SSITH cloudGFE P2 platforms

Secondary Requirements
==

#### Usability:

* should be trivial to use for non-technical users
* should incorporate all appropriate web usability design principles
* should have reasonable response times on SSITH hardware

#### Security:

* should use proper security measures and cryptography to ensure that
PII is properly protected and that only authorized election officials
can access the election official interface

#### Scalability

* should be able to handle a large number of voters (up to millions, as
would be the case in a county like Los Angeles, California)

#### Commodity Components

* should, to the maximum extent possible, use existing, well-documented,
publicly-available, open source commodity software (for, e.g., database
and UI functionality)

#### Additional Data

* should be able to handle upload and storage of photo ID images
* may store a list of the elections in which each voter voted
* may store information about whether, and for which elections, ballots
have been sent to (and received from) each voter by mail

Notes
==

This system does _not_ store information about precinct IDs, ballot
styles, etc. It is assumed that there is a separate GIS system that maps
addresses to appropriate information for each voting jurisdiction.
