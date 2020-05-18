PRAGMA foreign_keys=ON;

-- An election official account.
CREATE TABLE electionofficial (
	-- Unique identifier in the database.
	id INTEGER PRIMARY KEY,
	-- Username
	username TEXT UNIQUE NOT NULL,
	-- Password (hashed)
	hash TEXT NOT NULL
);

-- A registered voter.
CREATE TABLE voter (
	-- Unique identifier in the database.
	id integer primary key,
	-- last name
	lastname text not null,
	-- given names
	givennames text not null,
	-- residence address
	resaddress text not null,
	-- mailing address
	mailaddress text not null,
	-- registered party
	registeredparty text not null,
	-- birth date
	-- (stored as a unix epoch value.)
	birthdate integer not null,
	-- identification information.
	idinfo blob unique not null,
	-- registration status.
	status integer not null,
	-- initial registration time.
	-- (stored as a unix epoch value.)
	initialregtime integer not null,
	-- last record update time.
	-- (stored as a unix epoch value.)
	lastupdatetime integer not null,
	-- flag for voter confidentiality.
	confidential integer not null
);

-- Authenticated election official session.
CREATE TABLE electionofficialsession (
	-- Unique identifier in the database.
	id INTEGER PRIMARY KEY,
	-- Election official identifier attached to session.
	officialid INTEGER NOT NULL,
	-- Random cookie.
	token INTEGER NOT NULL,
	-- Creation time.
	-- (Stored as a UNIX epoch value.)
	ctime INTEGER NOT NULL,
	FOREIGN KEY(officialid) REFERENCES electionofficial(id)
);

-- A voter record update session.
CREATE TABLE voterupdatesession (
	-- Unique identifier in the database.
	id INTEGER PRIMARY KEY,
	-- Voter identifier attached to session.
	voterid INTEGER NOT NULL,
	-- Random cookie.
	token INTEGER NOT NULL,
	-- Creation time.
	-- (Stored as a UNIX epoch value.)
	ctime INTEGER NOT NULL,
	FOREIGN KEY(voterid) REFERENCES voter(id)
);

INSERT INTO voter
VALUES (1,
	   "lastname",
	   "firstname",
	   "1234 address",
	   "1234 address",
	   "whig",
	   507859200, -- 1986-02-04
	   CAST ("blob1" as BLOB),
	   0,
	   0,
	   0,
	   0);
