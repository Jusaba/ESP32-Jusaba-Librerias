# RTCManager

**English | [Español](README.es.md)**

Advanced NTP synchronization library for ESP32 with multiple server support, date validation, and robust error handling.

## ✨ Features

- ✅ **Multiple NTP servers** - Up to 3 servers with automatic fallback
- ✅ **Date validation** - Verifies realistic dates (2020-2050)
- ✅ **Configurable timeout** - Prevents indefinite blocking
- ✅ **Automatic timezone** - GMT and daylight saving time support
- ✅ **Optional debug** - Detailed logging for troubleshooting
- ✅ **Thread-safe** - Safe operations in multi-core
- ✅ **No dependencies** - Only WiFi.h included in ESP32
- ✅ **Bilingual API** - Methods available in Spanish and English

## 📦 Installation

### Arduino IDE
1. Download the `RTCManager` folder
2. Copy to `Documents/Arduino/libraries/`
3. Restart Arduino IDE

### PlatformIO
```ini
lib_deps = 
    https://github.com/Jusaba/ESP32-Jusaba-Librerias.git#libraries/RTCManager
```

## 🚀 Quick Start

```cpp
#include <WiFi.h>
#include <RTCManager.h>

void setup() {
    Serial.begin(115200);
    
    // Connect WiFi
    WiFi.begin("SSID", "password");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
    
    // Synchronize NTP (recommended)
    // You can use English or Spanish method names
    if (RTC::beginWithMultipleServers()) {
        // Or: RTC::beginConMultiplesServidores()
        Serial.println("✅ Time synchronized: " + RTC::getTimeStr());
    }
}

void loop() {
    if (RTC::isNtpSync()) {
        Serial.println(RTC::getTimeStr());
    }
    delay(1000);
}
```

## 📖 API Reference

> 🌍 **Note:** All methods are available in both Spanish and English.

### RTC::beginWithMultipleServers() / beginConMultiplesServidores()
NTP synchronization with multiple servers (recommended).

```cpp
// English
bool RTC::beginWithMultipleServers(unsigned long timeout_ms = 15000);

// Español
bool RTC::beginConMultiplesServidores(unsigned long timeout_ms = 15000);
```

**Parameters:**
- `timeout_ms` - Timeout in milliseconds (default: 15000)

**Returns:**
- `true` - Successful synchronization
- `false` - Timeout or error

**Example:**
```cpp
// 30 second timeout
if (RTC::beginWithMultipleServers(30000)) {
    // Or in Spanish: RTC::beginConMultiplesServidores(30000)
    Serial.println("NTP OK");
}
```

### RTC::begin()
NTP synchronization with a single server.

```cpp
void RTC::begin(
    const char* ntpServer = NTP_SERVER1,
    long gmtOffsetSec = GMT_OFFSET_SEC,
    int daylightOffsetSec = DAYLIGHT_OFFSET_SEC,
    unsigned long timeout_ms = 10000
);
```

**Parameters:**
- `ntpServer` - NTP server (default: "pool.ntp.org")
- `gmtOffsetSec` - GMT offset in seconds (default: 3600 for GMT+1)
- `daylightOffsetSec` - Daylight saving time in seconds (default: 3600)
- `timeout_ms` - Timeout in milliseconds (default: 10000)

**Example:**
```cpp
// GMT-5 (New York), no daylight saving
RTC::begin("time.google.com", -5*3600, 0);
```

### RTC::isNtpSync()
Verifies synchronization status.

```cpp
bool RTC::isNtpSync();
```

**Returns:**
- `true` - Successful synchronization
- `false` - No synchronization

**Example:**
```cpp
if (RTC::isNtpSync()) {
    // Use date/time with confidence
}
```

### RTC::getTimeStr()
Gets formatted current date/time.

```cpp
String RTC::getTimeStr();
```

**Returns:**
- String format "YYYY-MM-DD HH:MM:SS"
- "Error obteniendo hora" if it fails

**Example:**
```cpp
String hora = RTC::getTimeStr();
Serial.println(hora);  // 2025-11-28 15:30:45
```

## ⚙️ Configuration

### Custom NTP Servers

Define before `#include <RTCManager.h>`:

```cpp
#define NTP_SERVER1 "pool.ntp.org"
#define NTP_SERVER2 "time.cloudflare.com"
#define NTP_SERVER3 "time.google.com"

#include <RTCManager.h>
```

### Timezone

```cpp
// GMT+1 (Spain)
#define GMT_OFFSET_SEC 3600
#define DAYLIGHT_OFFSET_SEC 3600

// GMT-5 (New York)
#define GMT_OFFSET_SEC -18000
#define DAYLIGHT_OFFSET_SEC 0

// GMT+8 (China)
#define GMT_OFFSET_SEC 28800
#define DAYLIGHT_OFFSET_SEC 0

#include <RTCManager.h>
```

### Enable Debug

```cpp
#define RTCMANAGER_DEBUG
#include <RTCManager.h>
```

Shows in Serial:
```
[RTC] Iniciando sincronización NTP con múltiples servidores...
[RTC] Servidores NTP configurados:
[RTC]   - Servidor 1: pool.ntp.org
[RTC]   - Servidor 2: es.pool.ntp.org
[RTC]   - Servidor 3: time.google.com
[RTC] ✅ Sincronización NTP exitosa:
[RTC]    Fecha/Hora: 2025-11-28 15:30:45
```

## 📝 Examples

See `examples/` folder for more complete examples.

## 🔧 Troubleshooting

### Not synchronizing
- ✅ Verify active WiFi connection
- ✅ Increase timeout: `beginConMultiplesServidores(60000)`
- ✅ Enable debug: `#define RTCMANAGER_DEBUG`
- ✅ Verify firewall doesn't block UDP port 123

### Incorrect date
- ✅ Verify correct `GMT_OFFSET_SEC` for your timezone
- ✅ Adjust `DAYLIGHT_OFFSET_SEC` according to daylight saving time
- ✅ Use `beginConMultiplesServidores()` instead of `begin()`

### Blocking
- ✅ Reduce timeout if network is slow
- ✅ Call after connecting WiFi
- ✅ Don't call in `loop()`, only in `setup()`

## 📄 License

MIT License - See LICENSE in main repository

## 👤 Author

**Julián Salas Bartolomé**
- GitHub: [@Jusaba](https://github.com/Jusaba)

## 🔗 Links

- [Main repository](https://github.com/Jusaba/ESP32-Jusaba-Librerias)
- [Report issues](https://github.com/Jusaba/ESP32-Jusaba-Librerias/issues)
