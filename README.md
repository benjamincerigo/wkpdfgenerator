#PDF printer for complext javacscript rendering files

This is the Repository for the equidam report printer. 

The full documentation for the printer is in [Doumentation.md](Documentation.md)


## Get up and running
docker and docker compose is required
`docker-compose up`

Test by printing google.nl
```
docker-compose exec app bash
./bin/client
```

Once the job is finished there should be a pdf file in the reports directory

## Logs
/var/log/wkpdfgenerator.log
