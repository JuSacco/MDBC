# MDBC
A project that allow to connect multiple databases from a switch server who interacts with a client

# Compilation commands
  gcc mainServer.c -o switchServer posgresql.o mysql_conn.o firebird.o `mysql_config --cflags --libs` -lpq -lxml2 -lfb // For compile a object
  
  //Compile modules	
  • mySQL: gcc mysql_conn.c -Wall -g -c `mysql_config --cflags --libs`
  • postgreSQL: gcc posgresql.c -Wall -g -c -lpq -I/usr/include/postgresql/
  • firebird: gcc firebird.c -Wall -g -c -I /usr/include/firebird/ -lfb
  
# Version
	# Version 0.1
		- SWServer can do a query to MySQL

		- SWServer reads a config file for configure some parameters to do connection
	# Version 0.2
		- Functions to read configuration files of all diferent databases 
	# Version 0.3
		- mainServer now stablishes connection to firebird, posgresql and mysql

# Running environment
	Databases servers:
		MySQL 5.7 running on Windows 7
		PostgreSQL 11.1.1 with pgAdmin 4 running on Debian
		Firebird 3.0 running on Debian
	Switch server: running on Debian 
	

# TO-DO

- The client(on Java)
