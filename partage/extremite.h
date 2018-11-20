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
int createListeningServer(char * port,int fd);
int ext_out(int port,int fd);
