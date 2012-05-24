-- Registry module that player variables are stored in.
--
-- Variables that are temporary (not saved in the database) are stored
-- in the registry table.
--
-- require this module if needed in your scripts.

-- registry[] is saved to the db
-- account_registry[] is saved to the db
-- storage[] is temporary
local registry = {}
local account_registry = {}
local storage = {}

-- Used when a character logs out
function registry.destroy( CharID , reg )
	for k,v in pairs( reg[CharID] ) do
		if not k then break end
		reg[CharID[k]] = nil
	end
	reg[CharID] = nil
end

local function add( CharID , VarName , Value , reg )
	if not reg[CharID] then reg[CharID] = {} end
	reg[CharID][VarName] = Value
	return reg[CharID][VarName]
end

local function retrieve( CharID , VarName , Value , reg )
	return reg[CharID][VarName]
end

-- Called internally to assist with saving registries to the db
function LuaEngineSaveRegistry( char_id )
	local buf = {}
	
	-- Don't save any variables that don't have a value
	for k,v in pairs( registry[char_id] ) do
		if v == "" then k = nil end
		if v == nil then k = nil end
	end
	
	if not buf[char_id] then buf[char_id] = {} end
	
	for key,value in pairs( registry[char_id] ) do
		table.insert( buf[char_id] , tostring( key ) )
		table.insert( buf[char_id] , tostring( value ) )
	end
	
	saveregistry( buf[char_id], char_id )
end

function registry.set( CharID , VarName , Value )
	return add( CharID , VarName , Value , storage )
end

function registry.get( CharID , VarName )
	return retrieve( CharID , VarName , storage )
end

-- Permanent variables are saved to a cache when set and saved to the db later
function registry.setglobal( CharID , VarName , Value )
	return add( CharID , VarName , Value , registry )
end

function registry.getglobal( CharID , VarName , value )
	return retrieve( CharID , VarName , value , registry )
end

-- Account variables are also saved to a cache when set and saved later to the db
function registry.setglobal2( ID , VarName , Value )
	return add( ID , VarName , Value , account_registry )
end

function registry.getglobal2( ID , VarName )
	return retrieve( ID , VarName , Value , account_registry )
end

return registry