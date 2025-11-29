# Plantillas de AlarmScheduler

[English](README.md) | **Español**

## Descripción

Esta carpeta contiene **plantillas de ejemplo** para ayudarte a integrar la biblioteca AlarmScheduler en tu proyecto ESP32. Estas plantillas muestran patrones de configuración recomendados y utilidades de depuración específicas para la gestión de alarmas.

## Archivos

### `Configuracion.h`
Archivo de configuración de ejemplo con:
- Información del proyecto (nombre, versión, autor)
- Configuración de comunicación serie
- Configuración específica de AlarmScheduler (ruta JSON, valores por defecto, límites)
- Configuración del almacenamiento SPIFFS
- Sincronización de tiempo NTP/RTC (si usas RTCManager)
- Tipos de acciones de alarma y parámetros (personalizables para tu proyecto)

### `Debug.h`
Utilidades de depuración de ejemplo con:
- Flags de depuración por módulo (alarmas, WiFi, tiempo, SPIFFS, acciones)
- Macros de depuración para cada módulo (`DBG_ALM`, `DBG_WIFI`, `DBG_TIME`, etc.)
- Extensiones de la clase `DebugHelper`:
  - `printAlarmSummary()` - Estadísticas rápidas de alarmas
  - `printDayMask()` - Visualización legible de días
  - `printCurrentTime()` - Hora actual del ESP32
  - `printSPIFFSInfo()` - Uso del sistema de archivos
  - `listSPIFFSFiles()` - Listado de archivos
  - `printAlarmJSON()` - Impresión formateada de JSON de alarmas

## Cómo Usar

### Paso 1: Copiar a Tu Proyecto

Copia estas plantillas a la carpeta de tu sketch de Arduino (donde está tu archivo `.ino`):

```
MiProyecto/
├── MiProyecto.ino
├── Configuracion.h    ← Copiar y personalizar
└── Debug.h            ← Copiar y personalizar
```

### Paso 2: Personalizar

Edita los archivos para adaptarlos a las necesidades de tu proyecto:

**Configuracion.h:**
- Cambia el nombre del proyecto, versión, autor
- Ajusta los tipos de alarma para tu caso de uso (campanas, luces, notificaciones, etc.)
- Establece parámetros de alarma (duraciones, zonas, etc.)
- Configura la zona horaria y servidores NTP

**Debug.h:**
- Activa/desactiva módulos de depuración
- Añade macros de depuración personalizadas para tus funcionalidades
- Extiende `DebugHelper` con funciones específicas de tu proyecto

### Paso 3: Incluir en Tu Sketch

```cpp
#include "Configuracion.h"
#include "Debug.h"
#include <AlarmScheduler.h>

AlarmScheduler scheduler;

void setup() {
    Serial.begin(Config::Serial::BAUD_RATE);
    
    // Inicializar SPIFFS
    if (!SPIFFS.begin(Config::Storage::FORMAT_ON_FAIL)) {
        DBG("¡Fallo al montar SPIFFS!");
        return;
    }
    
    // Inicializar planificador
    scheduler.begin(Config::Alarms::LOAD_DEFAULTS);
    
    // Información de depuración
    DebugHelper::printSystemInfo();
    DebugHelper::printCurrentTime();
    DebugHelper::printAlarmSummary(&scheduler);
}
```

## Relación con las Plantillas Genéricas

La biblioteca AlarmScheduler también funciona con las **plantillas genéricas** en `templates/` en la raíz del repositorio:

```
ESP32-Jusaba-Librerias/
├── templates/              ← Plantillas genéricas (mínimas, universales)
│   ├── Configuracion.h
│   ├── Debug.h
│   └── README.md
└── libraries/
    └── AlarmScheduler/
        └── templates/      ← Plantillas específicas de la biblioteca (esta carpeta)
            ├── Configuracion.h
            ├── Debug.h
            └── README.md
```

### ¿Cuándo Usar Cuál?

- **Plantillas genéricas**: Iniciar un nuevo proyecto ESP32 con dependencias mínimas
- **Plantillas de AlarmScheduler**: Proyecto que utiliza específicamente funciones de programación de alarmas

### ¿Puedo Combinarlas?

¡Sí! Mejor práctica:

1. **Comienza con las plantillas genéricas** para la estructura básica
2. **Fusiona** las secciones específicas de AlarmScheduler en tu proyecto
3. **Extiende** con tu propia configuración personalizada

Ejemplo de estrategia de fusión:

```cpp
// Configuracion.h de tu proyecto
#ifndef CONFIGURACION_H
#define CONFIGURACION_H

// Mantén Config::Project y Config::Serial genéricos
namespace Config {
    namespace Project { ... }
    namespace Serial { ... }
}

// Añade secciones específicas de AlarmScheduler
namespace Config {
    namespace Alarms { ... }      // De la plantilla de AlarmScheduler
    namespace AlarmTypes { ... }  // De la plantilla de AlarmScheduler
}

// Añade tus propias secciones personalizadas
namespace Config {
    namespace MiFuncionalidad { ... }   // Tus adiciones
}

#endif
```

## Configuraciones de Ejemplo

### Sistema Simple de Campanas

```cpp
namespace Config {
    namespace AlarmTypes {
        const char TYPE_MORNING_BELL[] = "MAÑANA";
        const char TYPE_NOON_BELL[]    = "MEDIODIA";
        const char TYPE_EVENING_BELL[] = "TARDE";
    }
    
    namespace AlarmParams {
        const uint16_t DURATION_SHORT = 5;   // 5 segundos
        const uint16_t DURATION_LONG  = 30;  // 30 segundos
    }
}
```

### Iluminación Multi-Zona

```cpp
namespace Config {
    namespace AlarmTypes {
        const char TYPE_LIGHT[] = "LUZ";
    }
    
    namespace AlarmParams {
        const uint16_t ZONE_KITCHEN  = 1;
        const uint16_t ZONE_BEDROOM  = 2;
        const uint16_t ZONE_LIVING   = 3;
        const uint16_t ZONE_ALL      = 0;
    }
}
```

### Sistema de Notificaciones

```cpp
namespace Config {
    namespace AlarmTypes {
        const char TYPE_EMAIL[]    = "EMAIL";
        const char TYPE_TELEGRAM[] = "TELEGRAM";
        const char TYPE_PUSH[]     = "PUSH";
    }
}
```

## Flujo de Trabajo de Depuración

### Durante el Desarrollo

Activa todos los flags de depuración:

```cpp
#define DEBUG_ENABLED 1
#define DEBUG_ALARMSCHEDULER 1
#define DEBUG_WIFI 1
#define DEBUG_TIME 1
#define DEBUG_SPIFFS 1
```

Usa los helpers de depuración:

```cpp
void loop() {
    scheduler.check();
    
    // Imprimir estado cada minuto
    static unsigned long lastPrint = 0;
    if (millis() - lastPrint > 60000) {
        DebugHelper::printAlarmSummary(&scheduler);
        DebugHelper::printCurrentTime();
        lastPrint = millis();
    }
}
```

### En Producción

Desactiva la depuración para ahorrar memoria y mejorar el rendimiento:

```cpp
#define DEBUG_ENABLED 0
```

Todas las macros de depuración se convierten en no-ops, sin sobrecarga.

## Mejores Prácticas

1. **No modifiques** los archivos de plantilla originales - cópialos primero
2. **Control de versiones** de tus archivos personalizados
3. **Documenta** tus tipos de alarma y parámetros personalizados
4. **Usa namespaces** para organizar la configuración lógicamente
5. **Comenta** por qué se eligen ciertos valores
6. **Prueba** con depuración activada, despliega con depuración desactivada

## Ver También

- [Documentación de la Biblioteca AlarmScheduler](../README.es.md)
- [Plantillas Genéricas](../../../templates/README.md)
- [Plantillas de RTCManager](../../RTCManager/templates/README.md)
- [Sketch de Ejemplo](../examples/BasicAlarms/BasicAlarms.ino)

## Soporte

Para preguntas o problemas:
- GitHub Issues: [ESP32-Jusaba-Librerias](https://github.com/Jusaba/ESP32-Jusaba-Librerias/issues)
- Email: julian.salas.bartolome@gmail.com
