#include "socket.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <stdio.h>
#include <signal.h> 

int creer_serveur(int port){
	int socket_serveur;
	int bind_serveur;
	int optval = 1;
	struct sockaddr_in sockaddr;	
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(port);
	sockaddr.sin_addr.s_addr = INADDR_ANY;
	
	socket_serveur = socket (AF_INET, SOCK_STREAM, 0);
	/* Erreur dans la creation de la socket */
	if(socket_serveur == -1)
		perror("socket_serveur");
	initialiser_signaux();
	if (setsockopt(socket_serveur, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1)
		perror("Can not set SO_REUSEADDR option");
	/* Utilisation de la socket serveur */
	if((bind_serveur = bind(socket_serveur, (struct sockaddr *)&sockaddr, sizeof(sockaddr))) == -1)
		perror("bind socker_serveur");
	if(listen(socket_serveur, 10) == -1)
		perror("Listen socket_serveur");
	
	return socket_serveur;
}

void initialiser_signaux(void){
  struct sigaction sa;
  if(signal(SIGPIPE, SIG_IGN) == SIG_ERR)
    perror("signal");
  sa.sa_handler = traitement_signal;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags= SA_RESTART;
  if(sigaction(SIGCHLD, &sa, NULL) == -1)
    perror("sigaction(SIGCHLD)");
}

void traitement_signal(int sig){
  printf("Signal %d reçu\n", sig);
  fflush(stdout);
  waitpid(-1,NULL ,WNOHANG );
}


