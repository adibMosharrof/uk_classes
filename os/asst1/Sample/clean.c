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
  char password[20];

  id = shmget(KEY, SEGSIZE,0);/* get shared memory to store data*/
  read_count_id = shmget(RC_KEY, READCOUNT_SEGSIZE, 0);/* get shared memory to store data*/
  if (id <0 || read_count_id <0){
    perror("del: shmget failed");
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
  verify_password();
  Wait(sema_set,0); 
  FILE *fp;
  char line[256];
  fp = fopen("output.txt","w");
  fclose(fp);
  while(strcmp(ptr->name, "") !=0){
    if (ptr->is_removed == 0){
      fp = fopen("output.txt","a");
      write_file(ptr, fp);  
      fclose(fp);
    }
    ptr++;    
  }
  shmdt((char  *)ptr); /* detach the shared memory segment */
  shmdt((char  *)read_count); /* detach the shared memory segment */
  shmctl(id, IPC_RMID,(struct shmid_ds *)0); /* destroy the shared memory segment*/
  shmctl(read_count_id, IPC_RMID,(struct shmid_ds *)0); /* destroy the shared memory segment*/
  Signal(sema_set,0);
  semctl(sema_set,0,IPC_RMID); /*Remove the semaphore set */
  printf("Deleted shared memory and semaphores\n");
  exit(0);
}

void write_file(struct StudentInfo *ptr , FILE *fp){
  fprintf(fp, "%s", ptr->name);
  fprintf(fp,"%s", ptr->id);
  fprintf(fp, "%s", ptr->address);
  fprintf(fp, "%s", ptr->phone);
}

void verify_password(){
  char* password[20];
  printf("Enter Password \n");
  scanf("%s", &password);
  if (strcmp("000", password) !=0){
    printf("Incorrect Password");
    exit(0);
  }
}