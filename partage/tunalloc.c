#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h> 
#include <sys/stat.h>
#include <sys/ioctl.h>

#include <unistd.h>
#include <fcntl.h>
#include <linux/if.h>
#include <linux/if_tun.h>


void transmit(int src, int dst){
  printf("bla1\n");
  char buf[1600];//max ethernet ~1500 octets
  int r = 0;
  for(;;){
    r=read(src,buf,1500);
    if( r >=0){
      printf("%s\n",buf);
      write(dst,buf,r);
    }
  }	
  printf("bla2\n");
}

int tun_alloc(char *dev)
{
  struct ifreq ifr;
  int fd, err;

  if( (fd = open("/dev/net/tun", O_RDWR)) < 0 ){
    perror("alloc tun");
    exit(-1);
  }

  memset(&ifr, 0, sizeof(ifr));

  /* Flags: IFF_TUN   - TUN device (no Ethernet headers) 
   *        IFF_TAP   - TAP device  
   *
   *        IFF_NO_PI - Do not provide packet information  
   */ 
  ifr.ifr_flags = IFF_TUN; 
  if( *dev )
    strncpy(ifr.ifr_name, dev, IFNAMSIZ);

  if( (err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0 ){
    close(fd);
    return err;
  }
  strcpy(dev, ifr.ifr_name);
  return fd;
}      

int main (int argc, char** argv){
  if(argc < 3){
    printf("Usage : sudo tunalloc nom_tunnel fd_ecriture\n"); 
    exit(1);
  }

  int tunfd;
  printf("Creation de %s\n",argv[1]);
  tunfd = tun_alloc(argv[1]);
  printf("tunfd : %d\n",tunfd);
  printf("Faire la configuration de %s...\n",argv[1]);
 // system("sh config_tun.sh");  
  printf("Appuyez sur une touche pour continuer\n");
  getchar();
  printf("Interface %s Configuree:\n",argv[1]);
  system("ip addr");  

  printf("%d\n",atoi(argv[2]));
  transmit(tunfd, atoi(argv[2]));
  return 0;
}

