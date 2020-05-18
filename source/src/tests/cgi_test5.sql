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
INSERT INTO voterupdatesession
VALUES (1, 1, 1234, 1905375553);
