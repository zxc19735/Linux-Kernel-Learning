# 實驗二：字元驅動程式 (Character Device Driver) 與 System Call 實作

## 環境：Oracle VirualBox VM
* 作業系統：Ubuntu-24.03.3 LTS
* RAM：4GB
* 硬碟容量：20GB
* CPU：4核心
## 實驗目的：
理解 VFS (虛擬檔案系統) 如何將硬體抽象化為檔案。

## 流程：
1. 在 Ubuntu 上撰寫 my_chardev.c 與對應的 Makefile。
2. 使用 `make` 編譯並透過 insmod 載入核心模組。
3. 觀察 dmesg 獲取系統分配的 主裝置號 (Major Number)。
4. 在 /dev 目錄下建立硬體節點，手動將驅動程式與檔案系統連結。
5. 透過 `echo` 與 `head` 指令模擬應用程式 (User Space) ，對硬體進行讀寫操作。

## 步驟：
#### 建立並進入實驗二資料夾：
`mkdir kernel_lab2 && cd kernel_lab2`

#### 撰寫程式碼：
`nano my_chardev.c` (實作 read, write 與核心緩衝區管理)。`nano Makefile` (將編譯對象改為 my_chardev.o)。

#### 編譯與掛載：
執行 `make` 產生 my_chardev.ko。執行 `sudo insmod my_chardev.ko`。

#### 連結硬體節點：
查詢主裝置號：dmesg | tail (假設看到的號碼為 236)。

#### 建立裝置檔案：
`sudo mknod /dev/my_os_lab c 236 0`。

#### 調整權限：
`sudo chmod 666 /dev/my_os_lab`。

#### 測試數據傳輸：
寫入資料：`echo "Hello_OS_Kernel" > /dev/my_os_lab`。

#### 讀取資料：
`head -n 1 /dev/my_os_lab` (若成功，可在螢幕看到剛才寫入的字串)。

#### 觀察核心行為：
`sudo dmesg | tail` ，會看到 copy_from_user 與 copy_to_user 的處理紀錄。

清理環境：
`sudo rmmod my_chardev` 且 `sudo rm /dev/my_os_lab`。

#### 結果：
成功建立 User Space 與 Kernel Space 的溝通管道，實現資料的跨空間拷貝。

#### 遇到的困難與解決方案：

#### 補充說明：
copy_to_user / copy_from_user：這是核心開發最重要的 API。因為 User Space 與 Kernel Space 的記憶體是隔離的，核心必須使用專屬的搬運員來確保資料交換的安全性。

Major Number (主裝置號)：如同分機號碼，讓 Linux 核心知道要把 /dev/ 下的請求轉交給哪一個驅動程式。

Everything is a file：透過這個實驗，深刻體會到為什麼 Linux 把硬體操作抽象化為檔案操作，這大幅降低了應用程式開發者的門檻。
