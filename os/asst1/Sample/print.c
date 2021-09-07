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
  struct StudentInfo (*students)[NUM_STUDENTS];
  struct StudentInfo *ptr, *tmp;
  int sema_set;
  int* read_count;
  

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
  // increasing value of read count by 1
  *read_count = *read_count+1;
   show_read_count(*read_count);
 // if value of read_count==1 then Wait(semaset, 0);
  if (*read_count == 1){
    Wait(sema_set,0);
  }
  Signal(sema_set,1);
  while(strcmp(ptr->name, "") !=0){
    if (ptr->is_removed == 0){
      display(ptr);  
    }
    ptr++;    
    sleep(2);
  }
  Wait(sema_set,1); 
//   decrement read_count; // in shared memory
  *read_count = *read_count-1;
   show_read_count(*read_count);
  
// if value of read_count==0 then Signal(semaset, 0);
if(*read_count ==0){
  Signal(sema_set,0);
}
Signal(sema_set,1);
}

void display(struct StudentInfo *ptr ){
  printf("Name %s\n", ptr->name);
  printf("Id %s\n", ptr->id);
  printf("Address %s\n", ptr->address);
  printf("Phone %s\n", ptr->phone);
}

void show_read_count(int* count){
  printf("Read count is %d\n", count);
}