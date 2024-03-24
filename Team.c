#include "local.h"

pthread_t main_thread_id;
pthread_mutex_t initializer_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t shm_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition = PTHREAD_COND_INITIALIZER;

struct MEMORY *memptr;

void *employee_routine(void *arg);
void leader_Observe();
int cart;
void getFromStorage();

int semid, shmid;
char *shmptr;

int main(int argc, char *argv[])
{
    pid_t ppid = getppid();
    pid_t pid = getpid();
    //=================MAKE IT SENSETIVE TO SIGNALS==================== //
    if (sigset(SIGUSR1, getFromStorage) == SIG_ERR)
    {
        perror("Sigset can not set SIGUSR1");
        exit(SIGUSR1);
    }

    // ================GETTING AND DECLARING VARIABLES================= //
    if (argc != 7)
    {
        fprintf(stderr, "%s shelf_threshold min_employees_number max_employees_number teamIndex\n", argv[0]);
        exit(-1);
    }

    int shelfThreshold = atoi(argv[1]);

    int minTeamSize = atoi(argv[2]);
    int maxTeamSize = atoi(argv[3]);
    int teamIndex = atoi(argv[4]);
    int teamSize = generateRandomNumber(minTeamSize, maxTeamSize);
    int ItemsTypeCount = atoi(argv[5]);
    int number_of_teams = atoi(argv[6]);
    srand(pid);

    // ================GETTING AND ATTACHING MEMORY================= //

    if ((shmid = shmget((int)ppid, 0, 0)) != -1)
    {
        if ((shmptr = (char *)shmat(shmid, (char *)0, 0)) == (char *)-1)
        {
            perror("shmat -- TEAM -- attach");
            exit(1);
        }
        memptr = (struct MEMORY *)shmptr;
    }
    else
    {
        perror("shmget -- TEAM -- access1");
        exit(2);
    }

    // ================GETTING AND ATTACHING SEMAPHORE================= //

    if ((semid = semget((int)ppid, 2, 0)) == -1)
    {
        perror("semget -- TEAM -- access");
        exit(3);
    }

    // ================ADDING TEAM TO MEMORY================= //

    Team currentTeam;
    currentTeam.teamId = pid;
    currentTeam.teamSize = teamSize;

    memptr->teams[teamIndex] = currentTeam;

    //-------------------------------Creating Employees-----------
    pthread_t threads[teamSize];

    printf("Team ID: %d \t Number Of Employees: %d  \n", (int)memptr->teams[teamIndex].teamId, teamSize);

    // main thread

    main_thread_id = pthread_self();

    pthread_mutex_lock(&initializer_mutex);

    printf("Team Leader : %lu \n", (unsigned long)main_thread_id);

    for (int i = 0; i < teamSize - 1; i++)
    {

        if (pthread_create(&threads[i], NULL, employee_routine, (void *)&shelfThreshold) != 0)
        {
            perror("Failed to create Threads");
            exit(-4);
        }
    }

    pthread_mutex_unlock(&initializer_mutex);

    // wait for threads to complete
    for (int i = 0; i < teamSize - 1; i++)
        pthread_join(threads[i], NULL);

    pause();
}

void getFromStorage()
{

    acquire.sem_num = 0;

    if (semop(semid, &acquire, 1) == -1)
    {
        perror("semop -- TEAM LEAD -- acquire");
        exit(4);
    }

    printf("[TEAM %d LEAD] : AQQUIRING \n", getpid());

    printf("[TEAM %d LEAD] : Item %s has %d in Storage!\n", getpid(), memptr->items[memptr->itemToModify].itemName, memptr->items[memptr->itemToModify].itemStorageQuantity);
    if (memptr->items[memptr->itemToModify].itemStorageQuantity < 4)
    {
        cart = memptr->items[memptr->itemToModify].itemStorageQuantity;
    }
    else
    {
        cart = (int)(0.25 * memptr->items[memptr->itemToModify].itemStorageQuantity);
    }
    memptr->items[memptr->itemToModify].itemStorageQuantity -= cart;
    memptr->totalStorageItems -= cart;

    printf("[TEAM %d LEAD] : CART HAS QUANTITY %d\n", getpid(), cart);
    printf("[TEAM %d LEAD] : STORAGE NOW HAS %d\n", getpid(), memptr->items[memptr->itemToModify].itemStorageQuantity);
    memptr->moveCart=1;
    release.sem_num = 1;
    printf("[TEAM %d LEAD] : realese\n", getpid());

    if (semop(semid, &release, 1) == -1)
    {
        perror("semop -- TEAM LEAD -- release");
        exit(5);
    }

    pthread_cond_broadcast(&condition);
}

void *employee_routine(void *arg1)
{
    pthread_t thread_id = pthread_self();
    while (1)
    {
        pthread_mutex_lock(&initializer_mutex);
        // printf("Created Thread for Team %d : %lu \n", getpid(), (unsigned long)thread_id);
        pthread_cond_wait(&condition, &initializer_mutex);
        // printf("[%ld] : U poked %ld\n", thread_id, thread_id);
        pthread_mutex_unlock(&initializer_mutex);

        while (cart > 0)
        {
            

            acquire.sem_num = 1;

            if (semop(semid, &acquire, 1) == -1)
            {
                perror("semop -- TEAM EMP -- acquire");
                exit(4);
            }
            printf("[TEAM %d EMP] : AQQUIRING \n", getpid());

            cart--;
            memptr->items[memptr->itemToModify].itemShelfQuantity++;

            release.sem_num = 1;

            printf("[TEAM %d EMP] : realesed inner\n", getpid());

            if (semop(semid, &release, 1) == -1)
            {
                perror("semop -- TEAM EMP -- release inner");
                exit(5);
            }

            
        }

        if (cart <= 0)
        {
            memptr->moveCart=0;
            acquire.sem_num = 1;

            if (semop(semid, &acquire, 1) == -1)
            {
                perror("semop -- TEAM EMP -- acquire");
                exit(4);
            }
            printf("[TEAM %d EMP] : LAST AQQUIRING \n", getpid());
        }
        
        release.sem_num = 0;

        printf("[TEAM %d EMP] : realese outer\n", getpid());
        memptr->isCartEmpty=1;
        if (semop(semid, &release, 1) == -1)
        {
            perror("semop -- TEAM EMP -- release outer");
            exit(5);
        }
    }
}
