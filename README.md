#PDF printer for complext javacscript rendering files

This is the Repository for the equidam report printer. 
The full document for the printer is in Doumentation.md

## Get up and running


##Testing 
The ./client binary will gererate a pdf for this the url https://secure.equidam.dev/report/index.php.

./timeouttest will test the timeout of the server. Currently because of memory problems the full test does not work

A test message response will be shown that is the string version of the pdf
This pdf is corutped by the trasfer in the client but this is not important for the final version

wkhtmltopdf --no-stop-slow-scripts --javascript-delay 30000 'url' testwk.pdf

## Logs
/var/log/wkpdfgenerator.log

The process will be named ./wkpdfgenerator
ps -x  -o pid,ppid,tty,stat,args,wchan
ps -aux | grep wk

