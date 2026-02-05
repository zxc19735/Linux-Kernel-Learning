# 實驗一：Kernel Mode "Hello Workd" 
## 實驗目的
* 探索 LKM (可載入核心模組) 機制：理解 Linux 如何在不重新編譯核心、不重啟系統的情況下，動態擴展系統功能。

* 跨越 User Space 與 Kernel Space 的界線：實作在特權模式 (Ring 0) 下執行的程式碼，並理解其與一般應用程式在資源存取權限上的差異。

* 核心日誌與調試初探：掌握 printk 函數與 /var/log/syslog 的互動，建立核心層級的調試 (Debug) 基礎。

## 成果展示
將程式碼成功放進 kernel，並使用 printk 顯示字串
放一張展示圖

## 挑戰與心得
1. 在 Makefile 中需留意 Tab 與空白鍵的縮排問題，以 Tab 取代空格，避免編譯失敗。
2. 了解「動態載入」觀念，作業系統不需要重啟就能更新驅動程式的核心機制。
3. 「Ring 0 的權力與風險」：在核心層級，一個指標錯誤就可能導致整個系統當機（Kernel Panic），這與在 User-space 有 OS 保護的環境完全不同。著實體會到核心開發中「嚴謹度」的重要性。
4. 「事件驅動邏輯」：核心模組沒有 main()，而是透過 __init 和 __exit 註冊事件。這讓筆者對「被動觸發」的程式架構有了新的認識。

## 實作步驟
1. 建立並進入實驗一資料夾(以下程式碼都在同個資料夾下執行)：`mkdir kernel_lab1 && cd kernel_lab1` 
2. 撰寫程式碼：`nano hello.c` (貼上 hello.c 程式碼後，Ctrl+O 存檔，Ctrl+X 離開)
3. 撰寫編譯腳本：`nano Makefile` (貼上 Makefile 程式碼後，Ctrl+O 存檔，Ctrl+X 離開)
4. 開始編譯：`make` ，等待編譯過程，若成功編譯完成，輸入`ls`後會發現在資料夾裡頭出現 kernel object (hello.ko)
5. 載入核心：`sudo insmod hello.ko`
6. 在系統日誌查看載入核心訊息「Hello! 這是我在 Kernel 的第一步」：`sudo dmesg | tail`
7. 移除核心：`sudo rmmod hello`
8. 在系統日誌檢查移除核心訊息「Goodbye! 結束 Kernel 實驗」：`sudo dmesg | tail`

## 開發環境：Oracle VirualBox VM
* 作業系統：Ubuntu-24.03.3 LTS
* RAM：4GB
* 硬碟容量：20GB
* CPU：4核心
* 若尚未安裝編譯套件：可在 Ubuntu 開啟終端機，輸入 `sudo apt update`
`sudo apt install build-essential kmod linux-headers-$(uname -r)`

## 補充說明：
1. `insmod` (insert module)：hello.c 程式碼正式成為作業系統的一部份，並執行'hello_init' 。Kernel會把 .ko 檔案載入記憶體並分配權限，此時程式碼擁有高權限。
2. `rmmod` (remove module)：執行 `hello_exit` 並把記憶體還給系統。
3. makefile：做為一個傳聲筒，請系統編譯 hello.o，並將編譯的結果回傳給目前的資料夾。
## 補充說明 2：
因筆者主機的 OS 為 Windows，也可使用 SSH 連線，進行遠端操作 VM 裡的 terminal，SSH 連線步驟如下：
1. 在 Ubuntu 裡安裝 SSH 伺服器 開啟終端機輸入： `sudo apt update && sudo apt install openssh-server -y`
2. 在 VirtualBox 設定「通訊埠轉發」 (Port Forwarding) 因為 VM 躲在虛擬網卡後面，故通知 Windows：「當我連到 host 的 2222 port 時，轉給 VM 的 22 埠」
在 VirtualBox 選單：設定 -> 網路 -> 介面卡 1，確認是 NAT 模式
點開 進階 -> 通訊埠轉發
新增一條規則：名稱 SSH，通訊協定 TCP，主機 IP 127.0.0.1，主機連接埠 2222，客用 IP 留白，客用連接埠 22。
3. 在 Windows 連線 打開你的 Windows PowerShell 或 CMD，輸入： ssh -p 2222 你的Ubuntu帳號@127.0.0.1 (第一次連線時會詢問是否信任，輸入 yes，然後輸入 Ubuntu 密碼)。
