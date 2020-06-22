#!/usr/bin/bash

SERVER_LOC = $1

curl -I http://$1/bvrs/official_query_voters?lastname=%%
