#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

// 當執行 insmod (載入模組) 時，會進入這段程式碼
static int __init hello_init(void) {
	printk(KERN_INFO "Hello! 這是我在 Kernel 的第一步\n");
	return 0;
}

// 當你執行 rmmod (移除模組) 時，會進入這段程式碼
static void __exit hello_exit(void) {
	printk(KERN_INFO "Goodbye! 結束 Kernel 實驗\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple Hello World Kernel Module");
