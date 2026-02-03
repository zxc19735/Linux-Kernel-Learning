## 實驗三：核心定時器 (Kernel Timer) 與非同步機制
流程：
撰寫 my_timer.c，利用 timer_setup 初始化鬧鐘。

使用 mod_timer 設定鬧鐘在 5 秒後觸發，並在回呼函數 (Callback) 中重新設定，達成週期性任務。

透過 dmesg -w 觀察核心如何在不阻塞系統的情況下，自動噴出訊息。

步驟：
建立環境：
mkdir Lab3_Timer && cd Lab3_Timer (建立並進入資料夾)

編譯與測試：
make 編譯。

sudo insmod my_timer.ko (這時你會看到第一行啟動訊息)。

dmesg -w (進入觀察模式，每五秒會看到一次鬧鐘響起)。

觀察 jiffies：
注意印出的 jiffies 數值。這是一個隨時間遞增的計數器，代表系統開機以來的時鐘滴答數。

結果：
成功實作非同步任務。即使沒有使用者輸入，核心也能根據時間觸發特定行為。

補充說明：
jiffies：Linux 核心的時間單位。透過 jiffies + msecs_to_jiffies(5000)，我們能精準計算出「未來 5 秒」對應的核心時間。

Interrupt Context (中斷上下文)：定時器觸發時，程式碼運行在中斷上下文。這是一個高權限但受限的環境，不能在裡面執行 sleep，必須快進快出。
