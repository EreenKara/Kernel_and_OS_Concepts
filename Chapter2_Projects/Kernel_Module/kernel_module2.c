/*
Aynı assignment gibi gorunen ancak farkli bir sey yapamizi isteyen diger assignment
GOLDEN_RATIO_PRIME eklentisi.
*/

/*
Asagidaki yazdigimiz kod sunu yapiyormus ekledim yukledim, hem bunu hem kitap tarafindan verilen kodu derleyip module olarak yukledim.
Asagidaki kod sadece kendi eklendiginde calisiyor, herhangi bir baska module eklendiginde tetiklenmiyor.
Bu kodu module olarka yukledigimizde ve remove ederek kaldirdigimizde asagidaki atadigmiz fonksiyonlar 1 kere calisiyor. Bu kadar.
*/


#include <linux/init.h> // printk() icin gereken header bu ancak digerleri ne yapiyor bilemiyorum.
#include <linux/kernel.h>
#include <linux/module.h> 
#include <linux/hash.h> // GOLDEN_RATIO_PRIME bu headerda
#include <linux/gcd.h> // greatest common divisor, assignment'ta kullanmamızın istendigi diger degisken 


// Module eklendiginde calistirmak istedigimiz fonksiyon.
int simple_init(void){
    // Kernel'ın buffer'ına yazmamızı saglayan bir sistem cagirisi. Normalde printf()'in varsayilan out stream'i 
    // ekran iken printk()'in varsayilan cikti verdigi yer kernel bufferidir.
    // Bu buffer'i gozlemleyebilmek icin "dmesg" komutunu kullanmak gerekiyor CLI uzerinde. 
    printk(KERN_INFO "Loading Kernel Module\n");
    printk(KERN_INFO "%lu",GOLDEN_RATIO_PRIME);

    return 0;
}
// Module kaldirildiginda calistirmak istedigimiz fonksiyon.
void simple_exit(void)
{
    printk(KERN_INFO "Removing Kernel Module\n");
    printk(KERN_INFO "%lu",gcd(3300,24));
}

// yukaridaki fonksiyonlar istedigimiz yerlerde calissin diye kullandigimiz macrolar.
module_init(simple_init);
module_exit(simple_exit);


MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SSG");
