# PDF printer for complex javacscript rendering files

This is the repository for the Equidam report printer. 

The full documentation for the printer is found in the file [Documentation.md](Documentation.md)


## Get up and running
docker and docker compose is required
`docker-compose up`

Test by printing google.nl
```
docker-compose exec app bash
./bin/client
```

Once the job is finished there should be a PDF file in the reports directory.

## Logs
/var/log/wkpdfgenerator.log
