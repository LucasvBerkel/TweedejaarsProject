--[[
Copyright (c) 2014 Google Inc.

See LICENSE file for full terms of limited license.
]]

if not dqn then
    require "initenv"
end

local cmd = torch.CmdLine()
cmd:text()
cmd:text('Train Agent in Environment:')
cmd:text()
cmd:text('Options:')

cmd:option('-framework', '', 'name of training framework')
cmd:option('-env', '', 'name of environment to use')
cmd:option('-game_path', '', 'path to environment file (ROM)')
cmd:option('-env_params', '', 'string of environment parameters')
cmd:option('-pool_frms', '',
           'string of frame pooling parameters (e.g.: size=2,type="max")')
cmd:option('-actrep', 1, 'how many times to repeat action')
cmd:option('-random_starts', 0, 'play action 0 between 1 and random_starts ' ..
           'number of times at the start of each training episode')

cmd:option('-network', '', 'reload pretrained network')
cmd:option('-agent', '', 'name of agent file to use')
cmd:option('-agent_params', '', 'string of agent parameters')
cmd:option('-seed', 1, 'fixed input seed for repeatable experiments')
cmd:option('-saveNetworkParams', false,
           'saves the agent network in a separate file')

cmd:option('-games', 3, 'number of games to play')
cmd:option('-full_render', 1, 'enable full-size and RGB color rendering')

cmd:option('-verbose', 2,
           'the higher the level, the more information is printed to screen')
cmd:option('-threads', 1, 'number of BLAS threads')
cmd:option('-gpu', -1, 'gpu flag')

cmd:text()

local opt = cmd:parse(arg)
if opt.full_render > 0 then
  opt.full_render = true
else
  opt.full_render = false
end
--- General setup.
local game_env, game_actions, agent, opt = setup(opt)


-- override print to always flush the output
local old_print = print
local print = function(...)
    old_print(...)
    io.flush()
end

local learn_start = agent.learn_start
local start_time = sys.clock()
local reward_counts = {}
local episode_counts = {}
local time_history = {}
local v_history = {}
local qmax_history = {}
local td_history = {}
local reward_history = {}
local step = 0
time_history[1] = 0

local total_reward
local nrewards
local nepisodes
local episode_reward

local screen, reward, terminal = game_env:getState()

-- Delete all this! 👁 🍐
--local hard_stop = 0
--require 'image'
i = 0

print("Playing" .. opt.games .. "games")
local eval_time = sys.clock()
moves_file = io.open("moves.txt", "a")
io.output(moves_file)
for g=1,opt.games do


  screen, reward, terminal = game_env:newGame()

  total_reward = 0
  nrewards = 0
  nepisodes = 0
  episode_reward = 0


  while not terminal do
      local action_index = agent:perceive(reward, screen, terminal, true, 0.05)
      -- Play game in test mode (episodes don't end when losing a life)
      screen, reward, terminal = game_env:step(game_actions[action_index])
      if opt.full_render then
        -- print(game_env:pretty_screen(), type(game_env:pretty_screen()))
        -- print(game_env:pretty_screen():size())
        screen = torch.reshape(game_env:pretty_screen(), 448, 448, 2)
      else
        screen = torch.reshape(screen, 84, 84)
      end
      io.write(action_index, "\n")
      image.save("stills/im" .. i .. ".png", screen)
      i = i + 1
      -- if estep%1000 == 0 then collectgarbage() end

      -- record every reward
      episode_reward = episode_reward + reward
      if reward ~= 0 then
         nrewards = nrewards + 1
      end

      if terminal then
          total_reward = total_reward + episode_reward
          episode_reward = 0
          nepisodes = nepisodes + 1
      end
  end
end

io.close()
eval_time = sys.clock() - eval_time
start_time = start_time + eval_time
agent:compute_validation_statistics()
local ind = #reward_history+1
total_reward = total_reward/math.max(1, nepisodes)


if agent.v_avg then
   v_history[ind] = agent.v_avg
   td_history[ind] = agent.tderr_avg
   qmax_history[ind] = agent.q_max
end
print("V", v_history[ind], "TD error", td_history[ind], "Qmax", qmax_history[ind])

reward_history[ind] = total_reward
reward_counts[ind] = nrewards
episode_counts[ind] = nepisodes

time_history[ind+1] = sys.clock() - start_time

local time_dif = time_history[ind+1] - time_history[ind]

local training_rate = opt.actrep*opt.eval_freq/time_dif

print(string.format(
   '\nSteps: %d (frames: %d), reward: %.2f, epsilon: %.2f, lr: %G, ' ..
   'training time: %ds, training rate: %dfps, testing time: %ds, ' ..
   'testing rate: %dfps,  num. ep.: %d,  num. rewards: %d',
   step, step*opt.actrep, total_reward, agent.ep, agent.lr, time_dif,
   training_rate, eval_time, opt.actrep*opt.eval_steps/eval_time,
   nepisodes, nrewards))
