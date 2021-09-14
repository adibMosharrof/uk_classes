  /* File create.c */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "header.h"

int main()
{
  int i,id, read_count_id;
  struct StudentInfo *ptr;
  int sema_set;
  int* read_count;

  id = shmget(KEY, SEGSIZE,IPC_CREAT|0666);/* get shared memory to store data*/
  read_count_id = shmget(RC_KEY, READCOUNT_SEGSIZE, IPC_CREAT|0666);/* get shared memory to store data*/
  if (id <0 || read_count_id <0){
    perror("create: shmget failed");
    exit(1);
  }

  ptr=(struct StudentInfo * )shmat(id,0,0);/*attach the shared memory segment to the process's address space */
  read_count=(int * )shmat(read_count_id,0,0);/*attach the shared memory segment to the process's address space */
  if (ptr <= (struct StudentInfo *) (0) || read_count <= (int *) (0)) {
    perror("create: shmat failed");
    exit(2);
  }

  sema_set = GetSemaphs(SEMA_KEY, NUM_SEMAPHS); /* get a set of NUM_SEMAPHS semaphores*/
  if ((sema_set < 0) ){
    perror("create: semget failed");
    exit(2);
  }

  Wait(sema_set,0); 
  FILE *fp;
  char line[256];
  fp = fopen("input.txt","r");
  // initializing read count to zero
  *read_count = 0;
  tmp = ptr;
  while(!feof(fp)){
    fgets(&line, 100, fp);
    strcpy(ptr->name,strtok(line,"\n"));
    fgets(&line, 50, fp);
    strcpy(ptr->id,strtok(line,"\n"));
    fgets(&line, 100, fp);
    strcpy(ptr->address,strtok(line,"\n"));
    fgets(&line, 30, fp);
    strcpy(ptr->phone,strtok(line,"\n"));
    ptr->is_removed = 0;
    ptr++;
  }
  fclose(fp);
  printf("Loaded student information into shared memory\n");
  Signal(sema_set,0);
}

