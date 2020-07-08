Design of the BESSPIN Voter Registration System
===

The design of the BESSPIN Voter Registration System (BVRS) is written in Lando,
and its top level is [besspin_voter_registration_system.lando](besspin_voter_registration_system.lando).
[architectural_overview.pdf](architectural_overview.pdf) is a basic architecture diagram
(shown below), and [architectural_overview.graffle](architectural_overview.graffle) is its [OmniGraffle](https://omingroup.com/omnigraffle) source document. The Lando specification's top-level file is [besspin_voter_registration_system.lando](besspin_voter_registration_system.lando).

![BVRS Architectural Overview](architectural_overview.png)

The BVRS stores two (optionally three) main types of data:

- voter data
- election official authentication information
- (optional) election and ballot information

[data_model.pdf](data_model.pdf) is the basic data model (shown below), and [data_model.graffle](data_model.graffle) is its OmniGraffle source document; as is typical for such representations, the fields above the lines comprise the primary key in a corresponding database table, and boldfaced fields are other tables' primary keys. 

![BVRS Data Model](data_model.png)

This data model excludes the tables used within the web application framework for session tracking; however, those tables appear within the database schema in [database_schema.md](database_schema.md). The security properties and threat model are in [security_properties_and_threat_model.md](security_properties_and_threat_model.md).

