#!/bin/bash

. env.sh
idf.py build flash monitor
