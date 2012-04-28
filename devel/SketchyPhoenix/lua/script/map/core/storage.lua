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


-- Used when a character logs out
function registry.destroy( CharID , reg )
	while true do
		local k = next( reg[CharID] )
		if not k then break end
		reg[CharID][k] = nil
	end
end

local function add( CharID , VarName , Value , reg )
	if not reg[CharID] then reg[CharID] = {} end
	reg[CharID][VarName] = Value
	return reg[CharID][VarName]
end

local function retrieve( CharID , VarName , Value , reg )
	return reg[CharID][VarName]
end


return registry
