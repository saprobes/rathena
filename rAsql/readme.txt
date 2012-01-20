:######################################################################## 
:# File name: readme.txt
:# Created By: rAthena Development Team
:# Edited Last By: Gepard
:# V 1.1 20.01.2012
:######################################################################## 

This server is meant for locally TESTING rAthena only,
And should not be used for hosting a public rAthena server.
To host a public rAthena server, you should use a Dedicated Server or VPS.

Server uses the standard port 3306, and will only accepts connections from localhost.
The default username/password is ragnarok/ragnarok .

###################
Starting the server
###################
Run Server by double clicking on mysql_start.bat this creates a virtual drive.
The drive letter used is the first available letter found.

########################
Shutting down the server
########################
Double click on mysql_stop.bat. This shuts-down server and removes virtual drive

####################
Change virtual drive
####################
The server automatically detects the first free drive letter and uses that.
You can override this following these instructions:

   Edit mysql_start.bat
   Locate the following line:
    rem set Disk=w 
   Remove the rem and replace w with the letter you want to use.
   Example:
   set Disk=X
   Forces the server to use drive letter X

#######################
Change default password
#######################

#need more documenting#

the following line in mysql_stop.bat needs to be edited to the new password -
udrive\bin\mysqladmin.exe --port=3306 --user=ragnarok --password=ragnarok shutdown


-----------------------------------------------------------