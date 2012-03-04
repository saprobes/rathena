--[[
	Temporary script variables are stored in Lua and are organized like this:
	
	StorageTable[CharID][VariableName] = Value
	
	this is stored like a dictionary so it's rather easy to access variables without
	having to do a long string of loops to figure it out.
--]]

module("storage",package.seeall)
--
-- Temporary Player/NPC Variable Storage
--
local _V = {}

function Set(oid, name, value)
	local buf = "O_" .. oid
	if _V[buf] == nil then _V[buf] = {} end
	_V[buf][name] = value
	return _V[buf][name]
end

function Get(oid, name)
	local buf = "O_" .. oid
	return _V[buf][name]
end

function npcset(oid, name, value)
	return set(oid,name,value)
end

function npcget(oid, name, value)
	return get(oid,name,value)
end

function set( oid, name, value )
	return Set( oid, name, value )
end

function get( oid, name, value )
	return Get( oid, name, value )
end


--
-- Permanent Player Variable Storage
--

local _P = {}

function setstatic(cid, name, value)
	if _P[cid] == nil then _P[cid] = {} end
	_P[cid][name] = value
	return _P[cid][name]
end

function getstatic(cid, name)
	return _P[cid][name]
end

--

function Save_P(cid)

end