Basic socket and client made in cpp
Must have wkhtmltopdf installed 

install wkhtmltopdf
sudo apt-get update
sudo apt-get install openssl build-essential xorg libssl-dev xfonts-75dpi -y
wget -P /tmp http://download.gna.org/wkhtmltopdf/0.12/0.12.2/wkhtmltox-0.12.2_linux-trusty-amd64.deb
dpkg -i /tmp/wkhtmltox-0.12.2_linux-trusty-amd64.deb

do:

make
./bin/server &
./bin/client 

A test message response will be shown
ps -a  -o pid,ppid,tty,stat,args,wchan
