/**
 * Simple program demonstrating shared memory in POSIX systems.
 *
 * This is the producer process that writes to the shared memory region.
 *
 * Figure 3.16
 * 
 * To compile, enter
 * 	gcc shm-posix-producer.c -lrt
 *
 * @author Silberschatz, Galvin, and Gagne
 * Operating System Concepts  - Tenth Edition
 * Copyright John Wiley & Sons - 2018
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // POSIX API file islemleri
#include <string.h>
#include <fcntl.h> // open() flaglarini iceriyor

#include <sys/shm.h>
#include <sys/stat.h> // mode constants 
#include <sys/mman.h> // asil shm_open flena burda bulunuyor.
#include <sys/types.h>

int main()
{
	const int SIZE = 4096;
	const char *name = "OS";
	const char *message0= "Studying ";
	const char *message1= "Operating Systems ";
	const char *message2= "Is Fun!";

	int shm_fd;
	void *ptr;

	/* create the shared memory segment */
    // POSIX API'in file islemleri ile ayni sekilde dusunebilirsin.
    // shared memory'de de ozel bir file kullaniliyor. Ama mantik file ile ayni.
    // ilk once shared memory icin bir file olusturuyoruz. gerekli flag'leri belirtiyoruz.
	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

	/* configure the size of the shared memory segment */
    // file'in icini bosaltip reshape ediyoruz.
	ftruncate(shm_fd,SIZE);

	/* now map the shared memory segment in the address space of the process */
    // olusturdugumuz file'a yazmak ve okumak icin bir isaretci olusturuyoruz.
    // MAP_SHARED flag'i, file'in degisiklikleri butun process'lere gorunur olacagini belirtiyor. 
	ptr = mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (ptr == MAP_FAILED) {
		printf("Map failed\n");
		return -1;
	}

	/**
	 * Now write to the shared memory region.
 	 *
	 * Note we must increment the value of ptr after each write.
	 */
    // sprintf() fonksiyonu 1. parametreye 2.parametreyi yaz. demek oluyor.
    // ptr'yi mesajin boyutuyla toplayarak pointer aritmatigi yapiyoruz.
	sprintf(ptr,"%s",message0);
	ptr += strlen(message0);
	sprintf(ptr,"%s",message1);
	ptr += strlen(message1);
	sprintf(ptr,"%s",message2);
	ptr += strlen(message2);

	return 0;
}
