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

## 流程：
1. 在 Ubuntu 上撰寫 my_chardev.c 與對應的 Makefile。
2. 使用 `make` 編譯並透過 insmod 載入核心模組。
3. 觀察 dmesg 獲取系統分配的 主裝置號 (Major Number)。
4. 在 /dev 目錄下建立硬體節點，手動將驅動程式與檔案系統連結。
5. 透過 `echo` 與 `head` 指令模擬應用程式 (User Space) ，對硬體進行讀寫操作。

## 步驟：
1. 建立並進入實驗二資料夾：`mkdir kernel_lab2 && cd kernel_lab2` 。
2. 撰寫 my_chardev.c，實作 read, write 與核心緩衝區管理：`nano my_chardev.c`
3. 撰寫 Makefile：`nano Makefile`。
4. 編譯：`make` 產生 my_chardev.ko。
5. 載入核心： `sudo insmod my_chardev.ko`。
6. 查詢主設備號：dmesg | tail (假設看到的號碼為 236)。
7. 建立裝置檔案：`sudo mknod /dev/my_os_lab c 236 0`。
8. 調整權限為可讀寫：`sudo chmod 666 /dev/my_os_lab`。
9. 測試數據傳輸並寫入資料：`echo "Hello_OS_Kernel" > /dev/my_os_lab`。
10. 讀取資料：`head -n 1 /dev/my_os_lab` (若成功，可在螢幕看到剛才寫入的字串)。
11. 觀察核心行為：`sudo dmesg | tail` ，會看到 copy_from_user 與 copy_to_user 的處理紀錄。
12. 關閉程式並移除核心：`sudo rmmod my_chardev` 且 `sudo rm /dev/my_os_lab`。

## 挑戰與心得：
1. Everything is a file：透過這個實驗，深刻體會到為什麼 Linux 把硬體操作抽象化為檔案操作，這大幅降低了開發的門檻。
2. 實作 Software interrupt：透過 linux 的 dev_read、dev_write 等 system call 功能，實作了作業系統裡的 software interrupt (軟體中斷) 概念，看著教科書的定義和例子的應用，非常有成就感。
3. git 衝突與解決：這次實驗遇到了 git 衝突的問題。由於筆者在 VM 修改了新程式，但又在 github 上修改了 README.md 內容，讓地端與雲端都出現了新內容，這時就會產生衝突：當雲端有一份檔案，也有一份「同名但 git 還不認識」的檔案時，Git 為了保護資料不被雲端覆蓋，會選擇直接「罷工」，導致使用 git 的 pull 、push 指令時都發生了 error，所幸後來備份檔案後，單獨將新版的 REAMDE.md 加入地端的 project，才成功解決了版本不一致的問題。
4. 工廠與展覽館與單向通道：這次的 git 衝突問題，也讓筆者更深刻體悟到單向通道的重要性，在這個專案中，VM 就像是負責製作產品的工廠，github 則是負責展示產品的展覽館，而產品就是這次的 project，遵守著工廠只負責製造、展覽館只負責展示的單向通道規則，就能有效避免類似的問題。

## 環境：Oracle VirualBox VM
* 作業系統：Ubuntu-24.03.3 LTS
* RAM：4GB
* 硬碟容量：20GB
* CPU：4核心

## 補充說明：
1. copy_to_user / copy_from_user：這是核心開發重要的 API。因為 User Space 與 Kernel Space 的記憶體是隔離的，核心必須使用專屬的 API 來確保資料交換的安全性。
2. Major Number (主裝置號)：就像分機號碼，可讓 Linux 核心知道要把 /dev/ 下的請求轉交給哪一個驅動程式。
