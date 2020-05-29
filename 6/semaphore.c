#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <string.h>
#include <time.h>

void GeneralSem(int semId, int n, int d)
{
    struct sembuf op;
    op.sem_op = d;
    op.sem_flg = 0;
    op.sem_num = n;
    semop(semId, &op, 1);
}

void uncloseSem(int semId, char *check, int n)
{
    GeneralSem(semId, n, 1);
    check[n] = 0;
}

int closeSem(int semId, char *check, int n)
{
    int isBusy = check[n];
    GeneralSem(semId, n, -1);
    check[n] = 1;
    return isBusy;
}

void sort(int semId, int memId, char checkMemOffset, const size_t n)
{
    int *nums = (int *)shmat(memId, 0, 0);
    char *checks = shmat(memId, 0, 0) + checkMemOffset;

    for (int i = 0; i < n; i++)
    {
        int minInd = i;
        for (int j = i + 1; j < n; j++)
        {
            closeSem(semId, checks, i);
            closeSem(semId, checks, j);
            if (nums[j] < nums[minInd])
            {
                minInd = j;
            }
            uncloseSem(semId, checks, i);
            uncloseSem(semId, checks, j);
        }
        if (i != minInd)
        {
            closeSem(semId, checks, i);
            closeSem(semId, checks, minInd);
            int t = nums[i];
            nums[i] = nums[minInd];
            nums[minInd] = t;
            uncloseSem(semId, checks, i);
            uncloseSem(semId, checks, minInd);
        }
    }
}

void fill_random_nums(int *nums, int n, int min, int max)
{
    srand((unsigned)(time(0)));
    for (int i = 0; i < n; i++)
    {
        nums[i] = min + rand() % (max - min + 1);
    }
}

void print_nums(int *nums, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%d ", nums[i]);
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    const int N = atoi(argv[1]);
    const int min = atoi(argv[2]);
    const int max = atoi(argv[3]);

    int memId = shmget(IPC_PRIVATE, sizeof(int) * N + N, 0600 | IPC_CREAT | IPC_EXCL);
    int checkMemOffset = sizeof(int) * N;
    int semId = semget(IPC_PRIVATE, N, 0600 | IPC_CREAT);

    int *numbers = (int *)shmat(memId, 0, 0);
    char *checks = shmat(memId, 0, 0) + checkMemOffset;
    fill_random_nums(numbers, N, min, max);
    print_nums(numbers, N);

    for (int i = 0; i < N; i++)
    {
        uncloseSem(semId, checks, i);
    }

    int childId = fork();
    if (childId == 0)
    {
        sort(semId, memId, checkMemOffset, N);
    }
    else
    {
        int i = 0;
        int status;
        do
        {
            printf("%d: ", i);
            for (int j = 0; j < N; j++)
            {
                if (closeSem(semId, checks, j))
                {
                    printf("[%d] ", numbers[j]);
                }
                else
                {
                    printf("%d ", numbers[j]);
                }

                fflush(stdout);
                uncloseSem(semId, checks, j);
            }
            printf("\r\n");
            status = waitpid(childId, NULL, WNOHANG);
            i++;
        } while (!status);

        printf("Sort finished \r\n");
        print_nums(numbers, N);

        shmctl(memId, 0, IPC_RMID);
        semctl(semId, 0, IPC_RMID);
    }
}
