# MDBC
A project that allow to connect multiple databases from a switch server who interacts with a client

# Compilation commands
  gcc sql_conn.c -Wall -g -c `mysql_config --cflags --libs` // For compile a object
	
  gcc -o test switchServer.c mySql_conn.o `mysql_config --cflags --libs` // Compile a switchserver + sql obj

# Version
	# Version 0.0.1
		- SWServer can do a query to MySQL

		- SWServer reads a config file to configure some parameters to do connection


# TO-DO
  - From SWServer
		
		- Object for do connection to PostgreSQL and FireBird
	
		- Method to get metadata(like name of databases) from databases 
	
		- Method to establish a sock connection with the client
 
	

- The client(on Java)
