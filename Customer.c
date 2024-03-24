
#include "local.h"

int main(int argc, char *argv[])
{
  printf("==========================entered customer====================\n");

  int semid, shmid;
  pid_t ppid = getppid();
  pid_t pid = getpid();
  char *shmptr;
  struct MEMORY *memptr;
  int min_items;
  int max_items;
  int items_range;
  int item_quantity;

  // ================GETTING AND DECLARING VARIABLES================= //
  if (argc != 3)
  {
    fprintf(stderr, "%s here you go\n", argv[0]);

    exit(-1);
  }
  min_items = atoi(argv[1]);
  max_items = atoi(argv[2]);
  printf("min_items %d  max_items %d\n", min_items, max_items);
  items_range = generateRandomNumber(min_items, max_items);
  Item items[items_range];

  printf("Parent:%d\nCurrent:%d\n", ppid, pid);

  if ((shmid = shmget((int)ppid, 0, 0)) != -1)
  {
    if ((shmptr = (char *)shmat(shmid, (char *)0, 0)) == (char *)-1)
    {
      perror("shmat -- Customer -- attach");
      exit(1);
    }
    memptr = (struct MEMORY *)shmptr;
  }
  else
  {
    perror("shmget -- Customer -- access1");
    exit(2);
  }

  printf("MEMORY ACCESSED \n");

  /*
   * Access the semaphore set
   */

  if ((semid = semget((int)ppid, 2, 0)) == -1)
  {
    perror("semget -- Customer -- access");
    exit(3);
  }

  printf("Semaphore ACCESSED \n");

  srand(pid);

  /*
   * Acquiring
   */

  acquire.sem_num = 0;

  if (semop(semid, &acquire, 1) == -1)
  {
    perror("semop -- Customer -- acquire");
    exit(4);
  }
  printf("AQQUIRING \n");
  
  /*
   * Code Part
   */
  int item_index;
  printf(" $$$$$$$$$$$$$$$$   customer %d is shopping %d items $$$$$$$$$$$$\n", pid, items_range);
  for (int i = 0; i < items_range; i++)
  {

    srand(time(NULL));
    item_index = rand() % 17;
    printf("choosen item:%s index:%d shelfsquantity:%d \n", memptr->items[item_index].itemName, item_index,
           memptr->items[item_index].itemShelfQuantity);
    if (memptr->items[item_index].itemShelfQuantity > 0)
    {
      if (memptr->items[item_index].itemShelfQuantity == 1)
      {
        memptr->items[item_index].itemShelfQuantity--;
      }
      item_quantity = generateRandomNumber(1, (memptr->items[item_index].itemShelfQuantity / 2));
      if (memptr->items[item_index].itemShelfQuantity >= item_quantity)
      {

        printf("Customer item name:%s quantity on shelfs:%d customer quantity:%d\n", memptr->items[item_index].itemName,
               memptr->items[item_index].itemShelfQuantity, item_quantity);
        memptr->items[item_index].itemShelfQuantity = memptr->items[item_index].itemShelfQuantity - item_quantity;
        printf("Remain item name:%s quantity on shelfs:%d\n", memptr->items[item_index].itemName,
               memptr->items[item_index].itemShelfQuantity);
        // printf("DONE1\n");
      }
    }

    if (memptr->items[item_index].itemShelfQuantity == 0)
    {
      printf("Item %s not available or insufficient quantity\n", memptr->items[item_index].itemName);
      // printf("DONE2\n");
    }
    sleep(1);
  }

  // memptr->items[16].itemStorageQuantity--;

  /*
   * Releasing
   */
  memptr->customerEntered=0;
  release.sem_num = 0;
  printf("realese\n");

  if (semop(semid, &release, 1) == -1)
  {
    perror("semop -- Customer -- release");
    exit(5);
  }

  exit(0);
}
