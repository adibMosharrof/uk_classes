/*File change.c */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/sem.h>
#include <pwd.h>
#include "header.h"

int main(int argc, char* argv[])
{
 int i,id, read_count_id;
  struct StudentInfo *ptr;
  int sema_set;
  int* read_count;
  char student_id[50];
  char student_name[100];
  char student_phone[100];
  char student_address[100];
  char password[20];
  int found =0;
  int is_verified =0;

  id = shmget(KEY, SEGSIZE,0);/* get shared memory to store data*/
  read_count_id = shmget(RC_KEY, READCOUNT_SEGSIZE, 0);/* get shared memory to store data*/
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

sema_set=semget(SEMA_KEY, 0,0);
  if ((sema_set < 0) ){
    perror("create: semget failed");
    exit(2);
  }

  int action;
  while (1){
    display_menu();
    scanf("%d", &action);
    if (action >= 4 || action < 1){
      printf("Not a valid action entered\n");
      exit(0);
    }
  Wait(sema_set,0);  
  switch (action){
    case 1:
      if (verify_password() ==0){
          break;
      }     
      printf("Enter Student id \n");
      scanf("%s", &student_id);
      printf("Enter Student name \n");
      scanf("%s", &student_name);
      printf("Enter Student phone \n");
      scanf("%s", &student_phone);
      printf("Enter Student address \n");
      scanf("%s", &student_address);

      // Wait(sema_set,0); 
      printf("the value of sema_set=%d\n", sema_set); 
      found =0;
      // looping to the end of the student array to find the position to enter the new record
      while(strcmp(ptr->name, "") !=0){
        ptr++;    
      }
      
      strcpy(ptr->id,student_id);
      strcpy(ptr->name,student_name);
      strcpy(ptr->phone,student_phone);
      strcpy(ptr->address,student_address);
      printf("Added student %s\n", student_name);
      sleep(2);
      printf("Done Sleep in add\n");
      Signal(sema_set,0);
      break;
    case 2:      
      if (verify_password() ==0){
          break;
      }     
      printf("Enter Student id \n");
      scanf("%s", &student_id);
      // Wait(sema_set,0); 
      found =0;
      //looping over the student array to find the student to delete
      while(strcmp(ptr->name, "") !=0){
        //filtering by student id and checking whether the student has been deleted
        if (strcmp(strtok(ptr->id ,"\n"), student_id) == 0 && ptr->is_removed == 0 ){
          found = 1;
          //setting is removed to 1, which means the student is deleted and will not be read in the future
          // not actually deleting the record from memory, as it might create problems with looping
          // through the student array, as we are storing data as an array in the shared memory.
          ptr->is_removed =1;
          printf("Deleted student with id %s\n", student_id);
          break;
        }
        ptr++;    
      }
      if (found == 0){
        printf("record not found\n");
      } 

      Signal(sema_set,0);
      break;

    case 3:
      if (verify_password() ==0){
          break;
      }     
      printf("Enter Student id \n");
      scanf("%s", &student_id);
      // Wait(sema_set,0); 
      found =0;
      //looping over the student array to find the student to update
      while(strcmp(ptr->name, "") !=0){
        //filtering by student id and checking whether the student has been deleted
        if (strcmp(ptr->id, student_id) == 0 && ptr->is_removed == 0 ){
          found = 1;
          printf("Record Found\n");
          printf("Enter Name\n");
          scanf("%s", &student_name);
          printf("Enter Phone\n");
          scanf("%s", &student_phone);
          printf("Enter Address\n");
          scanf("%s", &student_address);
          //updating student information
          if (strcmp(student_name, "") != 0){
            strcpy(ptr->name,student_name);
          }
          if (strcmp(student_phone, "") != 0){
            strcpy(ptr->phone,student_phone);
          }
          if (strcmp(student_address, "") != 0){
            strcpy(ptr->address,student_address);
          }
          printf("Updated student with id %s\n", student_id);
          break;
        }
        ptr++;    
      }
      if (found == 0){
        printf("record not found\n");
      }
      Signal(sema_set,0);
      break;

    default:
      break;
  }
  Signal(sema_set,0);
  }
  
}

void display_menu(){
  printf("1 for add\n");
  printf("2 for delete\n");
  printf("3 for change\n");
  printf("4 for exit\n");
}

int verify_password(){
  char* password[20];
  printf("Enter Password \n");
  scanf("%s", &password);
  if (strcmp("000", password) !=0){
    printf("Incorrect Password\n");
    return 0;
  }
  return 1;
}