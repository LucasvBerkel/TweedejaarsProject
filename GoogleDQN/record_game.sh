#!/bin/bash

#FRAMEWORK="alewrap"
FRAMEWORK="SF"

if [ $FRAMEWORK = "SF" ]
	then ENV="SF_Control"
else
	if [ -z "$1" ]
		then echo "Please provide the name of the game, e.g.  ./run_cpu breakout "; exit 0
	fi
	ENV=$1
fi

#game_path=$PWD"/roms/"
game_path="/home/wijnand/Documents/git/TweedejaarsProject/SF_Control/control_frame_lib.so"
env_params="useRGB=false"
agent="NeuralQLearner"
n_replay=1
netfile="\"/home/wijnand/Documents/git/TweedejaarsProject/GoogleDQN/dqn/best.t7\""
update_freq=4

actrep=4
games=40

discount=0.99
seed=$(shuf -i 1-65000 -n 1)
learn_start=50000
pool_frms_type="\"max\""
pool_frms_size=2
initial_priority="false"
replay_memory=1000000
eps_end=0.1
eps_endt=replay_memory
lr=0.00025
agent_type="DQN3_0_1"
preproc_net="\"net_downsample_2x_full_y\""
state_dim=7056
ncols=1
agent_params="lr="$lr",ep=1,ep_end="$eps_end",ep_endt="$eps_endt",discount="$discount",hist_len=5,learn_start="$learn_start",replay_memory="$replay_memory",update_freq="$update_freq",n_replay="$n_replay",network="$netfile",preproc="$preproc_net",state_dim="$state_dim",minibatch_size=32,rescale_r=1,ncols="$ncols",bufferSize=512,valid_size=500,target_q=10000,clip_delta=1,min_reward=-1,max_reward=1"


gpu=0
random_starts=5
pool_frms="type="$pool_frms_type",size="$pool_frms_size

num_threads=4

args="-framework $FRAMEWORK -game_path $game_path -env $ENV -agent $agent -agent_params $agent_params -actrep $actrep -gpu $gpu -random_starts $random_starts -seed $seed -threads $num_threads -games $games -pool_frms $pool_frms"
echo $args

cd dqn
../torch/bin/luajit record_game.lua $args
