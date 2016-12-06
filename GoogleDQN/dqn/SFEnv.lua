--[[ Copyright 2014 Google Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
]]


function createEnv(romName, extraConfig)
    return SFEnv(romName, extraConfig)
end


-- Copies values from src to dst.
local function update(dst, src)
    for k, v in pairs(src) do
        dst[k] = v
    end
end

-- Copies the config. An error is raised on unknown params.
local function updateDefaults(dst, src)
    for k, v in pairs(src) do
        if dst[k] == nil then
            _print_usage(dst)
            error("unsupported param: " .. k)
        end
    end
    update(dst, src)
end

local Env = torch.class('SFEnv')
function Env:__init(romPath, extraConfig)
    self.config = {
        -- An additional reward signal can be provided
        -- after the end of one game.
        -- Note that many games don't change the score
        -- when loosing or gaining a life.
        gameOverReward=0,
        -- Screen display can be enabled.
        display=false,
        -- The RAM can be returned as an additional observation.
--        enableRamObs=false,
    }
    updateDefaults(self.config, extraConfig)

    self.win = nil
--    self.ale = alewrap.newAle(romPath)
--    local width = self.ale:getScreenWidth()
--    local height = self.ale:getScreenHeight()
--    local obsShapes = {{height, width}}
--    if self.config.enableRamObs then
--        obsShapes={{height, width}, {RAM_LENGTH}}
--    end
    self.envSpec = {
        nActions=3,
        obsShapes={{7056}},
    }
end

-- Returns a description of the observation shapes
-- and of the possible actions.
function Env:getEnvSpec()
    return self.envSpec
end

function Env:resetGame()
	-- Do the thingy from the .so here
end

-- Returns a list of observations.
-- The integer palette values are returned as the observation.
function Env:envStart()
    self:resetGame()
    return self:_generateObservations()
end


function Env:isTerminal()

	-- get terminal 
end

-- Does the specified actions and returns the (reward, observations) pair.
-- Valid actions:
--     {torch.Tensor(zeroBasedAction)}
-- The action number should be an integer from 0 to 17.
function Env:envStep(actions)
    assert(#actions == 1, "one action is expected")
    assert(actions[1]:nElement() == 1, "one discrete action is expected")

    if self:isTerminal() then
        self:isTerminal()
        -- The first screen of the game will be also
        -- provided as the observation.
        return self.config.gameOverReward, self:_generateObservations()
    end

    local reward = self.act(actions[1][1])
    return reward, self:_generateObservations()
end

function Env:act
	error("idk if this file is needed.")
end

function Env:getRgbFromPalette(obs)
	error("No rgb support yet.")
--    return alewrap.getRgbFromPalette(obs)
end

function Env:_createObs()
	error("I once trained for entire day without reaching this function.\n I pray this does nothing.")
--     The torch.data() function is provided by torchffi.
--    local width = self.getScreenWidth()
--    local height = self.getScreenHeight()
--    local obs = torch.ByteTensor(height, width)
--     What does this dooo?
--    self.ale:fillObs(torch.data(obs), obs:nElement())
--    return obs
end


function Env:_display(obs)
    require 'image'
    error("The display function has not yet been implemented.")
--    local frame = self:getRgbFromPalette(obs)
--    self.win = image.display({image=frame, win=self.win})
end

-- Generates the observations for the current step.
function Env:_generateObservations()
    local obs = self:_createObs()
    if self.config.display then
        self:_display(obs)
    end

    return {obs}
end

function Env:saveState()
    error("Saving states has not been implemented for this enviroment")
end

function Env:loadState()
	error("Loading states has not been implemented for this enviroment")
end

function Env:actions()
    local nactions = 3 -- change for other games
    local actions = torch.IntTensor(nactions)
    self.ale:actions(torch.data(actions), actions:nElement())
    return actions
end

function Env:lives()
    return 1
end

function Env:saveSnapshot()
	print("Warning: function 'saveSnapshot' is not implemented")
--    return self.ale:saveSnapshot()
end

function Env:restoreSnapshot(snapshot)
	print("Warning: function 'saveSnapshot' is not implemented")
--    self.ale:restoreSnapshot(snapshot)
end

function Env:getScreenWidth()
  return 7056
end

function Env:getScreenHeight()
  return 1
end

