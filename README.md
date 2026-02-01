# 實驗一：Kernel Mode "Hello Workd" 
## 實驗目的：
* 了解核心特權級別 (Ring 0)
## 環境：Oracle VirualBox VM
* 作業系統：Ubuntu-24.03.3 LTS
* RAM：4GB
* 硬碟容量：20GB
* CPU：4核心
* 安裝編譯套件：在 Ubuntu 開啟終端機，輸入 `sudo apt update`
`sudo apt install build-essential kmod linux-headers-$(uname -r)`
## 流程：
1. 在 Ubuntu 上撰寫一個 hello.c 和一個 Makefile 
2. 使用 `make` 編譯出一個 .ko (Kernel Object) 檔案
3. 執行 `sudo insmod hello.ko` (載入核心) 和 `sudo rmmod hello` (移除核心)
4. 使用 `dmesg` 觀察 output 是否顯示 Hello... 訊息，並了解「動態載入」觀念，及作業系統不需要重啟就能更新驅動程式的核心機制

## 步驟：
#### 建立並進入實驗一資料夾(以下程式碼都在同個資料夾下執行)：
`mkdir kernel_lab1 && cd kernel_lab1`
#### 撰寫程式碼：
`nano hello.c` (貼上 hello.c 程式碼後，Ctrl+O 存檔，Ctrl+X 離開)。`nano Makefile` (貼上 Makefile 程式碼後，Ctrl+O 存檔，Ctrl+X 離開)
#### 開始編譯：
`make` ，等待編譯過程，若成功編譯完成，輸入`ls`後會發現在資料夾裡頭出現 `hello.ko`
#### 載入核心並查看訊息：
`sudo insmod hello.ko` ，在系統日誌查看載入核心訊息 「Hello! 這是我在 Kernel 的第一步」：`sudo dmesg | tail`
#### 移除核心並查看訊息：
`sudo rmmod hello`，在系統日誌檢查移除核心訊息：「Hello! 這是我在 Kernel 的第一步」：`sudo dmesg | tail`

## 結果：
將程式碼成功放進 kernel，並使用 printk 顯示字串

## 補充說明：
1. `insmod` (insert module)：hello.c 程式碼正式成為作業系統的一部份，並執行'hello_init' 。Kernel會把 .ko 檔案載入記憶體並分配權限，此時程式碼擁有高權限
2. `rmmod` (remove module)：執行 `hello_exit` 並把記憶體還給系統
3. makefile` ：做為一個傳聲筒，請系統編譯 hello.o，並將編譯的結果回傳給目前的資料夾
## 遇到困難與解決方案：
在 Makefile 中需留意 Tab 與空白鍵的縮排問題，以 Tab 取代空格，避免編譯失敗
## 補充說明 2：
因筆者主機的 OS 為 Windows，也可使用 SSH 連線，進行遠端操作 VM 裡的 terminal，SSH 連線步驟如下：
1. 在 Ubuntu 裡安裝 SSH 伺服器 開啟終端機輸入： sudo apt update && sudo apt install openssh-server -y
2. 在 VirtualBox 設定「通訊埠轉發」 (Port Forwarding) 因為 VM 躲在虛擬網卡後面，故通知 Windows：「當我連到 host 的 2222 port 時，轉給 VM 的 22 埠」。
在 VirtualBox 選單：設定 -> 網路 -> 介面卡 1，確認是 NAT 模式。
點開 進階 -> 通訊埠轉發。
新增一條規則：名稱 SSH，通訊協定 TCP，主機 IP 127.0.0.1，主機連接埠 2222，客用 IP 留白，客用連接埠 22。
3. 在 Windows 連線 打開你的 Windows PowerShell 或 CMD，輸入： ssh -p 2222 你的Ubuntu帳號@127.0.0.1 (第一次連線時會詢問是否信任，輸入 yes，然後輸入 Ubuntu 密碼)
