# meteo-station
I'm building small raspberrypi based meteorology station integrated with server app to store and serve measurements

## Starting server app:
- cp .env.schema .env
- [fill .env with values]
- npm install
- npm run start

## Building and starting app on raspberrypi:
- cd device
- make all
