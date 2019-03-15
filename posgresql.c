#include <postgresql/libpq-fe.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <netdb.h>

void funcionPostgresql(char IPPostgrsql[], char PuertoPostgresql[],char bd[],char user[], char pwd[],int idsockc, char query[], char *response[])
{
   PGconn *conn;
   PGresult *res;
   int i,j;
   char respuesta[1024];
   memset(respuesta,0,1024);
   conn = PQsetdbLogin(IPPostgrsql,PuertoPostgresql,NULL,NULL,bd,user,pwd);
   if (PQstatus(conn) != CONNECTION_BAD){
      puts("entre a la BD");
      printf("Salida: %s\n",query);
      res = PQexec(conn, query);
      puts("segmentation");
      if (res != NULL && PGRES_TUPLES_OK == PQresultStatus(res))
         {
          puts("Res no es null y pgrestuples OK.....");
          for (i = 0 ; i <= PQntuples(res)-1;  i++)
             {
                for (j = 0 ; j < PQnfields(res); j++)
                    {
                      strcat(respuesta,PQgetvalue(res,i,j));
                      strcat(respuesta,"//");              
                    }
                strcat(respuesta,".\n");
             }
             strcat(respuesta,"\0");                
             PQclear(res);
         }
    }
    PQfinish(conn);
    strcpy(response, respuesta);
}