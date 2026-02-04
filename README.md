# Linux Kernel Learning Journey

這份 Repo 記錄筆者學習 Linux 核心開發的實驗過程。

## 實驗清單
* [Lab 1: Hello World](./Lab1_HelloWorld/README.md) - 學習核心模組的載入與編譯。
* [Lab 2: Character Device](./Lab2_CharDev/README.md) - 實作字元驅動程式與 User/Kernel 空間通訊。
* [Lab 3: Kernel Timer](./Lab3_Timer/README.md) - 探索非同步處理與核心定時器。

## 技術重點
* 使用 **C 語言** 撰寫 Linux Kernel Module (LKM)。
* 掌握 **copy_to_user** / **copy_from_user** 安全機制。
* 理解 **Jiffies** 與核心時間管理。

## 補充
1. 每個實驗都有提供各自的 Source Code以及 README.md，而在程式編譯的過程中會產生 .o、.ko 等產出物（Build Artifacts），為方便讀者閱讀以及實驗操作，筆者已製作 .gitignore 來過濾其他的編譯檔案，只上傳源碼（Source Code），不上傳產出物，讀者可直接將 Makefile 與程式碼下載，依照 README.md 提供的流程步驟，逐一完成各個實驗，
