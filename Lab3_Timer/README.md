# 實驗三：核心定時器 (Kernel Timer) 與非同步機制

## 實驗目的
* 撰寫 my_timer.c，利用 timer_setup 初始化鬧鐘
* 使用 mod_timer 設定鬧鐘在 5 秒後觸發，並在回呼函數 (Callback) 中重新設定，達成週期性任務
* 透過 dmesg -w 觀察核心如何在不阻塞系統的情況下，自動顯示訊息，並了解非同步機制運作

## 成果展示
放一張成果圖
成功實作非同步任務。即使沒有使用者輸入，核心也能根據時間觸發特定行為

## 挑戰與心得
* 震撼實驗：見證內部中斷 (Internel Interrupt) 與 Kernel Panic： 在實驗過程中，我主動測試了「不刪除定時器移除模組」的情況。當核心試圖跳轉至已釋放的記憶體位址執行 Callback 時，立即觸發了內部異常導致系統崩潰。這次經驗讓我深刻理解到 del_timer 不僅是釋放資源，更是維護系統穩定性的關鍵防線。

* 非同步預約機制 (Asynchronous Scheduling)： 這是我第一次體會到核心如何處理「預約任務」。不同於 User-space 使用 sleep() 會阻塞進程，核心定時器像是在硬體層級掛了個鬧鐘，時間到後由 外部中斷 (Hardware Interrupt) 觸發，這種「不佔用 CPU 等待」的非同步特性，是高效率系統的核心。

* 中斷上下文 (Interrupt Context) 的嚴格限制： 理解到 Callback 函數執行在中斷狀態下，不能休眠、不能呼叫耗時函數。這種「快進快出」的約束，讓我對作業系統的任務排程有了更立體的認識。

## 實作步驟
1. 建立並進入資料夾：`mkdir Lab3_Timer && cd Lab3_Timer` 
2. 撰寫程式 (程式碼可參考 my_timer.c)：`nano my_timer.c`
3. 撰寫編譯腳本 (可參考 Makefile)：`nano Makefile`
4. 編譯程式：`make`
5. 啟動核心：`sudo insmod my_timer.ko`
6. 查詢系統訊息，每每五秒會看到一次鬧鐘訊息響起：`dmesg -w`
7. 觀察 jiffies：印出的 jiffies 數值是一個隨時間遞增的計數器，代表系統開機以來的時鐘滴答數

## 開發環境
* VM：Oracle VirualBox VM
* 作業系統：Ubuntu-24.03.3 LTS
* RAM：4GB
* 硬碟容量：20GB
* CPU：4核心

## 補充說明
* jiffies：Linux 核心的時間單位。透過 jiffies + msecs_to_jiffies(5000)，可以精準計算出「未來 5 秒」對應的核心時間
* Interrupt Context (中斷上下文)：定時器觸發時，程式碼運行在中斷上下文，一個高權限但受限的環境，不能在裡面執行 sleep，必須快進快出
