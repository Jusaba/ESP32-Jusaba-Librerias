# RTCManager

Librería avanzada de sincronización NTP para ESP32 con soporte de múltiples servidores, validación de fechas y manejo robusto de errores.

## ✨ Características

- ✅ **Múltiples servidores NTP** - Hasta 3 servidores con fallback automático
- ✅ **Validación de fechas** - Verifica fechas realistas (2020-2050)
- ✅ **Timeout configurable** - Evita bloqueos indefinidos
- ✅ **Zona horaria automática** - Soporte GMT y horario de verano
- ✅ **Debug opcional** - Logging detallado para troubleshooting
- ✅ **Thread-safe** - Operaciones seguras en multi-core
- ✅ **Sin dependencias** - Solo WiFi.h incluido en ESP32

## 📦 Instalación

### Arduino IDE
1. Descarga la carpeta `RTCManager`
2. Copia a `Documents/Arduino/libraries/`
3. Reinicia Arduino IDE

### PlatformIO
```ini
lib_deps = 
    https://github.com/Jusaba/ESP32-Jusaba-Librerias.git#libraries/RTCManager
```

## 🚀 Uso Rápido

```cpp
#include <WiFi.h>
#include <RTCManager.h>

void setup() {
    Serial.begin(115200);
    
    // Conectar WiFi
    WiFi.begin("SSID", "password");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
    
    // Sincronizar NTP (recomendado)
    if (RTC::beginConMultiplesServidores()) {
        Serial.println("✅ Hora sincronizada: " + RTC::getTimeStr());
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

### RTC::beginConMultiplesServidores()
Sincronización NTP con múltiples servidores (recomendado).

```cpp
bool RTC::beginConMultiplesServidores(unsigned long timeout_ms = 15000);
```

**Parámetros:**
- `timeout_ms` - Timeout en milisegundos (defecto: 15000)

**Retorna:**
- `true` - Sincronización exitosa
- `false` - Timeout o error

**Ejemplo:**
```cpp
// Timeout de 30 segundos
if (RTC::beginConMultiplesServidores(30000)) {
    Serial.println("NTP OK");
}
```

### RTC::begin()
Sincronización NTP con un solo servidor.

```cpp
void RTC::begin(
    const char* ntpServer = NTP_SERVER1,
    long gmtOffsetSec = GMT_OFFSET_SEC,
    int daylightOffsetSec = DAYLIGHT_OFFSET_SEC,
    unsigned long timeout_ms = 10000
);
```

**Parámetros:**
- `ntpServer` - Servidor NTP (defecto: "pool.ntp.org")
- `gmtOffsetSec` - Offset GMT en segundos (defecto: 3600 para GMT+1)
- `daylightOffsetSec` - Horario de verano en segundos (defecto: 3600)
- `timeout_ms` - Timeout en milisegundos (defecto: 10000)

**Ejemplo:**
```cpp
// GMT-5 (Nueva York), sin horario verano
RTC::begin("time.google.com", -5*3600, 0);
```

### RTC::isNtpSync()
Verifica estado de sincronización.

```cpp
bool RTC::isNtpSync();
```

**Retorna:**
- `true` - Sincronización exitosa
- `false` - Sin sincronización

**Ejemplo:**
```cpp
if (RTC::isNtpSync()) {
    // Usar fecha/hora con confianza
}
```

### RTC::getTimeStr()
Obtiene fecha/hora actual formateada.

```cpp
String RTC::getTimeStr();
```

**Retorna:**
- String formato "YYYY-MM-DD HH:MM:SS"
- "Error obteniendo hora" si falla

**Ejemplo:**
```cpp
String hora = RTC::getTimeStr();
Serial.println(hora);  // 2025-11-28 15:30:45
```

## ⚙️ Configuración

### Servidores NTP Personalizados

Define antes de `#include <RTCManager.h>`:

```cpp
#define NTP_SERVER1 "pool.ntp.org"
#define NTP_SERVER2 "time.cloudflare.com"
#define NTP_SERVER3 "time.google.com"

#include <RTCManager.h>
```

### Zona Horaria

```cpp
// GMT+1 (España)
#define GMT_OFFSET_SEC 3600
#define DAYLIGHT_OFFSET_SEC 3600

// GMT-5 (Nueva York)
#define GMT_OFFSET_SEC -18000
#define DAYLIGHT_OFFSET_SEC 0

// GMT+8 (China)
#define GMT_OFFSET_SEC 28800
#define DAYLIGHT_OFFSET_SEC 0

#include <RTCManager.h>
```

### Habilitar Debug

```cpp
#define RTCMANAGER_DEBUG
#include <RTCManager.h>
```

Muestra en Serial:
```
[RTC] Iniciando sincronización NTP con múltiples servidores...
[RTC] Servidores NTP configurados:
[RTC]   - Servidor 1: pool.ntp.org
[RTC]   - Servidor 2: es.pool.ntp.org
[RTC]   - Servidor 3: time.google.com
[RTC] ✅ Sincronización NTP exitosa:
[RTC]    Fecha/Hora: 2025-11-28 15:30:45
```

## 📝 Ejemplos

Ver carpeta `examples/` para más ejemplos completos.

## 🔧 Troubleshooting

### No sincroniza
- ✅ Verifica conexión WiFi activa
- ✅ Aumenta timeout: `beginConMultiplesServidores(60000)`
- ✅ Activa debug: `#define RTCMANAGER_DEBUG`
- ✅ Verifica firewall no bloquea puerto UDP 123

### Fecha incorrecta
- ✅ Verifica `GMT_OFFSET_SEC` correcto para tu zona
- ✅ Ajusta `DAYLIGHT_OFFSET_SEC` según horario de verano
- ✅ Usa `beginConMultiplesServidores()` en lugar de `begin()`

### Bloqueos
- ✅ Reduce timeout si red es lenta
- ✅ Llama después de conectar WiFi
- ✅ No llames en `loop()`, solo en `setup()`

## 📄 Licencia

MIT License - Ver LICENSE en el repositorio principal

## 👤 Autor

**Julián Salas Bartolomé**
- GitHub: [@Jusaba](https://github.com/Jusaba)

## 🔗 Enlaces

- [Repositorio principal](https://github.com/Jusaba/ESP32-Jusaba-Librerias)
- [Reportar issues](https://github.com/Jusaba/ESP32-Jusaba-Librerias/issues)
