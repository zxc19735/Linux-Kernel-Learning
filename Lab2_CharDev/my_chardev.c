#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>        // 為了使用 file_operations
#include <linux/uaccess.h>   // 為了使用 copy_to_user/copy_from_user

#define DEVICE_NAME "my_os_device"
#define BUF_SIZE 1024

static int major;             // 主裝置號
static char kernel_buffer[BUF_SIZE]; // 核心內部的快取

// 當 User 執行 read(fd, buf, len) 時觸發
static ssize_t dev_read(struct file *filep, char __user *buffer, size_t len, loff_t *offset) {
    int error_count = 0;
    
    // 如果 offset 已經超過緩衝區大小，代表讀完了
    if (*offset >= BUF_SIZE)
	return 0;
    // 確保不會讀取超過緩衝區的大小
    if (len > BUF_SIZE - *offset) {
        len = BUF_SIZE - *offset;
    }

    // 將核心資料拷貝給 User
    error_count = copy_to_user(buffer, kernel_buffer + *offset, len);

    if (error_count == 0) {
        printk(KERN_INFO "MyDevice: 送出 %zu 個字元\n", len);
        *offset += len; // 更新讀取位置
        return len;    // 回傳「實際讀取了多少 bytes」
    } else {
        return -EFAULT; // 發生位址錯誤
    }
}

// 當 User 執行 write(fd, buf, len) 時觸發
static ssize_t dev_write(struct file *filep, const char __user *buffer, size_t len, loff_t *offset) {
    // 將 User 的 buffer 內容搬進核心 kernel_buffer
    unsigned long ret = copy_from_user(kernel_buffer, buffer, len);
    
    if (ret == 0) {
        printk(KERN_INFO "MyDevice: 接收到來自使用者的資料\n");
    }
    return len;
}

// 定義這個裝置支援的操作
static struct file_operations fops = {
    .read = dev_read,
    .write = dev_write,
};

static int __init chardev_init(void) {
    // 向系統註冊一個字元裝置
    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0) {
        printk(KERN_ALERT "MyDevice 註冊失敗\n");
        return major;
    }
    printk(KERN_INFO "MyDevice 已註冊，主裝置號為 %d\n", major);
    return 0;
}

static void __exit chardev_exit(void) {
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "MyDevice 已卸載\n");
}

module_init(chardev_init);
module_exit(chardev_exit);

MODULE_LICENSE("GPL");
