#!/usr/bin/env bash

file=${1%.*}

python3 plot.py --png_file $file.png $*
