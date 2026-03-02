# 實驗五：物理世界的中斷對接：GPIO 硬體觸發與核心 IRQ 處理

## 實驗目的
* **實踐硬體與核心的「握手」**：透過樹莓派實體 GPIO 針腳，接收外部按鈕的電壓訊號（Falling Edge），將機械動作轉化為 Linux 核心的中斷事件（Interrupt Event）。

* **破解動態編號迷思**：面對 Linux Kernel 6.12+ 的重大變更，學習如何從 /sys/kernel/debug/gpio 調查動態偏移量（Offset），精準定位物理針腳對應的核心編號（如 GPIO 17 映射至 529）。

* **體察物理訊號的不確定性**：觀察機械按鈕產生的彈跳（Debouncing）現象與核心處理中斷的延遲，理解為什麼複雜系統需要「中斷下半部（Bottom Half）」處理機制。

## 成果展示
待補上圖片

實體電路佈署：使用麵包板、杜邦線連結樹莓派 Pin 9 (GND) 與 Pin 11 (GPIO 17)，完成拉低電位（Active Low）的電路設計。

核心日誌驗證：透過 dmesg -w 成功補捉到按鈕觸發訊息，證明中斷處理程式（ISR）已正確掛載並執行。

中斷計數觀察：讀取 /proc/interrupts，確認 my_gpio_handler 的計數器隨物理按壓而增加。

## 挑戰與心得
* 動態 GPIO 編號的震撼教育 (EPROBE_DEFER)：在 Kernel 6.12 環境下，傳統的編號方式導致 Error 517 發生。透過追蹤核心 Debug 資訊，理解到 BCM2835 控制器基數已偏移至 512。這次經驗讓我體認到「核心版本演進」對驅動程式移植性的影響。

* 物理與數位間的「碎裂」 (Debouncing Issue)：實驗中發現物理按壓一次，日誌卻不一定只跳出一行，或連按多次卻因核心正在處理而遺失訊號。這讓我深刻體悟到物理訊號並非理想的方波，在核心層級進行「防彈跳」處理是維持系統穩定性的關鍵。

* 中斷上下文的極限約束 (ISR Constraint)：撰寫 request_irq 的回呼函數時，必須隨時警惕「不可睡眠」的原則。雖然目前僅使用 printk，但這讓我理解到，要在極短時間內回應硬體需求，代碼的效率與原子性（Atomicity）是首要考量。

* 硬體 Debug 的層級化思考：從檢查 Pin 9 的接地（Grounding）是否正確，到驗證按鈕在麵包板上的導通方向。這次實驗讓我補足了從「虛擬程式碼」到「實體電子流」之間的觀念。

## 實作步驟
1. 電路建置：在斷電狀態下，利用母對公杜邦線將 Pin 9 (GND) 接至麵包板負極，Pin 11 (GPIO 17) 接至按鈕端點。
2. 定位編號：查詢 /sys/kernel/debug/gpio 確定 GPIO 17 的動態編號為 529，並修正核心程式碼的 gpio_button 變數。
3. 編寫與註冊：使用 gpio_request 申請資源，並透過 request_irq 綁定 IRQF_TRIGGER_FALLING 觸發條件。
4. 掛載與觀察：sudo insmod 載入模組，並即時監控 /proc/interrupts 確認 IRQ 號碼已成功配發給 my_gpio_handler。
5. 壓力測試：透過物理連擊按鈕測試 ISR 的回應能力，並藉由 dmesg 觀察日誌噴出的頻率。

## 開發環境
* 硬體設備：Raspberry Pi Zero 2 WH、400孔麵包板、20cm 杜邦線、6x6mm 微動開關。
* 核心版本：Linux Kernel 6.12.47+rpt-rpi-v8 (ARM64)。
* 量測工具：/proc/interrupts、/sys/kernel/debug/gpio。

## 補充說明
* Falling Edge (下降緣)：指訊號由高電位（3.3V）轉向低電位（0V）的瞬間。
* ISR (Interrupt Service Routine)：核心響應硬體中斷的專門函數，執行優先權極高。
* Floating (浮接)：若無接地或上拉，腳位會因雜訊產生隨機訊號，導致中斷誤觸。
