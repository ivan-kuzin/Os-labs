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
#include <time.h>



void initSemValue(int semId, int n)
{
    struct sembuf op;
    op.sem_op = 1;
    op.sem_flg = 0;
    op.sem_num = n;
    semop(semId, &op, 1);
}

void tryToGetSemOrWait(int semId, int n)
{
    struct sembuf op;
    op.sem_op = -1;
    op.sem_flg = 0;
    op.sem_num = n;
    semop(semId, &op, 1);
}

void releaseSem(int semId, int n)
{
    initSemValue(semId, n);
}

int checkWin (int semId, int *mem)
{
    int res1, res2;
    if (mem[1]==mem[2])
    {
        res1 = 3;
        res1 = htons(res1);
        write (mem[3], &res1, sizeof(int));
    }
    else if ((mem[1]==1)&&(mem[2]==2))
    {
        res1 = 1;
        res2 = 2;
        res1 = htons(res1);
        res2 = htons(res2);
        write (mem[3], &res1, sizeof(int));
    }
    else if ((mem[1]==2)&&(mem[2]==1))
    {
        res1 = 2;
        res2 = 1;
        res1 = htons(res1);
        res2 = htons(res2);
        write (mem[3], &res1, sizeof(int));
    }
    else if ((mem[1]==1)&&(mem[2]==3))
    {
        res1 = 2;
        res2 = 1;
        res1 = htons(res1);
        res2 = htons(res2);
        write (mem[3], &res1, sizeof(int));
    }
    else if ((mem[1]==3)&&(mem[2]==1))
    {
        res1 = 1;
        res2 = 2;
        res1 = htons(res1);
        res2 = htons(res2);
        write (mem[3], &res1, sizeof(int));
    }
    else if ((mem[1]==2)&&(mem[2]==3))
    {
        res1 = 1;
        res2 = 2;
        res1 = htons(res1);
        res2 = htons(res2);
        write (mem[3], &res1, sizeof(int));
    }
    else if ((mem[1]==3)&&(mem[2]==2))
    {
        res1 = 2;
        res2 = 1;
        res1 = htons(res1);
        res2 = htons(res2);
        write (mem[3], &res1, sizeof(int));
    }
}

int checkWin1 (int semId, int *mem)
{
    int res1, res2;
    if (mem[1]==mem[2])
    {
        res1 = 3;
        res1 = htons(res1);
        write (mem[4], &res1, sizeof(int));
    }
    else if ((mem[1]==1)&&(mem[2]==2))
    {
        res1 = 1;
        res2 = 2;
        res1 = htons(res1);
        res2 = htons(res2);
        write (mem[4], &res2, sizeof(int));
    }
    else if ((mem[1]==2)&&(mem[2]==1))

    {
        res1 = 2;
        res2 = 1;
        res1 = htons(res1);
        res2 = htons(res2);
        write (mem[4], &res2, sizeof(int));
    }
    else if ((mem[1]==1)&&(mem[2]==3))
    {
        res1 = 2;
        res2 = 1;
        res1 = htons(res1);
        res2 = htons(res2);
        write (mem[4], &res2, sizeof(int));
    }
    else if ((mem[1]==3)&&(mem[2]==1))
    {
        res1 = 1;
        res2 = 2;
        res1 = htons(res1);
        res2 = htons(res2);
        write (mem[4], &res2, sizeof(int));
    }

    else if ((mem[1]==2)&&(mem[2]==3))
    {
        res1 = 1;
        res2 = 2;
        res1 = htons(res1);
        res2 = htons(res2);
        write (mem[4], &res2, sizeof(int));
    }
    else if ((mem[1]==3)&&(mem[2]==2))
    {
        res1 = 2;
        res2 = 1;
        res1 = htons(res1);
        res2 = htons(res2);
        write (mem[4], &res2, sizeof(int));
    }
}

int main(int argc, char *argv[]) {
	    int listenfd = 0;
	    struct sockaddr_in serv_addr;
	    char sendBuff[1025];
	    time_t ticks;
	    listenfd = socket(AF_INET, SOCK_STREAM, 0);
	    memset(&serv_addr, '0', sizeof(serv_addr));
	    memset(sendBuff, '0', sizeof(sendBuff));

	    serv_addr.sin_family = AF_INET;
	    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	    serv_addr.sin_port = htons(atoi(argv[1]));
	    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	    listen(listenfd, 10);

        int memId = shmget(IPC_PRIVATE, 5*sizeof(int), 0600|IPC_CREAT|IPC_EXCL);
        if (memId <= 0)
        {
    	    printf("error with shmget()\n");
    	    return -1;
        }
        int *mem = (int *)shmat(memId, 0, 0);
        if (NULL == mem)
        {
	        printf("error with shmat()\n");
	        return -2;
        }
        const size_t semCount = 6;
        int semId = semget(IPC_PRIVATE, semCount, 0600|IPC_CREAT);
        if (semId < 0)
        {
            perror("error with semget()");
            return -1;
        }
        else
        {
            printf("semaphore set id = %i\n", semId);
        }
	printf("1+\n");
            printf("2+\n");
            printf("3+\n");
            printf("4+\n");
           /* mem[3] = accept(listenfd, (struct sockaddr*)NULL, NULL); //ждем подключеня 1 клиента
	    printf("ID1 %d\n", mem[3]);
            mem[4] = accept(listenfd, (struct sockaddr*)NULL, NULL); //ждем подключеня 2 клиента */
	    printf("ID2 %d\n", mem[4]);
        pid_t childId = fork();
        if (childId < 0)
        {
            perror("error with fork()\n");
        }
        else if (childId > 0)
        {
	    releaseSem (semId, 1);
            printf("1-\n");
	    mem[3] = accept(listenfd, (struct sockaddr*)NULL, NULL); //ждем подключеня 1 клиента
	    tryToGetSemOrWait(semId, 1);
            printf("1+\n");
	    releaseSem (semId, 5);
	    printf("5-\n");
	   releaseSem (semId, 2);
            printf("2-\n");
            releaseSem (semId, 4);
            printf("4-\n");
            while (1)
            {
            int check = 0;
            check = htons (check);
            write (mem[3], &check, sizeof(int)); // отпраляем статус начала
            //write (mem[4], &check, sizeof(int));
            printf("Game start\n");
            printf("Wait 1\n");
            read (mem[3], &check, sizeof(int)); // ждем ход
            printf("Get 1\n");
            check = ntohs (check);
            printf("1  %d\n", check);
            mem[1]=check;  // конец хода
            releaseSem (semId, 0);
            printf("0-\n");
	    tryToGetSemOrWait(semId, 5);
            printf("5+\n");
	    tryToGetSemOrWait(semId, 2);
            printf("2+\n");
            checkWin (semId, mem); // проверяем результат
            printf("Check result success\n"); //end
            releaseSem (semId, 1);
            printf("1-\n");
            releaseSem (semId, 2);
            printf("2-\n");
	    int temp = mem[3];
            read (temp, &check, sizeof(int)); // читаем ответ о продолжени
            check = ntohs (check);
            if (check==2){
                close (mem[3]);
                mem[3] = accept(listenfd, (struct sockaddr*)NULL, NULL);
            }
            releaseSem (semId, 5);
            printf("5-\n");
	    tryToGetSemOrWait(semId, 0);
            printf("0+\n");
	    tryToGetSemOrWait(semId, 1);
            printf("1+\n");
            }
        }
        else 
        {
	    tryToGetSemOrWait(semId, 1);
            printf("c1+\n");
	    mem[4] = accept(listenfd, (struct sockaddr*)NULL, NULL); //ждем подключеня 2 клиента
	    releaseSem (semId, 1);
            printf("c1-\n");
            tryToGetSemOrWait(semId, 5);
	    printf("c5+\n");
	    tryToGetSemOrWait(semId, 2);
            printf("c2+\n");
	        tryToGetSemOrWait(semId, 4);
            printf("c4+\n");
            while (1)
            {  
	    int check = 0;
	write (mem[4], &check, sizeof(int));
            printf("C: Game start\n");
            printf("Wait 2\n");
	printf("Check ID2 %d\n", mem[4]);
	int spec;
            read (mem[4], &spec, sizeof(int)); // ждем ход
            printf("Get 2\n");
            check = ntohs (spec);
            printf("2  %d\n", check);
            mem[2]=check;
            releaseSem(semId, 5);
            printf("c5-\n");
	    tryToGetSemOrWait(semId, 0);
            printf("c0+\n");
            releaseSem(semId, 2);
            printf("c2-\n");
            tryToGetSemOrWait(semId, 1);
            printf("c1+\n");
            tryToGetSemOrWait(semId, 2);
            printf("c2+\n");

            checkWin1 (semId, mem); // проверяем результат
            printf("C: Check result success\n");
            read (mem[4], &check, sizeof(int)); //otwet
            check = ntohs (check);        
	    if (check==2){
                close (mem[4]);
                mem[4] = accept(listenfd, (struct sockaddr*)NULL, NULL);
            }

	    releaseSem(semId, 0);
            printf("c0-\n");
            tryToGetSemOrWait(semId, 5);
            printf("c5+\n");
            releaseSem(semId, 1);
            printf("c1-\n");
            }
        }

}
