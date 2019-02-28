#!/bin/bash

if [ "$#" -lt 2 ]; then
        echo "Usage: $0 <super_domain> <sub_domain>";
        exit
fi

test=0;
if [ "$#" -eq 3 ]; then
	test=1;
fi

API_KEY=`cat ~/git/config.txt | awk -F'=' '/^DYNDNS_API_KEY/{printf $2}'`
API_HOST="dyndns.untrobotics.com";

SUPER_DOMAIN="${1}";
SUB_DOMAIN="${2}";

ip=`/sbin/ifconfig | grep -Eo 'inet (addr:)?([0-9]*\.){3}[0-9]*' | grep -Eo '([0-9]*\.){3}[0-9]*' | grep -v '127.0.0.1'`;
ttl=60;

if [ "${test}" -eq 1 ]; then
	echo $ip;
	exit;
fi

wget "http://${API_HOST}/api/ip2host.php?sub_domain=${SUB_DOMAIN}&super_domain=${SUPER_DOMAIN}&API_KEY=${API_KEY}&ip=${ip}&ttl=${ttl}" -qO- && echo ""
