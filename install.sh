#!/bin/bash

# install script for Ubuntu 18.04 (Tegra-L4T-NVIDIA)

if [[ $EUID -ne 0 ]]; then
	echo "This script must be run as root"
	exit 1
fi

NAME="robopi"

apt-get install git runit runit-systemd jq -y

cd ~
mkdir -p bin
mkdir -p git

cd git
git clone https://github.com/untrobotics/IEEE-R5-2019 .

echo "Please input the dynamic DNS API key (from dyndns.untrobotics.com), followed by [ENTER]:"
read dyndns_api_key

echo "Please input the ngrok auth token (from ngrok.com), followed by [ENTER]:"
read ngrok_auth_token

echo "DYNDNS_API_KEY=${dyndns_api_key}" > ~/git/config.txt
echo "authtoken: ${ngrok_auth_token}" > ~/git/ngrok-config.yml

# add extra commands for debugging
cat ~/git/root/.bashrc >> ~/.bashrc

# set up dyndns
ln -s ~/git/bin/dyndns.sh ~/bin/dyndns.sh
echo "@reboot /root/bin/dyndns.sh untrobotics.com ${NAME}" > /tmp/crontab
echo "*/15 * * * * /root/bin/dyndns.sh untrobotics.com ${NAME}" >> /tmp/crontab
#crontab /tmp/crontab
# done with dyndns

# set up runit
ln -s ~/git/bin/ngrok ~/bin/ngrok
ln -s ~/git/bin/get-tunnel-url.sh ~/bin/get-tunnel-url.sh
#dpkg -i src/runit_2.1.2-9.2ubuntu1_arm64.deb # this will produce an error about upstart (on Ubuntu 15+), ignore it
#dpkg -i src/runit-systemd_2.1.2-9.2ubuntu1_all.deb
ln -s ~/git/runit-services/tunnel-service /etc/service/
# done with runit

# set up tunnel service?
echo "@reboot curl -s -X POST https://www.untrobotics.com/api/robots/tunnel -d '{\"tunnel\":\"\'`/root/bin/get-tunnel-url.sh\`'\",\"endpoint\":\"${NAME}\"}'" >> /tmp/crontab
echo "*/15 * * * * curl -s -X POST https://www.untrobotics.com/api/robots/tunnel -d '{\"tunnel\":\"\'`/root/bin/get-tunnel-url.sh\`'\",\"endpoint\":\"${NAME}\"}'" >> /tmp/crontab
# done with tunnel service

# set up cron
crontab /tmp/crontab
rm /tmp/crontab
# done with cron
