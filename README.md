#PDF printer for complext javacscript rendering files

This is the Repository for the equidam report printer. 
The full document for the printer is in [Doumentation.md](Documentation.md)

## Get up and running
`docker-compose up`

Test by printing google.nl
```
docker-compose exec app bash
./bin/client
```

Once the job is finished ther should be a pdf file in the reports

## Logs
/var/log/wkpdfgenerator.log

