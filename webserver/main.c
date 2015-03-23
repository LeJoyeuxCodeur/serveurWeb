#include "socket.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "fonction.h"

int main(void){
  int serveur = creer_serveur(8000);
  int client;
  int pid;
  FILE *file;
  http_request req;
  const char *intro = "Bienvenue, mortel, sur ce magnifique serveur\nIci ma gueule, quand tu parles, on te répond !\n";
  char buffer[1024];

  while(1){
    if((client = accept(serveur, NULL, NULL)) == -1)
      perror("accept");
    printf("Client connecté\n");
    fflush(stdout);
    if((pid=fork()) == -1){
      perror("fork");
    }else{
      if(pid!=0){
	if(close(client)==-1)
	  perror("close");
      }else{
	if((file=fdopen(client, "w+"))==NULL)
	  perror("fdopen");
	
	fgets_or_exit(buffer, 1024, file);
	skip_headers(file);
	if(parse_http_request(buffer, &req)==0)
	  send_response(file, 400, "Bad Request", "Bad request\r\n");
	else if(req.method == HTTP_UNSUPPORTED)
	  send_response(file, 405, "Method Not Allowed", "Method Not Allowed\r\n");
	else if(strcmp(req.url, "/")==0)
	  send_response(file, 200, "OK", intro);
	else
	  send_response(file, 404, "Not Found", "Not Found\r\n");
	fclose(file);
      }
    }
  }
  return 0;
}


