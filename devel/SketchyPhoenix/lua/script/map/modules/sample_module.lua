--[[
	Example of a module in Lua. If you're familiar with OOP concepts, you can see here
	that Lua is able to simulate OOP with tables.
	
	module() should not be used as that is deprecated in Lua 5.2
--]]

-- sample_module.lua

local SM = {}

-- private

local x = 1
local function test() print 'test' end

-- public

function SM.public() print("SM.public test") end
function SM.callprivatex() print(x) end
function SM.callprivatetest() test() end

return SM

-- example usage:
-- local sample = require 'sample_module'
-- sample.public()