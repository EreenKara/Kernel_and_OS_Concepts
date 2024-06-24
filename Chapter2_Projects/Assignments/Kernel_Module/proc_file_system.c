/*
proc, sözde file system olarak geciyor.
sadece kernel memory'de bulunuyor ve kernel ve process basina istatistikleri sorgulamada kullaniliyor.

Kernel memory yani buffer'i ile kullanici buffer'i ayri seyler.
sistem cagrialrini gercekleyen isletim ssitemi ornegin bir diksten okuma yapcagi zaman
bu okudugunu ilk once kernel memory'ye yazar ondan sonra user memory'ye gecirir.
Disk'e yazarkende yine ayni durum gecerlidir ve bezeri durumlarda da.

Asagidaki read fonksiyonunun'da yazilanlara da bak.

Bu dosya olusturma islemi bir kere gerceklestikten sonra ve read fonksiyonun calismasi her "cat /proc/hello" bizin ornegimizde "/proc/proc_file_system"
cagrildiginda gerceklesen
*/


/**
 * Farkli bir bakis acisiyla soyle dusun
 * bir tane file olsuturduk ve bu file'in sistem tarafindan nasil okunamsi gerektigini biz belirledik
 * Ayrica bu file icerisine normal sartlarda kernel ve process hakkinda bilgiler yazacagiz.
 *  
 */



// asagidaki 3 tanesi kernel'a module yükelmek ve printk() icin kullaniliyorlar.
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>


#include <linux/proc_fs.h>
#include <asm/uaccess.h>

#define BUFFER_SIZE 128

#define PROC_NAME "hello"
#define MESSAGE "Hello World\n"

/**
 * Function prototypes
 */
ssize_t proc_read(struct file *file, char *buf, size_t count, loff_t *pos);

static struct file_operations proc_ops = {
    .owner = THIS_MODULE,
    .read = proc_read, // ne zaman bu proc "hello" dosyasi okunursa cagiralacak fonksiyon.
};


/* This function is called when the module is loaded. */
int proc_init(void)
{

    // creates the /proc/hello entry
    // the following function call is a wrapper for
    // proc_create_data() passing NULL as the last argument
    // 2. arguman "0" olan yaratılan dosyanin modu yani izinleri felan.
    proc_create(PROC_NAME, 0, NULL, &proc_ops);

    printk(KERN_INFO "/proc/%s created\n", PROC_NAME);

	return 0;
}

/* This function is called when the module is removed. */
void proc_exit(void) {

    // removes the /proc/hello entry
    remove_proc_entry(PROC_NAME, NULL);

    printk( KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

/**
 * This function is called each time the /proc/hello is read.
 * 
 * This function is called repeatedly until it returns 0, so
 * there must be logic that ensures it ultimately(en sonunda) returns 0
 * once it has collected the data that is to go into the 
 * corresponding /proc file.
 *
 * params:
 *
 * file:
 * buradaki buffer'in olmasinin sebebi asagidaki sistem cagrilarini gercekleyen os'in
 * kernel buffer'a yazma islemi yapip bizim oraya ulasamayacak olmamiz. Bu yuzden bunu
 * user buffer'a gecirmeliyiz. Neden kernel memory cunku biz proc file islemi yapiyoruz.
 * 
 * AYRICA "char __user *usr_buf" tanimlamasi linux kernel tanimlamasinda bulunan bir macro sanirim
 * ilgili tanimlamanin user memory'de oldugunu belirtmeye yariyor.
 * buf: buffer in user space
 * count:
 * pos:
 */
ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos)
{
    int rv = 0;
    char buffer[BUFFER_SIZE];
    static int completed = 0;

    if (completed) {
        completed = 0;
        return 0;
    }

    completed = 1;

    // geri donus degeri kac tane karakter basildigidir. Sondaki NULL karakteri haric.
    rv = sprintf(buffer, "Hello World\n");

    // copies the contents of buffer to userspace usr_buf
    // kernel memory'den user memory'ye gecis.
    copy_to_user(usr_buf, buffer, rv);

    return rv;
}


/* Macros for registering module entry and exit points. */
module_init( proc_init );
module_exit( proc_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hello Module");
MODULE_AUTHOR("SGG");

