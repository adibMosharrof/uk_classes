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
  int i, id, read_count_id;
  struct StudentInfo *students_ptr;
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
  students_ptr=(struct StudentInfo * )shmat(id,0,0);/*attach the shared memory segment to the process's address space */
  read_count=(int * )shmat(read_count_id,0,0);/*attach the shared memory segment to the process's address space */
  // if (students <= (struct StudentInfo *) (0) || read_count <= (int *) (0)) {
  if (students_ptr <= (struct StudentInfo *) (0) || read_count <= (int *) (0)) {
    perror("create: shmat failed");
    exit(2);
}
  sema_set=semget(SEMA_KEY, 0,0);
  // sema_set = GetSemaphs(SEMA_KEY, NUM_SEMAPHS); /* get a set of NUM_SEMAPHS semaphores*/
  if ((sema_set < 0) ){
    perror("create: semget failed");
    exit(2);
  }

  Wait(sema_set,1); 
  *read_count = *read_count +1;
  if (*read_count ==1){
    Wait(sema_set,0);
  }
  Signal(sema_set,1);

  found =0;
  while(strcmp(students_ptr->name, "") !=0){
    if (strcmp(strtok(students_ptr->id ,"\n"),argv[1]) == 0 && students_ptr->is_removed == 0 ){
      // display(students_ptr);  
      DisplayStudent(students_ptr);
      printf("student display");
      found = 1;
      break;
    }
    students_ptr++;    
  }
  if (found == 0){
	  printf("record not found\n");
  }
  printf("Starting sleep in query\n");
  // sleep(10);
  printf("Ending sleep in query\n");
  Wait(sema_set,1);
  *read_count = *read_count -1;
  if (*read_count ==0){
    Signal(sema_set,0);
  }
  Signal(sema_set,1);
}