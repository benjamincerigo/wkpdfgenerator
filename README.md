# PDF printer for complex web pages that rely on Javascript to create final html

This is a fork of the repository that was build for the [Equidam](htts://www.equidam.com) report printer. 

The full documentation for the printer is found in the file [Documentation.md](Documentation.md)


## Get up and running
docker and docker compose is required and use this command to get up and runnning:

`docker-compose up`

Test by printing google.nl. In a new terminal window:
```
docker-compose exec app bash
./bin/client
```

Once the job is finished there should be a PDF file in the reports directory.

## Logs
/var/log/wkpdfgenerator.log
