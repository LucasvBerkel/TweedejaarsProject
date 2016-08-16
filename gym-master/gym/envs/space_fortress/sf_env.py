#!/usr/bin/env python
# -*- coding: utf-8 -*-


import gym
from gym.utils import seeding
from gym import spaces
import ctypes
from time import sleep
from sys import platform
import datetime
import numpy as np
import cv2
#import atexit
import os
import csv

import sys

class SFEnv(gym.Env):
	# Human renders a full RGB version of the game at the original size, while minimal only shows
	# the data the network
	# Human_Sleep is the same as above, but with an added delay to approximately emulate the
	# original game's speed
	metadata = {'render.modes': ['rgb_array', 'human', 'human_sleep', 'minimal', 'minimal_sleep'], 'configure.required' : True}
	# Open AI gym action space id and the keycode

	def __init__(self, game='SFS'):
		if game=="SFS":
			self.game_name = "Simple Space Fortress V2"
		elif game=="SF":
			self.game_name = "Space Fortress"
		elif game=="AIM":
			self.game_name = "Aiming Task"
		else:
			import sys
			print("Invalid game name")

		self.mode = 'rgb_array' # This gets overwritten by configure
		self.game = game
		self.prev_score = 0
		self.screen_height = 448
		self.screen_width = 448
		self.scale = 5.6 # The amount of (down) scaling of the screen height and width
		# Space, left, right, up, nothing
		actions_SFS = {0 : 32,  1 : 65363, 2 : 65362, 3 : 65361}

		# stat collectors
		self.terminal_states = []


		self._seed()
		if game.lower().startswith("sfs"):
			self._action_set = actions_SFS
		else:
			pass

#		atexit.register(exit_handler)

		# The number of bytes to read in from the returned image pointer
		self.n_bytes = ((int(self.screen_height/self.scale)) * (int(self.screen_width/self.scale)))
		# ... which happens to be equal to the amount of pixels in the image
		self.observation_space = spaces.Box(low=0, high=255, shape=(1, self.n_bytes))
		self.action_space = gym.spaces.Discrete(len(self._action_set))



	@property
	def _n_actions(self):
		return len(self._action_set)

	def _seed(self, seed=None):
		self.np_random, seed = seeding.np_random(seed)
		return [seed]

	def _step(self, a):
		# print(a)
		action = self._action_set[a] # Select the action from the action dict
		self.act(action)
		ob = np.ctypeslib.as_array(self.update().contents)
		reward = self.score() - self.prev_score
		ending = self.terminal_state()
#		if self.write_stats:  	# maybe write out the stats here?
		if ending:
			self.terminal_states += [ending]
			self.prev_score = 0
			return ob, 0, True, {} # no reward in terminal state
		self.prev_score = self.score()
		return ob, reward, False, {}


	# We ignore the mode parameter here because it's set in _configure
	# Not entirely sure what close here does, although you probably have to implemented this
	# behaviour yourself
	def _render(self, mode=None, close=False):
		if not self.mode == 'rgb_array':
			zzz = 1
			if self.mode.startswith('minimal'):
				new_frame = self.screen().contents
				img = np.ctypeslib.as_array(new_frame)
				img = np.reshape(img, (int(self.screen_height/self.scale), int(self.screen_width/self.scale)))
				if self.mode == 'minimal_sleep':
					zzz = 44
			elif self.mode.startswith('human'):
				new_frame = self.pretty_screen().contents
				img = np.ctypeslib.as_array(new_frame)
				if self.mode=='human_sleep':
					zzz = 43 # Sleep for about 50 ms, the original delay (more because it seemed fast)
#					zzz = 0.048
				img = np.reshape(img, (self.screen_height, self.screen_width, 2))
				img = cv2.cvtColor(img, cv2.COLOR_BGR5652RGB)
				img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
			if self.record_path is not None:
				current_time = str(datetime.datetime.now().time().isoformat()).replace("/", ":")
				cv2.imwrite(self.record_path + "/sf" + current_time + ".png", img)
			cv2.imshow(self.game_name, img)
			cv2.waitKey(zzz)
#		print('Done rendering')

	# return: (states, observations)
	def _reset(self):
		self.reset_sf()
		self.prev_score = 0
		screen = self.screen().contents
		obv = np.ctypeslib.as_array(screen)
		return obv # For some reason should show the observation


	def write_out_stats(self , file_id=None):
		current_time = str(datetime.datetime.now().time().isoformat()).replace("/", ":")
		id = file_id if file_id else current_time
		SHIP_WON = 1 # some constant from the c interface
		keys = ["Won"]
		with open(os.path.join('gym_stats', self.game_name+"-"+id+'.csv'), 'wb') as csvfile:
			dict_writer = csv.DictWriter(csvfile, fieldnames=keys)
			dict_writer.writeheader()
			for t in self.terminal_states:
				dict_writer.writerow({"Won" : t == 1})

		self.terminal_states = []

			# ...
			# Add more rows here

		csvfile.close()




#	def exit_handler(self):
#		print("Writing stats...")
#		self._close()


	def _close(self):
#		if self.write_stats:
#			self.write_out_stats()
		# maybe condition the stats?
#		self.write_out_stats()
		self.stop_drawing()

	def _configure(self, mode='rgb_array', record_path=None, write_stats=True):
		self.mode = mode
		os = platform
		# Init the opencv window
		if self.mode != 'rgb_array':
			cv2.namedWindow(self.game_name)

		if self.mode.startswith('human'):
			libname = "sf_frame_lib_FULL.so"
		else:
			libname = "sf_frame_lib.so"

		if os.startswith('linux'): # come up with something nicer for this:
			libpath = "/home/wijnand/Documents/git/TweedejaarsProject/gym-master/gym/envs/space_fortress/linux2"
		elif os.startswith('darwin'):
			libpath = "/Users/rijnderwever/Desktop/NLR/NLR/TweedejaarsProject/gym-master/gym/envs/space_fortress/darwin"

		self.update = ctypes.CDLL(libpath + '/'+libname).update_frame_SF
		self.init = ctypes.CDLL(libpath +'/'+libname).start_drawing
		self.act = ctypes.CDLL(libpath +'/'+libname).set_key
		self.reset_sf = ctypes.CDLL(libpath +'/'+libname).reset_sf
		self.screen = ctypes.CDLL(libpath +'/'+libname).get_screen
		self.terminal_state = ctypes.CDLL(libpath +'/'+libname).get_terminal_state
		self.score = ctypes.CDLL(libpath +'/'+libname).get_score
		self.stop_drawing = ctypes.CDLL(libpath +'/'+libname).stop_drawing
		self.pretty_screen = ctypes.CDLL(libpath +'/'+libname).get_original_screen

		# Configure how many bytes to read in from the pointer
		# c_ubyte is equal to unsigned char
		self.update.restype = ctypes.POINTER(ctypes.c_ubyte * self.n_bytes)
		self.screen.restype = ctypes.POINTER(ctypes.c_ubyte * self.n_bytes)
		# 468 * 448 * 2 (original size times something to do with 16 bit images)
		sixteen_bit_img_bytes = self.screen_width * self.screen_height * 2
		self.pretty_screen.restype = ctypes.POINTER(ctypes.c_ubyte * sixteen_bit_img_bytes)


		# Initialize the game's drawing context and it's variables
		# I would rather that this be in the init method, but the OpenAI developer himself stated
		# that if some functionality of an enviroment depends on the render mode, the only way
		# to handle this is to write a configure method, a method that is only callable after the
		# init
		self.init()
		self.record_path = record_path
