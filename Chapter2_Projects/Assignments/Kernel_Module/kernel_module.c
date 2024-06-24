/*
Direkt olarak kernel'a kod yazacağız. Herhangi bir hata sistemin crash vermesine sebep olacaktır.
Ancak direkt oalrak kernel ile haberleşmemizi sağlayan direkt oalrak kernel'a metod yazmak sistemi hızlandıracaktır.
Bunun bir benzer çalışma mantığını CLI'a komut, module yazarken de yapabilirsin.
CLI'da bulunan herhangi bir komut ornedin "cd directory", "ls -l" veya "touch ./newFile.txt" gibi komutlar
arka planda module mantigi ile olusturulmus birer programdir ve CLI uzerinde bunların her bir cagrilmasinda
CLI gider buprogrmalari bulur ve ona geçilmiş parametrelerle bu programı execute eder.
Boylece bizler CLI'ı tamamen yeniden derleyip deployment etmeye gerek kalmadan komut yazmış oluyoruz.
Çok uzunv e sık tekrar edecek olan ardışık işlemleri böylelikle otomatikleştirebiliyoruz.
Elbetteki CLI'ın da bunu desteklemesi gerekiyor yani moduler yapıyı destekleyecek
şekilde inşaa edilmiiş bir CLI üzerinde bunu yapabiliriz. 
*/

/*
Hazırda sistemde bulunan modülleri görüntülemek için "lsmod"
*/

/*
Bu dosyayı derlemesi için ktiabin verdigi makefile'i kullandim.
Makefile belirli bir dosyadaki Makefile'ı kullanarak bizim dosyamızı derliyor.
Ondan sonra geri dosyanın ciktilarını bulundugumuz dosyaya koymak icin 
M=$(PWD) degiskenine derlenen dosyasinin directorysini atıyor. 
Makefile'ı calistirmak istedigim dosya ile aynı isme sahip.o uzantılı dosyayı belirterek make edebiliyorum ve olusan modul'u
"sudo insmod dosya_ismi.ko" diyerek ekleyip
"sudo rmmod dosya_ismi" diyerek kalldırıyorum.
*/


/*
Asagidaki yazdigimiz kod sunu yapiyormus ekledim yukledim, hem bunu hem kitap tarafindan verilen kodu derleyip module olarak yukledim.
Asagidaki kod sadece kendi eklendiginde calisiyor, herhangi bir baska module eklendiginde tetiklenmiyor.
Bu kodu module olarka yukledigimizde ve remove ederek kaldirdigimizde asagidaki atadigmiz fonksiyonlar 1 kere calisiyor. Bu kadar.
*/


#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>


// Module eklendiginde calistirmak istedigimiz fonksiyon.
int simple_init(void){
    // Kernel'ın buffer'ına yazmamızı saglayan bir sistem cagirisi. Normalde printf()'in varsayilan out stream'i 
    // ekran iken printk()'in varsayilan cikti verdigi yer kernel bufferidir.
    // Bu buffer'i gozlemleyebilmek icin "dmesg" komutunu kullanmak gerekiyor CLI uzerinde. 
    printk(KERN_INFO "Loading Kernel Module\n");
    return 0;
}
// Module kaldirildiginda calistirmak istedigimiz fonksiyon.
void simple_exit(void)
{
    printk(KERN_INFO "Removing Kernel Module\n");
}

// yukaridaki fonksiyonlar istedigimiz yerlerde calissin diye kullandigimiz macrolar.
module_init(simple_init);
module_exit(simple_exit);


MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SSG");
