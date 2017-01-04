#!/usr/bin/env bash

file=${1%.*}

if [ -d "/home/wijnand" ]; then
  python2.7 src/plot.py --png_file $file.png $*
else
  python src/plot.py --png_file $file.png $*
fi
