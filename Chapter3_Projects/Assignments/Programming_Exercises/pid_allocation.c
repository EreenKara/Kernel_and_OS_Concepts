#include <stdio.h>
#include <stdlib.h>

#define MIN_PID 300
#define MAX_PID 5000
#define TOTAL_PID (MAX_PID-MIN_PID)
typedef struct  {
    unsigned char value : 1; // buradaki ": 1" ifadesi bunu bir bitmap yapiyor
} bitmap_t;
bitmap_t * bitmap;
int main(){

    printf("sizeof : %d\n",sizeof(bitmap_t));
    return 0;
}


// BITMAP'i olustur.
int allocate_map(void)
{
    bitmap=(bitmap_t*)malloc(sizeof(bitmap_t)*TOTAL_PID);
    if(bitmap==NULL) return -1;
    for (size_t i = 0; i < TOTAL_PID; i++)
    {
        bitmap[i].value=0;
    }
    
    return 1; //successful
}
int deallocate_map(void)
{
    free(bitmap);
    return 1; //successful
}

// Uygun bir tane process id atamasi yap. 
int allocate_pid(void)
{
    int pid=0;
    for (; pid < TOTAL_PID; pid++)
    {
        if(bitmap[pid].value==0) break;
    }
    if(pid>=TOTAL_PID) return -1; // FAIL
    bitmap[pid].value = 1;
    // MIN_PID dahil ediyorum ki pid number istedigim aralikta olsun
    return (pid+MIN_PID); // SUCCESFULL
}

void release_pid(int pid)
{
    int index = pid-MIN_PID;
    bitmap[index].value = 0;
}  