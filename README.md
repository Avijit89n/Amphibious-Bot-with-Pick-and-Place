# 🚀 Amphibious Bot with Pick-and-Place Mechanism

## 📌 Project Overview
This project is an **amphibious bot** equipped with a **pick-and-place mechanism**, controlled wirelessly using **NRF24L01 modules**. The bot operates on **land and water**, receiving data via **UART protocol**, and processes commands using a **custom-built controller**.

## 🔧 Hardware Components
### 🖥 Microcontrollers
- **Arduino Uno** – Main motherboard for processing and control  
- **Arduino Nano** – Handles NRF24L01 transmission and reception  

### 📡 Communication
- **NRF24L01 Long-Range Modules** – Wireless data transmission  
- **UART Protocol** – Used for data sharing between the receiver and main microcontroller  

### ⚙️ Pick-and-Place Mechanism
- **10 RPM Johnson Motor** – Raises and lowers the arm  
- **Servo Motor** – Controls the gripper for picking and placing objects  

### 🔌 Motor Drivers
- **2x BTS7960 Modules** – Controls the motors efficiently  

### 🔋 Power Supply
- **2200mAh LiPo Battery** – Powers the full system  

## 🛠 How It Works
1. The **controller** sends signals via **NRF24L01** to the bot.  
2. The **Arduino Nano** receives signals and transfers data using **UART** to the **Arduino Uno**.  
3. The **pick-and-place arm** is operated with a **10 RPM Johnson motor** and a **servo motor**.  
4. The **BTS7960 motor drivers** handle movement control.  
5. The bot operates **wirelessly** and is **powered by a 2200mAh LiPo battery**.  

## 🔗 Connections & Wiring
| Component               | Connection Details |
|-------------------------|--------------------|
| **Arduino Uno**         | Main control unit  |
| **Arduino Nano**        | NRF24L01 communication & UART data transfer |
| **NRF24L01 Modules**    | Handles wireless communication |
| **10 RPM Johnson Motor** | Controlled by BTS7960 for arm movement |
| **Servo Motor**         | Controlled via PWM from Arduino |
| **BTS7960 Motor Drivers** | Drives movement motors |
| **2200mAh LiPo Battery** | Powers the bot |

## 📌 Features
✅ **Amphibious operation** – Works on both land and water  
✅ **Wireless control** – Uses **NRF24L01** modules for long-range communication  
✅ **Custom-built controller** – For precise movement control  
✅ **Pick-and-place system** – Allows object manipulation  
✅ **UART-based communication** – Ensures efficient data transfer  

## 🚀 Future Enhancements
- **FPV Camera** – For real-time video streaming  
- **Autonomous Navigation** – Using sensors for obstacle detection  
- **Enhanced Waterproofing** – For better performance on water  

## 📜 License
This project is licensed under the **MIT License**. See the [LICENSE](./LICENSE) file for details.  

---

🔧 **Developed by:** Avijit Biswas  
📅 **Last Updated:** 2025-03-21  
📂 **Repository:** [GitHub Link]  
