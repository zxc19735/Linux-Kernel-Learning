# 實驗二：字元驅動程式 (Character Device Driver) 與 System Call 實作
  
## 實驗目的：
* 深究 VFS 抽象機制：理解 Linux 如何透過虛擬檔案系統 (VFS) 將複雜的硬體操作抽象化為統一的檔案介面，實踐「Everything is a file」的核心哲學。
* 掌握 kernel space 與 user space 的通訊：實作 copy_to_user 與 copy_from_user 函數，學習如何在保護模式下安全地跨越記憶體邊界（Memory Boundary）進行數據交換。
* 實作字元驅動程式 (Character Device)：學習主設備號 (Major numbers) 的分配與檔案操作 (file_operations) 的對接。

## 結果展示：
放一張成果圖
成功建立 User Space 與 Kernel Space 的溝通管道，實現資料的跨空間的讀取與寫入。

## 挑戰與心得
#### 核心與技術體悟
* VFS 抽象層的實戰應用：透過字元驅動的開發，深刻理解 VFS 屏除硬體差異的強大。從 User-space 來看，操作驅動程式與操作普通文字檔無異，這種架構設計大幅降低了應用層開發的複雜度。

* 系統呼叫 (System Call) 與軟體中斷的對接：本實驗實作了 dev_read 與 dev_write 等後端函數，這正是作業系統處理「軟體中斷」的具體流程。當 User-space 呼叫 read() 時，會觸發 Trap 進入 Kernel-mode，看著課本上的理論在自己寫的核心模組中運作，是非常紮實的成就感。

#### 工程素養：Git 版本控管與流程優化
* 衝突處理 (Conflict Resolution)：在環境遷移過程中，遭遇了地端與雲端 README 同時變動導致的 Git 衝突（Rejected error）。這讓我學會如何利用 rebase 與 backup-merge 策略恢復開發進度，而非盲目刪除檔案。

* 開發工作流 (Workflow) 的確立：這次經驗讓我確立了「工廠與展覽館」的開發模型——將 VM 視為開發工廠（Factory），GitHub 視為最終成品的展示區（Exhibition）。遵守「單向變更、先 Pull 後 Push」的紀律，是維持多人協作與異地開發穩定性的基石。

## 實作步驟：
1. 建立並進入實驗二資料夾：`mkdir kernel_lab2 && cd kernel_lab2` 。
2. 撰寫 my_chardev.c，實作 read, write 與核心緩衝區管理：`nano my_chardev.c`
3. 撰寫 Makefile：`nano Makefile`。
4. 編譯：`make` 產生 my_chardev.ko。
5. 載入核心： `sudo insmod my_chardev.ko`。
6. 查詢 dmesg 獲取系統分配的 主設備號：`dmesg | tail` (假設看到的號碼為 236)。
7. 在 /dev 目錄下建立硬體節點，手動將驅動程式與檔案系統連結：`sudo mknod /dev/my_os_lab c 236 0`。
8. 調整權限為可讀寫：`sudo chmod 666 /dev/my_os_lab`。
9. 透過 `echo` 與 `head` 模擬應用程式 (User Space) ，對硬體進行寫入與讀取操作：`echo "Hello_OS_Kernel" > /dev/my_os_lab` 、`head -n 1 /dev/my_os_lab` (若成功，可在螢幕看到剛才寫入的字串)。
11. 觀察核心行為：`sudo dmesg | tail` ，會看到 copy_from_user 與 copy_to_user 的處理紀錄。
12. 關閉程式並移除核心：`sudo rmmod my_chardev` 且 `sudo rm /dev/my_os_lab`。

## 開發環境：
* VM：Oracle VirualBox VM
* 作業系統：Ubuntu-24.03.3 LTS
* RAM：4GB
* 硬碟容量：20GB
* CPU：4核心

## 補充說明：
1. copy_to_user / copy_from_user：這是核心開發重要的 API。因為 User Space 與 Kernel Space 的記憶體是隔離的，核心必須使用專屬的 API 來確保資料交換的安全性。
2. Major Number (主設備號)：就像分機號碼，可讓 Linux 核心知道要把 /dev/ 下的請求轉交給哪一個驅動程式。
