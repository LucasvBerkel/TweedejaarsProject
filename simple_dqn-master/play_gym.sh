#!/usr/bin/env bash

snapshot=$1
#full=${1##*/}
game=$2

if [ "$1" == "-h" ]; then
	echo "Plays a single game with the specified weights. "
	echo "Usage: ./play.sh WEIGHTS GAME"
  exit 1;
fi

shift;shift


python src/main.py  $game --environment gym --play_games 1 --display_screen true --load_weights $snapshot  $*
