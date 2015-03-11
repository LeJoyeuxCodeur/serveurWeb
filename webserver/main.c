#include "socket.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void){
  int serveur = creer_serveur(8000);
  int client;
  int pid;
  FILE *file;
  const char *intro = "Bienvenue, mortel, sur ce magnifique serveur d'echo\nIci ma gueule, quand tu parles, on te répond !\nCependant, tu auras la grosse impression de dialoguer... tout seul !\nHeureusement, tu connais ça, espèce de sans amis!\nTu peux t'en aller après avoir pris assez de vents dans la tête avec un ctrl + c\nSalut rémi sans amis!\n";
  char buffer[1024];
  char reqS1[512];
  char reqS2[512];
  int reqI1, reqI2;
  int firstLi=1;
  int requestTrue=0;
  
  while(1){
    if((client = accept(serveur, NULL, NULL)) == -1)
      perror("accept");
    printf("Client connecté\n");
    fflush(stdout);
    printf("%s",intro);
    if((pid=fork()) == -1){
	    perror("fork");
    }else{
      if(pid!=0){
	if(close(client)==-1)
	  perror("close");
      }else{
	if((file=fdopen(client, "w+"))==NULL)
	  perror("fdopen");
	
	
	while(fgets(buffer, 1024, file)!=NULL){
	  if(firstLi==1){
	    if(sscanf(buffer, "%s %s HTTP/%d.%d", reqS1, reqS2 ,&reqI1 ,&reqI2)==4){
	      if(strcmp(reqS1, "GET")==0){
		if(reqI1==1 && (reqI2 == 1 || reqI2==0)){
		  requestTrue=1;
		}
	      }
	    }
	  }
	  firstLi++;
	  if(strcmp(buffer,"\n")==0 || strcmp(buffer,"\r\n")==0)
	    break;
	}
	if(requestTrue==1){
	  if(strcmp(reqS2, "/")==0){
	  fprintf(file, "HTTP/1.1 200 OK\r\n");
	  fprintf(file, intro);
	  }else{
	    fprintf(file, "HTTP/1.1 404 File Not Found\r\nConnection: close\r\nContent-length: 17\r\r\n404 File Not Found\r\n");
	  }
	}else
	  fprintf(file, "HTTP/1.1 400 Bad Request\r\nConnection: close\r\nContent-length: 17\r\r\n400 Bad request\r\n");
	printf("Client déconnecté\n");
	exit(0);
	fflush(stdout);
      }
    }
  }
  return 0;
}


