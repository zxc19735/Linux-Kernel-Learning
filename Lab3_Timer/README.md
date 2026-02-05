# 實驗三：核心定時器 (Kernel Timer) 與非同步機制

## 實驗目的
待補上

## 成果展示
放一張成果圖
成功實作非同步任務。即使沒有使用者輸入，核心也能根據時間觸發特定行為。

## 挑戰與心得

## 實作步驟
1. 建立並進入資料夾：`mkdir Lab3_Timer && cd Lab3_Timer` 
2. 撰寫程式 (程式碼可參考 my_timer.c)：`nano my_chardev.c`
3. 撰寫編譯腳本 (可參考 Makefile)：`nano Makefile`
4. 編譯程式：`make`
5. 啟動核心：`sudo insmod my_timer.ko`
6. 查詢系統訊息，每每五秒會看到一次鬧鐘訊息響起：`dmesg -w`
7. 觀察 jiffies：印出的 jiffies 數值是一個隨時間遞增的計數器，代表系統開機以來的時鐘滴答數。

## 流程
撰寫 my_timer.c，利用 timer_setup 初始化鬧鐘。

使用 mod_timer 設定鬧鐘在 5 秒後觸發，並在回呼函數 (Callback) 中重新設定，達成週期性任務。

透過 dmesg -w 觀察核心如何在不阻塞系統的情況下，自動噴出訊息。


## 開發環境
* VM：Oracle VirualBox VM
* 作業系統：Ubuntu-24.03.3 LTS
* RAM：4GB
* 硬碟容量：20GB
* CPU：4核心

## 補充說明
* jiffies：Linux 核心的時間單位。透過 jiffies + msecs_to_jiffies(5000)，可以精準計算出「未來 5 秒」對應的核心時間
* Interrupt Context (中斷上下文)：定時器觸發時，程式碼運行在中斷上下文，一個高權限但受限的環境，不能在裡面執行 sleep，必須快進快出
