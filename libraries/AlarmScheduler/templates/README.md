# AlarmScheduler Templates

[Español](README.es.md) | **English**

## Overview

This folder contains **example templates** to help you integrate the AlarmScheduler library into your ESP32 project. These templates show recommended configuration patterns and debug utilities specific to alarm management.

## Files

### `Configuracion.h`
Example configuration file with:
- Project information (name, version, author)
- Serial communication settings
- AlarmScheduler-specific configuration (JSON file path, defaults, limits)
- SPIFFS storage settings
- NTP/RTC time synchronization (if using RTCManager)
- Alarm action types and parameters (customizable for your project)

### `Debug.h`
Example debug utilities with:
- Module-specific debug flags (alarms, WiFi, time, SPIFFS, actions)
- Debug macros for each module (`DBG_ALM`, `DBG_WIFI`, `DBG_TIME`, etc.)
- `DebugHelper` class extensions:
  - `printAlarmSummary()` - Quick alarm statistics
  - `printDayMask()` - Human-readable day display
  - `printCurrentTime()` - ESP32 current time
  - `printSPIFFSInfo()` - File system usage
  - `listSPIFFSFiles()` - File listing
  - `printAlarmJSON()` - Pretty-print alarm JSON

## How to Use

### Step 1: Copy to Your Project

Copy these templates to your Arduino sketch folder (where your `.ino` file is):

```
MyProject/
├── MyProject.ino
├── Configuracion.h    ← Copy and customize
└── Debug.h            ← Copy and customize
```

### Step 2: Customize

Edit the files to match your project needs:

**Configuracion.h:**
- Change project name, version, author
- Adjust alarm types for your use case (bells, lights, notifications, etc.)
- Set alarm parameters (durations, zones, etc.)
- Configure time zone and NTP servers

**Debug.h:**
- Enable/disable debug modules
- Add custom debug macros for your features
- Extend `DebugHelper` with project-specific functions

### Step 3: Include in Your Sketch

```cpp
#include "Configuracion.h"
#include "Debug.h"
#include <AlarmScheduler.h>

AlarmScheduler scheduler;

void setup() {
    Serial.begin(Config::Serial::BAUD_RATE);
    
    // Initialize SPIFFS
    if (!SPIFFS.begin(Config::Storage::FORMAT_ON_FAIL)) {
        DBG("SPIFFS mount failed!");
        return;
    }
    
    // Initialize scheduler
    scheduler.begin(Config::Alarms::LOAD_DEFAULTS);
    
    // Debug info
    DebugHelper::printSystemInfo();
    DebugHelper::printCurrentTime();
    DebugHelper::printAlarmSummary(&scheduler);
}
```

## Relationship with Generic Templates

The AlarmScheduler library also works with the **generic templates** in `templates/` at the repository root:

```
ESP32-Jusaba-Librerias/
├── templates/              ← Generic templates (minimal, universal)
│   ├── Configuracion.h
│   ├── Debug.h
│   └── README.md
└── libraries/
    └── AlarmScheduler/
        └── templates/      ← Library-specific templates (this folder)
            ├── Configuracion.h
            ├── Debug.h
            └── README.md
```

### When to Use Which?

- **Generic templates**: Starting a new ESP32 project with minimal dependencies
- **AlarmScheduler templates**: Project specifically using alarm scheduling features

### Can I Combine Them?

Yes! Best practice:

1. **Start with generic** templates for basic structure
2. **Merge** AlarmScheduler-specific sections into your project
3. **Extend** with your own custom configuration

Example merge strategy:

```cpp
// Your project's Configuracion.h
#ifndef CONFIGURACION_H
#define CONFIGURACION_H

// Keep generic Config::Project and Config::Serial
namespace Config {
    namespace Project { ... }
    namespace Serial { ... }
}

// Add AlarmScheduler-specific sections
namespace Config {
    namespace Alarms { ... }      // From AlarmScheduler template
    namespace AlarmTypes { ... }  // From AlarmScheduler template
}

// Add your own custom sections
namespace Config {
    namespace MyFeature { ... }   // Your additions
}

#endif
```

## Example Configurations

### Simple Bell System

```cpp
namespace Config {
    namespace AlarmTypes {
        const char TYPE_MORNING_BELL[] = "MORNING";
        const char TYPE_NOON_BELL[]    = "NOON";
        const char TYPE_EVENING_BELL[] = "EVENING";
    }
    
    namespace AlarmParams {
        const uint16_t DURATION_SHORT = 5;   // 5 seconds
        const uint16_t DURATION_LONG  = 30;  // 30 seconds
    }
}
```

### Multi-Zone Lighting

```cpp
namespace Config {
    namespace AlarmTypes {
        const char TYPE_LIGHT[] = "LIGHT";
    }
    
    namespace AlarmParams {
        const uint16_t ZONE_KITCHEN  = 1;
        const uint16_t ZONE_BEDROOM  = 2;
        const uint16_t ZONE_LIVING   = 3;
        const uint16_t ZONE_ALL      = 0;
    }
}
```

### Notification System

```cpp
namespace Config {
    namespace AlarmTypes {
        const char TYPE_EMAIL[]    = "EMAIL";
        const char TYPE_TELEGRAM[] = "TELEGRAM";
        const char TYPE_PUSH[]     = "PUSH";
    }
}
```

## Debug Workflow

### During Development

Enable all debug flags:

```cpp
#define DEBUG_ENABLED 1
#define DEBUG_ALARMSCHEDULER 1
#define DEBUG_WIFI 1
#define DEBUG_TIME 1
#define DEBUG_SPIFFS 1
```

Use debug helpers:

```cpp
void loop() {
    scheduler.check();
    
    // Print status every minute
    static unsigned long lastPrint = 0;
    if (millis() - lastPrint > 60000) {
        DebugHelper::printAlarmSummary(&scheduler);
        DebugHelper::printCurrentTime();
        lastPrint = millis();
    }
}
```

### In Production

Disable debug to save memory and improve performance:

```cpp
#define DEBUG_ENABLED 0
```

All debug macros become no-ops, zero overhead.

## Best Practices

1. **Don't modify** the original template files - copy them first
2. **Version control** your customized files
3. **Document** your custom alarm types and parameters
4. **Use namespaces** to organize configuration logically
5. **Comment** why certain values are chosen
6. **Test** with debug enabled, deploy with debug disabled

## See Also

- [AlarmScheduler Library Documentation](../README.md)
- [Generic Templates](../../../templates/README.md)
- [RTCManager Templates](../../RTCManager/templates/README.md)
- [Example Sketch](../examples/BasicAlarms/BasicAlarms.ino)

## Support

For questions or issues:
- GitHub Issues: [ESP32-Jusaba-Librerias](https://github.com/Jusaba/ESP32-Jusaba-Librerias/issues)
- Email: julian.salas.bartolome@gmail.com
