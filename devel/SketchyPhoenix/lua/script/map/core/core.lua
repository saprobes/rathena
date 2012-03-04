--[[
	Dependencies for the map-server scripts.
--]]

local usermodulepath = ";./script/map/modules/?.lua"
local usermodulepathc = ";./script/map/modules/?.luac"
package.path = package.path .. usermodulepath .. usermodulepathc

dofile "script/map/core/functions.lua"
dofile "script/map/core/storage.lua"
dofile "script/map/core/const.lua"
dofile "script/map/core/events.lua"
