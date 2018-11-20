#include "extremite.h"
#include "iftun.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>

/* taille maximale des lignes */
#define MAXLIGNE 80
#define CIAO "Au revoir ...\n"

int cpforever(int src,int dst){
    char buf[1024];
    printf("\n%d\n\n",src);
    for(;;){
        int r=read(src, buf, 1024);
        if(r<0){
            perror("read\n");
            return(-1);
        }
        if(!r){
            return 0;
        }
        int w=write(dst,buf,r);
        if(w<0){
            perror("write\n");
            return -1;
        }
    }

}


int createListeningServer(char * port,int fd){

int s,n; /* descripteurs de socket */
  int len,on; /* utilitaires divers */
  struct addrinfo * resol; /* résolution */
  struct addrinfo indic = {AI_PASSIVE, /* Toute interface */
                           PF_INET6,SOCK_STREAM,0, /* IP mode connecté */
                           0,NULL,NULL,NULL};
  struct sockaddr_in6 client; /* adresse de socket du client */
  int err; /* code d'erreur */

printf("Ecoute sur le port %s",port);
  err = getaddrinfo(NULL,port,&indic,&resol);
  if (err<0){
    fprintf(stderr,"Résolution: %s\n",gai_strerror(err));
    exit(2);
  }

  /* Création de la socket, de type TCP / IP */
  if ((s=socket(resol->ai_family,resol->ai_socktype,resol->ai_protocol))<0) {
    perror("allocation de socket");
    exit(3);
  }
  fprintf(stderr,"le n° de la socket est : %i\n",s);

  /* On rend le port réutilisable rapidement /!\ */
  on = 1;
  if (setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on))<0) {
    perror("option socket");
    exit(4);
  }
  fprintf(stderr,"Option(s) OK!\n");

  /* Association de la socket s à l'adresse obtenue par résolution */
  if (bind(s,resol->ai_addr,sizeof(struct sockaddr_in6))<0) {
        printf("\ntest %d\n",s);
    perror("bind");
    exit(5);
  }
  freeaddrinfo(resol); /* /!\ Libération mémoire */
  fprintf(stderr,"bind!\n");

  /* la socket est prête à recevoir */
  if (listen(s,SOMAXCONN)<0) {
    perror("listen");
    exit(6);
  }
  fprintf(stderr,"listen!\n");

  while(1) {
    /* attendre et gérer indéfiniment les connexions entrantes */
    len=sizeof(struct sockaddr_in);
    if( (n=accept(s,(struct sockaddr *)&client,(socklen_t*)&len)) < 0 ) {
      perror("accept");
      exit(7);
    }
    /* Nom réseau du client */
    char hotec[NI_MAXHOST];  char portc[NI_MAXSERV];
    err = getnameinfo((struct sockaddr*)&client,len,hotec,NI_MAXHOST,portc,NI_MAXSERV,0);
    if (err < 0 ){
      fprintf(stderr,"résolution client (%i): %s\n",n,gai_strerror(err));
    }else{
      fprintf(stderr,"accept! (%i) ip=%s port=%s\n",n,hotec,portc);
    }
    /* traitement */
    ext_out(s,fd);
  }
  return EXIT_SUCCESS;


}

int ext_out(int fdserv,int fd){

    if(fdserv<0){
        perror("Server not launch properly\n");
        return -1;
    }

    if(cpforever(fdserv,fd)<0){
        perror("CpError in ext_out\n");
        return -1;
    }

    return 0;
}
int main(){
createListeningServer("1234",1);
return 0;
}
