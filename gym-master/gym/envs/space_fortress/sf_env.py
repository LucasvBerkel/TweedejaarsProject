#!/usr/bin/env python
# -*- coding: utf-8 -*- 


import gym
from gym.utils import seeding
from gym import spaces
import ctypes
from time import sleep
from sys import platform
import numpy as np
import cv2

class SFEnv(gym.Env):
	# Human renders a full RGB version of the game at the original size, while minimal only shows
	# the data the network 
	# Human_Sleep is the same as above, but with an added delay to approximately emulate the
	# original game's speed
	metadata = {'render.modes': ['rgb_array', 'human', 'human_sleep', 'minimal'], 'configure.required' : True}
	# Open AI gym action space id and the keycode 

	def __init__(self, game='SF'):
		self.game = game
		self.prev_score = 0
		self.screen_height = 468
		self.screen_width = 448
		self.scale = 5 # The amount of (down) scaling of the screen height and width 
		# Space, left, right, up, nothing
		actions_SF = {0 : 32,  1 : 65361, 2 : 65363, 3 : 65362, 4 : 0}
		self._seed()
		if game.lower().startswith("sf"):
			self._action_set = actions_SF
		else:
			pass

		# The number of bytes to read in from the returned image pointer
		self.n_bytes = ((self.screen_height//self.scale) * (self.screen_width//self.scale))
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
		action = self._action_set[a]
		self.act(action)
#		print('Bef as array')
		new_frame = self.update().contents
		ob = np.ctypeslib.as_array(new_frame)
		reward = self.score() - self.prev_score
		self.prev_score = self.score()
#		print('After as array')
		return ob, reward, self.terminal_state(), {}


	# We ignore the mode parameter here because it's set in _configure
	# Not entirely sure what close here does, although you probably have to implemented this 
	# behaviour yourself
	def _render(self, mode=None, close=False): 
		if not self.mode == 'rgb_array':
			zzz = 1
			if self.mode=='minimal':
				new_frame = self.screen().contents
				img = np.ctypeslib.as_array(new_frame)
				img = np.reshape(img, (self.screen_height//self.scale, self.screen_width//self.scale))
			elif self.mode.startswith('human'):
				new_frame = self.pretty_screen().contents
				img = np.ctypeslib.as_array(new_frame)
				if mode=='human_sleep':	
					zzz = 57 # Sleep for about 50 ms, the original delay (more because it seemed fast)
#					zzz = 0.048
				img = np.reshape(img, (self.screen_height, self.screen_width, 2))
				img = cv2.cvtColor(img, cv2.COLOR_BGR5652RGB)
				img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
			cv2.imshow('Space Fortress', img)
#			sleep(0.001)
			cv2.waitKey(zzz)
#		print('Done rendering')

	# return: (states, observations)
	def _reset(self):
		self.reset()
		screen = self.screen().contents
		obv = np.ctypeslib.as_array(screen)
		return obv # For some reason should show the observation 	

	def _close(self):
		self.stop_drawing()

	def _configure(self, mode='rgb_array'):
		self.mode = mode
		os = platform

		# Init the opencv window
		if self.mode != 'rgb_array':
			cv2.namedWindow('Space Fortress')

		if self.mode.startswith('human'):
			libname = "sf_frame_lib_FULL.so"
		else:
			libname = "sf_frame_lib.so"

		self.update = ctypes.CDLL('./'+os+'/'+libname).update_frame_SF
		self.init = ctypes.CDLL('./'+os+'/'+libname).start_drawing
		self.act = ctypes.CDLL('./'+os+'/'+libname).set_key
		self.reset = ctypes.CDLL('./'+os+'/'+libname).reset_sf
		self.screen = ctypes.CDLL('./'+os+'/'+libname).get_screen
		self.terminal_state = ctypes.CDLL('./'+os+'/'+libname).get_terminal_state
		self.score = ctypes.CDLL('./'+os+'/'+libname).get_score
		self.stop_drawing = ctypes.CDLL('./'+os+'/'+libname).stop_drawing
		self.pretty_screen = ctypes.CDLL('./'+os+'/'+libname).get_original_screen

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













