#include "local.h"

int selectRandomTeam(int, int);

int main()
{

    static ushort start_val[2] = {1, 0};
    int semid;
    union semun arg;
    int indexVariable = 0;
    srand(getpid());
    FILE *file;
    char line[MAX_LINE_LENGTH];
    int selectedTeam;

    // ================SHARED MEMORY================= //

    int shm_id = shmget((int)getpid(), sizeof(struct MEMORY), IPC_CREAT | 0666);
    struct MEMORY *shared_memory = (struct MEMORY *)shmat(shm_id, NULL, 0);
    printf("shared memory id in parent: %d\n", shm_id);

    // ================SEMAPHORE================= //

    if ((semid = semget((int)getpid(), 2,
                        IPC_CREAT | 0666)) != -1)
    {
        arg.array = start_val;

        if (semctl(semid, 0, SETALL, arg) == -1)
        {
            perror("semctl -- parent -- initialization");
            exit(3);
        }
    }
    else
    {
        perror("semget -- parent -- creation");
        exit(4);
    }

    // ================READING RANGES================= //

    int ranges[NUM_RANGES][2];

    file = fopen("ranges.txt", "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < NUM_RANGES; i++)
    {

        char buffer[50];
        if (fgets(buffer, sizeof(buffer), file) != NULL)
        {
            sscanf(buffer, "%*s %d %d", &ranges[i][0], &ranges[i][1]);
        }
        else
        {
            fprintf(stderr, "Error reading file\n");
            fclose(file);
            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < NUM_RANGES; i++)
    {
        printf("Range %d: Min = %d, Max = %d\n", i + 1, ranges[i][0], ranges[i][1]);
    }

    fclose(file);

    // ================READING ITEMS================= //

    file = fopen("items.txt", "r");

    if (file == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    char itemName[50];
    int itemShelfQuantity;
    int itemStorageQuantity;

    int itemIndex = 0;

    while (fscanf(file, "%s %d %d\n", itemName, &itemShelfQuantity, &itemStorageQuantity) == 3 && itemIndex < ITEMSCOUNT)
    {
        // Create a new item
        Item newItem;
        strcpy(newItem.itemName, strdup(itemName)); // Using strdup to duplicate the string
        newItem.itemShelfQuantity = itemShelfQuantity;
        newItem.itemStorageQuantity = itemStorageQuantity;
        shared_memory->items[itemIndex] = newItem;
        itemIndex++;
    }

    fclose(file);

    // ================FINDING TOTAL STORAGE ITEMS================= //***********************

    int totalStorageItems = 0;
    for (int i = 0; i < ITEMSCOUNT; i++)
    {
        totalStorageItems += shared_memory->items[i].itemStorageQuantity;
    }

    shared_memory->totalStorageItems = totalStorageItems;

    // ================GENERATING RANDOMS================= //

    int shelvingteamsNo = generateRandomNumber(ranges[0][0], ranges[0][1]);
    int shelfThreshold = generateRandomNumber(ranges[2][0], ranges[2][1]);
    int customerArrivalRange = generateRandomNumber(ranges[3][0], ranges[3][1]);
    int runtime = generateRandomNumber(ranges[4][0], ranges[4][1]);
    printf("SHLEF THRESHOLD %d\n", shelfThreshold);
    shared_memory->THRESHOLD = shelfThreshold;

    // ================FORKING CHILDREN================= //
    char arg1[20], arg2[20], arg3[20], arg4[20], arg5[20], arg6[20];
    char carg7[20], carg8[20];
    sprintf(arg1, "%d", shelfThreshold);
    sprintf(arg2, "%d", ranges[1][0]);
    sprintf(arg3, "%d", ranges[1][1]);
    sprintf(arg5, "%d", itemIndex);
    sprintf(arg6, "%d", shelvingteamsNo);
    sprintf(carg7, "%d", ranges[5][0]);
    sprintf(carg8, "%d", ranges[5][1]);

    //-------------------------Team Fork---------------------------//
    for (int i = 0; i < shelvingteamsNo; i++)
    {

        sprintf(arg4, "%d", indexVariable);
        pid_t pid = fork();

        if (pid < 0)
        {
            // Error occurred during fork
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            // Child process

            // Convert integers to strings
            execl(
                "./Team",
                "./Team",
                arg1,
                arg2,
                arg3,
                arg4,
                arg5,
                arg6,
                (char *)NULL);

            // If execl fails
            perror("Execl failed");
            exit(EXIT_FAILURE);
        }
        else
        {
            // Parent process
            printf("  process: PID=%d, Child PID=%d\n", getpid(), pid);
        }
        indexVariable++;
        sleep(2);
    }
    shared_memory->numOfTeams = indexVariable;

    // ================FORKING OPENGL ================== //

    pid_t pid_openGl = fork();

    if (pid_openGl < 0)
    {
        // Error occurred during fork
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }
    else if (pid_openGl == 0)
    {
        // Child process
        printf("HIIIIII\n");
        // Convert integers to strings
        execl(
            "./openGL",
            "./openGL",
            (char *)NULL);

        // If execl fails
        perror("OPENGL: Execl failed");

        exit(EXIT_FAILURE);
    }
    else
    {
        // Parent process
        printf("  process: PID=%d, Child PID=%d\n", getpid(), pid_openGl);
    }
    sleep(1);
    //-------------------------Customer Fork--------------------------------//

    while (1)
    {

        if (runtime <= 0 || shared_memory->totalStorageItems <= 0)
        {
            exit(0);
        }

        pid_t pid_2 = fork();

        if (pid_2 < 0)
        {
            // Error occurred during fork
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }
        else if (pid_2 == 0)
        {
            // Child process
            shared_memory->customerEntered = 1;
            execl(
                "./Customer",
                "./Customer",
                carg7,
                carg8,
                (char *)NULL);

            // If execl fails
            perror("Execl failed");
            exit(EXIT_FAILURE);
        }
        else
        {
            // kill(shared_memory->openGLID, SIGUSR2);
            acquire.sem_num = 0;

            if (semop(semid, &acquire, 1) == -1)
            {
                perror("semop -- PARENT -- acquire");
                exit(4);
            }
            printf("[PARENT] : AQQUIRING \n");

            // Parent process
            printf("Parent process: PID=%d, Child PID=%d\n", getpid(), pid_2);
            for (int i = 0; i < ITEMSCOUNT; i++)
            {
                if (shared_memory->items[i].itemShelfQuantity < shelfThreshold)
                {
                    printf("ITEM %s is less THAN THRESOLD %d\n", shared_memory->items[i].itemName, shelfThreshold);
                    selectedTeam = selectRandomTeam(0, shelvingteamsNo - 1);
                    shared_memory->itemToModify = i;
                    kill(shared_memory->teams[selectedTeam].teamId, SIGUSR1);
                    sleep(1);
                }
            }

            printf("[Parent] : TOTAL STORAGE COUNT %d\n", shared_memory->totalStorageItems);

            release.sem_num = 0;
            printf("[PARENT] : realese\n");

            if (semop(semid, &release, 1) == -1)
            {
                perror("semop -- PARENT -- release");
                exit(5);
            }
        }

        for (int i = 0; i < customerArrivalRange; i++)
        {
            if (runtime > 0)
            {
                runtime--;
                shared_memory->runtime = runtime;
                sleep(1);
            }
        }
    }

    return 0;
}

int selectRandomTeam(int min, int max)
{
    // Seed the random number generator with the current PID
    srand(time(NULL));

    // Generate a random number within the specified range
    int random_number = rand() % (max - min + 1) + min;

    return random_number;
}