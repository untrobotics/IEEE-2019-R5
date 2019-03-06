#!/bin/bash

if [ "$#" -lt 1 ]; then
        echo "Usage: $0 <key name>";
        exit
fi

key_name="${1,,}"; # convert to lowercase

if [ "${key_name}" == "tunnel" ]; then
	API_KEY=`cat ~/git/config.txt | awk -F'=' '/^DYNDNS_API_KEY/{printf $2}'`
elif [ "${key_name}" == "dyndns" ]; then
	API_KEY=`cat ~/git/config.txt | awk -F'=' '/^DYNDNS_API_KEY/{printf $2}'`
else
	echo -n "No entry for key name '${key_name}'";
	exit;
fi

echo -n $API_KEY;
