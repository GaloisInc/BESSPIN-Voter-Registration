BESSPIN Voter Registration System Database Schema
===

The database schema for the BVRS will appear here. As we are using SQLite for the
FETT implementation, the database schema will essentially be a mirror of the data
model, with INTEGER keys, separate enumeration tables to map the enumerated types
(using REFERENCES in the schema for foreign key constraints, see [here](https://www.sqlite.org/foreignkeys.html)), and everything that can't be 
represented as an INTEGER represented as TEXT. 
