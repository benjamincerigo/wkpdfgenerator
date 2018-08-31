wkpdfgenerator

This project is the pdf generator for Equidam using the library wkhtmltopdf  ( https://github.com/wkhtmltopdf/wkhtmltopdf )

The main structure of the project is
A server created by Equidam which receives a URL and returns the PDF for that URL. 

The cilent request must be 


<url>\r\nENDOFFILE\r\n 

The URL must be a legitimate URL ( see src/Url.cpp for list )
The \r\n must be used to indicate the end of line and the ENDOFFILE must be used to indicate you have finished the request. 
If you send more then one URL each with a new line the last URL you send will be used. 

The Server will return back. 

The PDF document if there are no errors
400 Bad Request - if the URL is not legit
                - if the URL gives a http error when the wkhtmltopdf lib retrives it

500 Internal Server Error - if there is an error in the server itself (ie. a send is not done correctly or a problem with the fork )
                          - if there is an error with the printer ( when the wkhtmltopdf cannot print it kills itself )

The server writes every file that it makes on the server with the name EquidamReport-YEAR-MONTH-DAY-TIMESTAMP-UID-CID(-AID if set )-PID.pdf

Some extra things to know about the server
The 10-wkpdfgenerator.conf to configure the logging.

The wkhtmltopdf lib must be installed using the .deb which should make the correct files in /urs/local/include. Which will contain the wkhtmltox headers. 
This also means that the wkhtmltopdf can run without xserver. Meaning that you don't need a graphics card to process the html. 
HOWEVER BEAWARE - that the lib can be a bit temperamental and will not print some things - so make sure your html, css and js does not contain errors. 

The make file will make 4 files. 
- wkpdfgenerator (is the server and main program )
- client (A simple cilent to test the server - the pdf will be printed wrongly due to string problems )
- timeouttest (A cimple cilent that will not call ENDOFFILE testing the servers timeout funcationallity )
- fulltest ( This is the full test but due to memory limitiations and time limmitiations was not completed)

Wkpdfgenerator
WHen the program is run a daemon_init function is called however this is commented out for the docker. This will deamonise the server and cause the logging to go to rsyslog and if you use put the 10-wkpdfgenerator.conf into rsyslog.d then the logs will be put into /var/log/wkpdfgenerator.log

Each log line has the pid of the logging process and the errno status. Be aware that this might not be the correct errno for the log line due to the fact that the errno is passed on from the last errno. 

The instal.sh file is used for the installation of the wkpdfgenerator on the equidam local setup. It can also be helpful if you need to install as it should do most of the work for you. ( Emphasis on should )

The README.md also has some helpful commands for testing the server and the wkhtmltopdf commmandline ( this command line comes with the lib)

Some places where I got some help. 
http://wkhtmltopdf.org/libwkhtmltox/ - documentation of the lib
https://github.com/wkhtmltopdf/wkhtmltopdf/issues - the issuse page of the wkhtmltoprinter
https://github.com/shihyu/Linux_Programming/blob/master/books/UNIX%20Network%20Programming%28Volume1,3rd%29.pdf 
http://www.unixnetworkprogramming.com/ - This is where most of the code for the server comes from . 
http://tldp.org/LDP/LG/issue74/tougher.html - Where I got the main Socket code
