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
	
	while(1){
		if((client = accept(serveur, NULL, NULL)) == -1)
			perror("accept");
		printf("Client connecté\n");
		fflush(stdout);
		write(client, intro, strlen(intro));
		if((pid=fork()) == -1){
		  perror("fork");
		}else{
		  if(pid!=0){
		    if(close(client)==-1)
		      perror("close");
		  }else{
		    file=fdopen(client, "w+");
		    while(fgets(buffer, 1024, file)!=NULL){
		      fprintf(file, "YouCantTestMyServer %s" ,buffer);
		    }
		    printf("Client déconnecté\n");
		    exit(0);
		    fflush(stdout);
		  }
		}
	}
	return 0;
}
