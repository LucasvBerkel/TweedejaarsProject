# -*- coding: utf-8 -*-

#!/usr/bin/env python
import gym
import sys
import numpy as np
from random import random
import cv2 # remove at one point
from time import sleep


env = gym.make('SFS-v0')

# Configure enviroment
#-------------------------------

env.configure(mode="human", record_path=None, no_direction=False, frame_skip=2)

moves = []
with open("moves.txt", "r") as f:
	moves = [int(m) for m in f.readlines()]
f.close()

for i, m in enumerate(moves):
	observation, reward, done, _ = env.step(m-1) # -1 because lua has non-zero indexing

	env.save_image("seq_im/im"+str(i)+".png")

	if done:
		env.reset()
	#			print("Done!")
	#			count += 1
				# print("Episode finished after {} timesteps".format(t+1))
	#				break

#	env.write_out_stats("test")
#	env.close()
