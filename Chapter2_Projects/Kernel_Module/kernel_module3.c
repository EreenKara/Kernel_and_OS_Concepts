
#include <linux/init.h> // printk() icin gereken header bu ancak digerleri ne yapiyor bilemiyorum.
#include <linux/kernel.h>
#include <linux/module.h> 

// bir process'in Round robin gibi calisrken kac saniye calisagini belirten HZ, time tick
#include <asm/param.h> // HZ yani 1 saniyede kac kez interrupt oluyor onu soyluyor bizlere.

#include <linux/jiffies.h> // isletim sisteminin boot oldugundan beri kac kez timer interrupt gerceklesti 
// onu track eden degisken olan jiffies degiskenini barindiran header.

unsigned long time_passed_away,first_interrupt_number,interval_interrupt_number;

// Module eklendiginde calistirmak istedigimiz fonksiyon.
int simple_init(void){
    // Kernel'ın buffer'ına yazmamızı saglayan bir sistem cagirisi. Normalde printf()'in varsayilan out stream'i 
    // ekran iken printk()'in varsayilan cikti verdigi yer kernel bufferidir.
    // Bu buffer'i gozlemleyebilmek icin "dmesg" komutunu kullanmak gerekiyor CLI uzerinde. 
    printk(KERN_INFO "Loading Kernel Module");
    printk(KERN_INFO "%d",HZ);
    printk(KERN_INFO "jiffies :%lu",jiffies);
    first_interrupt_number=jiffies;
    
    return 0;
}
// Module kaldirildiginda calistirmak istedigimiz fonksiyon.
void simple_exit(void)
{
    printk(KERN_INFO "Removing Kernel Module");
    printk(KERN_INFO "jiffies :%lu",jiffies);
    interval_interrupt_number=jiffies - first_interrupt_number;
    time_passed_away = interval_interrupt_number/HZ;
    printk(KERN_INFO "time_passed_since_load: %lu",time_passed_away);
}


// yukaridaki fonksiyonlar istedigimiz yerlerde calissin diye kullandigimiz macrolar.
module_init(simple_init);
module_exit(simple_exit);


MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SSG");
