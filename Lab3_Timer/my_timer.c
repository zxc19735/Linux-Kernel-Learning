#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/timer.h> // 核心定時器標頭檔

static struct timer_list my_timer;

// 鬧鐘響了以後要執行的動作
static void my_timer_callback(struct timer_list *t) {
    printk(KERN_INFO "Alarm: 鬧鐘響了！核心正在執行非同步任務\n");
    
    // 如果想計時器一直響，則在結尾重新設定下一次時間
    mod_timer(&my_timer, jiffies + msecs_to_jiffies(5000));
}

static int __init timer_init(void) {
    printk(KERN_INFO "Timer Module: 鬧鐘已設定，5秒後響起\n");
    
    // 1. 初始化定時器
    timer_setup(&my_timer, my_timer_callback, 0);
    
    // 2. 設定過期時間 (目前的 tick 數 + 5000 毫秒轉換成的 tick 數)
    mod_timer(&my_timer, jiffies + msecs_to_jiffies(5000));
    
    return 0;
}

static void __exit timer_exit(void) {
    // 3. 務必程式執行後要刪除定時器，否則 rmmod 後鬧鐘響了會導致核心崩潰產生panic
    del_timer(&my_timer);
    printk(KERN_INFO "Timer Module: 鬧鐘已移除\n");
}

module_init(timer_init);
module_exit(timer_exit);
MODULE_LICENSE("GPL");
