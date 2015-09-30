Basic socket and client made in cpp
Must have wkhtmltopdf installed 

install wkhtmltopdf:
sudo apt-get update
sudo apt-get install openssl build-essential xorg libssl-dev xfonts-75dpi -y
wget -P /tmp http://download.gna.org/wkhtmltopdf/0.12/0.12.2/wkhtmltox-0.12.2_linux-trusty-amd64.deb
dpkg -i /tmp/wkhtmltox-0.12.2_linux-trusty-amd64.deb

set up the loging file:
sudo cp 10-wkpdfgenerator.conf /etc/rsyslog.d/
sudo service rsyslog restart

do:
make

run:
cd ./bin
./wkpdfgenerator


test:
// Test the report generator for https://secure.equidam.dev/report/index.php
./client 
// Test the report generator for https://secure.equidam.dev/pdfreport/pdfreport_all.php?uid=100&t=ahrgF4BEtQevm3ZzkpJ5YNUxBS7kRKkr
./client oldreport
// Test the timeout of the socket 
./timeouttest

A test message response will be shown that is the string version of the pdf
This pdf is corutped by the trasfer in the client but this is not important for the final version

view log in 
/var/log/wkpdfgenerator.log

The process will be named ./wkpdfgenerator
ps -x  -o pid,ppid,tty,stat,args,wchan

Try the make
cd /vagrant/reports/; echo -e 'https://secure.equidam.dev/report/index.php?uid=100&cid=100&t=ahrgF4BEtQevm3ZzkpJ5YNUxBS7kRKkr&pdf=true\r\nENDOFFILE\r\n' | nc 127.0.0.1 30000 > testnewjava.pdf

wkhtmltopdf --no-stop-slow-scripts --javascript-delay 30000 'https://secure.equidam.dev/report/index.php?uid=100&cid=100&t=ahrgF4BEtQevm3ZzkpJ5YNUxBS7kRKkr&pdf=true' testwk.pdf

