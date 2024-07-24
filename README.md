# meteo-station
I'm building small raspberrypi-based meteorology station integrated with server app to store and serve measurements

## Starting server app:
- [install liquibase]
- [install postgres and set up db]
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
- source env.sh
- make all
