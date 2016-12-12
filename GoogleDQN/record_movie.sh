if [ -z "$1" ]
  then echo "Please provide the name of the output file."; exit 0
fi
if [ -z "$2" ]
  then echo "Please provide the desired frame rate, e.g. 20"; exit 0
fi

./record_game.sh 10
cd dqn/stills
avconv -r $2 -threads auto -qscale 1 -i im%d.png $1.mp4
rm *.png
cd ../..
