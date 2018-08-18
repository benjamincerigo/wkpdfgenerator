FROM ubuntu:14.04
RUN  apt-get update
RUN apt-get install openssl build-essential xorg libssl-dev xfonts-75dpi -y
RUN apt-get install wget -y
RUN wget -P /tmp https://downloads.wkhtmltopdf.org/0.12/0.12.5/wkhtmltox_0.12.5-1.trusty_amd64.deb
RUN apt-get install fontconfig -y
RUN dpkg -i /tmp/wkhtmltox_0.12.5-1.trusty_amd64.deb

COPY 10-wkpdfgenerator.conf /etc/rsyslog.d/

RUN ldconfig

WORKDIR /wkpdfgenerator

ENTRYPOINT ./install.sh

CMD ./bin/wkpdfgenerator 
