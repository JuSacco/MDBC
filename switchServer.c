#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

typedef struct configuracion{
    char ip[50];
    char user[50];
    char pwd[50];
    char dbName[50];
}config_t;

config_t* configReader(){
 FILE* archivo= NULL;
 char* confPath= "server.conf";
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
              strncpy(&conf->user,&lectura,50);
              break;
            case 3:
              strncpy(&conf->pwd,&lectura,50);
              break;
            case 4:
              strncpy(&conf->dbName,&lectura,50);
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


int main(){
	char *resp;
     printf("Wellcome to MDBC \n");
     printf(">Multi Database Connector\n\n\n");
     config_t* conf = NULL;
     conf = configReader();
     printf("\nValores establecidos -----\n");
     printf("IP: %s\n",&conf->ip);
     printf("User: %s\n",&conf->user);
     printf("PWD: %s\n",&conf->pwd);
     printf("DBName: %s\n",&conf->dbName);
     printf("--------------------------\n");
     //funcionMysql(&conf->ip,&conf->user,&conf->pwd,&conf->dbName,"select * from city");
     return 0;
}
