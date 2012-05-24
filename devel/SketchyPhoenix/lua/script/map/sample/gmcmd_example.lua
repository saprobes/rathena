-- Ok here's an example of how the scripted GM commands work.
-- addgmcommand() does exactly what it says on the tin :V 
-- The first parameter is the command name, while the second param is the function it calls if the command is found.
-- It's a basic implementation but these kind of commands do have full access to the rest of the
-- scripting engine, so feel free to get creative here!
local dbt = require( "script/map/core/storage" )

function gmcmd_heal()
	percentheal(100,100)
	disp("Your HP/SP have been restored!")
	print("health restored")
end

function scriptreg()
	dbt.setglobal( char_id, "TestK1", "TestV1" )
	dbt.setglobal( char_id, "TestK2", "TestV2" )
	dbt.setglobal( char_id, "TestK3", "TestV3" )
	dbt.setglobal( char_id, "TestK4", "TestV4" )
	LuaEngineSaveRegistry( char_id )
end

addgmcommand("scriptheal",gmcmd_heal)
addgmcommand("scriptreg",scriptreg)