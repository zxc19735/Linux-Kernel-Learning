# Linux Kernel Learning Journey

這個 Repo 記錄了我學習 Linux 核心開發的實驗過程。

## 實驗清單
* [Lab 1: Hello World](./Lab1_HelloWorld/README.md) - 學習核心模組的載入與編譯。
* [Lab 2: Character Device](./Lab2_CharDev/README.md) - 實作字元驅動程式與 User/Kernel 空間通訊。
* [Lab 3: Kernel Timer](./Lab3_Timer/README.md) - 探索非同步處理與核心定時器。

## 技術重點
* 使用 **C 語言** 撰寫 Linux Kernel Module (LKM)。
* 掌握 **copy_to_user** / **copy_from_user** 安全機制。
* 理解 **Jiffies** 與核心時間管理。
