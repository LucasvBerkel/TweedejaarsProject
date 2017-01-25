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


-- The GameEnvironment class.
-- the torch.class call creates a global variable containing this class. You can access it's
-- init by calling it as a function with "_opt" as an argument
local gameEnv = torch.class('SFGameEnvironment') -- Maybe should use a dot
local ffi = require "ffi"

function gameEnv:__init(_opt)

	-- Load the .so game lib
	self.lib = ffi.load(_opt.game_path)
	print("INIT! 🍧")

	-- [[ is a fucking string in lua
	if _opt.full_render then
		print("full rendering!")
		ffi.cdef([[
		void start_drawing();
		unsigned char* update_screen();
		void SF_iteration();
		void set_key(int key_value);
		float get_score();
		int get_terminal_state();
		void reset_sf();
		unsigned char* get_original_screen();
		]])
	else
		ffi.cdef([[
		void start_drawing();
		unsigned char* update_screen();
		void SF_iteration();
		void set_key(int key_value);
		float get_score();
		int get_terminal_state();
		void reset_sf();
		]])
	end

	local n_pixels = 84*84

	self.buf = ffi.new("uint8_t[?]", n_pixels)
	if _opt.full_render then
		self.pretty_buf = ffi.new("uint8_t[?]", 448*448*2)
		self.pretty_frame = torch.ByteTensor(448*448*2)
	end
	self.frame = torch.ByteTensor(n_pixels)

	self.lib.start_drawing()

  local _opt = _opt or {}


    -- defaults to emulator speed
    self.game_path      = _opt.game_path or '.'
    self.verbose        = _opt.verbose or 0
    self._actrep        = _opt.actrep or 1
    self._random_starts = _opt.random_starts or 1
    -- GameScreen is something like a class or something
--    self._screen        = alewrap.GameScreen(_opt.pool_frms, _opt.gpu)
    self:reset(_opt.env, _opt.env_params, _opt.gpu)
    return self
end


function gameEnv:_updateState(reward, terminal)
    self._state.reward       = reward
    self._state.terminal     = terminal
    self._state.prev_lives   = 1
    self._state.lives        = 1
    return self
end


function gameEnv:getState()
    -- grab the screen again only if the state has been updated in the meantime
--    self._state.observation = self._state.observation or self._screen:grab():clone()
--    self._state.observation:copy(self._screen:grab())
--
--    -- lives will not be reported externally
   	self.buf = self.lib.update_screen()
	ffi.copy(self.frame:data(), self.buf, 7056)
	self._state.observation = self.frame
    return self._state.observation, self._state.reward, self._state.terminal
end


function gameEnv:reset(_env, _params, _gpu)
    local env
--    local params = _params or {useRGB=true}
    -- if no game name given use previous name if available
--    if self.gamse then
    env = "SFS"
    env = _env or env


    self._actions   = self:getActions()
   	-- this function comes from  SF_Layer
    -- idk fuck this maybe, can do this above
--    self.game       = alewrap.game(env, params, self.game_path)

    -- start the game
    if self.verbose > 0 then
        print('\nPlaying:', "SFS")
    end

    self.lib.reset_sf()

    self:_resetState()
    self:_updateState(self:_step(1))
    self:getState()
    return self
end


function gameEnv:_resetState()
--    self._screen:clear()
    self._state = self._state or {}
    return self
end


-- Function plays `action` in the game and return game state.
function gameEnv:_step(action)

	self.lib.set_key(action)
	self.lib.SF_iteration()

--	  .so code here
--    assert(action)
--    local x = self.game:play(action)
--    self._screen:paint(x.data)
    return self.lib.get_score(), self.lib.get_terminal_state() == 1
end


-- Function plays one random action in the game and return game state.
function gameEnv:_randomStep()
    return self:_step(torch.random(#self._actions))
end


function gameEnv:step(action, training)
    -- accumulate rewards over actrep action repeats
    local cumulated_reward = 0
    local frame, reward, terminal
    for i=1,self._actrep do
        -- Take selected action; (ATARI games' actions start with action "0".)
        reward, terminal = self:_step(action)

        -- accumulate instantaneous reward
        cumulated_reward = cumulated_reward + reward
        -- Loosing a life will trigger a terminal signal in training mode.
        -- We assume that a "life" IS an episode during training, but not during testing


        -- game over, no point to repeat current action
        if terminal then break end
    end
   	self.buf = self.lib.update_screen()
   	-- 7056 is the amount of pixels of a 84x84 frame
   	-- which you do times a char, which is like 1 byte, so times 1
	ffi.copy(self.frame:data(), self.buf, 7056)
    -- lives is always equal to 1
    -- (or make it equal to the terminal? Yeah do that and give this a special .so)
    self:_updateState(cumulated_reward, terminal)
    return self:getState()
end


--[[ Function advances the emulator state until a new game starts and returns
this state. The new game may be a different one, in the sense that playing back
the exact same sequence of actions will result in different outcomes.
]]
function gameEnv:newGame()
--    local reward, terminal
--    terminal = self._state.terminal
--    while not terminal do
	self.lib.reset_sf()
    reward, terminal = self:_randomStep()
--    end
--    self._screen:clear()
    -- take one null action in the new game
    self.buf = self.lib.update_screen()
    ffi.copy(self.frame:data(), self.buf, 7056)
    return self:_updateState(reward, terminal):getState()
end


--[[ Function advances the emulator state until a new (random) game starts and
returns this state.
]]
function gameEnv:nextRandomGame(k)
    local obs, reward, terminal = self:newGame()
--    k = k or torch.random(self._random_starts)
--    for i=1,k-1 do
--        reward, terminal  = self:_step(1)
--        if terminal then
--            print(string.format('WARNING: Terminal signal received after %d 0-steps', i))
--        end
--    end
    return self:_updateState(self:_step(1)):getState()
end


--[[ Function returns the number total number of pixels in one frame/observation
from the current game.
]]
function gameEnv:nObsFeature()
--    return self.game:nObsFeature()
	return 7056
end

function gameEnv:pretty_screen()
	self.pretty_buf = self.lib.get_original_screen()
	ffi.copy(self.pretty_frame:data(), self.pretty_buf, 448*448*2)
	return self.pretty_frame
end

-- Function returns a table with valid actions in the current game.
function gameEnv:getActions()
	-- Adjust this to not control tasks
	local str = require("string")
	if str.find(self.game_path, "aim") then
	    return {65361, 32, 65363}
	elseif str.find(self.game_path, "control") then
	    return {65361, 65362, 65363}
	else -- Assume SFS
		return {65361, 65362, 65363, 32} -- Add a NOOP?
	end
end
