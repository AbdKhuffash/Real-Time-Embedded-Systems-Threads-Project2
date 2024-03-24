// header file for the supermarket project

#ifndef __LOCAL_H__
#define __LOCAL_H__

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <wait.h>
#include <signal.h>
#include <pthread.h>

#define ITEMSCOUNT 17
#define MAX_LINE_LENGTH 1000
#define NUM_RANGES 6
#define TEAMSCOUNT 50

union semun
{
    int val;
    struct semid_ds *buf;
    ushort *array;
};

struct sembuf acquire = {0, -1, SEM_UNDO},
              release = {0, 1, SEM_UNDO};

typedef struct
{
    char itemName[50];
    int itemShelfQuantity;
    int itemStorageQuantity;
} Item;

typedef struct
{
    pid_t teamId;
    int teamSize;

} Team;

struct MEMORY
{
    Item items[ITEMSCOUNT];
    Team teams[TEAMSCOUNT];
    int itemToModify;
    int totalStorageItems;
    int openGLID;
    int leaderPoked;
    int employeesPoked;
    int customerEntered;
    int runtime;
    int moveCart;
    int isCartEmpty;
    int THRESHOLD;
    int numOfTeams;
};

int generateRandomNumber(int min, int max)
{
    // Seed the random number generator with the current PID
    srand(getpid());

    // Generate a random number within the specified range
    int random_number = rand() % (max - min + 1) + min;

    return random_number;
}

int isInteger(char *str)
{
    if (str == NULL || *str == '\0')
    {
        // Handle the case of an empty string or a NULL pointer
        return 0; // Not an integer
    }

    // Check each character in the string
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (!isdigit((unsigned char)str[i]) && (i == 0 && str[i] != '-'))
        {
            // If the first character is not a digit (except for a minus sign), it's not an integer
            return 0; // Not an integer
        }
    }

    // If the loop completes, the string is a valid integer
    return 1; // Is an integer
}

#endif
