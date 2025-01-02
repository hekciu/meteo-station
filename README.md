# meteo-station
I'm building small raspberrypi-based meteorology station integrated with server app to store and serve measurements

# current goals ->
- write better http parser at server (parse once, return one struct)
- write own http implementation at client (instead of curl)
- resolve bunch fo TODOS related to security and stuff
- add support for DHT22 temperature sensor

## Configuring database for server app
- sudo apt install postgresql # at least 9.1 is needed
- sudo systemctl start postgresql.service
- sudo -u postgres createuser \<username\>
- sudo -u postgres createdb \<dbname\>
- sudo -u postgres psql
- > alter user \<username\> with encrypted password '\<password\>';
- > grant all privileges on database \<dbname\> to \<username\>;

### /db directory contains pseudo version management system for db
### it authorizes to db through provided credentials and configures it properly
### you could do it from any GNU/Linux machine that has psql installed
- cp env.sh.template env.sh
### fill env.sh with values
source update_database.sh

## Starting server app(server/):
- sudo apt install libpq-dev
- cp env.sh.template env.sh
### fill env.sh with values
- ./run.sh

## Starting nodejs server app(legacy/server/):
### Do not do it, it's legacy, inside legacy/, it does not work etc

## Building and starting app on raspberrypi(device/):
- sudo apt-get install libcurl4-openssl-dev
### install pigpio library if it's not present on your machine(but it should be I guess)
- cp env.sh.template env.sh
### fill env.sh with values
- sudo bash run.sh
