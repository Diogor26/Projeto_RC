#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <time.h>

struct infomarcao{
    char data[40];
    char local[40];
    char crime[40];
    char nome[40];
    char hora[10];
}info;

int main(){
    int clientSocket, portNum, nBytes, nSize;
    char buffer[1024], buffer2[1024];
    struct sockaddr_in serverAddr;
    socklen_t addr_size;
    int opcao=0;
    char user[100];
    char pass[100];
    char teste[100];
    char aux[1];
    
    /*Create UDP socket*/
    clientSocket = socket(PF_INET, SOCK_DGRAM, 0);
    
    /*Configure settings in address struct*/
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(7891);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
    
    /*Initialize size variable to be used later on*/
    addr_size = sizeof serverAddr;
    
    
    while(1)
    {
		
		printf("\nBem vindo\n App para profissionais de saude!!");
		z:
		printf("\nMenu\n");
		printf("1-Login\n");
		printf("Opcao:");
		scanf("%d", &opcao);
		
		
		if(opcao==1)
		{
			do	
			{		
			printf("\nInsira o seu login: ");
			scanf("%s", user);
			nBytes=strlen(user)+1;
			sendto(clientSocket,user,nBytes,0,(struct sockaddr *)&serverAddr,addr_size);
			//sendto(clientSocket,user,nBytes,0,(struct sockaddr *)&serverAddr,addr_size);
			fflush(stdin);
	
			printf("\nInsira a pass: ");
			scanf("%s", pass);
			
			printf("\n%s\n", pass);
			
			nBytes=strlen(pass)+1;
			sendto(clientSocket,pass,nBytes,0,(struct sockaddr *)&serverAddr,addr_size);			
			//sendto(clientSocket," ",nBytes,0,(struct sockaddr *)&serverAddr,addr_size);//por causa de um bug
			fflush(stdin);
			aux[0]="\0";
			nBytes = recvfrom(clientSocket,aux,1,0,NULL, NULL);
			printf("%c\n", aux[0]);
			if(aux[0]=='1')
			{
				printf("Login efetuado com sucesso!!\n");
			}
			else if(aux[0]!='1')
			{
				printf("Utilizador ou password errada! Tente novamente\n");
				
			}
			}while(aux[0]!='1');
			//enviar dados para o server para conferir se as credenciais existem
					
		}
		
		else 
		{
			printf("\nInsira uma opcao valida");
			goto z;

		}
		
		//sendto(clientSocket,"1", 1,0,(struct sockaddr *)&serverAddr,addr_size);
		
		
		// Apresentação do crime
		
		printf("Queixa a presentar:\n");
		
		printf("Data: ");
        scanf("%s", info.data);
        
        printf("\nHora: ");
        scanf("%s", info.hora);
        
        printf("\nLocal: ");
        scanf("%s", info.local);
        
        
        
        printf("\nTipo de Agressao: ");
        scanf("%s", info.crime);
        
        
        
        printf("\nNome: ");
        scanf("%s", info.nome);
        
        snprintf(buffer, sizeof(buffer),"\"%s\";\"%s\";\"%s\";\"%s\";\"%s\"", info.data, info.hora, info.local, info.crime, info.nome);
        
        nBytes = strlen(buffer) + 1;
        
        sendto(clientSocket,buffer, nBytes,0,(struct sockaddr *)&serverAddr,addr_size);
        printf("Obrigado pela sua colaboracao!"); 
    
 }   
    return 0;
}
