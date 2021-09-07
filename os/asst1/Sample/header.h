
/* header.h */
#define KEY  ((key_t)(124344)) /*change it to last five digits of your SID*/
#define RC_KEY  ((key_t)(4345)) /*change it to last five digits of your SID*/
#define NUM_STUDENTS 50
#define READCOUNT_SEGSIZE sizeof(int)
#define SEGSIZE  NUM_STUDENTS*sizeof(struct StudentInfo)

#define NUM_SEMAPHS 2
#define SEMA_KEY   ((key_t)(44456)) /* change this to last four digits of SID */

struct StudentInfo{
  char name[100];
  char id[50];
  char address[100];
  char phone[30];
  char whoModified[30];
  int is_removed;
};

void Wait(int semaph, int n);
void Signal(int semaph, int n);
int GetSemaphs(key_t k, int n);
