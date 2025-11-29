# AlarmScheduler

[English](README.md) | **Español**

> Biblioteca avanzada de programación de alarmas para ESP32 con gestión web, persistencia JSON y sistema flexible de callbacks.

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](../../LICENSE)
[![Platform](https://img.shields.io/badge/platform-ESP32-blue.svg)](https://www.espressif.com/en/products/socs/esp32)
[![Arduino](https://img.shields.io/badge/Arduino-Library-green.svg)](https://www.arduino.cc/)

## Características

- ✅ **Múltiples tipos de alarma**: Hora fija, comodín (cualquier hora/minuto), basada en intervalos
- ✅ **Programación por días**: Días individuales, múltiples días o todos los días
- ✅ **Gestión web**: Crear, modificar, activar/desactivar alarmas vía interfaz web
- ✅ **Persistencia JSON**: Guardar/cargar alarmas personalizables desde SPIFFS
- ✅ **Callbacks flexibles**: Métodos miembro, funciones externas (con/sin parámetros)
- ✅ **Alarmas de sistema + personalizables**: Distinguir entre alarmas fijas del sistema y editables por usuario
- ✅ **API bilingüe**: Nombres de métodos en español con alias en inglés
- ✅ **Cero dependencias**: Funciona independiente, se integra fácilmente con RTCManager para sincronización horaria
- ✅ **Ejecución eficiente**: Caché inteligente previene ejecuciones duplicadas
- ✅ **Soporte de depuración**: Macros de depuración y funciones auxiliares completas

## Instalación

### Arduino IDE

1. Descarga este repositorio como ZIP
2. En Arduino IDE: **Sketch → Incluir Biblioteca → Añadir biblioteca .ZIP**
3. Selecciona el archivo ZIP descargado
4. Reinicia Arduino IDE

### PlatformIO

Añade a tu `platformio.ini`:

```ini
lib_deps =
    https://github.com/Jusaba/ESP32-Jusaba-Librerias.git#main
    bblanchon/ArduinoJson@^6.21.0
```

### Instalación Manual

1. Clona este repositorio
2. Copia `libraries/AlarmScheduler` a tu carpeta de librerías de Arduino
3. Reinicia Arduino IDE

## Dependencias

- **ArduinoJson** (v6.x) - Serialización/deserialización JSON
- **SPIFFS** - Sistema de archivos ESP32 (incluido en el core ESP32)

### Opcional (Recomendado)

- **RTCManager** - Para sincronización horaria NTP/RTC (de este repositorio)
- **WiFi** - Para integración con servidor web

## Inicio Rápido

### Alarma Básica del Sistema

```cpp
#include <AlarmScheduler.h>

AlarmScheduler scheduler;

// Función callback
void campanaMañana(uint16_t duracion) {
    Serial.printf("Campana de la mañana sonando durante %u segundos\n", duracion);
    // Tu código de control de campana aquí
}

void setup() {
    Serial.begin(115200);
    
    // Inicializar planificador
    scheduler.begin();
    
    // Añadir alarma: Todos los días a las 08:00, llamar campanaMañana(10)
    scheduler.addExternal(
        DOW_TODOS,      // Todos los días
        8,              // 08:00 horas
        0,              // 00 minutos
        0,              // Sin intervalo (hora fija)
        campanaMañana,  // Función callback
        10              // Parámetro (duración)
    );
}

void loop() {
    scheduler.check();  // Verificar y ejecutar alarmas pendientes
    delay(1000);        // Verificar cada segundo
}
```

### Alarma Personalizable (Gestión Web)

```cpp
#include <AlarmScheduler.h>
#include <SPIFFS.h>

AlarmScheduler scheduler;

void accionCampana(uint16_t parametro) {
    Serial.printf("Acción de campana personalizada: %u\n", parametro);
}

void setup() {
    Serial.begin(115200);
    
    // Inicializar SPIFFS
    if (!SPIFFS.begin(true)) {
        Serial.println("¡Fallo al inicializar SPIFFS!");
        return;
    }
    
    // Inicializar planificador (carga alarmas guardadas desde JSON)
    scheduler.begin(true);  // true = crear por defecto si está vacío
    
    // Añadir alarma personalizable
    scheduler.addPersonalizable(
        "Campana Mañana",              // Nombre
        "Campana diaria de la mañana", // Descripción
        DOW_TODOS,                     // Todos los días
        8, 0,                          // 08:00
        "CAMPANA",                     // Cadena de tipo (para UI web)
        10,                            // Parámetro
        accionCampana,                 // Callback
        true                           // Habilitada
    );
}

void loop() {
    scheduler.check();
    delay(1000);
}
```

## Tipos de Alarma

### 1. Alarma de Hora Fija

Se ejecuta a una hora específica en días específicos:

```cpp
scheduler.addExternal(
    DOW_LUNES | DOW_MIERCOLES | DOW_VIERNES,  // Lun, Mié, Vie
    14, 30,     // 14:30
    0,          // Sin intervalo
    miCallback,
    param
);
```

### 2. Alarma Comodín

Usa `ALARM_WILDCARD` (255) para cualquier hora o minuto:

```cpp
// Ejecutar cada hora en el minuto 0 (XX:00)
scheduler.addExternal(
    DOW_TODOS,
    ALARM_WILDCARD,  // Cualquier hora
    0,               // Minuto 0
    0,
    callbackCadaHora,
    0
);

// Ejecutar cada minuto de cada hora (XX:XX)
scheduler.addExternal(
    DOW_TODOS,
    ALARM_WILDCARD,  // Cualquier hora
    ALARM_WILDCARD,  // Cualquier minuto
    0,
    callbackCadaMinuto,
    0
);
```

### 3. Alarma de Intervalo

Se ejecuta repetidamente a intervalos fijos:

```cpp
// Ejecutar cada 15 minutos, anclada en XX:00
scheduler.addExternal(
    DOW_TODOS,
    ALARM_WILDCARD,  // Cualquier hora
    0,               // Anclar en minuto 0
    15,              // Repetir cada 15 minutos
    callbackIntervalo,
    0
);
// Se ejecutará en: 00:00, 00:15, 00:30, 00:45, 01:00, 01:15, ...
```

## Referencia de la API

### Métodos Principales

#### `bool begin(bool loadDefaults = true)`

Inicializar el planificador de alarmas.

- **Parámetros:**
  - `loadDefaults`: Crear alarmas por defecto si no existe archivo JSON
- **Retorna:** `true` si tiene éxito

```cpp
scheduler.begin();        // Cargar alarmas guardadas
scheduler.begin(false);   // No crear por defecto
```

#### `void check()`

Verificar y ejecutar alarmas pendientes. **Llama esto en `loop()`**.

```cpp
void loop() {
    scheduler.check();
    delay(1000);
}
```

### Añadir Alarmas del Sistema

#### `uint8_t add(mascaraDias, hora, minuto, intervalo, metodo, parametro, habilitada)`

Añadir alarma con callback de método miembro.

```cpp
class MiClase {
    AlarmScheduler scheduler;
    
    void miMetodo(uint16_t param) {
        Serial.printf("Método llamado con: %u\n", param);
    }
    
    void setup() {
        scheduler.add(DOW_TODOS, 12, 0, 0, &MiClase::miMetodo, 42, true);
    }
};
```

#### `uint8_t addExternal(mascaraDias, hora, minuto, intervalo, callback, parametro, habilitada)`

Añadir alarma con función externa callback (con parámetro).

```cpp
void miFuncion(uint16_t param) {
    Serial.printf("Función llamada: %u\n", param);
}

scheduler.addExternal(DOW_TODOS, 9, 0, 0, miFuncion, 123, true);
```

#### `uint8_t addExternal0(mascaraDias, hora, minuto, intervalo, callback, habilitada)`

Añadir alarma con callback sin parámetros.

```cpp
void callbackSimple() {
    Serial.println("¡Callback simple!");
}

scheduler.addExternal0(DOW_TODOS, 18, 0, 0, callbackSimple, true);
```

### Gestionar Alarmas

#### `void enable(uint8_t indice)` / `void disable(uint8_t indice)`

Habilitar o deshabilitar una alarma por índice.

```cpp
scheduler.disable(0);  // Deshabilitar primera alarma
scheduler.enable(0);   // Re-habilitarla
```

#### `uint8_t count()` / `void clear()`

Obtener número de alarmas o eliminar todas.

```cpp
Serial.printf("Total de alarmas: %u\n", scheduler.count());
scheduler.clear();  // Eliminar todas las alarmas
```

#### `const Alarm* get(uint8_t indice)`

Obtener acceso de solo lectura a una alarma.

```cpp
const Alarm* alarma = scheduler.get(0);
if (alarma) {
    Serial.printf("Alarma a las %02u:%02u\n", alarma->hour, alarma->minute);
}
```

### Alarmas Personalizables (Gestión Web)

#### Nombres en Español

```cpp
// Añadir
uint8_t addPersonalizable(nombre, descripcion, mascaraDias, hora, minuto, 
                          tipoString, parametro, callback, habilitada);

// Modificar
bool modificarPersonalizable(idWeb, nombre, descripcion, mascaraDias, hora, 
                             minuto, tipoString, habilitada, callback, parametro);

// Eliminar
bool eliminarPersonalizable(int idWeb);

// Habilitar/Deshabilitar
bool habilitarPersonalizable(int idWeb, bool estado);

// Obtener JSON
String obtenerPersonalizablesJSON();
String obtenerEstadisticasJSON();

// Cargar/Guardar
bool cargarPersonalizablesDesdeJSON();
bool guardarPersonalizablesEnJSON();
```

#### Alias en Inglés

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

## Máscaras de Días

Usa OR bit a bit para combinar días:

```cpp
// Constantes en español
DOW_DOMINGO   (0x01)  // Domingo
DOW_LUNES     (0x02)  // Lunes
DOW_MARTES    (0x04)  // Martes
DOW_MIERCOLES (0x08)  // Miércoles
DOW_JUEVES    (0x10)  // Jueves
DOW_VIERNES   (0x20)  // Viernes
DOW_SABADO    (0x40)  // Sábado
DOW_TODOS     (0x7F)  // Todos los días

// Alias en inglés
DOW_SUNDAY, DOW_MONDAY, DOW_TUESDAY, DOW_WEDNESDAY,
DOW_THURSDAY, DOW_FRIDAY, DOW_SATURDAY, DOW_ALL

// Ejemplos
DOW_LUNES | DOW_MIERCOLES | DOW_VIERNES  // Lun, Mié, Vie
DOW_SABADO | DOW_DOMINGO                  // Fin de semana
DOW_TODOS                                  // Todos los días
```

## Ejemplo de Integración Web

```cpp
#include <WebServer.h>
#include <AlarmScheduler.h>

WebServer server(80);
AlarmScheduler scheduler;

void handleObtenerAlarmas() {
    String json = scheduler.obtenerPersonalizablesJSON();
    server.send(200, "application/json", json);
}

void handleAñadirAlarma() {
    // Parsear datos POST
    String nombre = server.arg("nombre");
    int dia = server.arg("dia").toInt();
    int hora = server.arg("hora").toInt();
    int minuto = server.arg("minuto").toInt();
    
    uint8_t mascaraDias = (dia == 0) ? DOW_TODOS : (1 << (dia - 1));
    
    scheduler.addPersonalizable(
        nombre.c_str(),
        "Alarma creada por usuario",
        mascaraDias, hora, minuto,
        "PERSONALIZADA", 0,
        miCallback,
        true
    );
    
    server.send(200, "text/plain", "Alarma añadida");
}

void handleEliminarAlarma() {
    int id = server.arg("id").toInt();
    bool exito = scheduler.eliminarPersonalizable(id);
    server.send(exito ? 200 : 404, "text/plain", 
                exito ? "Eliminada" : "No encontrada");
}

void setup() {
    SPIFFS.begin(true);
    scheduler.begin();
    
    server.on("/api/alarmas", HTTP_GET, handleObtenerAlarmas);
    server.on("/api/alarmas/añadir", HTTP_POST, handleAñadirAlarma);
    server.on("/api/alarmas/eliminar", HTTP_DELETE, handleEliminarAlarma);
    
    server.begin();
}

void loop() {
    server.handleClient();
    scheduler.check();
}
```

## Formato JSON

### Archivo de Alarmas Personalizables (`/customizable_alarms.json`)

```json
{
  "version": "1.0",
  "timestamp": 123456,
  "total": 2,
  "alarms": [
    {
      "id": 1,
      "name": "Campana Mañana",
      "description": "Campana diaria de la mañana",
      "day": 0,
      "hour": 8,
      "minute": 0,
      "action": "CAMPANA",
      "parameter": 10,
      "enabled": true
    },
    {
      "id": 2,
      "name": "Luz Tarde",
      "description": "Encender luces de la tarde",
      "day": 0,
      "hour": 18,
      "minute": 30,
      "action": "LUZ",
      "parameter": 1,
      "enabled": true
    }
  ]
}
```

### Respuesta JSON de Obtener Alarmas

```json
{
  "version": "1.0",
  "timestamp": 123456,
  "total": 2,
  "alarms": [
    {
      "id": 1,
      "name": "Campana Mañana",
      "description": "Campana diaria de la mañana",
      "day": 0,
      "dayName": "Every day",
      "hour": 8,
      "minute": 0,
      "timeText": "08:00",
      "action": "CAMPANA",
      "parameter": 10,
      "enabled": true,
      "arrayIndex": 0
    }
  ]
}
```

## Plantillas

La carpeta `templates/` contiene archivos de ejemplo de configuración y depuración:

- **Configuracion.h** - Configuración del proyecto, tipos de alarma, parámetros
- **Debug.h** - Macros de depuración y funciones auxiliares
- **README.md** - Guía detallada de uso de plantillas

Consulta [templates/README.es.md](templates/README.es.md) para documentación completa.

## Ejemplos

Consulta [examples/BasicAlarms](examples/BasicAlarms/) para un ejemplo completo funcional.

## Integración con RTCManager

Para sincronización horaria precisa, usa con RTCManager:

```cpp
#include <RTCManager.h>
#include <AlarmScheduler.h>

RTCManager rtc;
AlarmScheduler scheduler;

void setup() {
    // Inicializar RTC con sincronización NTP
    const char* servidores[] = {"pool.ntp.org", "time.nist.gov"};
    rtc.beginConMultiplesServidores(servidores, 2, 3600, 3600);
    
    // Inicializar planificador
    scheduler.begin();
}

void loop() {
    rtc.check();        // Mantener la hora sincronizada
    scheduler.check();  // Verificar alarmas
    delay(1000);
}
```

## Depuración

Habilitar salida de depuración:

```cpp
#define DEBUG_ENABLED 1
#define DEBUG_ALARMSCHEDULER 1
#include "Debug.h"
#include <AlarmScheduler.h>

AlarmScheduler scheduler;

void setup() {
    Serial.begin(115200);
    scheduler.begin();
    
    // Imprimir todas las alarmas
    scheduler.printAllAlarms();
    
    // Usar funciones auxiliares de depuración
    DebugHelper::printAlarmSummary(&scheduler);
    DebugHelper::printCurrentTime();
}
```

## Uso Avanzado

### Acciones de Alarma Personalizadas por Tipo

```cpp
void ejecutarAccionAlarma(const char* tipoString, uint16_t parametro) {
    if (strcmp(tipoString, "CAMPANA") == 0) {
        tocarCampana(parametro);  // parametro = duración
    } else if (strcmp(tipoString, "LUZ") == 0) {
        cambiarLuz(parametro);  // parametro = zona
    } else if (strcmp(tipoString, "NOTIFICACION") == 0) {
        enviarNotificacion(parametro);  // parametro = ID de mensaje
    }
}

void callbackGenerico(uint16_t param) {
    const Alarm* alarma = scheduler.get(/* buscar por contexto de ejecución */);
    if (alarma) {
        ejecutarAccionAlarma(alarma->typeString, alarma->parameter);
    }
}
```

### Modificación Dinámica de Alarmas

```cpp
// Encontrar alarma por ID web y modificarla
int idWeb = 5;
scheduler.modificarPersonalizable(
    idWeb,
    "Nombre Actualizado",
    "Descripción actualizada",
    DOW_LUNES | DOW_VIERNES,  // Lun y Vie
    9, 30,                     // 09:30
    "CAMPANA",
    true,                      // habilitada
    miCallback,
    15                         // nuevo parámetro
);
```

### Estadísticas y Monitorización

```cpp
String stats = scheduler.obtenerEstadisticasJSON();
// Retorna:
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

## Solución de Problemas

### Las alarmas no se ejecutan

1. **Verificar sincronización horaria**: Asegurar que `getLocalTime()` funciona
2. **Verificar que la alarma está habilitada**: `alarm->enabled == true`
3. **Verificar máscara de días**: La alarma debe coincidir con el día de la semana actual
4. **Llamar a `check()` regularmente**: Al menos una vez por segundo
5. **Habilitar depuración**: Ver qué está sucediendo

### JSON no se guarda

1. **Inicializar SPIFFS**: Llamar `SPIFFS.begin(true)` antes de `scheduler.begin()`
2. **Verificar espacio libre**: Usar `SPIFFS.totalBytes()` y `usedBytes()`
3. **Verificar que existe el archivo**: `SPIFFS.exists("/customizable_alarms.json")`

### Ejecuciones duplicadas

- La biblioteca previene esto automáticamente vía caché
- Si sucede, verificar que tu callback es idempotente
- Resetear caché con `scheduler.resetCache()` después de cambios de hora

## Contribuir

Consulta [CONTRIBUTING.md](../../docs/contributing.md) para las directrices.

## Licencia

Licencia MIT - ver archivo [LICENSE](../../LICENSE).

## Autor

**Julián Salas Bartolomé**
- Email: julian.salas.bartolome@gmail.com
- GitHub: [@Jusaba](https://github.com/Jusaba)

## Registro de Cambios

### v1.0.0 (2025-11-29)
- Lanzamiento inicial
- Alarmas de hora fija, comodín e intervalo
- Gestión web con persistencia JSON
- API bilingüe (Español + Inglés)
- Integración con SPIFFS
- Soporte completo de depuración

## Bibliotecas Relacionadas

- [RTCManager](../RTCManager/) - Sincronización horaria NTP/RTC
- [ArduinoJson](https://github.com/bblanchon/ArduinoJson) - Manejo de JSON

## Soporte

- 📧 Email: julian.salas.bartolome@gmail.com
- 🐛 Issues: [GitHub Issues](https://github.com/Jusaba/ESP32-Jusaba-Librerias/issues)
- 📖 Wiki: [GitHub Wiki](https://github.com/Jusaba/ESP32-Jusaba-Librerias/wiki)
