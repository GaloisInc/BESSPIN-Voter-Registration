#!/usr/bin/env python3
from faker import Faker
import sqlite3
import sys
import random
from datetime import datetime
import calendar

conn = sqlite3.connect(sys.argv[1])
conn.set_trace_callback(print)
for id in range(100):
    frank = Faker()
    address = frank.address()
    zipcode = frank.zipcode()
    state = frank.state_abbr()
    mail_address = address
    party = ["SSITH", "REBEL"][random.randint(0,1)]
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
        (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
    """, (
        id,
        frank.last_name(),
        frank.first_name(),
        address,
        zipcode,
        state,
        mail_address,
        zipcode,
        state,
        party,
        int(dob),
        "/path/to/%s" % id,
        random.randint(0,1),
        0,
        0,
        random.randint(0,1),
    ))
conn.commit()
conn.close()
