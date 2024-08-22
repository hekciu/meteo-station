# meteo-station
I'm building small raspberrypi-based meteorology station integrated with server app to store and serve measurements

## Configuring database for server app
- sudo apt install postgresql # at least 9.1 is needed
- sudo systemctl start postgresql.service
- sudo -u postgres createuser \<username\>
- sudo -u postgres createdb \<dbname\>
- sudo -u postgres psql
- > alter user \<username\> with encrypted password '\<password\>';
- > grant all privileges of database \<dbname\> to \<username\>;

### /db directory contains pseudo version management system for db
### it authorizes to db through provided credentials and configures it properly
### you could do it from any linux machine that has psql installed
- cp env.sh.template env.sh
### fill env.sh with values
source update_database.sh


## Starting server app:
- cp .env.template .env
### fill .env with values
- npm install
- npm run start

## Building and starting app on raspberrypi:
- sudo apt-get install libcurl4-openssl-dev
### install pigpio library if it's not present on your machine(but it should be I guess)
- cd device
- cp env.sh.template env.sh
### fill env.sh with values
- sudo bash run.sh
