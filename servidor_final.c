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
		  		  
		    //char cred[5000];
		    char opcao[100];
		    char aux[40];
		    char check[1];
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
	
			while(1)
			{
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
					printf("sucess\n");
					sendto(udpSocket,"1",1,0,(struct sockaddr *)&serverStorage,addr_size);
					fflush(stdout);
										//Recebe queixa
					nBytes = recvfrom(udpSocket,buffer,1024,0,(struct sockaddr *)&serverStorage, &addr_size);
					printf("\n Submissao de crime: \n%s\n", buffer);
					ficheiro=fopen("crime.txt", "a");
					
					if(ficheiro==NULL)
					{
						printf("Erro ao abrir ficheiro");
						exit(1);
					}
					
					fprintf(ficheiro,"%s\n", buffer);
					fclose(ficheiro);
					printf("\nProcesso completo! Crime reportado guardado!\n\n");
					break;
				}
				else 
				{
					printf("Nao existe\n");
					sendto(udpSocket,"0",1,0,(struct sockaddr *)&serverStorage,addr_size);
					fflush(stdout);
					break;
				}		
			}//fim ciclo while
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
	char menu_inicial[100];
	int nread=0;
	
	nread= read(client_fd, menu_inicial,100-1); //ler o username
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
		
		//falta mandar para o administrador
	}
	
	if(menu_inicial[0]=='2')//login e posterior
	{
		User list[20];
		ocorrencia crimes[20];	
			
		User Login()
		{
			char username [BUF_SIZE];
			char password [BUF_SIZE];
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
			char aux[100]={"1"};
			char opcao[100];

			int nread=0;
			
			printf("\nUtilizador válido!\n");
			printf("\nO nuimero enviasdo foi %s", aux);
			write(client_fd, aux, strlen(aux));
			
			nread= read(client_fd, opcao,100-1); //ler o que deseja fazer
			opcao[nread]='\0';
			printf("\no opcao e: %s \n", opcao);
			
			if(opcao[0]=='1') //quando o psp quer ler crimes
			{
				printf("\nQuero ler os crimes");
				FILE *ficheiro;
				ficheiro=fopen("crime.txt", "r");
						
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
			}
			if(opcao[0]=='2')//procurar crimes por local
			{
				char filtro_1[100];
				char *mensagem="Insira o local para aplicar o filtro";
				write(client_fd, mensagem,strlen(mensagem));

				printf("Vamos ler o local: \n");
				nread= read(client_fd, filtro_1,100-1); //ler o local
				filtro_1[nread]='\0';
				printf("\n O filtro 1 e %s \n", filtro_1);
				
				
				FILE *locais;
				locais=fopen("crime.txt", "r");
				if (locais==NULL)
				{
					printf("\nErro no ficheiro");
					exit(1);
				}
				printf("\nFicheiro existe");
				
				for(int i=0; i<20; i++)
				{
					char data[10];
					char local[10];
					char crime[10];
					char nome[10];
					char hora[10];
					fscanf(locais,"%s%s%s%s%s", data, hora, local, crime, nome);
						
					strcpy(crimes[i].data,data);
					strcpy(crimes[i].hora,hora);
					strcpy(crimes[i].local,local);
					strcpy(crimes[i].crime,crime);
					strcpy(crimes[i].nome,nome);			
				}
				fclose(locais);
								
				for(int z=0; z<20;z++)
				{					
					if(strcmp(crimes[z].local, filtro_1)==0)
					printf("\n  %s  %s  %s  %s  %s", crimes[z].data, crimes[z].hora, crimes[z].local, crimes[z].crime, crimes[z].nome);				
				}	
			}
			else
			{
				printf("\nErro");
			}
		}
		
		else 
		{
			printf("\nUtilizador nao existente\n");
			fflush(stdout);
		}
	}

	fflush(stdout);
	close(client_fd); 
}

void process_client__(int client_fd)//gestor
{	
	User list[20];	
			
	User Login()
	{
		char username [BUF_SIZE];
		char password [BUF_SIZE];
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
		fflush(stdin);
		printf("\nGestor válido!\n");
		write(client_fd, "\nGestor válido!\n", strlen("\nGestor válido!\n"));		
	}
	else 
	{
		printf("\nGestor nao existente\n");
		write(client_fd, ("\nGestor nao existente\n"), strlen("\nGestor nao existente\n"));
		fflush(stdout);
	}	
		
	fclose(ocorrencias);
	fflush(stdout);
	close(client_fd);
}

void erro(char *msg)
{
	printf("Erro: %s\n", msg);
	exit(-1);
}
