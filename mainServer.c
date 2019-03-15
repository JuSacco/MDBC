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
//XML 
#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/xmlIO.h>
#include <libxml2/libxml/xinclude.h>
#include <libxml2/libxml/tree.h>

//#define IPSerBD "192.168.0.106"
#define IPSerBD "127.0.0.1"
#define PuertoSerBD 6666

typedef struct configuracion{
    char ip[50];
    char port[50];
    char user[50];
    char pwd[50];
}config_t;

typedef struct databases{
    char fbdb[50];
    char pqdb[50];
    char mydb[50];
}dbases;

void parse_query(xmlDocPtr, xmlNodePtr, char *, char *);

void parse_query(xmlDocPtr doc, xmlNodePtr cur,char *db, char *query)
{
        xmlChar *key;
        cur = cur->xmlChildrenNode;
        while (cur != NULL) {
                if ((!xmlStrcmp(cur->name, (const xmlChar *)"database"))) {
                        key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
                        printf("Database: %s\n", key);
                        strcpy(db, key);
                        xmlFree(key);
                }
                if ((!xmlStrcmp(cur->name, (const xmlChar *)"sql"))) {
                        key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
                        printf("Consulta: %s\n", key);
                        strcpy(query, key);
                        xmlFree(key);
                }
                cur = cur->next;
        }
}


dbases* databasesConfig(){
 FILE* archivo= NULL;
 char* confPath= "databases.conf";
 char lectura[50];
 dbases* conf = malloc(sizeof(config_t));
 archivo = fopen(confPath,"r");
    if (archivo == NULL){
      printf("\nError: El archivo de configuracion no se pudo cargar\n Verifique si este se encuentra en la carpeta del servidor\n");
    }else{
      char c;
      int i=0;
      while (feof(archivo) == 0){
        fscanf(archivo,"%s\n",&lectura);
        c = lectura[0];
        if (c == '#'){
          continue;
        }else{
          i=i+1;
          switch (i){
            case 1:
              strncpy(&conf->fbdb,&lectura,50);
              break;
            case 2:
              strncpy(&conf->pqdb,&lectura,50);
              break;
            case 3:
              strncpy(&conf->mydb,&lectura,50);
              break;
            default:
              break;
          }
        }
      }
      fclose(archivo);
      return conf;
    }
}

config_t* mysqlConfigReader(){
 FILE* archivo= NULL;
 char* confPath= "mysql/mysql.conf";
 char lectura[50];
 config_t* conf = malloc(sizeof(config_t));
 archivo = fopen(confPath,"r");
    if (archivo == NULL){
      printf("\nError: El archivo de configuracion no se pudo cargar\n Verifique si este se encuentra en la carpeta del servidor\n");
    }else{
      char c;
      int i=0;
      while (feof(archivo) == 0){
        fscanf(archivo,"%s\n",&lectura);
        c = lectura[0];
        if (c == '#'){
          continue;
        }else{
          i=i+1;
          switch (i){
            case 1:
              strncpy(&conf->ip,&lectura,50);
              break;
            case 2:
              strncpy(&conf->port,&lectura,50);
              break;
            case 3:
              strncpy(&conf->user,&lectura,50);
              break;
            case 4:
              strncpy(&conf->pwd,&lectura,50);
              break;
            default:
              break;
          }
        }
      }
      fclose(archivo);
      return conf;
    }
}

config_t* postgresqlConfigReader(){
 FILE* archivo= NULL;
 char* confPath= "postgresql/postgresql.conf";
 char lectura[50];
 config_t* conf = malloc(sizeof(config_t));
 archivo = fopen(confPath,"r");
    if (archivo == NULL){
      printf("\nError: El archivo de configuracion no se pudo cargar\n Verifique si este se encuentra en la carpeta del servidor\n");
    }else{
      char c;
      int i=0;
      while (feof(archivo) == 0){
        fscanf(archivo,"%s\n",&lectura);
        c = lectura[0];
        if (c == '#'){
          continue;
        }else{
          i=i+1;
          switch (i){
            case 1:
              strncpy(&conf->ip,&lectura,50);
              break;
            case 2:
              strncpy(&conf->port,&lectura,50);
              break;
            case 3:
              strncpy(&conf->user,&lectura,50);
              break;
            case 4:
              strncpy(&conf->pwd,&lectura,50);
              break;
            default:
              break;
          }
        }
      }
      fclose(archivo);
      return conf;
    }
}

config_t* firebirdConfigReader(){
 FILE* archivo= NULL;
 char* confPath= "firebird/firebird.conf";
 char lectura[50];
 config_t* conf = malloc(sizeof(config_t));
 archivo = fopen(confPath,"r");
    if (archivo == NULL){
      printf("\nError: El archivo de configuracion no se pudo cargar\n Verifique si este se encuentra en la carpeta del servidor\n");
    }else{
      char c;
      int i=0;
      while (feof(archivo) == 0){
        fscanf(archivo,"%s\n",&lectura);
        c = lectura[0];
        if (c == '#'){
          continue;
        }else{
          i=i+1;
          switch (i){
            case 1:
              strncpy(&conf->ip,&lectura,50);
              break;
            case 2:
              strncpy(&conf->port,&lectura,50);
              break;
            case 3:
              strncpy(&conf->user,&lectura,50);
              break;
            case 4:
              strncpy(&conf->pwd,&lectura,50);
              break;
            default:
              break;
          }
        }
      }
      fclose(archivo);
      return conf;
    }
}

void initSocket(config_t *confmysql, config_t *confpsql, config_t *conffb, dbases *dbinfo){
  struct sockaddr_in s_sock,c_sock;
  int idsocks,idsockc;
  int lensock = sizeof(struct sockaddr_in);
  char response[20000];
  char db[50];
  char query[1024];
  xmlDocPtr doc;
  xmlNodePtr cur;
  idsocks = socket(AF_INET, SOCK_STREAM, 0);
  printf("idsocks %i\n",idsocks);
  s_sock.sin_family      = AF_INET;
  s_sock.sin_port        = htons(PuertoSerBD);
  s_sock.sin_addr.s_addr = inet_addr(IPSerBD);
  memset(s_sock.sin_zero,0,8);
  printf("bind %i\n", bind(idsocks,(struct sockaddr *) &s_sock,lensock));
  printf("listen %d\n",listen(idsocks,5));
    while(1)
      {
        printf("Esperando conexion\n");
        idsockc = accept(idsocks,(struct sockaddr *)&c_sock,&lensock);
        if(idsockc != -1)
          {
            printf("Conexion aceptada. Cliente: %i\n",idsockc);
            char buf[1024];
            int nb;
            int end = 0;
            nb = recv(idsockc,buf,1024,0);
            buf[nb]='\0';
            printf("Llego al server: %s %i\n",buf,nb);
            doc = xmlParseDoc(buf);
            cur = xmlDocGetRootElement(doc);
            parse_query(doc,cur,db,query);
            printf("Codigo a ejecutar: %s\nBase de datos:%s",query,db);
            if(strcmp(db,dbinfo->pqdb)== 0){
              funcionPostgresql(&confpsql->ip, &confpsql->port, db, &confpsql->user, &confpsql->pwd,idsockc,query,response);            
              write(idsockc,response,sizeof(response));
            }
            if(strcmp(db,dbinfo->mydb)== 0){
              funcionMysql(confmysql->ip, confmysql->user, confmysql->pwd, db, idsockc, query, response);
              write(idsockc,response,sizeof(response));
            }
            if(strcmp(db,dbinfo->fbdb)== 0){
              funcionFirebird(conffb->user, conffb->pwd, conffb->ip,"sysdba", idsockc, query, response);
              write(idsockc,response,sizeof(response));
            }
            close(idsockc);
          }
        else
          {
              printf("Conexion rechazada %i \n",idsockc);
          }
      }
  }

int main(){
     printf("Wellcome to MDBC \n");
     printf(">Multi Database Connector\n\n\n");
     config_t* confmysql = NULL;
     config_t* confpsql = NULL;
     config_t* conffb = NULL;
     dbases* dbconfig = NULL;
     confmysql = mysqlConfigReader();
     printf("mySQL---------------------\n");
     printf("IP: %s\n",&confmysql->ip);
     printf("Puerto: %s\n",&confmysql->port);
     printf("User: %s\n",&confmysql->user);
     printf("PWD: %s\n",&confmysql->pwd);
     printf("--------------------------\n");
     confpsql = postgresqlConfigReader();
     printf("postgreSQL----------------\n");
     printf("IP: %s\n",&confpsql->ip);
     printf("Puerto: %s\n",&confpsql->port);
     printf("User: %s\n",&confpsql->user);
     printf("PWD: %s\n",&confpsql->pwd);
     printf("--------------------------\n");
     conffb = firebirdConfigReader();
     printf("Firebird----------------\n");
     printf("IP: %s\n",&conffb->ip);
     printf("Puerto: %s\n",&conffb->port);
     printf("User: %s\n",&conffb->user);
     printf("PWD: %s\n",&conffb->pwd);
     printf("--------------------------\n");
     printf("Leyendo configuraciones de BD\n");
     dbconfig = databasesConfig();
     printf("--------------------------\n");
     printf("Iniciando socket\n");
     initSocket(confmysql,confpsql,conffb,dbconfig);
    return 0;
}

