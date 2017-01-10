# -*- coding: utf-8 -*-

#!/usr/bin/env python
import gym
import sys
import numpy as np
from random import random
render_mode = "minimal"
current_key = 3


env = gym.make('SFC-v0')

# Configure enviroment
#-------------------------------
env.configure(mode=render_mode, record_path=None, no_direction=False, frame_skip=1)
actions = [1, 2]

for t in range(250000):
	env.render()
	observation, reward, done, info = env.step(actions[t%2])
#	print(reward)

