/**
 * Figure 3.35
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define SIZE 5

int nums[SIZE] = {0,1,2,3,4};
// parent ve child bu kismi paylasmiyorlar. Ayri ayri sahipler o yuzden asagida parent ve chil ayri ayri ciktilar basacaklar.

int main()
{
	int i; 
    	pid_t pid;

    	pid = fork();

    	if (pid == 0) {
      		for (i = 0; i < SIZE; i++) {
        		nums[i] *= -i;
        		printf("CHILD: %d\n",nums[i]); /* LINE X */
                // bu hesaplamadan parent'in haberi olmaz.
      		} 
    	}
    	else if (pid > 0) { 
      		wait(NULL);

      		for (i = 0; i < SIZE; i++)
        		printf("PARENT: %d\n",nums[i]); /* LINE Y */
    	}

    	return 0;
  }
