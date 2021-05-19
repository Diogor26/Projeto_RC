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
    int clientSocket, nBytes;
    char buffer[1024];
    struct sockaddr_in serverAddr;
    socklen_t addr_size;
    char user[100];
    char pass[100];
    char aux[100];
    char menu[100];
    char menu_2[100];
    char anom[100];
    int hours, minutes, day, month, year;
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
		menu_inicial:
		printf("\nMenu\n");
		printf("\n1-Criar conta");
		printf("\n2-Login\n");
		printf("Opcao:");
		scanf("%s", menu);
		
		if(menu[0]=='1')//criar conta
		{
			char username_novo[100];
			char password_novo[100];

			printf("\nVamos criar uma conta");
			sendto(clientSocket,"1",strlen("1")+1,0,(struct sockaddr *)&serverAddr,addr_size);
			  
			printf("\nInsira o username: \n");
			scanf("%s", username_novo);
			sendto(clientSocket,username_novo,strlen(username_novo)+1,0,(struct sockaddr *)&serverAddr,addr_size);
			 
			printf("\n Insira a Password: \n");
			scanf("%s", password_novo);
			sendto(clientSocket,password_novo,strlen(password_novo)+1,0,(struct sockaddr *)&serverAddr,addr_size);
			printf("\nAguarde que o gestor do sistema aprove a conta");
			goto menu_inicial;
		}
		
		
		if(menu[0]=='2')//login
		{
			sendto(clientSocket,"2",strlen("2")+1,0,(struct sockaddr *)&serverAddr,addr_size);
			
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
			nBytes = recvfrom(clientSocket,aux,1,0,NULL, NULL);
			printf("%c\n", aux[0]);
			fflush(stdin);
			
			if(aux[0]=='1')
			{
				printf("Login efetuado com sucesso!!\n");
				menu_principal:
				printf("\nMenu");
				printf("\n1->comunicar um crime");
				printf("\n2->alterar password da conta");
				printf("\n3->Botao ALARME");
				printf("\n4->apagar conta");
				printf("\n5->Help");
				scanf("%s", menu_2);
				fflush(stdin);
				printf("---->%s", menu_2);
				sendto(clientSocket,menu_2,strlen(menu_2)+1,0,(struct sockaddr *)&serverAddr,addr_size);
				
				
				if(menu_2[0]=='1')//comunicar crime
				{
					fflush(stdin);
					time_t now;
					time(&now);
					struct tm *local=localtime(&now);
					
					hours=local->tm_hour;
					minutes= local->tm_min;
					
					day=local->tm_mday;
					month= local->tm_mon +1;
					year=local->tm_year +1900;
					
					/*if(hours>12)
					{
						hours-=12;
					}*/
					
					printf("Queixa a presentar:\n");
					
					/*printf("Data: ");
					scanf("%s", info.data);
					
					printf("\nHora: ");
					scanf("%s", info.hora);
					*/
					snprintf(info.hora, sizeof(info.hora), "%d:%d", hours, minutes);
					snprintf(info.data, sizeof(info.data), "%02d/%02d/%d",day, month, year);
					
					printf("\nLocal: ");
					scanf("%s", info.local);				
					
					printf("\nTipo de Agressao: ");
					scanf("%s", info.crime);
					
					nome:			
					printf("\nNome: ");
					printf("\nNota: Se desejar fazer uma denuncia anonima insira '2', caso contrário insira '1'");
					scanf("%s", anom);
					
					if(anom[0]=='1')
					{
						printf("Insira o nome");
						scanf("%s", info.nome);
						
						snprintf(buffer, sizeof(buffer),"%s %s %s %s %s", info.data, info.hora, info.local, info.crime, info.nome);
					
						nBytes = strlen(buffer) + 1;
					
						sendto(clientSocket,buffer, nBytes,0,(struct sockaddr *)&serverAddr,addr_size);
						printf("Obrigado pela sua colaboracao!"); 
						
						goto menu_principal;
					}
					if(anom[0]=='2')
					{
						strcpy(info.nome,"ANONIMO"); 
						
						snprintf(buffer, sizeof(buffer),"%s %s %s %s %s", info.data, info.hora, info.local, info.crime, info.nome);
					
						nBytes = strlen(buffer) + 1;
					
						sendto(clientSocket,buffer, nBytes,0,(struct sockaddr *)&serverAddr,addr_size);
						printf("Obrigado pela sua colaboracao!"); 
						
						goto menu_principal;
					}
					
					else
					{
						printf("Opcao invalida");
						goto nome;
					}
					
				}
				if(menu_2[0]=='2')//alterar pass da conta
				{
					char nova_pass[100];
					printf("Insira a nova palavra pass");
					scanf("%s", nova_pass);
					 
					 
					sendto(clientSocket,nova_pass, strlen(nova_pass)+1,0,(struct sockaddr *)&serverAddr,addr_size);
					
					goto menu_principal;
				}
				if(menu_2[0]=='3')//pedir socorro
				{
					printf("\nVamos enviar um alarme para a PSP em 3, 2, 1\n");
					
					sendto(clientSocket,user, strlen(user)+1,0,(struct sockaddr *)&serverAddr,addr_size);
					printf("\nO alarme esta a ser acionado");
					
					goto menu_principal;
				}
				if(menu_2[0]=='4')//apagar conta 
				{
					char certeza[100];
					printf("deseja mesmo apagar a conta?");
					scanf("%s", certeza);
					
					if(certeza[0]=='1')
					{
						sendto(clientSocket,certeza, strlen(certeza)+1,0,(struct sockaddr *)&serverAddr,addr_size);
						goto menu_inicial;
					}
					
					else
					{	
					    printf("\nNao vamos apagar");	
					    goto menu_principal;
					}								
				}
				if(menu_2[0]=='5')//sistema de ajuda
				{
					char ajuda[100];
					menu_ajuda:
					printf("\n1-> Como comunicar um crime?");
					printf("\n2->Objetivo da aplicação?");
					printf("\n3->Como funciona o sistema de alarme?");
					scanf("%s", ajuda);
					
					if(ajuda[0]=='1')
					{
						printf("\nPara comunicar um crime será necessário colocar a data, hora, local o tipo de agressão e o Nome ");
						goto menu_principal;
					}
					if(ajuda[0]=='2')
					{
						printf("\nEsta aplicação tem como objetivo a proteção dos profissionais de saude que se sintam ameaçados em ambiente profissonal");
						goto menu_principal;
					}
					if(ajuda[0]=='3')
					{
						printf("\nQuando prime o sistema de alarme, é enviado instantaneamente um alarme para um profissional de agente de segurança, que irá aparecer o mais breve possível");
						goto menu_principal;
					}
					else
					{
						printf("Opcao invalida");
						goto menu_ajuda;
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
			    printf("\nUtilizador inválido!");
			    goto menu_inicial;
			}
			
		}
		
		else 
		{
			printf("\nInsira uma opcao valida");
			goto menu_inicial;

		}
 }   
  return 0;
}
