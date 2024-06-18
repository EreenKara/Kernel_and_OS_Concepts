#include <fcntl.h> // open() sistem çağırsı için flag'ların tanımlandıgı header
#include <stdio.h> // printf ve benzerlerinin tanımlandıgı header
#include <stdlib.h> // NULL malloc exit gibi fonksyionların tanımlandıgı header
#include <unistd.h> // read, write, close sistem cagrilarini barindiran header file. POSIX API, SADECE Linux'ta calisacak demek, Windows API değil.
// DOSYA YOLU
// argv[2] nereden   
// argv[3] nereye 

// Asagidaki butun ehr sey fprintf gibi bu isi biraz daha basitlestirmek icin bulunan bir API kullanılarak'ta yapılabilirdi. 
// Ancak POSIX API kullanılarak yapılması istendi.

int main(int argc, char** argv){

	int file_descriptor_read;
	int file_descriptor_write;
	int read_from_byte;
	int written_byte;
	// Sonuna koyacagimiz null karakterinden dolayi 101 byte
	// sadece bir dosyadan digerine yazarken sonuna NULL karakteri koymaya gerkek yok.
	// Ancak prinf() gibi bir fonksyionda kullanacakken bu fonksiyon NULL karakterine kadar ekrana basma yaptığından sonuna NULL karakteri konulmalı.
	int buff_size=101;
	char* buff=(char*)malloc(sizeof(char)*buff_size);
	



	if(argc<3){
		fprintf(stderr,"Dosya yolunu girmediniz."); // Dosya yolu girilmediginde fprintf ile hatayi gostermemiz gerek.
		// perror sistem bir error urettiginde yani sistemde onceden tanimli
		// bir hata uretildigi takdirde hata'nin ne oldugunu ogrenmek icin kullanıyoruz.
		// Bu yuzden burda perror kullanmamiz gerekir.
		// perror("Dosyayi girmediniz")
		exit(1);
	}
	// open(okunacak_yol,flags, mode ) -> Mode parametresini egerki yeni bir dosya olusturuyorsan kullan
	// Flag'lar file'ı nasil acacagini tanimlamana yariyor 
	// File'ın zaten exist olmasını beklediğimizden ve sadece okumak için açtığımdan O_RDONLY yeter
	file_descriptor_read=open(argv[1],(O_RDONLY)); // EGER hata varsa 0'dan kucuk bir deger donduruyor.
	if(file_descriptor_read<0){
		// UNIX sistemi eğer bir hata varsa onu "errno" değişkenine kaydediyor. Integer value olarak.		
		// perror #include <errno.h> içerisinde bulunan errno global değişkeninin içerisindeki hata numarasına göre hatanın ne olduğunu basıyor.YAZI OLARAK.
		perror("Dosya read_only olarak acilmaya calisirken karsilasilan hata");
		exit(1);
	}


	// Dosya yoksa olustur(O_CREAT), varsa icini bosalt(O_TRUNC)  ve sadece yazma modunda aç. 
	file_descriptor_write = open(argv[2],(O_WRONLY|O_CREAT|O_TRUNC),0644); // Burada bir mode tanımladık cunku dosya yoksa olusturulacak.
	if(file_descriptor_write<0){
		// UNIX sistemi eğer bir hata varsa onu "errno" değişkenine kaydediyor. Integer value olarak.		
		// perror #include <errno.h> içerisinde bulunan errno global değişkeninin içerisindeki hata numarasına göre hatanın ne olduğunu basıyor.YAZI OLARAK.
		perror("Dosya acilmaya calisirken hata meydana geldi");
		exit(1);
	}

	// OKU VE YAZ
	
	while((read_from_byte=read(file_descriptor_read,buff,buff_size))>0)
	{
		// istedigimiz miktarin altinda okuma yaptigi takdirde sikinti cikmasin diye
		// buff_size'i degil de read_from_byte'i verdim parametre olarak
		written_byte=write(file_descriptor_write,buff,read_from_byte); // geriye kaç byte yazildigini donduruyor.
	}
	
	

	// OKUMA BITTI 
	if(close(file_descriptor_read)<0) { perror("Dosya_read kapatilirken bir hata meydana geldi"); exit(1);}
	// YAZMA BITTI
	if(close(file_descriptor_write)<0) { perror("Dosya_write kapatilirken bir hata meydana geldi"); exit(1);}
	free(buff);
	
	
	return 0;
}


/*
Eğerki bu programı çalıştırdığında hangi sistem çağrılarını yaptigini gozlemlemek istiyorsan 
"strace ./program" komutunu calistirarak gozlemleyebilirsin.
ps ve top komutları ile ise anlık oalrak bellek ve islemci kullanımlarını process bazından counter based olarak gozlemleyebilirsin.
strace bunları geçmişe dönük tracing yaparka tutuyor. Current değil.
Ayrıca strace sistem cagrilarinin listesini tutuyor.  
*/