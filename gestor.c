
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

typedef struct //struct para a psp. Login/pass
{
  char name[10];
  char password[10];
  char funcao[10];
} aprovar;

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
  char mensagen[100];
  char username_novo [100];
  char password_novo[100];
  char estado[5000];
  char menu_inicial[100];
  char opcao[100];
  char aux[100];
  char nome_aut[100];
  char autorizacao[100];
  char ler_contas;
  
  printf("Bem vindo ao programa do gestor\n");
  printf("\n1->Login");
  scanf("%s", menu_inicial);
  
  if(menu_inicial[0]=='1')//fazer login
  {
      printf("\nLogin");
	 
	  
	  printf("\nInsira o username: \n");
	  scanf("%s", username_novo);
	  write(fd, username_novo, strlen(username_novo));
	  
	  printf("\n Insira a Password: \n");
	  scanf("%s", password_novo);
	  write(fd, password_novo, strlen(password_novo));
	  
	  nread=read(fd, opcao, 30-1);
	  opcao[nread] = '\0'; 
	  printf("A opcaoa tem o seguinte: ->%s\n", opcao);
	  
	  if(opcao[0]=='1') //utilizador validado
	  {	  
		  printf("\nUtlizador Válido");
		  printf("\nMenu");
		  printf("\n1->Aprovar criação de contas");
		  printf("\n2->Eliminar contas psp");
		  printf("\n3->Eliminar contas saude");
		  printf("\n4->Alterar palavra pass");
		  printf("\n5->Eliminar conta pessoal");
		  scanf("%s", aux);
		  write(fd, aux, strlen(aux));
		  
		  if(aux[0]=='1')//aprovar criacao de contas
		  {		
		      
		      FILE *leitura_aprovacao, *temp;
			  leitura_aprovacao=fopen("contas_aprovar.txt", "r");
			  temp=fopen("temporario.txt", "w");
			
			  if(leitura_aprovacao==NULL)
			  {
				  printf("\nErro, ficheiro inexistente");
				  exit(1);
			  }
		      
		      printf("\nEstas sao as contas para autorizar: \n");
			  
			  while((ler_contas=fgetc(leitura_aprovacao))!=EOF)
		      printf("%c", ler_contas);
		      fclose(leitura_aprovacao);
			  			  
			  FILE *leitura_aprovacao2;
			  leitura_aprovacao2=fopen("contas_aprovar.txt", "r");
		      printf("\nInsra o nome das quais quer autorizar: ");
		      scanf("%s", nome_aut);
		      
		      char login[100];
		      char pass[100];
		      char funcao[100];
		      printf("\n vou pasar para o while");
		      
		      while(fscanf(leitura_aprovacao2, "%s %s %s", login, pass, funcao)!=EOF)
		      {
				  if(strcmp(login, nome_aut)==0)
				  {
					  printf("\nentro no 1o if");
					  if(strcmp(funcao, "psp")==0)//caso seja uma conta da psp
					  {
						  printf("e um psp");
						  FILE *credenciais_psp;
						  credenciais_psp=fopen("credenciais_psp.txt", "a");
								
						  if(credenciais_psp==NULL)
						  {
							  printf("\nErro, ficheiro inexistente");
							  exit(1);
						  }
						  
						  fprintf(credenciais_psp, "%s %s\n", login, pass);
						  fclose(credenciais_psp);
					  }
				  }
				  else
				  {
					fprintf(temp,"%s %s %s\n", login, pass, funcao);
				  }
			  }
			  fclose(leitura_aprovacao2);
			  fclose(temp);
			  remove("contas_aprovar.txt");
			  rename("temporario.txt", "contas_aprovar.txt");
		      
		      /*for(int z=0; z<20;z++)
		      {
				  if(strcmp(list[z].name, nome_aut)==0)//caso haja algum nome que o utulizar introduziu
				  {					  
					  if(strcmp(list[z].funcao, "psp")==0)//caso seja uma conta da psp
					  {
						  printf("\nMeter no credenciais psp");
						  
						  FILE *credenciais_psp;
						  credenciais_psp=fopen("credenciais_psp.txt", "a");
								
						  if(credenciais_psp==NULL)
						  {
							  printf("\nErro, ficheiro inexistente");
							  exit(1);
						  }
						  
						  fprintf(credenciais_psp, "%s %s", list[z].name, list[z].password);
						  fclose(credenciais_psp);
						  remove("contas_aprovar.txt");					  
									  
						  fclose(leitura_aprovacao);
					  }
					  if(strcmp(list[z].funcao, "saude")==0)
					  {
						  
						  printf("\nMeter no credenciais saude");
						  
						  FILE *credenciais_saude;
						  credenciais_saude=fopen("credenciais_saude.txt", "a");
								
						  if(credenciais_saude==NULL)
						  {
							  printf("\nErro, ficheiro inexistente");
							  exit(1);
						  }
						  
						  fprintf(credenciais_saude, "%s %s", list[z].name, list[z].password);
						  fclose(credenciais_saude);
						  remove("contas_aprovar.txt");					  
									  
						  fclose(leitura_aprovacao);
						  goto sair_ciclo;
				      }
					 
					  else
					  goto nenhum;
				  
		 }
			  nenhum:
			  printf("\nNenhuma conta encontrada");
			  
			  sair_ciclo:
			  printf("\nConta inserida com sucesso");*/			 
		 }
		  if(aux[0]=='2')//eliminar contas psp
		  {
			 char contas_psp[100];
			 char conta_apagar[100];
			  
			 nread=read(fd, contas_psp, 100-1);
			 contas_psp[nread] = '\0'; 
			 printf("\nEstas sao as contas atualmente: \n%s", contas_psp);
			 
			 printf("\nQual a conta que quer apagar?");
			 scanf("%s", conta_apagar);
			 write(fd, conta_apagar, strlen(conta_apagar));
		  }
		 
		  if(aux[0]=='3')//eliminar contas saude
		  {
			 char contas_saude[100];
			 char conta_saude_apagar[100];
			  
			 nread=read(fd, contas_saude, 100-1);
			 contas_saude[nread] = '\0'; 
			 printf("\nEstas sao as contas atualmente: \n%s", contas_saude);
			 
			 printf("\nQual a conta que quer apagar?");
			 scanf("%s", conta_saude_apagar);
			 write(fd, conta_saude_apagar, strlen(conta_saude_apagar));
				
		  }
		 
		  if(aux[0]=='4')//alterar pass 
		  {
			  char mensagem[100];
			  char mudar_pass[100];
			  
			  nread=read(fd, mensagem, 100-1);
			  mensagem[nread] = '\0'; 
			  printf("\n%s\n", mensagem);
			  
			  scanf("%s", mudar_pass);
			  write(fd, mudar_pass, strlen(mudar_pass));
		  }
		  
		  if(aux[0]=='5')//eliminar conta
		  {
			  char op[100];
			  char mensagem_2[100];
			  char mensagem_3[100];
			  nread=read(fd, mensagem_2, 100-1);
			  mensagem_2[nread] = '\0'; 
			  printf("\n%s\n", mensagem_2);
			  scanf("%s", op);
			  
			  if(op[0]=='1')
			  write(fd, "1", strlen("1"));
			  
			  nread=read(fd, mensagem_3, 100-1);
			  mensagem_3[nread] = '\0'; 
			  printf("\n%s\n", mensagem_3);
			  
			  if(mensagem_2[0]=='1')
			  {
				  printf("\nconta apagada");

				  exit(1);
			  }
				
		  }
	  }
   }
   
   else
   printf("\nOpcao invalida");
	
	  
  fflush(stdout);
  close(fd);
}

void erro(char *msg)
{
	printf("Erro: %s\n", msg);
	exit(-1);
}
