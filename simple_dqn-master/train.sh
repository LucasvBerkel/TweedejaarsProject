#!/usr/bin/env bash

game_vers=$1
experiment=$2
game=${game_vers%-*}
date="$(date +%s)"
weights_prefix=runs/$game/${date}_$experiment/weights/${game}_${experiment}
results=runs/$game/${date}_$experiment/${game}_${experiment}.csv

mkdir -p runs/$game/${date}_$experiment/weights

if [ -d "/home/wijnand" ]; then
  python2.7 src/main.py --save_weights_prefix $weights_prefix \
--csv_file $results $game_vers "${@:3}"
else
  python src/main.py --save_weights_prefix $weights_prefix \
--csv_file $results $game_vers "${@:3}"
fi
