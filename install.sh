#!/bin/bash
MY_PATH="`dirname \"$0\"`"              # relative
MY_PATH="`( cd \"$MY_PATH\" && pwd )`"  # absolutized and normalized
MY_PATH="/wkpdfgenerator"
if [ -z "$MY_PATH" ] ; then
    # error; for some reason, the path is not accessible
    # to the script (e.g. permissions re-evaled after suid)
  exit 1  # fail
fi
sudo ldconfig
ldconfig -p | grep wkhtmltox
if [[ -n $(ldconfig -p | grep wkhtmltox) ]]; then
    echo "libwkhtmltox already installed"
else
    sudo apt-get update
    sudo apt-get install openssl build-essential xorg libssl-dev xfonts-75dpi -y
    wget -P /tmp http://download.gna.org/wkhtmltopdf/0.12/0.12.2/wkhtmltox-0.12.2_linux-trusty-amd64.deb
    sudo dpkg -i /tmp/wkhtmltox-0.12.2_linux-trusty-amd64.deb
    sudo rm -f /tmp/wkhtmltox-0.12.2_linux-trusty-amd64.deb
    sudo ldconfig
    echo "Installed wkhtmltox libary the head should be in /usr/local/include/"
fi

if [ ! -f /etc/rsyslog.d/10-wkpdfgenerator.conf ]; then
    echo "Adding the conf file to etc/rsyslog.d/"
    sudo cp $MY_PATH/10-wkpdfgenerator.conf /etc/rsyslog.d/
    sudo service rsyslog restart
else
    echo "Configure already installed"
fi

cd $MY_PATH
mkdir -p ./reports
make
cd ./bin
if [[ -n $(ldconfig -p | grep wkhtmltox) ]]; then
    ./wkpdfgenerator
else 
    echo "still wont work"
fi
