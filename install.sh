#!/bin/bash

# install script for Ubuntu 18.04 (Tegra-L4T-NVIDIA)

if [[ $EUID -ne 0 ]]; then
	echo "This script must be run as root"
	exit 1
fi

cd ~
mkdir bin
mkdir git
mkdir src

cd git
git clone https://github.com/untrobotics/IEEE-R5-2019

echo "Please input the dynamic dns API key (from dyndns.untrobotics.com), followed by [ENTER]:"
read dyndns_api_key
echo "DYNDNS_API_KEY=${dyndns_api_key}" > ~/git/config.txt

# add extra commands for debugging
cat ~/git/root/.bashrc > ~/.bashrc

# set up dyndns
ln -s ~/git/bin/dyndns.sh ~/bin/dyndns.sh
echo "*/15 * * * * /root/bin/dyndns.sh untrobotics.com jetson" > /tmp/crontab
crontab /tmp/crontab
# done with dyndns

