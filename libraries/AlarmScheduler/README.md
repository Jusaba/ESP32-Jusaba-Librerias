# AlarmScheduler

[Español](README.es.md) | **English**

> Advanced alarm scheduling library for ESP32 with web management, JSON persistence, and flexible callback system.

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](../../LICENSE)
[![Platform](https://img.shields.io/badge/platform-ESP32-blue.svg)](https://www.espressif.com/en/products/socs/esp32)
[![Arduino](https://img.shields.io/badge/Arduino-Library-green.svg)](https://www.arduino.cc/)

## Features

- ✅ **Multiple alarm types**: Fixed time, wildcard (any hour/minute), interval-based
- ✅ **Day scheduling**: Individual days, multiple days, or every day
- ✅ **Web management**: Create, modify, enable/disable alarms via web interface
- ✅ **JSON persistence**: Save/load customizable alarms from SPIFFS
- ✅ **Flexible callbacks**: Member methods, external functions (with/without parameters)
- ✅ **System + customizable alarms**: Distinguish between fixed system alarms and user-editable ones
- ✅ **Bilingual API**: Spanish method names with English aliases
- ✅ **Zero dependencies**: Works standalone, integrates easily with RTCManager for time sync
- ✅ **Efficient execution**: Smart caching prevents duplicate executions
- ✅ **Debug support**: Comprehensive debug macros and helper functions

## Installation

### Arduino IDE

1. Download this repository as ZIP
2. In Arduino IDE: **Sketch → Include Library → Add .ZIP Library**
3. Select the downloaded ZIP file
4. Restart Arduino IDE

### PlatformIO

Add to your `platformio.ini`:

```ini
lib_deps =
    https://github.com/Jusaba/ESP32-Jusaba-Librerias.git#main
    bblanchon/ArduinoJson@^6.21.0
```

### Manual Installation

1. Clone this repository
2. Copy `libraries/AlarmScheduler` to your Arduino libraries folder
3. Restart Arduino IDE

## Dependencies

- **ArduinoJson** (v6.x) - JSON serialization/deserialization
- **SPIFFS** - ESP32 file system (included in ESP32 core)

### Optional (Recommended)

- **RTCManager** - For NTP/RTC time synchronization (from this repository)
- **WiFi** - For web server integration

## Quick Start

### Basic System Alarm

```cpp
#include <AlarmScheduler.h>

AlarmScheduler scheduler;

// Callback function
void morningBell(uint16_t duration) {
    Serial.printf("Morning bell ringing for %u seconds\n", duration);
    // Your bell control code here
}

void setup() {
    Serial.begin(115200);
    
    // Initialize scheduler
    scheduler.begin();
    
    // Add alarm: Every day at 08:00, call morningBell(10)
    scheduler.addExternal(
        DOW_ALL,        // Every day
        8,              // 08:00 hours
        0,              // 00 minutes
        0,              // No interval (fixed time)
        morningBell,    // Callback function
        10              // Parameter (duration)
    );
}

void loop() {
    scheduler.check();  // Check and execute due alarms
    delay(1000);        // Check every second
}
```

### Customizable Alarm (Web Management)

```cpp
#include <AlarmScheduler.h>
#include <SPIFFS.h>

AlarmScheduler scheduler;

void bellAction(uint16_t parameter) {
    Serial.printf("Custom bell action: %u\n", parameter);
}

void setup() {
    Serial.begin(115200);
    
    // Initialize SPIFFS
    if (!SPIFFS.begin(true)) {
        Serial.println("SPIFFS initialization failed!");
        return;
    }
    
    // Initialize scheduler (loads saved alarms from JSON)
    scheduler.begin(true);  // true = create defaults if empty
    
    // Add customizable alarm
    scheduler.addCustomizable(
        "Morning Bell",              // Name
        "Daily morning bell at 8am", // Description
        DOW_ALL,                     // Every day
        8, 0,                        // 08:00
        "BELL",                      // Type string (for web UI)
        10,                          // Parameter
        bellAction,                  // Callback
        true                         // Enabled
    );
}

void loop() {
    scheduler.check();
    delay(1000);
}
```

## Alarm Types

### 1. Fixed Time Alarm

Executes at a specific time on specific days:

```cpp
scheduler.addExternal(
    DOW_LUNES | DOW_MIERCOLES | DOW_VIERNES,  // Mon, Wed, Fri
    14, 30,     // 14:30
    0,          // No interval
    myCallback,
    param
);
```

### 2. Wildcard Alarm

Use `ALARM_WILDCARD` (255) for any hour or minute:

```cpp
// Execute every hour at minute 0 (XX:00)
scheduler.addExternal(
    DOW_ALL,
    ALARM_WILDCARD,  // Any hour
    0,               // Minute 0
    0,
    hourlyCallback,
    0
);

// Execute every minute of every hour (XX:XX)
scheduler.addExternal(
    DOW_ALL,
    ALARM_WILDCARD,  // Any hour
    ALARM_WILDCARD,  // Any minute
    0,
    everyMinuteCallback,
    0
);
```

### 3. Interval Alarm

Executes repeatedly at fixed intervals:

```cpp
// Execute every 15 minutes, anchored at XX:00
scheduler.addExternal(
    DOW_ALL,
    ALARM_WILDCARD,  // Any hour
    0,               // Anchor at minute 0
    15,              // Repeat every 15 minutes
    intervalCallback,
    0
);
// Will execute at: 00:00, 00:15, 00:30, 00:45, 01:00, 01:15, ...
```

## API Reference

### Core Methods

#### `bool begin(bool loadDefaults = true)`

Initialize the alarm scheduler.

- **Parameters:**
  - `loadDefaults`: Create default alarms if JSON file doesn't exist
- **Returns:** `true` on success

```cpp
scheduler.begin();        // Load saved alarms
scheduler.begin(false);   // Don't create defaults
```

#### `void check()`

Check and execute due alarms. **Call this in `loop()`**.

```cpp
void loop() {
    scheduler.check();
    delay(1000);
}
```

### Adding System Alarms

#### `uint8_t add(dayMask, hour, minute, interval, method, parameter, enabled)`

Add alarm with member method callback.

```cpp
class MyClass {
    AlarmScheduler scheduler;
    
    void myMethod(uint16_t param) {
        Serial.printf("Method called with: %u\n", param);
    }
    
    void setup() {
        scheduler.add(DOW_ALL, 12, 0, 0, &MyClass::myMethod, 42, true);
    }
};
```

#### `uint8_t addExternal(dayMask, hour, minute, interval, callback, parameter, enabled)`

Add alarm with external function callback (with parameter).

```cpp
void myFunction(uint16_t param) {
    Serial.printf("Function called: %u\n", param);
}

scheduler.addExternal(DOW_ALL, 9, 0, 0, myFunction, 123, true);
```

#### `uint8_t addExternal0(dayMask, hour, minute, interval, callback, enabled)`

Add alarm with parameterless callback.

```cpp
void simpleCallback() {
    Serial.println("Simple callback!");
}

scheduler.addExternal0(DOW_ALL, 18, 0, 0, simpleCallback, true);
```

### Managing Alarms

#### `void enable(uint8_t index)` / `void disable(uint8_t index)`

Enable or disable an alarm by index.

```cpp
scheduler.disable(0);  // Disable first alarm
scheduler.enable(0);   // Re-enable it
```

#### `uint8_t count()` / `void clear()`

Get alarm count or remove all alarms.

```cpp
Serial.printf("Total alarms: %u\n", scheduler.count());
scheduler.clear();  // Remove all alarms
```

#### `const Alarm* get(uint8_t index)`

Get read-only access to alarm.

```cpp
const Alarm* alarm = scheduler.get(0);
if (alarm) {
    Serial.printf("Alarm at %02u:%02u\n", alarm->hour, alarm->minute);
}
```

### Customizable Alarms (Web Management)

#### Spanish Names

```cpp
// Add
uint8_t addPersonalizable(nombre, descripcion, mascaraDias, hora, minuto, 
                          tipoString, parametro, callback, habilitada);

// Modify
bool modificarPersonalizable(idWeb, nombre, descripcion, mascaraDias, hora, 
                             minuto, tipoString, habilitada, callback, parametro);

// Delete
bool eliminarPersonalizable(int idWeb);

// Enable/Disable
bool habilitarPersonalizable(int idWeb, bool estado);

// Get JSON
String obtenerPersonalizablesJSON();
String obtenerEstadisticasJSON();

// Load/Save
bool cargarPersonalizablesDesdeJSON();
bool guardarPersonalizablesEnJSON();
```

#### English Aliases

```cpp
// Add
uint8_t addCustomizable(name, description, dayMask, hour, minute, 
                        typeString, parameter, callback, enabled);

// Modify
bool modifyCustomizable(webId, name, description, dayMask, hour, minute, 
                        typeString, enabled, callback, parameter);

// Delete
bool deleteCustomizable(int webId);

// Enable/Disable
bool enableCustomizable(int webId, bool state);

// Get JSON
String getCustomizablesJSON();
String getStatisticsJSON();

// Load/Save
bool loadCustomizablesFromJSON();
bool saveCustomizablesToJSON();
```

## Day Masks

Use bitwise OR to combine days:

```cpp
// Spanish constants
DOW_DOMINGO   (0x01)  // Sunday
DOW_LUNES     (0x02)  // Monday
DOW_MARTES    (0x04)  // Tuesday
DOW_MIERCOLES (0x08)  // Wednesday
DOW_JUEVES    (0x10)  // Thursday
DOW_VIERNES   (0x20)  // Friday
DOW_SABADO    (0x40)  // Saturday
DOW_TODOS     (0x7F)  // Every day

// English aliases
DOW_SUNDAY, DOW_MONDAY, DOW_TUESDAY, DOW_WEDNESDAY,
DOW_THURSDAY, DOW_FRIDAY, DOW_SATURDAY, DOW_ALL

// Examples
DOW_LUNES | DOW_MIERCOLES | DOW_VIERNES  // Mon, Wed, Fri
DOW_SABADO | DOW_DOMINGO                  // Weekend
DOW_TODOS                                  // Every day
```

## Web Integration Example

```cpp
#include <WebServer.h>
#include <AlarmScheduler.h>

WebServer server(80);
AlarmScheduler scheduler;

void handleGetAlarms() {
    String json = scheduler.getCustomizablesJSON();
    server.send(200, "application/json", json);
}

void handleAddAlarm() {
    // Parse POST data
    String name = server.arg("name");
    int day = server.arg("day").toInt();
    int hour = server.arg("hour").toInt();
    int minute = server.arg("minute").toInt();
    
    uint8_t dayMask = (day == 0) ? DOW_ALL : (1 << (day - 1));
    
    scheduler.addCustomizable(
        name.c_str(),
        "User created alarm",
        dayMask, hour, minute,
        "CUSTOM", 0,
        myCallback,
        true
    );
    
    server.send(200, "text/plain", "Alarm added");
}

void handleDeleteAlarm() {
    int id = server.arg("id").toInt();
    bool success = scheduler.deleteCustomizable(id);
    server.send(success ? 200 : 404, "text/plain", 
                success ? "Deleted" : "Not found");
}

void setup() {
    SPIFFS.begin(true);
    scheduler.begin();
    
    server.on("/api/alarms", HTTP_GET, handleGetAlarms);
    server.on("/api/alarms/add", HTTP_POST, handleAddAlarm);
    server.on("/api/alarms/delete", HTTP_DELETE, handleDeleteAlarm);
    
    server.begin();
}

void loop() {
    server.handleClient();
    scheduler.check();
}
```

## JSON Format

### Customizable Alarms File (`/customizable_alarms.json`)

```json
{
  "version": "1.0",
  "timestamp": 123456,
  "total": 2,
  "alarms": [
    {
      "id": 1,
      "name": "Morning Bell",
      "description": "Daily morning bell",
      "day": 0,
      "hour": 8,
      "minute": 0,
      "action": "BELL",
      "parameter": 10,
      "enabled": true
    },
    {
      "id": 2,
      "name": "Evening Light",
      "description": "Turn on evening lights",
      "day": 0,
      "hour": 18,
      "minute": 30,
      "action": "LIGHT",
      "parameter": 1,
      "enabled": true
    }
  ]
}
```

### Get Alarms JSON Response

```json
{
  "version": "1.0",
  "timestamp": 123456,
  "total": 2,
  "alarms": [
    {
      "id": 1,
      "name": "Morning Bell",
      "description": "Daily morning bell",
      "day": 0,
      "dayName": "Every day",
      "hour": 8,
      "minute": 0,
      "timeText": "08:00",
      "action": "BELL",
      "parameter": 10,
      "enabled": true,
      "arrayIndex": 0
    }
  ]
}
```

## Templates

The `templates/` folder contains example configuration and debug files:

- **Configuracion.h** - Project configuration, alarm types, parameters
- **Debug.h** - Debug macros and helper functions
- **README.md** - Detailed template usage guide

See [templates/README.md](templates/README.md) for complete documentation.

## Examples

See [examples/BasicAlarms](examples/BasicAlarms/) for a complete working example.

## Integration with RTCManager

For accurate time synchronization, use with RTCManager:

```cpp
#include <RTCManager.h>
#include <AlarmScheduler.h>

RTCManager rtc;
AlarmScheduler scheduler;

void setup() {
    // Initialize RTC with NTP sync
    const char* servers[] = {"pool.ntp.org", "time.nist.gov"};
    rtc.beginWithMultipleServers(servers, 2, 3600, 3600);
    
    // Initialize scheduler
    scheduler.begin();
}

void loop() {
    rtc.check();        // Keep time synchronized
    scheduler.check();  // Check alarms
    delay(1000);
}
```

## Debug

Enable debug output:

```cpp
#define DEBUG_ENABLED 1
#define DEBUG_ALARMSCHEDULER 1
#include "Debug.h"
#include <AlarmScheduler.h>

AlarmScheduler scheduler;

void setup() {
    Serial.begin(115200);
    scheduler.begin();
    
    // Print all alarms
    scheduler.printAllAlarms();
    
    // Use debug helpers
    DebugHelper::printAlarmSummary(&scheduler);
    DebugHelper::printCurrentTime();
}
```

## Advanced Usage

### Custom Alarm Actions by Type

```cpp
void executeAlarmAction(const char* typeString, uint16_t parameter) {
    if (strcmp(typeString, "BELL") == 0) {
        ringBell(parameter);  // parameter = duration
    } else if (strcmp(typeString, "LIGHT") == 0) {
        toggleLight(parameter);  // parameter = zone
    } else if (strcmp(typeString, "NOTIFICATION") == 0) {
        sendNotification(parameter);  // parameter = message ID
    }
}

void genericCallback(uint16_t param) {
    const Alarm* alarm = scheduler.get(/* find by execution context */);
    if (alarm) {
        executeAlarmAction(alarm->typeString, alarm->parameter);
    }
}
```

### Dynamic Alarm Modification

```cpp
// Find alarm by web ID and modify it
int webId = 5;
scheduler.modifyCustomizable(
    webId,
    "Updated Name",
    "Updated description",
    DOW_LUNES | DOW_VIERNES,  // Mon and Fri
    9, 30,                     // 09:30
    "BELL",
    true,                      // enabled
    myCallback,
    15                         // new parameter
);
```

### Statistics and Monitoring

```cpp
String stats = scheduler.getStatisticsJSON();
// Returns:
// {
//   "module": "AlarmScheduler",
//   "totalAlarms": 10,
//   "system": 4,
//   "customizable": 6,
//   "enabled": 8,
//   "disabled": 2,
//   "freeSpace": 6,
//   "maxAlarms": 16,
//   ...
// }
```

## Troubleshooting

### Alarms not executing

1. **Check time sync**: Ensure `getLocalTime()` works
2. **Verify alarm is enabled**: `alarm->enabled == true`
3. **Check day mask**: Alarm must match current weekday
4. **Call `check()` regularly**: At least once per second
5. **Enable debug**: See what's happening

### JSON not saving

1. **Initialize SPIFFS**: Call `SPIFFS.begin(true)` before `scheduler.begin()`
2. **Check free space**: Use `SPIFFS.totalBytes()` and `usedBytes()`
3. **Verify file exists**: `SPIFFS.exists("/customizable_alarms.json")`

### Duplicate executions

- The library prevents this automatically via caching
- If it happens, check your callback is idempotent
- Reset cache with `scheduler.resetCache()` after time changes

## Contributing

See [CONTRIBUTING.md](../../docs/contributing.md) for guidelines.

## License

MIT License - see [LICENSE](../../LICENSE) file.

## Author

**Julián Salas Bartolomé**
- Email: julian.salas.bartolome@gmail.com
- GitHub: [@Jusaba](https://github.com/Jusaba)

## Changelog

### v1.0.0 (2025-11-29)
- Initial release
- Fixed time, wildcard, and interval alarms
- Web management with JSON persistence
- Bilingual API (Spanish + English)
- SPIFFS integration
- Comprehensive debug support

## Related Libraries

- [RTCManager](../RTCManager/) - NTP/RTC time synchronization
- [ArduinoJson](https://github.com/bblanchon/ArduinoJson) - JSON handling

## Support

- 📧 Email: julian.salas.bartolome@gmail.com
- 🐛 Issues: [GitHub Issues](https://github.com/Jusaba/ESP32-Jusaba-Librerias/issues)
- 📖 Wiki: [GitHub Wiki](https://github.com/Jusaba/ESP32-Jusaba-Librerias/wiki)
