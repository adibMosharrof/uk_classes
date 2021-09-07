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

int main(int argc, char *argv[])
{
  int i,id, read_count_id;
  struct StudentInfo (*students)[NUM_STUDENTS];
  struct StudentInfo *ptr, *tmp;
  int sema_set;
  int* read_count;
  int found = 0;
  
  id = shmget(KEY, SEGSIZE,0);/* get shared memory to store data*/
  read_count_id = shmget(RC_KEY, READCOUNT_SEGSIZE, 0);/* get shared memory to store data*/
  if (id <0 || read_count_id <0){
    perror("create: shmget failed");
    exit(1);
}
  // students=(struct StudentInfo * )shmat(id,0,0);/*attach the shared memory segment to the process's address space */
  ptr=(struct StudentInfo * )shmat(id,0,0);/*attach the shared memory segment to the process's address space */
  read_count=(int * )shmat(read_count_id,0,0);/*attach the shared memory segment to the process's address space */
  // if (students <= (struct StudentInfo *) (0) || read_count <= (int *) (0)) {
  if (ptr <= (struct StudentInfo *) (0) || read_count <= (int *) (0)) {
    perror("create: shmat failed");
    exit(2);
}

  sema_set = GetSemaphs(SEMA_KEY, NUM_SEMAPHS); /* get a set of NUM_SEMAPHS
						   semaphores*/
  if ((sema_set < 0) ){
    perror("create: semget failed");
    exit(2);
  }

  Wait(sema_set,1); 
  found =0;
  while(strcmp(ptr->name, "") !=0){
    if (strcmp(strtok(ptr->id ,"\n"),argv[1]) == 0 && ptr->is_removed == 0 ){
      display(ptr);  
      found = 1;
      break;
    }
    ptr++;    
  }
  if (found == 0){
	  printf("record not found\n");
  }

  Signal(sema_set,1);
}

void display(struct StudentInfo *ptr ){
  printf("Name %s\n", ptr->name);
  printf("Id %s\n", ptr->id);
  printf("Address %s\n", ptr->address);
  printf("Phone %s\n", ptr->phone);
}