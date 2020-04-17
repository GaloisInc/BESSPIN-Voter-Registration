# Security Properties and Threat Model for the BESSPIN Voter Registration System (BVRS)

## Background

Most information stored in a voter registration system is public information; it is generally possible for anybody who can access a website to _check_ a person's voter registration status, given that person's name, birth date, and (sometimes) address. The exception to this is when individuals ask upon registration that their information remain confidential to the electoral authority (this can occur for various reasons, such as avoiding abusive domestic situations by not giving away one's current address). However, _registering_ a person to vote or _modifying_ a voter registration record requires either non-public information, such as identification information distributed by the state (e.g., a driver's license or ID) or the electoral authority (e.g., an existing voter registration number), or explicit authorization to make such changes (e.g., being an election official with access to modify the voter registration database). 

## Security Properties

- __communication confidentiality__: all communications on public networks must be encrypted in transit
- __data integrity__: the data store must not be modified by the BVRS server software in ways that are not authorized; if it is so modified externally (as, for example, by direct editing of the database or log files outside of the system's normal mode of operation), the modifications should be detectable
- __voter private data confidentiality__: a voter's private data must remain confidential to that voter and election officials (EOs); note that publicly available information like name, birth date, and address is not considered "private data" for this purpose (with the exception of "confidential" voters, see below)
- __"confidential" voter data confidentiality__: _all_ data about voters marked "confidential" must remain confidential to everyone except EOs; i.e., the public interface of the BVRS must report that there is no matching record for a confidential voter registration, regardless of how much of that voter's correct information is provided
- __voter data update authorization__: existing voter registration data may only be changed or deleted by EOs, unless the correct private data used for the initial registration is provided
- __voter participation/ballot information update authorization__: election participation information (if implemented) and ballot tracking information (if implemented) may only be modified by EOs

### Notes:

It is explicitly _not_ a requirement for this implementation that an entity with administrative privileges on the BVRS server machine cannot violate data confidentiality; the encryption and key management associated with protecting the data at rest from privileged access on the server machine have significant overhead that is likely not reasonable for the FPGA-based implementation platform or the FETT competition. 

## Threat Model

The following threats are "legitimate" for the purposes of the FETT competition; other threats, like denial of service by flooding the server's bandwidth or by flooding the server with a high request load, are _not_ legitimate because they are not subject to any of SSITH's mitigations.

- __privilege escalation__: gaining access to the EO interface without a valid EO username and password; note that privilege escalation on the underlying infrastructure is also possible, but is part of the infrastructure's threat model and applies to all applications running on it
- __undetected data modification__: changing the database in a way that is both (1) not in accordance with the intended functionality of the BVRS and (2) not detectable in a straightforward fashion (e.g., by auditing the database against the logs)
- __unauthorized data modification__: changing the database via the public BVRS interface in an unauthorized way (e.g., by maliciously crafting a request to the server that bypasses authorization checks for modifying one's own voter registration)
- __unauthorized data retrieval__: retrieving information from the database that the public is not authorized to see (e.g., by accessing an individual's full voter registration record without all their personal information, or by accessing a confidential voter registration record outside the election official interface)
- __data corruption__: causing data in the database to be corrupted using either the public or EO BVRS interfaces (e.g., by maliciously crafting a request or series of requests that result in a voter being recorded as having voted in an election before they were registered)
- __local denial of service__: causing the BVRS server application to crash (e.g., by sending a maliciously crafted request) using non-escalated privileges; one could also cause other parts of the infrastructure (the Linux/FreeBSD kernel, the web server) to crash, but that is covered by the infrastructure's threat model
