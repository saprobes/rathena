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

--[[

This example of a lua module uses tables to store information similar to how a class stores information
if you are familiar with basic OOP concepts.

Anything defined locally to the table can only be accessed from inside the table, meaning you can't 
call the test() function directly with sample.test() - it has to be called from another function inside
the table, like sample.callprivatetest().

Using modules, you are able to organize your code into seperate parts that, like a toolbox, you can
simply use them when needed.

--]]