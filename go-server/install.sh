#!/bin/bash

FOLDER_NAME=meteo-station-server


mkdir -p /usr/bin/$FOLDER_NAME
cp env.sh /usr/bin/$FOLDER_NAME/env.sh
cp run.sh /usr/bin/$FOLDER_NAME/run.sh
cp server /usr/bin/$FOLDER_NAME/server

cp meteo-station-server.service /etc/systemd/system/meteo-station-server.service
