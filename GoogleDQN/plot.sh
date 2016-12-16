#!/usr/bin/env bash

file=${1%.*}

if [ -d "/home/wijnand" ]; then
  python2.7 plot.py --png_file $file.png $*
else
  python plot.py --png_file $file.png $*
fi
