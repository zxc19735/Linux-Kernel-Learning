# Journey from Kernel Space (VM) to Physical Hardware (Raspberry Pi)

這是一份關於 Linux 核心開發的深度實作記錄。透過從 VirtualBox 虛擬機出發，最終落地於實體 Raspberry Pi Zero 2 WH，筆者實踐了對驅動開發以及硬體抽象層的應用。

## 實驗清單
* [實驗一：Kernel Mode "Hello World"](./Lab1_HelloWorld/README.md) - 學習核心模組的載入與編譯。
* [實驗二：Linux 驅動程式開發 (Device Driver) 與 VFS 核心機制實作](./Lab2_CharDev/README.md) - 實作字元驅動程式與 User/Kernel 空間通訊。
* [實驗三：核心定時器 (Kernel Timer) 與非同步機制](./Lab3_Timer/README.md) - 探索非同步處理與核心定時器。
* [實驗四：跨越 VM 佈署 Hardware：樹莓派硬體通訊與開發環境拓荒](./Lab4_Pi_Onboarding/README.md) - 轉移開發戰場，踏入 Raspberry Pi Zero 2 WH 世界


## 技術重點
* 使用 **C 語言** 撰寫 Linux Kernel Module (LKM)。
* 掌握 **copy_to_user** / **copy_from_user** 安全機制。
* 理解 **Jiffies** 與核心時間管理。

## 專案維護
1. 這份 Repo 遵守 Clean Architecture 原則。透過 .gitignore 精準過濾編譯過程產生的中間產物（如 .o、.ko、.mod.c 等 Build Artifacts），僅保留原始碼（Source Code）、建置腳本（Makefile）與實驗結果截圖。
2. 這種作法確保了版本庫的輕量化與純粹性，讀者可直接複製本專案，在 Linux 環境下達成 One-Command Build（執行 `make` 即可重現實驗成果）。
3. 依照各個 README.md 提供的流程步驟，讀者即能逐一完成各個實驗。
