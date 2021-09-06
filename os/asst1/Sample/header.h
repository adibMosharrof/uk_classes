
/* header.h */
#define KEY  ((key_t)(124345)) /*change it to last five digits of your SID*/
#define NUM_STUDENTS 11
#define SEGSIZE  NUM_STUDENTS*sizeof(struct StudentInfo)

#define NUM_SEMAPHS 5
#define SEMA_KEY   ((key_t)(44456)) /* change this to last four digits of SID */

struct StudentInfo{
  char name[20];
  char id[20];
  char address[50];
  char phone[15];
  char whoModified[10];
};

void Wait(int semaph, int n);
void Signal(int semaph, int n);
int GetSemaphs(key_t k, int n);
