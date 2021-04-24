//App para agente de segurança
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
<<<<<<< HEAD
  char opcao[30];
  char mensagen[100];
  char username [100];
  char password[100];
  char username_novo [100];
  char password_novo[100];
  char crime[5000];
  char aux[100];
  char menu_inicial[100];
  char filtro_1[100];
  
  printf("Bem vindo ao programa para PSP\n");
  printf("1->Criar conta");
  printf("2->Login");
  scanf("%s", menu_inicial);
  
  if(menu_inicial[0]=='1')//criar conta
  {
	  printf("\nVamos criar uma conta");
	  write(fd, "1", strlen("1"));
	  
	  printf("\nInsira o username: \n");
	  scanf("%s", username_novo);
	  write(fd, username_novo, strlen(username_novo));
	  
	  printf("\n Insira a Password: \n");
	  scanf("%s", password_novo);
	  write(fd, password_novo, strlen(password_novo));
  }
  
  if(menu_inicial[0]=='2')//login
  { 
	  write(fd, "2", strlen("2"));
=======
  int opcao=0;
  char mensagen[100];
  char username [100];
  char password[100];
  char crime[5000];
  char aux[100];
  
  printf("Bem vindo ao programa de consulta de crimes\n");
  printf("\nMenu");
  printf("\n1->Login");
  scanf("%d", &opcao);
  
  if(opcao==1)
  {
>>>>>>> e04beff408aaa093cd60f36a8ee0738d2fce947b
	  printf("\nLogin: \n");
	  scanf("%s", username);
	  write(fd, username, strlen(username));
	  
	  printf("\nPassword: \n");
	  scanf("%s", password);
	  write(fd, password, strlen(password));
	  
<<<<<<< HEAD
	  nread=read(fd, opcao, 30-1);
	  opcao[nread] = '\0'; 
	  printf("A opcaoa tem o seguinte: ->%s\n", opcao);
		
	  if(opcao[0]=='1') //utilizador validado
	  {	  
		  printf("\nUtlizador Válido");
		  printf("\nMenu");
		  printf("\n1->consultar crimes ocorridos");
		  printf("\n2->Consultar crimes por filtros");
		  scanf("%s", aux);
		  write(fd, aux, strlen(aux));
		  
		  if(aux[0]=='1')//consultar crimes
		  {
			  printf("\nOs crimes registados são: ");
			  nread=read(fd, crime, 5000-1);
			  crime[nread] = '\0'; 
			  printf("\n%s\n", crime);
		 }
		 
		 if(aux[0]=='2')//crimes por filtros
		 {
		      printf("\nProcurar crimes por local: \nInsira o local ");
		      scanf("%s", filtro_1);
		      write(fd, filtro_1, strlen(filtro_1));

		 }
		 else
		 printf("\nOpcao invalida");		  
	  }
	  else
	  printf("\nUtilizador inválido");
  }
  
  else
  printf("\nOpcao nao valida");
  


=======
	  nread=read(fd, aux, 100-1); //ler os crimes
      aux[nread] = '\0'; 
      printf("\n%s", aux);//printa os crimes
      
      if (aux[0]==1)
      {
	  printf("\nOs crimes registados são: \n");
	  nread=read(fd, crime, BUF_SIZE-1); //ler os crimes
      crime[nread] = '\0'; 
      printf("\n%s", crime);//printa os crimes
		}
	  if(aux[0]!=1)
	  {
		  	
		printf("\nutilzador nao validor");

		}
  
	  
  }
	  
>>>>>>> e04beff408aaa093cd60f36a8ee0738d2fce947b
  fflush(stdout);
  close(fd);
}

void erro(char *msg)
{
	printf("Erro: %s\n", msg);
	exit(-1);
}
