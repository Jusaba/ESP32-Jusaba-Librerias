# ESP32 Jusaba Libraries

**English | [Español](README.es.md)**

Collection of reusable libraries for ESP32 projects developed by Jusaba.

## 📚 Available Libraries

### RTCManager
Advanced NTP time synchronization management with multiple servers and date validation.

- ✅ NTP synchronization with redundancy (3 servers)
- ✅ Automatic date validation
- ✅ Configurable timeout
- ✅ Timezone and daylight saving time handling
- ✅ Automatic fallback between servers

[View full documentation →](libraries/RTCManager/README.md)

## 🚀 Installation

### Method 1: Arduino IDE
1. Download the repository as ZIP
2. In Arduino IDE: `Sketch` → `Include Library` → `Add .ZIP Library`
3. Select the `libraries/RTCManager` folder

### Method 2: Manual
1. Clone the repository:
   ```bash
   git clone https://github.com/Jusaba/ESP32-Jusaba-Librerias.git
   ```
2. Copy the `libraries/RTCManager` folder to your Arduino libraries folder:
   - Windows: `Documents/Arduino/libraries/`
   - Linux: `~/Arduino/libraries/`
   - macOS: `~/Documents/Arduino/libraries/`

### Method 3: PlatformIO
Add to `platformio.ini`:
```ini
lib_deps = 
    https://github.com/Jusaba/ESP32-Jusaba-Librerias.git#libraries/RTCManager
```

## 📖 Quick Start

```cpp
#include <RTCManager.h>

void setup() {
    Serial.begin(115200);
    WiFi.begin("SSID", "password");
    
    // Wait for WiFi connection
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
    
    // Synchronize with multiple NTP servers
    if (RTC::beginConMultiplesServidores()) {
        Serial.println("✅ NTP synchronization successful");
        Serial.println("Time: " + RTC::getTimeStr());
    }
}

void loop() {
    if (RTC::isNtpSync()) {
        Serial.println(RTC::getTimeStr());
    }
    delay(1000);
}
```

## 🛠️ Upcoming Libraries

- [ ] TelegramService - Telegram client with custom commands
- [ ] OTAService - OTA updates from GitHub Releases
- [ ] WebServerManager - Async web server with WebSockets
- [ ] SPIFFSManager - Simplified SPIFFS file management

## 🤝 Contributing

Have a useful ESP32 library? See [CONTRIBUTING.md](docs/contributing.md)

## 📄 License

MIT License - See [LICENSE](LICENSE) for details

## 👤 Author

**Julián Salas Bartolomé**
- GitHub: [@Jusaba](https://github.com/Jusaba)

## 📝 Changelog

### v1.0.0 (2025-11-28)
- ✨ First release
- ✨ RTCManager: NTP synchronization with multiple servers
