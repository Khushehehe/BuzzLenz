## **BuzzLens - Hackathon Project** 🚀  

### **Overview**  
BuzzLens is a smart glasses project designed to enhance driver safety using real-time alerts and GPS tracking. The system detects drowsiness and notifies the driver while also sharing location data via Ubidots.

## 📂 Folder Structure  
/BuzzLens
 ├── main.cpp              # ESP32 Code for the project
 ├── platformio.ini        # PlatformIO configuration file
 ├── README.md             # Project documentation
 └── LICENSE               # License file (if applicable)
## 🔧 Setup Instructions  

### 1️⃣ Clone this Repository  
```sh
git clone https://github.com/Khushehehe/BuzzLenz.git
cd BuzzLenz
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
lib_deps = 
    bblanchon/ArduinoJson @ ^6.20.0
    ubidots/UbidotsESPMQTT @ ^3.0.0
pio lib install
## ▶️ Usage Instructions  

1. **Upload the Code:**  
   - Open the code in Arduino IDE or VS Code (PlatformIO).  
   - Select the correct ESP32 board and COM port.  
   - Compile and upload the code to your ESP32 board.  

2. **Connect to WiFi:**  
   - Ensure the **WiFi credentials** in the code are correct.  
   - Power on the ESP32 and wait for the serial monitor to display "Connected to WiFi".  

3. **Check Sensor & Alerts:**  
   - The system detects drowsiness using an **IR sensor**.  
   - If the driver's eyes remain closed for **1.5 seconds**, a **buzzer alert** will trigger.  

4. **GPS & Cloud Monitoring:**  
   - The ESP32 fetches geolocation data via the **Google API**.  
   - Location updates are sent to **Ubidots** every **10 seconds**.  
   - You can view real-time data on your Ubidots dashboard.  

## 🎥 Demo & Contribution  

### 🔗 Live Demo 
https://drive.google.com/file/d/1Kbcw73yJhnhTRNc6o7lB4TmctQVN4-aT/view?usp=sharing 

### 🤝 Contributing  
Contributions are welcome! Follow these steps:  
1. **Fork** this repository.  
2. **Clone** your fork:  
   ```sh
   git clone https://github.com/your-username/BuzzLenz.git
   cd BuzzLenz
git checkout -b feature-name
## 📜 License  
This project is open-source under the [MIT License](LICENSE).  

## 📧 Contact  
For any queries or suggestions, feel free to reach out:  
- **Email:** [ashubhatt987833@gmail.com]  
- **GitHub Issues:** [Create an Issue](https://github.com/Khushehehe/BuzzLenz/issues)  
