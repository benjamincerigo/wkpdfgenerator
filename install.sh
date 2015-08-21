#!/bin/bash
# This is untested if it does not work, Work out why and make it work. 
if [[ -n $(ldconfig -p | grep "libwkhtmltox") ]]; then
    echo "libwkhtmltox already installed"
else
    sudo apt-get update
    sudo apt-get install openssl build-essential xorg libssl-dev xfonts-75dpi -y
    wget -P /tmp http://download.gna.org/wkhtmltopdf/0.12/0.12.2/wkhtmltox-0.12.2_linux-trusty-amd64.deb
    dpkg -i /tmp/wkhtmltox-0.12.2_linux-trusty-amd64.deb
fi

if [ ! -f /etc/rsyslog.d/10-wkpdfgenerator.conf ]; then
    sudo cp 10-wkpdfgenerator.conf /etc/rsyslog.d/
    sudo service rsyslog restart
fi

make
cd ./bin
./wkpdfgenerator
