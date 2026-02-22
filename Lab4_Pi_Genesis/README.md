# 實驗四：跨越 VM 佈署 Hardware：樹莓派硬體通訊與開發環境拓荒

## 實驗目的
* **從虛擬轉向實體硬體**：將 Linux 核心實驗從 VM 環境遷移至真實 ARM 架構 SoC（Raspberry Pi），面對並解決實體硬體初始化中的不確定性。
* **建構開發者「一線通」機制**：透過修改核心啟動參數，實作 USB Gadget 模式，達成僅需一條 USB 線即可同時供應電力與提供 Serial Console 通訊。
* **佈署移動式開發工作流**：建立基於 WiFi SSH 與實體 Serial Port 的雙軌通訊，確保在資源受限的外部環境（如無區域網路）下，依然具備完整的程式碼編輯與 Debug 能力。

## 成果展示
(待貼上)

## 挑戰與心得
* 硬體層級的 Debug ： 本實驗最大的挑戰在於處理 Windows 與樹莓派之間的「握手失敗」。在無法辨識裝置的情況下，必須具備從線材物理特性（Data vs Power only）到核心驅動層（DWC2 Controller）的層級化排除能力。這讓筆者深刻體會到「驅動程式不僅僅只是程式碼，更是軟硬體間的精準契約」。

* 核心參數的權衡與配置： 理解 `/boot/firmware/` 下 `config.txt` 與 `cmdline.txt` 的交互作用。例如 `otg_mode=1` 的衝突處理，這讓筆者對嵌入式系統在引導階段（Bootloader Stage）如何動態加載核心模組有了實務上的認識。

* 序列埠通訊的底層磨練 (TTY Abstraction)： 不同於 SSH 具備完善的網路層包裝，Serial Port 回歸到最原始的 TTY 序列傳輸。在實驗中遇到字元回顯（Echo）與控制碼（如 Backspace 的 ASCII 處理）的偏差，這讓筆者深刻體認到即使是簡單的輸入輸出，在底層韌體世界中都需要精準的時序與驅動匹配。

* 資源受限下的 Lite OS 策略：為了追求執行效率，本實驗捨棄了圖形化桌面，改用 Lite OS (Headless Mode)。這強迫筆者必須脫離對圖形介面的依賴，改用 nmcli 管理網路、用 nano/vim 編輯配置。這種在「資源受限」環境下的開發習慣，正是嵌入式開發的核心精神。

## 實作步驟
1. 系統初始化： 使用 Raspberry Pi Imager 燒錄 64-bit Lite OS，並預設 WiFi 與 SSH 憑證。
2. 核心模式配置： 編輯 `config.txt` 加入 dtoverlay=dwc2 以啟用 USB OTG 驅動。
3. 啟用 Gadget 模組： 編輯 `cmdline.txt` 於 `rootwait` 後加入 `modules-load=dwc2,g_serial` (選用相容性最高的 Serial 協議)。
4. 掛載登入服務： 透過 SSH 進入系統，執行 `sudo systemctl enable serial-getty@ttyGS0.service` 啟動序列終端。
5. 驅動對接： 在 Windows 端手動指派「USB 序列裝置」驅動程式，完成實體與虛擬埠的綁定。
6. 環境連線測試： 關閉 WiFi 並使用 MobaXterm 連接 COM Port，確認在離線狀態下仍能完全控制系統。

## 開發環境
* 硬體主機： 樹莓派 Raspberry Pi Zero 2 WH 開發板 (ARM 架構)
* Host 電腦：Windows 10 筆記型電腦 (或 Win11)
* 傳輸介面： Micro USB Data Cable (Support Data Transfer)
* 連線軟體： MobaXterm (SSH / Serial)

## 補充說明
* USB Gadget Mode： 指 Linux 裝置模擬成另一個電腦的周邊設備（如網卡、序列埠）。
* DWC2 Controller： 樹莓派內建的 Synopsys USB 控制器驅動，是達成 OTG 模式的核心。
* Headless Setup： 指在不接顯示器、鍵盤的情況下完成系統配置，是嵌入式開發的標準作業模式。
