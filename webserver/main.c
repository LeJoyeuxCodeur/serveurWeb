#include "socket.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

int main(void){
	int serveur = creer_serveur(8000);
	int client;
	int alt;
	const char *intro = "Bienvenue, mortel, sur ce magnifique serveur d'echo\nIci ma gueule, quand tu parles, on te répond !\nCependant, tu auras la grosse impression de dialoguer... tout seul !\nHeureusement, tu connais ça, espèce de sans amis!\nTu peux t'en aller après avoir pris assez de vents dans la tête avec un ctrl + c\nSalut rémi sans amis!\n";
	char buffer[256];
	
	while(1){
		if((client = accept(serveur, NULL, NULL)) == -1)
			perror("accept");
		write(client, intro, strlen(intro));
		while((alt = read(client, buffer, 256)))
			write(client, buffer, alt);
	}
	return 0;
}
