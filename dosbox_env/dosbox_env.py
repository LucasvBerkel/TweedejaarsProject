import gym
from gym.utils import seeding
from gym import spaces
import numpy as np
from time import sleep
import sendpress
import getframe
import findwindow
import os
import string
import subprocess
import fastforward


class DOSBoxEnv(gym.Env):
	metadata = {'render.modes': ['rgb_array', 'human']}
	# Open AI gym action space id and the keycode 
	
	def dosbox_get_window(self):
		return findwindow.findDOSBoxWindow()

	def dosbox_get_screen(self):
		return np.array(getframe.getFrame(self.window_id)) # Return numpy ndarry with grayscale lumiance values

	def dosbox_send_action(self, a):
		# Read in the file after the sent event
		sendpress.sendPress(self.window_id, a)

	def __init__(self, game='aim'):
		actions_aim = {0 : 32,  1 : 65361, 2 : 65363}
		actions_SF = {0 : 32,  1 : 65361, 2 : 65363, 3 : 65362}
		# start the dosbox-x enviroment with SF/aim launched (or maybe later so that we first can acces 
		# it's resolution?, set the resolution in advance with the conf file?)
		# try to finilaze the init before the game has started, as to give the system a change to respond
		# border_size = # Calculate in a nicer way? 
		# score_size = 
		self._seed()
		subprocess.Popen(["dosbox-x", "-c", " cd " + game.upper(), "-c", "OBJ\\" + game.upper()])
		if game == 'aim':
			self._action_set = actions_aim	
		else:
			self._action_set = actions_SF

		self.prev_score = None
		self.score_path = os.getenv("HOME") + "/DOS/" + game.upper() + "/SAVE/SCORE.TXT" 

		# Find the Dosbox window ID with the C++ function 
		# size = cat $(dosbox-x -printconf) | grep windowresolution
		try:
			# screen_width, screen_height = size.split("=")[1].split("x")
			raise ValueError
		except ValueError as e:
			# provide a better guess
			print("No dosbox window size found, please set it in your .conf! \n Guessing size of 800x600")
			screen_width, screen_height = (444, 223)

		self.observation_space = spaces.Box(low=0, high=15, shape=(screen_height * screen_width,))
		self.action_space = gym.spaces.Discrete(len(self._action_set))
		sleep(3)
		# Set the game to a faster speed
		self.window_id = self.dosbox_get_window()
		fastforward.fastForward(self.window_id)

	@property
	def _n_actions(self):
		return len(self._action_set) 

	def _seed(self, seed=None):
		self.np_random, seed = seeding.np_random(seed)
		return [seed]

	def _step(self, a):
		# reward = 0.0
		# action = self._action_set[a]
		# num_steps = self.np_random.randint(2, 5)
		# for _ in range(num_steps):
		# reward += self.ale.act(action)
		action = self._action_set[a]
		# consider doing more than one action like above
		print("Somebody asked for a screen")
		reward = 0.0
		done = False

		with open(self.score_path, 'r') as score_file:
			self.dosbox_send_action(action)
			reward = score_file.read() # Read in the reward
			if reward == "FALSE": # Done with the game, read this string in from the DOSBox score file
				done = True
				reward = self.prev_score
			else:
				# print("Very unrewardiing", reward)
				try:
					reward = int(reward)
				except ValueError: # Sometimes reward is equal to the empty string for some reason
					print('empty score')
					reward = self.prev_score


		score_file.close()
		ob = self.dosbox_get_screen()
		self.prev_score = reward
		return ob, reward, done, {}

	def _render(self, mode='human', close='IK GEEF GEEN FUCK'):
		# Nothing special to be done (or so it seems?), but if needed some sort of pause/play like process
		# can be done to emulate rendering
		return self.dosbox_get_screen() # Possibly problematic, maybe need to return dosbox_screen array

			
	# return: (states, observations)
	def _reset(self):
		self.dosbox_send_action(65289) # Get past the gameover screen, code for the 'tab key'
		return self.dosbox_get_screen() # For some reason should show the observation 
