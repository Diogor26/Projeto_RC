
#include <string.h>
#include <stdio.h>

//para server UDP
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <ctype.h>

//para server TCP
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <ctype.h>

void process_client__(int fd);
void process_client(int fd);
void erro(char *msg);

#define BUF_SIZE	1024

typedef struct //struct para a psp. Login/pass
{
  char name[10];
  char password[10];
} User;

typedef struct {
    char data[40];
    char local[40];
    char crime[40];
    char nome[40];
    char hora[10];
}ocorrencia;


int main()
{
    int id=fork();   

    while (1)
    {
        if (id == 0)//Server UDP-> para profissional de saude
        {			
			typedef struct
			{
				char name[10];
				char password[10];
			}User;

        	User list[20];
  
			
			char buffer[1024];
		    char user_saude[40];
		    char pass_saude[40];
		  	char menu_inicial[100];
		  	char status[100];
		  	char mensagem[100];
		  	char nome[100];
		    FILE * ficheiro;
		    
		    int udpSocket, nBytes, nSize;

		    struct sockaddr_in serverAddr, clientAddr;
			struct sockaddr_storage serverStorage;
  
			socklen_t addr_size, client_addr_size;	

			/*Create UDP socket*/
			udpSocket = socket(PF_INET, SOCK_DGRAM, 0);

			/*Configure settings in address struct*/
			serverAddr.sin_family = AF_INET;
			serverAddr.sin_port = htons(7891);
			serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
			memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

			/*Bind socket with address struct*/
			bind(udpSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

			/*Initialize size variable to be used later on*/
			addr_size = sizeof serverStorage;
			
			printf("\nVAmos comecar");
			menu_inicial:
			nBytes = recvfrom(udpSocket,menu_inicial,strlen(menu_inicial)+1,0,(struct sockaddr *)&serverStorage, &addr_size);//recebe o username
			printf("A opcao inicial e: %s \n", menu_inicial);
			
			
			
			if(menu_inicial[0]=='1')//criar conta
			{
				char username_novo [100];
				char password_novo [100];
				
				printf("\nTOu pronto para criar conta");
				nBytes = recvfrom(udpSocket,username_novo,strlen(username_novo)+10,0,(struct sockaddr *)&serverStorage, &addr_size);//recebe o username
				printf("\no username e: %s \n", username_novo);
						
				nBytes = recvfrom(udpSocket,password_novo,strlen(password_novo)+10,0,(struct sockaddr *)&serverStorage, &addr_size);
				printf("\na password e: %s\n", password_novo);
				
				FILE *ficheiro;
				ficheiro=fopen("contas_aprovar.txt", "a");
				
				if(ficheiro==NULL)
				{
					printf("\nErro");
					exit(1);
				}
				fprintf(ficheiro,"%s %s %s\n", username_novo, password_novo, "saude");
				
				fclose(ficheiro);
				
				goto menu_inicial;
			}
			if(menu_inicial[0]=='2')//fazer login
			{
				printf("\nVamos fazer login");
				
				User Login()
				{
					User lg;
					nBytes = recvfrom(udpSocket,user_saude,40,0,(struct sockaddr *)&serverStorage, &addr_size);//recebe o username
					printf("User:%s\n", user_saude);
					
					nBytes = recvfrom(udpSocket,pass_saude,40,0,(struct sockaddr *)&serverStorage, &addr_size);//recebe a pass
					printf("Pass:%s\n", pass_saude);
				 
					strcpy(lg.name, user_saude);
					strcpy(lg.password, pass_saude);
					//nBytes = recvfrom(udpSocket,aux,40,0,(struct sockaddr *)&serverStorage, &addr_size);//por causa de um bug
					return lg;	 
				}
		
				int exist (User u)
				{
					for(int i=0; i<20; i++)
					{
						if(strcmp(list[i].name, u.name)==0 && strcmp(list[i].password, u.password)==0)
						{
							return 1;
						}	
					}
				return -1;
				}
				
				
				User u;
			
				//ir verificar se as credenciais existem	
				//nBytes = recvfrom(udpSocket,check,1,0,(struct sockaddr *)&serverStorage, &addr_size);
				ficheiro=fopen("credenciais_saude.txt", "r");
				
				if(ficheiro==NULL)
				{
					printf("Erro ao abrir ficheiro");
					exit(1);
				}
			
				for(int i=0; i<20; i++)
				{
					char uname[10];
					char upassword[10];
					fscanf(ficheiro,"%s%s", uname, upassword);
					
					strcpy(list[i].name,uname);
					strcpy(list[i].password,upassword);
				}
				fclose(ficheiro);
				u=Login();
			
			
				if(exist(u)==1) //a credêncial e valida?
				{
					sendto(udpSocket,"1",1+1,0,(struct sockaddr *)&serverStorage,addr_size);
					printf("sucess\n");
					char menu[100]=" ";
					
					menu_principal:
					
					nBytes = recvfrom(udpSocket,menu,strlen(menu)+1,0,(struct sockaddr *)&serverStorage, &addr_size);
					printf("a char opcao e :--------------->%s---", menu);
					
					if(menu[0]=='1')
					{											//Recebe queixa
						nBytes = recvfrom(udpSocket,buffer,1024,0,(struct sockaddr *)&serverStorage, &addr_size);
						printf("\n Submissao de crime: \n%s\n", buffer);
						ficheiro=fopen("crimes.txt", "a");
						
						if(ficheiro==NULL)
						{
							printf("Erro ao abrir ficheiro");
							exit(1);
						}
						
						fprintf(ficheiro,"%s\n", buffer);
						fclose(ficheiro);
						printf("\nProcesso completo! Crime reportado guardado!\n\n");
						
						goto menu_principal;
					}
					if(menu[0]=='2')//alterar pass
					{
						char login[100];
						char pass[100];
						char nova_pass[100];
						nBytes = recvfrom(udpSocket,nova_pass,strlen(nova_pass)+1,0,(struct sockaddr *)&serverStorage, &addr_size);

						printf("\n-------------------->%s", nova_pass);
						
						FILE *pass_saude, *temp;
						
						pass_saude=fopen("credenciais_saude.txt", "r");
						temp=fopen("temporario.txt", "w");
						
						if (pass_saude==NULL)
						{
							printf("\nErro no ficheiro");
							exit(1);
						}

						
						while(fscanf(pass_saude, "%s %s", login, pass)!=EOF)
						{
							if(strcmp(user_saude, login)!=0)
							{
								fprintf(temp,"%s %s\n", login, pass);
							}
							else
							{
								fprintf(temp,"%s %s\n", login, nova_pass);
							}		
						}
						fclose(pass_saude);
						fclose(temp);
						remove("credenciais_saude.txt");
						rename("temporario.txt", "credenciais_saude.txt");	
						goto menu_principal;
						
					}
					if(menu[0]=='3')//botao de alarme
					{
						char nome_vitima[100];
						
						FILE *urgencia;
					
						urgencia=fopen("urgencia.txt", "a");
						//temp=fopen("temporario.txt", "w");
							
						if (urgencia==NULL)
						{
							printf("\nErro no ficheiro");
							exit(1);
						}					
						
						nBytes = recvfrom(udpSocket,nome_vitima,strlen(nome_vitima)+1,0,(struct sockaddr *)&serverStorage, &addr_size);	
						fprintf(urgencia, "%s %s\n", nome_vitima, "perigo");
										
						fclose(urgencia);    		
					}
					if(menu[0]=='4')//apagar conta
					{
						char apagar[100];
						char logar[100];
						char pass[100];
						nBytes = recvfrom(udpSocket,apagar,strlen(apagar)+1,0,(struct sockaddr *)&serverStorage, &addr_size);
						
						if(apagar[0]=='1')
						{
							FILE *pass_saude, *temp;
					
							pass_saude=fopen("credenciais_saude.txt", "r");
							temp=fopen("temporario.txt", "w");
							
							if (pass_saude==NULL)
							{
								printf("\nErro no ficheiro");
								exit(1);
							}
							
							if (temp==NULL)
							{
								printf("\nErro no ficheiro");
								exit(1);
							}
							
							
							while(fscanf(pass_saude, "%s %s", logar, pass)!=EOF)
							{
								if(strcmp(user_saude, logar)!=0)
								{
									fprintf(temp,"%s %s\n", logar, pass);
								}
								printf("\napaguei a conta");
							}
							fclose(pass_saude);
							fclose(temp);
							remove("credenciais_saude.txt");
							rename("temporario.txt", "credenciais_saude.txt");
							
							printf("\nConta apagada com sucesso");
							
							goto menu_inicial;
						}
					}
					else
					{
						printf("\nOpcao invalida");
						goto menu_principal;
					}
				}
				else 
				{
					sendto(udpSocket,"0",1+1,0,(struct sockaddr *)&serverStorage,addr_size);
					printf("\nNao existe\n");
					fflush(stdout);
					goto menu_inicial;
					
				}		
			}
			else
			{
				printf("\nNao existe\n");
				fflush(stdout);
				goto menu_inicial;
			}	
			
		return 0;
        }//fim do servidor UDP
        
        else //Serveres TCP para psp e gestores
        {
			int x=fork();
        	
       		if(x==0)//Server TCP porto 8000-> para a PSP
       		{
				#define SERVER_Port  8000
				
				int fd, client;
				struct sockaddr_in addr, client_addr;
				int client_addr_size;
				
				//bzero((void *) &addr, sizeof(addr));
				addr.sin_family      = AF_INET;
				addr.sin_addr.s_addr = htonl(INADDR_ANY);
				addr.sin_port        = htons(SERVER_Port);
				
				if ( (fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
					erro("na funcao socket");
				if ( bind(fd,(struct sockaddr*)&addr,sizeof(addr)) < 0)
					erro("na funcao bind");
				if( listen(fd, 5) < 0) 
					erro("na funcao listen");
				
				int nclientes=0;
					
				while (1) 
				{
					client_addr_size = sizeof(client_addr);
					client = accept(fd,(struct sockaddr *)&client_addr,&client_addr_size);
					nclientes++;
					if (client > 0) 
					{
						if (fork() == 0) 
						{
							printf("n cliente psp-------------->%d", client);
							close(fd);
							process_client(client);
							exit(0);
						}
						close(client);
					}
				}
			return 0;  
			}    		
       		
       		else// server TCP com Porto 9000-> para o gestor 
			{				
				#define SERVER  9000

				int fd, client;
				struct sockaddr_in addr, client_addr;
				int client_addr_size;
				
				//bzero((void *) &addr, sizeof(addr));
				addr.sin_family      = AF_INET;
				addr.sin_addr.s_addr = htonl(INADDR_ANY);
				addr.sin_port        = htons(SERVER);
				
				if ( (fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
					erro("na funcao socket");
				if ( bind(fd,(struct sockaddr*)&addr,sizeof(addr)) < 0)
					erro("na funcao bind");
				if( listen(fd, 5) < 0) 
					erro("na funcao listen");
				
				int nclientes=0;
					
				while (1) 
				{
					client_addr_size = sizeof(client_addr);
					client = accept(fd,(struct sockaddr *)&client_addr,&client_addr_size);
					nclientes++;
					if (client > 0) 
					{
						if (fork() == 0) 
						{
							printf("n cliente saude-------------->%d", client);
							close(fd);
							process_client__(client);
							exit(0);
						}
						close(client);
					}
				}
				return 0;  
			}				
       }        
	}	
return 0;
}

void process_client(int client_fd)//psp
{
	char username [BUF_SIZE];
	char password [BUF_SIZE];
	char menu_inicial[100];
	int nread=0;
	
	
	menu_inicial:
	nread= read(client_fd, menu_inicial,100-1); //ler a opcao inicial
	menu_inicial[nread]='\0';
	printf("A opcao inicial e: %s \n", menu_inicial);
	
	if(menu_inicial[0]=='1')//criar conta
	{
		char username_novo [BUF_SIZE];
		char password_novo [BUF_SIZE];
		
		printf("\nTOu pronto para criar conta");
		nread= read(client_fd, username_novo,BUF_SIZE-1); //ler o username
		username_novo[nread]='\0';
		printf("\no username e: %s \n", username_novo);
				
		nread=read(client_fd, password_novo,BUF_SIZE-1); //ler o password
		password_novo[nread]='\0';
		printf("\na password e: %s\n", password_novo);
		
		FILE *ficheiro;
		ficheiro=fopen("contas_aprovar.txt", "a");
		
		if(ficheiro==NULL)
		{
			printf("\nErro");
			exit(1);
		}
		fprintf(ficheiro,"%s %s %s\n", username_novo, password_novo, "psp");
		
		fclose(ficheiro);
		
		goto menu_inicial;
	}
	
	if(menu_inicial[0]=='2')//login e posterior
	{
		User list[20];

		User Login()
		{
			
			int nread=0;
			
			User lg;
			nread= read(client_fd, username,BUF_SIZE-1); //ler o username
			username[nread]='\0';
			printf("\no username e: %s \n", username);
				
			nread=read(client_fd, password,BUF_SIZE-1); //ler o password
			password[nread]='\0';
			printf("\na password e: %s\n", password);
			  
			strcpy(lg.name, username);
			strcpy(lg.password, password);
			return lg;	 
		}
	
		int exist (User u)
		{
			for(int i=0; i<20; i++)
			{
				if(strcmp(list[i].name, u.name)==0 && strcmp(list[i].password, u.password)==0)
				{
					return 1;
				}
			}
			return -1;
		}
	
		User u;
		
		FILE *ocorrencias;
		ocorrencias=fopen("credenciais_psp.txt", "r");
		
		if(ocorrencias==NULL)
		{
			printf("\nErro, ficheiro inexistente");
			exit(1);
		}

		for(int i=0; i<20; i++)
		{
			char uname[10];
			char upassword[10];
			fscanf(ocorrencias,"%s%s", uname, upassword);
					
			strcpy(list[i].name,uname);
			strcpy(list[i].password,upassword);
		}
		fclose(ocorrencias);
		u=Login();
		fflush(stdout);
		
				
		if(exist(u)==1) //a credêncial e valida
		{
			printf("\nUtilizador válido!\n");
			
			FILE *botao_alarme;
			botao_alarme=fopen("urgencia.txt", "r");
			
			char opcao[100];
			int nread=0;
			
			//printf("\nO nuimero enviasdo foi %s", aux);
				
			if(botao_alarme==NULL)
			{
				write(client_fd, "1", strlen("1"));				
				
				menu_principal:
				nread= read(client_fd, opcao,100-1); //ler o que deseja fazer
				opcao[nread]='\0';
				printf("\no opcao e: %s \n", opcao);
				
				if(opcao[0]=='1') //quando o psp quer ler crimes
				{
				printf("\nQuero ler os crimes");
				FILE *ficheiro;
				ficheiro=fopen("crimes.txt", "r");
						
				if(ficheiro==NULL)
				{
					printf("\nErro ao abrir ficheiro");
					exit(1);
				}
				
				char leitura [5000]= " "; //deixar aqui senao fica com lixo
				char crimes[5000]=" ";
				
				while(fgets(crimes,sizeof(crimes), ficheiro)!=NULL)
				{
					strcat(leitura, crimes);
					fflush(stdin);
				}
				fclose(ficheiro);
				
				printf("Vou enviar os crimes\n");
				printf("\n%s", leitura);
						
				write(client_fd, leitura ,strlen(leitura));//envia os crimes para o o psp

				fflush(stdout);
				
				goto menu_principal;
			}
			
				if(opcao[0]=='2')//procurar crimes por local
				{
					char filtro_1[100];
					
					char data[100];
					char hora[100];
					char local[100];
					char crime[100];
					char nome[100];
					char *mensagem="Insira o local para aplicar o filtro";
					write(client_fd, mensagem,strlen(mensagem));

					printf("Vamos ler o local: \n");
					nread= read(client_fd, filtro_1,100-1); //ler o local
					filtro_1[nread]='\0';	
					printf("\n%s", filtro_1);		
					
					FILE *locais, *temp;
					locais=fopen("crimes.txt", "r");
					temp=fopen("temporario.txt", "w");
					if (locais==NULL)
					{
						printf("\nErro no ficheiro");
						exit(1);
					}
				
					while(fscanf(locais,"%s %s %s %s %s", data, hora, local, crime, nome)!=EOF)
					{
						if(strcmp(local, filtro_1)==0)
						{
							
							fprintf(temp, "%s  %s  %s  %s  %s\n", data, hora,local, crime, nome);	
							printf("%s  %s  %s  %s  %s\n", data, hora,local, crime, nome);			
						}
					}
					fclose(temp);
					write(client_fd, "1", strlen("1"));
			

					goto menu_principal;	
				}
				
				if(opcao[0]=='3')//procurar crimes por data
				{
					char filtro_1[100];
					
					char data[100];
					char hora[100];
					char local[100];
					char crime[100];
					char nome[100];
					char *mensagem="Insira o hora no formato dd/mm/aaaa para aplicar o filtro";
					write(client_fd, mensagem,strlen(mensagem));

					printf("Vamos ler a hora: \n");
					nread= read(client_fd, filtro_1,100-1); //ler a hora
					filtro_1[nread]='\0';	
					printf("\n%s", filtro_1);		
					
					FILE *locais, *temp;
					locais=fopen("crimes.txt", "r");
					temp=fopen("temporario.txt", "w");
					
					if (locais==NULL)
					{
						printf("\nErro no ficheiro");
						exit(1);
					}
								
					while(fscanf(locais,"%s %s %s %s %s", data, hora, local, crime, nome)!=EOF)
					{
						if(strcmp(data, filtro_1)==0)
						{
							fprintf(temp, "%s  %s  %s  %s  %s\n", data, hora,local, crime, nome);	
							printf("%s  %s  %s  %s  %s\n", data, hora,local, crime, nome);			
						}
					}
					fclose(temp);
					write(client_fd, "1", strlen("1"));
		
					goto menu_principal;	
				}
				
				if(opcao[0]=='4') //para alterar pass
				{
					char nova_pass[100];
					char login[100];
					char pass[100];
					
					write(client_fd, "Qual a nova password?", strlen("Qual a nova password?"));
					nread= read(client_fd, nova_pass,100-1); //ler o local
					nova_pass[nread]='\0';
					printf("\n-------------------->%s", nova_pass);
					
					FILE *pass_psp, *temp;
					
					pass_psp=fopen("credenciais_psp.txt", "r");
					temp=fopen("temporario.txt", "w");
					
					if (pass_psp==NULL)
					{
						printf("\nErro no ficheiro");
						exit(1);
					}

					while(fscanf(pass_psp, "%s %s", login, pass)!=EOF)
					{
						if(strcmp(username, login)!=0)
						{
							fprintf(temp,"%s %s\n", login, pass);
						}
						else
						{
							fprintf(temp,"%s %s\n", login, nova_pass);
						}		
					}
					
					fclose(pass_psp);
					fclose(temp);
					remove("credenciais_psp.txt");
					rename("temporario.txt", "credenciais_psp.txt");
					
					goto menu_principal;						
				}
				if(opcao[0]=='5')//apagar a conta 
				{
					char logar[100];
					char pass[100];
					char choose[100];
					write(client_fd, "\nDeseja mesmo eleminar a conta?", strlen("\nDeseja mesmo eleminar a conta?"));
					
					nread= read(client_fd, choose,100-1); //ler o local
					choose[nread]='\0';
					printf("\n-------------------->%s", choose);
					
					if(choose[0]=='1')//quer msm apagar a conta
					{
						FILE *pass_psp, *temp;
						
						pass_psp=fopen("credenciais_psp.txt", "r");
						temp=fopen("temporario.txt", "w");
						
						if (pass_psp==NULL)
						{
							printf("\nErro no ficheiro");
							exit(1);
						}
						
						if (temp==NULL)
						{
							printf("\nErro no ficheiro");
							exit(1);
						}
						
						
						while(fscanf(pass_psp, "%s %s", logar, pass)!=EOF)
						{
							if(strcmp(username, logar)!=0)
							{
								fprintf(temp,"%s %s\n", logar, pass);
							}
							printf("\napaguei a conta");
						}
						fclose(pass_psp);
						fclose(temp);
						remove("credenciais_psp.txt");
						rename("temporario.txt", "credenciais_psp.txt");
						
						printf("\nConta apagada com sucesso");
						write(client_fd, "1", strlen("1"));	
									
					}
				}	
				
			
				else
				{
					printf("\nOpcao invalida");
					goto menu_principal;
				}
			}
			else
			{				
				system("clear");
				write(client_fd, "2", strlen("2"));
				printf("\nHA GENTE EM PERIGO!!!!!");
				goto menu_principal;
			}
		}
		
		else //informar psp que nao e utilizador valido
		{
			write(client_fd, "3", strlen("3"));
			printf("\nUtilizador nao existente\n");
			fflush(stdout);
			
			goto menu_inicial;
		}
	}

	fflush(stdout);
	close(client_fd); 
}

void process_client__(int client_fd)//gestor
{	
	char username [BUF_SIZE];
	char password [BUF_SIZE];
	char menu_inicial[100];
	int nread=0;
	
	
	menu_inicial:
	nread= read(client_fd, menu_inicial,100-1); //ler a opcao inicial
	menu_inicial[nread]='\0';
	printf("A opcao inicial e: %s \n", menu_inicial);
		
	
	if(menu_inicial[0]=='1')
	{
		User list[20];
		User Login()
		{
			int nread=0;
			

			User lg;
			
			
			nread= read(client_fd, username,BUF_SIZE-1); //ler o username
			username[nread]='\0';
			printf("\no username e: %s \n", username);
				
			nread=read(client_fd, password,BUF_SIZE-1); //ler o password
			password[nread]='\0';
			printf("\na password e: %s\n", password);
			  
			strcpy(lg.name, username);
			strcpy(lg.password, password);
			return lg;	 
		}
	
		int exist (User u)
		{
			for(int i=0; i<20; i++)
			{
				if(strcmp(list[i].name, u.name)==0 && strcmp(list[i].password, u.password)==0)
				{
					return 1;
				}
			}
			return -1;
		}
	
		User u;
		
		FILE *ocorrencias;
		ocorrencias=fopen("credenciais_gestor.txt", "r");
		
			if(ocorrencias==NULL)
		{
			printf("\nErro, ficheiro inexistente");
			exit(1);
		}

		for(int i=0; i<20; i++)
		{
			char uname[10];
			char upassword[10];
			fscanf(ocorrencias,"%s%s", uname, upassword);
					
			strcpy(list[i].name,uname);
			strcpy(list[i].password,upassword);
		}
		fclose(ocorrencias);
		u=Login();
		fflush(stdout);
		
		if(exist(u)==1) //a credêncial e valida, mostramos as oorrẽncias
		{
			int nread=0;
			char opcao[100];
			fflush(stdin);
			printf("\nGestor válido!\n");
			write(client_fd, "1", strlen("1"));	
			
			menu_principal:	
			
			nread= read(client_fd, opcao,100-1); //ler o que deseja fazer
			opcao[nread]='\0';

			if(opcao[0]=='2')//eliminar contas psp
			{
				char contas_psp[100];
				char contas_aux[100];
				char conta_apagar[100];
				
				FILE *ficheiro;
				ficheiro=fopen("credenciais_psp.txt", "r");
							
				if(ficheiro==NULL)
				{
					printf("\nErro ao abrir ficheiro");
					exit(1);
				}
				
				while(fgets(contas_aux,sizeof(contas_aux), ficheiro)!=NULL)
				{
					strcat(contas_psp, contas_aux);
					fflush(stdin);
				}
				fclose(ficheiro);
				
				write(client_fd, contas_psp ,strlen(contas_psp));//envia os as contas para apagar
				
				char logar[100];
				char pass[100];

				nread= read(client_fd, conta_apagar,100-1); //ler a conta para apagar
				conta_apagar[nread]='\0';
				printf("\n-------------------->%s", conta_apagar);
					
				FILE *pass_psp, *temp;
					
				pass_psp=fopen("credenciais_psp.txt", "r");
				temp=fopen("temporario.txt", "w");
					
				if (pass_psp==NULL)
				{
					printf("\nErro no ficheiro");
					exit(1);
				}
						
				if (temp==NULL)
				{
					printf("\nErro no ficheiro");
					exit(1);
				}
						
				while(fscanf(pass_psp, "%s %s", logar, pass)!=EOF)
				{
					if(strcmp(conta_apagar, logar)!=0)
					{
						fprintf(temp,"%s %s\n", logar, pass);
					}
					else
					printf("\nConta aoagada com sucesso");
				}
				printf("\nAcao concluida");
				fclose(pass_psp);
				fclose(temp);
				
				remove("credenciais_psp.txt");
				rename("temporario.txt", "credenciais_psp.txt");
				
				goto menu_principal;					
			}
			
			if(opcao[0]=='3')//eliminar conta saude
			{
				char contas_saude[100];
				char contas_saude_aux[100];
				char conta_para_apagar[100];
				
				FILE *ficheiro;
				ficheiro=fopen("credenciais_saude.txt", "r");
							
				if(ficheiro==NULL)
				{
					printf("\nErro ao abrir ficheiro");
					exit(1);
				}
				
				while(fgets(contas_saude_aux,sizeof(contas_saude_aux), ficheiro)!=NULL)
				{
					strcat(contas_saude, contas_saude_aux);
					fflush(stdin);
				}
				fclose(ficheiro);
				
				write(client_fd, contas_saude ,strlen(contas_saude));//envia os as contas para apagar
				
				char logar[100];
				char pass[100];

				nread= read(client_fd, conta_para_apagar,100-1); //ler a conta para apagar
				conta_para_apagar[nread]='\0';
				printf("\n-------------------->%s", conta_para_apagar);
					
				FILE *pass_saude, *temp;
					
				pass_saude=fopen("credenciais_saude.txt", "r");
				temp=fopen("temporario.txt", "w");
					
				if (pass_saude==NULL)
				{
					printf("\nErro no ficheiro");
					exit(1);
				}
						
				if (temp==NULL)
				{
					printf("\nErro no ficheiro");
					exit(1);
				}
						
					
				while(fscanf(pass_saude, "%s %s", logar, pass)!=EOF)
				{
					if(strcmp(conta_para_apagar, logar)!=0)
					{
						fprintf(temp,"%s %s\n", logar, pass);
					}
				}
				printf("\nAcao concluida");
				fclose(pass_saude);
				fclose(temp);
				
				remove("credenciais_saude.txt");
				rename("temporario.txt", "credenciais_saude.txt");	
				
				goto menu_principal;
			}
			
			if(opcao[0]=='4')//alterar pass
			{
				char nova_pass[100]= " ";
				char login[100];
				char pass[100];
				
				write(client_fd, "Qual a nova password?", strlen("Qual a nova password?"));
				nread= read(client_fd, nova_pass,100-1); //ler a pass nova
				nova_pass[nread]='\0';
				printf("\n-------------------->%s", nova_pass);
					
				FILE *pass_psp, *temp;
					
				pass_psp=fopen("credenciais_gestor.txt", "r");
				temp=fopen("temporario.txt", "w");
					
				if (pass_psp==NULL)
				{
					printf("\nErro no ficheiro");
					exit(1);
				}	
					
				while(fscanf(pass_psp, "%s %s", login, pass)!=EOF)
				{
					if(strcmp(username, login)!=0)
					{
						fprintf(temp,"%s %s\n", login, pass);
					}
					else
					{
						fprintf(temp,"%s %s\n", login, nova_pass);
					}		
				}
				fclose(pass_psp);
				fclose(temp);
				remove("credenciais_gestor.txt");
				rename("temporario.txt", "credenciais_gestor.txt");	
				
				goto menu_principal;
			}
			
			if(opcao[0]=='5')//eliminar conta 
			{
				char logar[100];
				char pass[100];
				char choose[100];
				write(client_fd, "\nDeseja mesmo eleminar a conta?", strlen("\nDeseja mesmo eleminar a conta?"));
					
				nread= read(client_fd, choose,100-1); //ler o local
				choose[nread]='\0';
				printf("\n-------------------->%s", choose);
				
				if(choose[0]=='1')//quer msm apagar a conta
				{
					FILE *pass_saude, *temp;
					
					pass_saude=fopen("credenciais_gestor.txt", "r");
					temp=fopen("temporario.txt", "w");
					
					if (pass_saude==NULL)
					{
						printf("\nErro no ficheiro");
						exit(1);
					}
						
					if (temp==NULL)
					{
						printf("\nErro no ficheiro");
						exit(1);
					}
							
					while(fscanf(pass_saude, "%s %s", logar, pass)!=EOF)
					{
						if(strcmp(username, logar)!=0)
						{
							fprintf(temp,"%s %s\n", logar, pass);
						}
						printf("\napaguei a conta");
					}
					fclose(pass_saude);
					fclose(temp);
					remove("credenciais_gestor.txt");
					rename("temporario.txt", "credenciais_gestor.txt");
					
					printf("\nConta apagada com sucesso");
					write(client_fd, "1", strlen("1"));
					
					goto menu_principal;	
				}
			}
			else
			{
				printf("\nOpcao incorreta");
				
				goto menu_principal;
			}
		}
		else 
		{
			printf("\nGestor nao existente\n");
			write(client_fd, ("\nGestor nao existente\n"), strlen("\nGestor nao existente\n"));
			fflush(stdout);
			
			goto menu_inicial;
		}
		fclose(ocorrencias);
	}
	else
	{
		printf("\nOpcao inválida");	
		goto menu_inicial;
	}
	

fflush(stdout);
close(client_fd);
}

void erro(char *msg)
{
	printf("Erro: %s\n", msg);
	exit(-1);
}
