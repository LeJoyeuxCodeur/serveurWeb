#include "socket.h"
#include "fonction.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

char *fgets_or_exit(char *buffer, int size, FILE *stream){
  char *alt;
  if((alt=fgets(buffer, size, stream))==NULL)
    exit(0);
  else
    return alt;
}

int parse_http_request(const char *request_line, http_request *request){
  int lu;
  char method[512];
  char source[512];
  int version;
  int version2;
  char carbage[512];
  if((lu=sscanf(request_line, "%s %s HTTP/%d.%d %s", method, source, &version, &version2, carbage))==4){
    /*Test sur GET*/
    if(strcmp(method, "GET")==0)
      request->method=HTTP_GET;
    else{
      request->method=HTTP_UNSUPPORTED;
      return 0;
    }
    request->major_version=version;
    request->minor_version=version2;
    request->url=source;
    /*Test sur la version 1.0 ou 1.1*/
    if(request->major_version!=1 || (request->minor_version!=0 && request->minor_version!=1))
      return 0;
    return 1;
  }else{
    return 0;
  }
}

void skip_headers(FILE *client){
  char buff[1024];
  while(1){
    if(fgets(buff, 1024, client)==NULL){
      perror("skipHeader");
    }
    /*boucle jusqu'à la porchaine chaine vide*/
    if(strcmp(buff, "\n")==0 || strcmp(buff, "\r\n")==0)
      break;
  }
}

void send_status (FILE *client, int code, const char *reason_phrase){
  if(fprintf(client, "HTTP/1.1 %d %s\r\n", code, reason_phrase)<0)
    perror("send_status");
}

void send_response(FILE *client, int code, const char *reason_phrase, const char *message_body){
  send_status(client, code, reason_phrase);
  if(fprintf(client, message_body)<0)
    perror("send_response");
}
