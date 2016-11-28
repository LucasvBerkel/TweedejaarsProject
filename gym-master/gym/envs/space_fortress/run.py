# -*- coding: utf-8 -*-

#!/usr/bin/env python
import gym
import sys
import numpy as np
from random import random
import cv2 # remove at one point
from time import sleep
try:
	from pynput.keyboard import Key, Listener
except:
	print("You don't seem to have 'pynput' installed 😕 . Install it with 'pip install pynput'. \n")
	exit(0)



def on_press(key):
	#		LEFT = 0
	#		UP = 1
	#		RIGHT = 2
	#		SHOOT = 3
	global current_key

	key_to_action = {Key.left : 0, Key.up : 1, Key.right : 2, Key.space : 3, Key.down : 3}

	if key in key_to_action.keys():
		current_key = key_to_action[key]
	else:
		current_key = 4


def on_release(key):
	pass

global render_mode
render_mode = "minimal_debug"
current_key = 3
if render_mode.endswith("debug"):
	print("Note that this script should be run as super user under OS X 👁")


def on_release(key):
	pass

global render_mode
render_mode = "minimal_debug"
current_key = 3
if render_mode.endswith("debug"):
	print("Note that this script should be run as super user under OS X 👁")


env = gym.make('SFC-v0')
# setting the render mode does not actually	work

# Configure enviroment

#-------------------------------
# ❗️👁❗️NOTE THE NO_DIR PARAM
env.configure(mode=render_mode, record_path=None, no_direction=False, lib_suffix="GRID")

# env.step = env._step2

# Collect events until released
count = 0
with Listener(on_press=on_press, on_release=on_release) as listener:
	for game in range(5):
		env.reset()
		for t in range(250000):
			env.render()
			if render_mode.endswith('debug'):
				action = current_key
			else:
				action = env.action_space.sample()


			# Uncomment this for perfect play 👌
	#           ==============================
	#			if 0.7 < random():
	#				action = env.best_action()
	#			else:
	#				env.best_waction()
	#			==============================
			observation, reward, done, info = env.step(action)

#			print(reward)

			if done:
				print("terminal")
				break
	#			print("Done!")
	#			count += 1
				# print("Episode finished after {} timesteps".format(t+1))
	#				break


			# Uncomment this for perfect play 👌
#           ==============================
			# action = env.best_action()
#			==============================
			observation, reward, done, info = env.step(action)

			print(reward)
			if done:
				print("terminal")
				break
#			print("Done!")
#			count += 1
				# print("Episode finished after {} timesteps".format(t+1))
#				break
	print(count)
#	env.write_out_stats("test")
#	env.close()
