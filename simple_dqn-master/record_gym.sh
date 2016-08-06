#!/usr/bin/env bash

if [ "$1" == "-h" ]; then
	echo "This program saves a video of one round of a specified game played with given weights. "
	echo "Usage: ./record_gym.sh WEIGHTS GAME VIDEO_NAME"
  exit 1;
fi

snapshot=$1
#full=${1##*/}
game=$2
file=$3
#rom="roms/$game.bin"
shift;shift;shift

#rm -f -r videos/$game
mkdir -p videos # create dir if it does not exitst
mkdir -p videos/$game

python src/main.py $game --environment gym --play_games 1 --record_screen_path videos/$game --load_weights $snapshot $*
type ffmpeg 2>/dev/null || { avconv -r 20  -pattern_type glob -i videos/$game/*.png -f mov -c:v libx264 -y videos/$file.mov; exit 1; }
ffmpeg -r 20  -pattern_type glob -i videos/$game/*.png -f mov -c:v libx264 -y videos/$file.mov
#rm videos/$game/*.png
