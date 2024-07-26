# meteo-station
I'm building small raspberrypi-based meteorology station integrated with server app to store and serve measurements

## Starting server app:
- sudo apt install postgresql
- sudo systemctl start postgresql.service
- sudo -u postgres createuser --interactive # create role called 'meteo-station'
- sudo -u postgres createdb meteo-station
- sudo adduser meteo-station
- sudo -u meteo-station psql -d meteo-station
- cp .env.schema .env
- [fill .env with values]
- npm install
- npm run start

## Building and starting app on raspberrypi:
- sudo apt-get install libcurl4-openssl-dev
- [install pigpio library if it's not present on your machine(but it should be I guess)]
- cd device
- cp env.sh.template env.sh
- [fill env.sh with values]
- sudo bash run.sh
