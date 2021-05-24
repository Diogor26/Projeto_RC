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
int emergencia();

int main(int argc, char *argv[]) 
{
  char endServer[100];
  int fd, nread;
  struct sockaddr_in addr;
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
  char opcao[30];
  char username [100];
  char password[100];
  char username_novo [100];
  char password_novo[100];
  char crime[5000];
  char aux[100];
  char menu_inicial[100];
  char mudar_pass[100];
  char ch;
  
  menu_inicial:
  system("clear");
  printf("\nBem vindo ao programa para PSP\n");
  printf("\n1->Criar conta");
  printf("\n2->Login");
  scanf("%s", menu_inicial);
  
  if(menu_inicial[0]=='1')//criar conta
  {
	  printf("\nVamos criar uma conta\n");
	  write(fd, "1", strlen("1"));
	  
	  printf("\nInsira o username: \n");
	  scanf("%s", username_novo);
	  write(fd, username_novo, strlen(username_novo));
	  
	  printf("\n Insira a Password: \n");
	  scanf("%s", password_novo);
	  write(fd, password_novo, strlen(password_novo));
	  
	  printf("\nAguarde que o gestor aprove a sua conta");
	  
	  goto menu_inicial;
  }
  
  if(menu_inicial[0]=='2')//login
  { 
	  write(fd, "2", strlen("2"));
	  printf("\nLogin: \n");
	  scanf("%s", username);
	  write(fd, username, strlen(username));
	  printf("\nPassword: \n");
	  scanf("%s", password);
	  
	  write(fd, password, strlen(password));
	  
	  nread=read(fd, opcao, 30-1);
	  opcao[nread] = '\0'; 
		
	  if(opcao[0]=='1') //utilizador validado
	  {	  
		  int urgencia_=0;
		  
		  system("clear");
		  printf("\nLogin efetuado com sucesso!!\n");
		 
		  menu_principal:
		  system("clear");
		  urgencia_=emergencia();
		  //printf("------------->%d", urgencia_);
		  printf("\nMenu");
		  printf("\n1->consultar crimes ocorridos");
		  printf("\n2->Consultar crimes por local");
		  printf("\n3->Consultar crimes por data");
		  printf("\n4->alterar password da conta");
		  printf("\n5->apagar conta");
		  printf("\n6->sistema de ajuda help");
		  printf("\n7->Profissional de saude em perigo\n");
		  
		  if(urgencia_==1)//no caso de haber profissionais em perigo
		  {
			  printf("\nProfissional em perigo!!!!\nConsulte o ponto 7 do menu\n");
		  }
		  scanf("%s", aux);
		  //printf("---------------->%s", aux);
		  write(fd, aux, strlen(aux));
		  
		  if(aux[0]=='1')//consultar crimes
		  {
			  system("clear");
			  printf("\nConsultar crimes\n");
			  urgencia_=emergencia();

			  if(urgencia_==1)//no caso de haber profissionais em perigo
		  	  {
				   printf("\nALERTA: Profissional em perigo!!!!\nConsulte o ponto 7 do menu principal\n");
		      }
			  printf("\nOs crimes registados são: ");
			  nread=read(fd, crime, 5000-1);
			  crime[nread] = '\0'; 
			  printf("\n%s\n", crime);
			   
			  char sair_if[100];
			  sair_ciclo__:
			 
			  printf("\nPara sair digite '1'");
			  scanf("\n%s", sair_if);

			  if(sair_if[0]=='1')
			  goto menu_principal;

			  else
			  goto sair_ciclo__;
		  }
		  
		  if(aux[0]=='2')//crimes por local
		  {
			 system("clear");
			 printf("\nProcurar crimes por locais\n");
			 urgencia_=emergencia();

			 if(urgencia_==1)//no caso de haber profissionais em perigo
		  	 {
			       printf("\nALERTA: Profissional em perigo!!!!\nConsulte o ponto 7 do menu\n");
		     }

			 char outra[100];
			 char filtro_1[100];
			 char sair[100];
			 
			 char data[100];
			 char hora[100];
			 char local[100];
			 char crime[100];
			 char nome[100];
			 
			 printf("\nLer por filtros os crimes");
			 nread=read(fd, outra, 100-1);
			 outra[nread] = '\0'; 
			 printf("\n%s\n", outra);
			 scanf("%s", filtro_1);
		     printf("\nfiltro= %s\n", filtro_1);
		     write(fd, filtro_1, strlen(filtro_1));
				
			 nread=read(fd, sair, 100-1);
			 sair[nread] = '\0';
			 
			 FILE *temp;
				temp=fopen("temporario.txt", "r");
				
			 if(sair[0]=='1')
			 {			 
				 while(fscanf(temp,"%s %s %s %s %s", data, hora, local, crime, nome)!=EOF)
				 {
					 printf("\n");
					printf("%s  %s  %s  %s  %s\n", data, hora,local, crime, nome);
				 }	
				 fclose(temp);
				 remove("temporario.txt");		
			 }

			 char sair_if[100];
			 sair_ciclo:
			 
			 printf("\nPara sair digite '1'");
			 scanf("\n%s", sair_if);

			 if(sair_if[0]=='1')
			 goto menu_principal;

			 else
			 goto sair_ciclo;
		  }
		  
		  if(aux[0]=='3')//crimes por hora
		  {
			 system("clear");
			 printf("\nProcurar crimes por hora\n");
			 urgencia_=emergencia();

			 if(urgencia_==1)//no caso de haber profissionais em perigo
		  	 {
				 printf("\nALERTA: Profissional em perigo!!!!\nConsulte o ponto 7 do menu\n");
		     }

			 char outra[100];
			 char filtro_1[100];
			 char sair[100];
			 
			 char data[100];
			 char hora[100];
			 char local[100];
			 char crime[100];
			 char nome[100];
			 
			 printf("\nAplicar filtro das horas dos crimes");
			 nread=read(fd, outra, 100-1);
			 outra[nread] = '\0'; 
			 printf("\n%s\n", outra);
			 scanf("%s", filtro_1);
		     printf("\nfiltro= %s\n", filtro_1);
		     write(fd, filtro_1, strlen(filtro_1));
			
		
			 nread=read(fd, sair, 100-1);
			 sair[nread] = '\0';
			 
			 FILE *temp;
				temp=fopen("temporario.txt", "r");
				
			 if(sair[0]=='1')
			 {			 
				 while(fscanf(temp,"%s %s %s %s %s", data, hora, local, crime, nome)!=EOF)
				 {
					printf("%s  %s  %s  %s  %s\n", data, hora,local, crime, nome);
				 }	
				 fclose(temp);
			     remove("temporario.txt");		
			 }

			 char sair_if[100];
			 sair_ciclo_3:
			 
			 printf("\nPara sair digite '1'");
			 scanf("\n%s", sair_if);

			 if(sair_if[0]=='1')
			 goto menu_principal;

			 else
			 goto sair_ciclo_3;
			 
		  }
		  if(aux[0]=='4')//alterar pass
		  {
			  system("clear");
			  printf("\nAlterar palavra pass da conta\n");
			  char mensagem[100];
			  nread=read(fd, mensagem, 100-1);
			  mensagem[nread] = '\0'; 
			  printf("\n%s\n", mensagem);
			  
			  scanf("%s", mudar_pass);
			  write(fd, mudar_pass, strlen(mudar_pass));

			  printf("\nPass alterada com sucesso");
			  
			  char sair_if[100];
			  sair_ciclo_6:
			 
			  printf("\nPara sair digite '1'");
			  scanf("\n%s", sair_if);

			  if(sair_if[0]=='1')
			  goto menu_principal;

			  else
			  goto sair_ciclo_6;
		  }		  
		  if(aux[0]=='5')//apagar conta
		  {
			  system("clear");
			  printf("\nApagar conta");
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
			  else
			  goto menu_principal;		  
		  }
		  if(aux[0]=='6')//sistema de ajuda
		  {
			  system("clear");
			  printf("\nSistema de ajuda\n");
			  urgencia_=emergencia();

			  if(urgencia_==1)//no caso de haber profissionais em perigo
		  	  {
				   printf("\nALERTA: Profissional em perigo!!!!\nConsulte o ponto 7 do menu\n");
		      }

			  char ajuda[100];
			  menu_ajuda:
			  printf("\n1->Como consultar um crime?");
			  printf("\n2->Objetivo da aplicação?");
			  printf("\n3->Como prestar auxilio a um profissinal de saude?");
			  scanf("%s", ajuda);
			  write(fd, ajuda, strlen(ajuda));

			  if(ajuda[0]=='1' || ajuda[0]=='2' || ajuda[0]=='3')
			  {
				  char mensagem[10000]=" ";
				  nread=read(fd, mensagem, 10000-1);
	  			  mensagem[nread] = '\0';

				  printf("\n%s", mensagem);

				  char sair_if[100];
				  sair_ciclo_2:
					
				  printf("\nPara sair digite '1'");
				  scanf("\n%s", sair_if);

				  if(sair_if[0]=='1')
				  goto menu_principal;

				  else
				  goto sair_ciclo_2;
			  }
			  else
			  {
				  printf("Opcao invalida");
				  goto menu_ajuda;
			  }
		  }
		  if(aux[0]=='7')//prestar auxilio profissional de saude
		  {
			  system("clear");
			  char salvar[100];
			  system("clear");
			  char urgencia[100];
			  printf("\nPrestar auxilio ao profissional de saude");

			  FILE *consultar, *temp, *consultar_2;
					
			  consultar=fopen("urgencia.txt", "r");
			  consultar_2=fopen("urgencia.txt", "r");
			  temp=fopen("temporario_2.txt", "w");
					
			  if (consultar==NULL)
			  {
				printf("\nErro no ficheiro");
				exit(1);
			  }

			  while(fgets(urgencia,sizeof(urgencia), consultar)!=NULL)
			  {
			     printf("\n%s", urgencia);
			  }
			  printf("\nInsira o nome de quem quer socorrer: ");
			  scanf("\n%s", salvar);
					
			  while(fscanf(consultar, "%s", urgencia)!=EOF)
			  {
			      if(strcmp(urgencia, salvar)!=0)
				  {
						fprintf(temp,"%s\n", urgencia);
				  }	
			  }
					
			  fclose(consultar);
			  fclose(temp);
			  remove("urgencia.txt");
			  rename("temporario_2.txt", "urgencia.txt");

			 char sair_if[100];
			 sair_ciclo_5:
			 
			 printf("\nPara sair digite '1'");
			 scanf("\n%s", sair_if);

			 if(sair_if[0]=='1')
			 goto menu_principal;

			 else
			 goto sair_ciclo_5;
		  }
		  else
		  printf("\nOpcao invalida");		
		  goto menu_principal; 
		 
	   }	   
	   else
	   printf("\nUtilizador inválido");
	   goto menu_inicial;
  }
  
  else
  {
      printf("\nOpcao nao valida");
	  goto menu_inicial;
  }
  
  fflush(stdout);
  close(fd);
}

void erro(char *msg)
{
	printf("Erro: %s\n", msg);
	exit(-1);
}

int emergencia()//identificar pessoas em perigo
{
	char opcao[100];
	char urgente[100];
	FILE *emergencia;
	emergencia=fopen("urgencia.txt", "r");
	
	while(fscanf(emergencia, "%s", urgente)!=EOF)

	if(urgente=='\0')
	return 0;

	else
	return 1;

	fclose(emergencia);
}
