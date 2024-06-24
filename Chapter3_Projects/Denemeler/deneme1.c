/*
16 adet process olusuyor
*/

#include <stdio.h>
#include <unistd.h>

int main()
{
	int i;

	for (i = 0; i < 4; i++)
		fork();
	printf("Process id: %d",getpid());
	return 0;
}
