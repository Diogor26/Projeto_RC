//App para gestor
//Sockets TCP


#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>

#define BUF_SIZE 100

void erro(char *msg);

int main(int argc, char *argv[]) 
{
  char endServer[100], buffer[BUF_SIZE];
  int fd, client, nread;
  struct sockaddr_in addr, client_addr;
  int client_addr_size;
  struct hostent *hostPtr;

  strcpy(endServer, argv[1]);
  if ((hostPtr = gethostbyname(endServer)) == 0) 
  {
    printf("Couldn t get host address.\n");
    exit(-1);
  }

  bzero((void *) &addr, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = ((struct in_addr *)(hostPtr->h_addr))->s_addr;
  addr.sin_port = htons((short) atoi(argv[2]));

  if((fd = socket(AF_INET,SOCK_STREAM,0)) == -1)
	erro("socket");
  if( connect(fd,(struct sockaddr *)&addr,sizeof (addr)) < 0)
	erro("Connect");
	
  //diferença do codigo original
  int opcao=0;
  char mensagen[100];
  char username [100];
  char password[100];
  char estado[5000];
  
  printf("Bem vindo ao programa de gestão\n");
  printf("\nMenu");
  printf("\n1->Login");
  scanf("%d", &opcao);
  
  if(opcao==1)
  {
	  printf("\nLogin: \n");
	  scanf("%s", username);
	  write(fd, username, strlen(username));
	  
	  printf("\nPassword: \n");
	  scanf("%s", password);
	  write(fd, password, strlen(password));
	  
	  nread=read(fd, estado, BUF_SIZE-1); //ler o estado da autenticação
      estado[nread] = '\0'; 
      printf("\n%s", estado);//printa o estado 
	  
  }
	  
  fflush(stdout);
  close(fd);
}

void erro(char *msg)
{
	printf("Erro: %s\n", msg);
	exit(-1);
}
