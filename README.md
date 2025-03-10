# Smart Bokashi Monitoring System  

## 📌 Project Description  
This project is a **Smart Bokashi Compost Monitoring System** using an **ESP32** and various sensors to track temperature, moisture, pH, and water level. The data is sent to **Blynk** for remote monitoring.  

## 🛠️ Hardware Used  
- **ESP32**  
- **RS485 Soil Sensor (SN-3001-TR-ECTHPH-N01)**  
- **MAX485 Module**  
- **DHT22 Temperature & Humidity Sensor**  
- **Water Level Sensor**  

## 📦 Software & Libraries  
- **Arduino IDE**  
- **Blynk** (`BlynkSimpleEsp32.h`)  
- **ModbusMaster** (`ModbusMaster.h`)  
- **DHT Sensor Library** (`DHT.h`)  
- **WiFi** (`WiFi.h`, `WiFiClient.h`)  

## 🚀 Features  
✔️ Real-time monitoring of compost temperature, moisture, and pH.  
✔️ Water level detection.  
✔️ Data visualization on **Blynk app**.  

## 📜 How to Use  
1. **Upload the code** to the ESP32.  
2. **Connect sensors** as per the circuit diagram.  
3. **Connect to WiFi** and open the **Blynk app** to monitor data.  

## 📷 Circuit Diagram  
- `schematic_diagram.pdf`  
- `wiring_diagram.jpg`  

## 📝 Notes  
⚠️ **Don't forget to replace WiFi credentials and Blynk tokens in the code before uploading.**  
