require( 'DBI' )
dbh, err = DBI.Connect( 'SQLite3' , 'registry.db' )
dbh:autocommit(true)
DBI.Do( dbh, "CREATE TABLE IF NOT EXISTS registry(charID INTEGER, key TEXT, value TEXT)" )	

function SaveRegistry(char_id)
	local stmt = assert( dbh:prepare( "INSERT INTO registry( charID , key , value ) values( ? , ? , ? )" ) )

	for k,v in pairs( reg ) do
		stmt:execute( char_id , k , v )
	end
	
	for k,v in pairs( reg ) do
		if k then k = nil end
	end
	reg = nil
	
	stmt:close()
end

function LoadRegistry(char_id)
	local stmt = assert( dbh:prepare( "SELECT key,value FROM registry WHERE charID=?" ) )
	stmt:execute(char_id)
	
	local tbl = {}
	local row = stmt:fetch(true)
	while row do
		tbl[row.key] = row.value
	end
	
	--Here be a function to tell char server to send this TOP PRIORITY MESSAGE THAT'S REALLY IMPORTANT
	--to the map server. Too lazy to write that as of now :(
	stmt:close()
end