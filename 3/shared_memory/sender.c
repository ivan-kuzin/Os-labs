#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <time.h>

int main()
{
	srand((unsigned)(time(0)));
		
	int memId = shmget(IPC_PRIVATE, sizeof(int) * 20, 0600|IPC_CREAT|IPC_EXCL);
	
	int* numbers = (int*)shmat(memId, 0, 0);

	for (size_t i = 0;i < 20;i++) 
	{
			numbers[i] = rand() % 10000;
	}
	
	char callbuf[1024];
	sprintf(callbuf, "./receiver %i %i", memId, 20);
	system(callbuf);
	
	return 0;
}
