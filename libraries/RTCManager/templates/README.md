# Plantillas de Configuración

Esta carpeta contiene archivos plantilla reutilizables para tus proyectos ESP32.

## 📄 Archivos Disponibles

### Debug.h
Sistema modular de debug con macros condicionales.

**Características:**
- ✅ Debug por módulos (WiFi, OTA, Server, etc.)
- ✅ Cero overhead en producción
- ✅ Soporte printf con formato
- ✅ Helper para info del sistema

**Uso básico:**
```cpp
#define DEBUG              // Habilitar debug general
#define DEBUGWIFI          // Habilitar debug WiFi
#include "Debug.h"

void setup() {
    Serial.begin(115200);
    DBG("Sistema iniciado");
    DBG_WIFI("Conectando...");
    DebugHelper::printSystemInfo();
}
```

### Configuracion.h
Sistema centralizado de configuración con namespaces.

**Características:**
- ✅ Organización modular
- ✅ Evita colisiones de nombres
- ✅ Type-safe
- ✅ Valores constexpr y runtime

**Uso básico:**
```cpp
#include "Configuracion.h"

void setup() {
    Serial.begin(Config::Serial::BAUD_RATE);
    pinMode(Config::Pins::LED_BUILTIN, OUTPUT);
    WiFi.begin(Config::WiFi::SSID, Config::WiFi::PASSWORD);
}

void loop() {
    if (Config::Debug::ENABLED) {
        Serial.println("Debug activo");
    }
}
```

## 🚀 Cómo Usar

1. **Copiar a tu proyecto:**
   ```bash
   cp templates/Debug.h tu_proyecto/
   cp templates/Configuracion.h tu_proyecto/
   ```

2. **Personalizar:**
   - Abre los archivos y modifica los valores
   - Añade/elimina módulos según necesites
   - Comenta #define que no uses

3. **Incluir en tu código:**
   ```cpp
   #include "Debug.h"
   #include "Configuracion.h"
   ```

## 📝 Personalización

### Añadir módulo de debug

En `Debug.h`:
```cpp
// 1. Definir el módulo
#define DEBUGMIMODULO

// 2. Crear las macros
#ifdef DEBUGMIMODULO
    #define DBG_MIMODULO(msg) Serial.println(String("[MIMODULO] ") + msg)
    #define DBG_MIMODULO_PRINTF(fmt, ...) Serial.printf("[MIMODULO] " fmt "\n", ##__VA_ARGS__)
#else
    #define DBG_MIMODULO(msg)
    #define DBG_MIMODULO_PRINTF(fmt, ...)
#endif

// 3. Usar en tu código
DBG_MIMODULO("Inicializando mi módulo");
```

### Añadir namespace de configuración

En `Configuracion.h`:
```cpp
namespace Config {
    namespace MiModulo {
        constexpr int PIN = 25;
        inline String NOMBRE = "Mi Módulo";
        constexpr unsigned long INTERVALO_MS = 5000;
    }
}

// Usar:
pinMode(Config::MiModulo::PIN, OUTPUT);
Serial.println(Config::MiModulo::NOMBRE);
```

## 💡 Mejores Prácticas

### Debug.h
- ✅ Comenta todos los #define en producción
- ✅ Usa DBG_PRINTF para mensajes con formato
- ✅ Activa solo los módulos que necesites debugear
- ✅ Usa DebugHelper para info del sistema

### Configuracion.h
- ✅ Usa `constexpr` para valores fijos (pines, baudrate)
- ✅ Usa `inline` para valores que pueden cambiar (SSID, passwords)
- ✅ Agrupa configuración relacionada en namespaces
- ✅ Documenta valores por defecto

## 🔧 Ejemplos Completos

### Proyecto IoT Básico

```cpp
// main.ino
#define DEBUG
#define DEBUGWIFI
#include "Debug.h"
#include "Configuracion.h"

void setup() {
    Serial.begin(Config::Serial::BAUD_RATE);
    DBG("Sistema iniciando...");
    
    pinMode(Config::Pins::LED_BUILTIN, OUTPUT);
    
    DBG_WIFI("Conectando a WiFi...");
    WiFi.begin(Config::WiFi::SSID, Config::WiFi::PASSWORD);
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        DBG_WIFI_PRINT(".");
    }
    
    DBG_WIFI_PRINTF("Conectado! IP: %s", WiFi.localIP().toString().c_str());
    DebugHelper::printSystemInfo();
}

void loop() {
    digitalWrite(Config::Pins::LED_BUILTIN, HIGH);
    delay(Config::Timing::LOOP_DELAY_MS);
    digitalWrite(Config::Pins::LED_BUILTIN, LOW);
    delay(Config::Timing::LOOP_DELAY_MS);
}
```

### Servidor Web con OTA

```cpp
#define DEBUG
#define DEBUGSERVER
#define DEBUGOTA
#include "Debug.h"
#include "Configuracion.h"
#include <WebServer.h>

WebServer server(Config::WebServer::PORT);

void setup() {
    Serial.begin(Config::Serial::BAUD_RATE);
    WiFi.begin(Config::WiFi::SSID, Config::WiFi::PASSWORD);
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
    
    server.on("/", []() {
        DBG_SRV("Petición recibida en /");
        server.send(200, "text/plain", "Hola desde ESP32!");
    });
    
    server.begin();
    DBG_SRV_PRINTF("Servidor iniciado en puerto %d", Config::WebServer::PORT);
}

void loop() {
    server.handleClient();
}
```

## 📚 Recursos Adicionales

- [Documentación RTCManager](../README.md)
- [Guía de contribución](../../../docs/contributing.md)
- [Repositorio principal](https://github.com/Jusaba/ESP32-Jusaba-Librerias)

## 📄 Licencia

MIT License - Ver LICENSE en el repositorio principal
