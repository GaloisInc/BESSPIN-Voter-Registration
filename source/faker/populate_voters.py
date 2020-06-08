#!/usr/bin/env python3
from faker import Faker
import sqlite3
import sys
import random
from datetime import datetime
import calendar

conn = sqlite3.connect(sys.argv[1])
conn.set_trace_callback(print)
for id in range(4,110):
    frank = Faker()
    address = frank.address()
    zipcode = frank.zipcode()
    state = frank.state_abbr()
    mail_address = address
    party = ["SSITH", "REBEL", "Unaffiliated"][random.randint(0,2)]
    dob = calendar.timegm(frank.date_of_birth(minimum_age=18).timetuple())
    initialregtime = calendar.timegm(frank.date_this_decade().timetuple())
    lastupdatetime = calendar.timegm(frank.date_this_year().timetuple())
        
    conn.execute("""
    INSERT INTO voter(
        id,
        lastname,
        givennames,
        resaddress,
        resaddress2,
        reszip,
        resstate,
        mailaddress,
        mailaddress2,
        mailzip,
        mailstate,
        registeredparty,
        birthdate,
        idinfo,
        status,
        initialregtime,
        lastupdatetime,
        confidential)
        VALUES
        (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
    """, (
        id,                     # id
        frank.last_name(),      # lastname
        frank.first_name(),     # givennames
        address,                # res address 
        "",                     # res address 2
        zipcode,                # res zipcode
        state,                  # res state
        mail_address,           # mail address
        "",                     # mail address 2
        zipcode,                # mail zipcode
        state,                  # mail state
        party,                  # party offiliation
        int(dob),               # birthdate
        "MY ID: %s" % id,       # id file (just using row id here)
        random.randint(0,1),    # voter status?
        initialregtime,         # initial voter registration date
        lastupdatetime,         # last updated info on date
        random.randint(0,1),    # confidential?
    ))
conn.commit()
conn.close()
