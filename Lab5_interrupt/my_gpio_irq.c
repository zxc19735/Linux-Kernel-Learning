#include <linux/module.h>    // 所有核心模組必備
#include <linux/kernel.h>    // 包含 KERN_INFO 等日誌等級
#include <linux/gpio.h>      // 傳統 GPIO 操作介面
#include <linux/interrupt.h> // 中斷處理相關 API

// 定義我們要使用的 GPIO 編號 (BCM 17，對應實體 Pin 17，新版GPIO 529)
static unsigned int gpio_button = 529;
// 用來儲存從 GPIO 轉換過來的中斷號碼 (IRQ number)
static int irq_number;

/**
 * @brief 中斷處理函數 (ISR - Interrupt Service Routine)
 * 當按鈕按下，發生 Falling Edge (高電位變低電位) 時，核心會呼叫此函數
 */
static irqreturn_t lab5_gpio_irq_handler(int irq, void *dev_id) {
    // 為了效能與穩定，中斷處理內只做最簡單的 printk
    printk(KERN_INFO "Lab5: [Interrupt] 偵測到按鈕觸發！GPIO 17 狀態改變。\n");

    // 必須回傳 IRQ_HANDLED 告訴核心，我們已經成功處理這個中斷
    return IRQ_HANDLED;
}

/**
 * @brief 模組初始化函數
 */
static int __init lab5_init(void) {
    int result;

    printk(KERN_INFO "Lab5: 正在啟動 GPIO 中斷實驗模組...\n");

    // 1. 檢查 GPIO 是否有效
    if (!gpio_is_valid(gpio_button)) {
        printk(KERN_ERR "Lab5: 錯誤：GPIO %d 無效\n", gpio_button);
        return -ENODEV;
    }

    // 2. 申請使用該 GPIO 資源 (標籤設為 "rpi-gpio-interrupt")
    result = gpio_request(gpio_button, "rpi-gpio-interrupt");
    if (result < 0) {
        printk(KERN_ERR "Lab5: 無法申請 GPIO %d\n", gpio_button);
        return result;
    }

    // 3. 設定 GPIO 為輸入模式
    gpio_direction_input(gpio_button);

    // 4. 將 GPIO 編號轉換為系統的中斷號碼 (IRQ)
    irq_number = gpio_to_irq(gpio_button);
    printk(KERN_INFO "Lab5: GPIO %d 對應的系統中斷號碼為 %d\n", gpio_button, irq_number);

    // 5. 註冊中斷處理程式
    // 參數說明：
    // irq_number: 系統中斷號
    // handler: 剛剛定義的 lab5_gpio_irq_handler 函數
    // IRQF_TRIGGER_FALLING: 觸發條件為「高變低」(因為我們接 GND)
    // "my_gpio_handler": 顯示在 /proc/interrupts 的名稱
    // dev_id: 傳遞給 handler 的自定義資料，這裡給 NULL
    result = request_irq(irq_number,
                         (irq_handler_t) lab5_gpio_irq_handler,
                         IRQF_TRIGGER_FALLING,
                         "my_gpio_handler",
                         NULL);

    if (result < 0) {
        printk(KERN_ERR "Lab5: 無法註冊中斷處理程式 (錯誤代碼 %d)\n", result);
        gpio_free(gpio_button);
        return result;
    }

    printk(KERN_INFO "Lab5: 模組載入成功！請觀察按鈕觸發。\n");
    return 0;
}

/**
 * @brief 模組卸載函數
 */
static void __exit lab5_exit(void) {
    printk(KERN_INFO "Lab5: 正在清理並卸載模組...\n");

    // 1. 釋放中斷資源 (必須與註冊時成對出現)
    free_irq(irq_number, NULL);

    // 2. 釋放 GPIO 資源
    gpio_free(gpio_button);

    printk(KERN_INFO "Lab5: 模組已安全卸載。\n");
}

// *** 關鍵：這兩行絕對不能漏掉，否則函數定義了也不會被執行 ***
module_init(lab5_init);
module_exit(lab5_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Lab 5: Stable GPIO Interrupt Demo for Kernel 6.12");
