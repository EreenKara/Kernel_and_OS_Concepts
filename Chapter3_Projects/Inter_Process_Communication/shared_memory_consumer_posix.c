/**
 * Simple program demonstrating shared memory in POSIX systems.
 *
 * This is the consumer process
 *
 * Figure 3.18
 *
 * To compile, enter
 *	gcc shm-posix-consumer.c -lrt
 *
 * @author Gagne, Galvin, Silberschatz
 * Operating System Concepts - Tenth Edition
 * Copyright John Wiley & Sons - 2018
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

int main()
{
	const char *name = "OS";
	const int SIZE = 4096;

	int shm_fd;
	void *ptr;
	int i;

	/* open the shared memory segment */
	// POSIX API'in file islemleri ile ayni sekilde dusunebilirsin.
    // shared memory'de de ozel bir file kullaniliyor. Ama mantik file ile ayni.
    // ilk once shared memory icin bir file olusturuyoruz. gerekli flag'leri belirtiyoruz.
	shm_fd = shm_open(name, O_RDONLY, 0666);
	if (shm_fd == -1) {
		printf("shared memory failed\n");
		exit(-1);
	}

	// CONSUMER'da truncate yok farkedebilecegin gibi. Cunku okuma yapacagiz. Icindekiler bize lazim.

	/* now map the shared memory segment in the address space of the process */
	// olusturdugumuz file'a yazmak ve okumak icin bir isaretci olusturuyoruz.
    // MAP_SHARED flag'i, file'in degisiklikleri butun process'lere gorunur olacagini belirtiyor. 
	ptr = mmap(0,SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
	if (ptr == MAP_FAILED) {
		printf("Map failed\n");
		exit(-1);
	}

	/* now read from the shared memory region */
	printf("%s",(char *)ptr);

	/* remove the shared memory segment */
	// file'i silme islemi. olusturdugumuz name'i belirterek siliyoruz.
	if (shm_unlink(name) == -1) {
		printf("Error removing %s\n",name);
		exit(-1);
	}

	return 0;
}
