INSERT INTO voter VALUES
(
  0, "lastname", "givennames", "resaddress", "mailaddress", "registeredparty",
  -544881600, "blob", 1, 0, 0, 0
),
(
  1, "secret_lastname", "secret_givennames", "resaddress", "mailaddress", "registeredparty",
  -744881600, "secret_blob1", 1, 0, 0, 1 -- confidential
),
(
  2, "secret_lastname", "secret_givennames", "resaddress", "mailaddress", "registeredparty",
  -744881600, "blob1", 1, 0, 0, 0
),
(
  3, "secret_lastname", "secret_givennames", "resaddress", "mailaddress", "registeredparty",
  -544881600, "secret_blob2", 1, 0, 0, 1 -- confidential
);

INSERT INTO electionofficial VALUES (
  0, "love2vote", "AXXGEW7kIXJS."
);
