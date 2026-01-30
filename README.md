# 實驗一：Kernel Mode "Hello Workd" 
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
`mkdir ~/kernel_lab && cd ~/kernel_lab`
#### 撰寫程式碼：
`nano hello.c` (貼上 hello.c 程式碼後，Ctrl+O 存檔，Ctrl+X 離開)。`nano Makefile` (貼上 Makefile 程式碼後，Ctrl+O 存檔，Ctrl+X 離開) (提醒：在 Makefile 中，若貼上時發現程式碼前有空格，刪掉後以 Tab 取代，避免編譯失敗)
#### 開始編譯：
`make` ，等待編譯過程，若成功編譯完成，輸入`ls`後會發現在資料夾裡頭出現 `hello.ko`
#### 載入核心並查看訊息：
`sudo insmod hello.ko` ，在系統日誌查看載入核心訊息 「Hello! 這是我在 Kernel 的第一步」：`sudo dmesg | tail`
#### 移除核心並查看訊息：
`sudo rmmod hello`，在系統日誌檢查移除核心訊息：「Hello! 這是我在 Kernel 的第一步」：`sudo dmesg | tail`

## 補充說明：
1. `insmod` (insert module)：hello.c 程式碼正式成為作業系統的一部份，並執行'hello_init' 。Kernel會把 .ko 檔案載入記憶體並分配權限，此時程式碼擁有高權限
2. `rmmod` (remove module)：執行 `hello_exit` 並把記憶體還給系統
3. 
