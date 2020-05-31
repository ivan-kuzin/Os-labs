#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <iostream>


void game(int sockfd)
{
    int check;
    while (1)
    {
    read (sockfd, &check, sizeof(int)); // получаем инфомацию, что оба игрока готовы
    check = ntohs(check);
	char in;
    do
    {
         printf("Game start: Choose: 1 - stone, 2 - clippers, 3 - papper\n");
	printf("Scan..\n");
        scanf("%c", &in);
	printf("Scan..\n");
	 //printf("Scan %d\n", check);
    } while ((in!='1')&&(in!='2')&&(in!='3'));
    switch (in)
    {
	case '1':
	check = 1;
	break;
	case '2':
	check = 2;
	break;
	case '3':
	check = 3;
	break;
    }
     printf("You prepare %d\n", check);
    check = htons(check);
    write (sockfd, &check, sizeof(int)); // отправляем ход
    printf("Send success\n");
    printf("Waiting for results\n");
    read (sockfd, &check, sizeof(int)); // получаем результаты игры
    check = ntohs(check);
    if (check == 1) 
    {
        printf("You Win!\n");
    }
    else if (check == 2)
    {
        printf("You Lose!\n");
    }
    else if (check == 3)
    {
        printf("Stand off\n");
    }
    printf("Would you like play once again: Choose: 1 - Yes, 2 - No\n");
    scanf("%d", &check);
    check = htons(check);
    write (sockfd, &check, sizeof(int)); // отправляем наше решение
    if (ntohs(check)==2) // если не хочет играть, завершаем игру
    break;
    printf ("Wait..."); // Ждет второго игрока в случае продолжения
    }
}

	int main(int argc, char *argv[]) {
	    int sockfd = 0, n = 0;
	    char recvBuff[1024];
	    struct sockaddr_in serv_addr;
	    if(argc != 3) {
	        printf("\n Usage: %s <ip of server> \n",argv[0]);
	        return 1;
	    }
	    memset(recvBuff, '0',sizeof(recvBuff));
	    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	    {
	        printf("\n Error : Could not create socket \n");
	        return 1;
	    }
	    memset(&serv_addr, '0', sizeof(serv_addr));
	    serv_addr.sin_family = AF_INET;
	    serv_addr.sin_port = htons(atoi(argv[2]));
	    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
	    {
	        printf("\n inet_pton error occured\n");
	        return 1;
	    }
	    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	    {
	       printf("\n Error : Connect Failed \n");
	       return 1;
	    }
        printf("Connection sucsess\n");
        printf("Wait..\n");
        game(sockfd);
    }
