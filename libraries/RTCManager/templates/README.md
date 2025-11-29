# Plantillas para Proyectos con RTCManager

Esta carpeta contiene archivos de configuración y debug **específicos para usar con RTCManager**, listos para copiar a tus proyectos.

---

## 📄 Archivos Disponibles

### 🐛 Debug.h
Sistema de debug específico para RTCManager con macros condicionales.

**Incluye 3 niveles de debug:**
- `DEBUG` → Mensajes generales del sistema
- `DEBUGWIFI` → Debug de conexión WiFi (necesario para NTP)
- `DEBUGRTC` → Debug de sincronización NTP/RTC

**Características clave:**
- ✅ Específico para RTCManager (no incluye módulos innecesarios)
- ✅ Cero overhead en producción (se elimina al compilar)
- ✅ Helpers útiles: `printWiFiStatus()`, `printCurrentTime()`
- ✅ Macros con formato printf: `DBG_PRINTF()`, `DBG_RTC_PRINTF()`

---

### ⚙️ Configuracion.h
Configuración centralizada para RTCManager con namespaces organizados.

**Incluye solo lo necesario para RTCManager:**
- `Config::Project` → Información del proyecto
- `Config::Serial` → Baudrate para comunicación
- `Config::WiFi` → Credenciales WiFi (necesarias para NTP)
- `Config::Time` → **Configuración NTP/RTC** ⭐
  - Servidores NTP
  - Zona horaria (GMT offset)
  - Horario de verano (DST)
  - Intervalos de sincronización

**Características clave:**
- ✅ Solo configuración relevante para RTCManager
- ✅ Tabla de zonas horarias (España, Latinoamérica, etc.)
- ✅ Valores `constexpr` (evaluados en compilación)
- ✅ Valores `inline` (modificables en runtime)

---

## 🚀 Cómo Usar en Proyectos Grandes

### Paso 1: Copiar los Archivos a Tu Proyecto

Si tu proyecto tiene esta estructura:
```
MiProyecto/
├── MiProyecto.ino          ← Tu sketch principal
├── otras_funciones.cpp
├── otras_funciones.h
└── libraries/
    └── RTCManager/         ← Librería instalada
```

**Copia los archivos al mismo nivel que tu `.ino`:**

```bash
# Desde la carpeta de RTCManager
cp libraries/RTCManager/templates/Configuracion.h MiProyecto/
cp libraries/RTCManager/templates/Debug.h MiProyecto/
```

Resultado:
```
MiProyecto/
├── MiProyecto.ino          ← Tu sketch principal
├── Configuracion.h         ← ✅ Copiado
├── Debug.h                 ← ✅ Copiado
├── otras_funciones.cpp
├── otras_funciones.h
└── libraries/
    └── RTCManager/
```

---

### Paso 2: Personalizar los Archivos

#### A) Editar `Configuracion.h`

Abre `Configuracion.h` y personaliza:

```cpp
namespace Config {
    // 1. Información de tu proyecto
    namespace Project {
        inline String NAME = "Reloj Digital IoT";      // ← Tu nombre
        inline String VERSION = "2.1.0";               // ← Tu versión
        inline String AUTHOR = "Juan Pérez";           // ← Tu nombre
    }
    
    // 2. Configuración WiFi (OBLIGATORIO para NTP)
    namespace WiFi {
        inline String SSID = "MiWiFi_2.4GHz";         // ← Tu SSID
        inline String PASSWORD = "mipassword123";      // ← Tu contraseña
    }
    
    // 3. Zona horaria (ajusta según tu ubicación)
    namespace Time {
        // España (península): GMT+1 = 3600
        // Argentina: GMT-3 = -10800
        // México (centro): GMT-6 = -21600
        constexpr long GMT_OFFSET_SEC = 3600;          // ← Tu zona
        constexpr int DAYLIGHT_OFFSET_SEC = 3600;      // ← Horario verano
        
        // Servidores NTP (puedes usar los regionales)
        inline String NTP_SERVER1 = "es.pool.ntp.org"; // ← Opcional
        inline String NTP_SERVER2 = "time.google.com";
        inline String NTP_SERVER3 = "time.cloudflare.com";
    }
}
```

#### B) Editar `Debug.h`

Abre `Debug.h` y activa el debug que necesites:

```cpp
// Durante desarrollo: activa todo
#define DEBUG          // ← Mensajes generales
#define DEBUGWIFI      // ← Debug WiFi
#define DEBUGRTC       // ← Debug NTP/RTC

// Para depurar solo WiFi: comenta DEBUG y DEBUGRTC
//#define DEBUG
#define DEBUGWIFI
//#define DEBUGRTC

// Para producción: comenta TODO
//#define DEBUG
//#define DEBUGWIFI
//#define DEBUGRTC
```

---

### Paso 3: Usar en Tu Sketch Principal

#### Estructura Recomendada del `.ino`

```cpp
// ========== ARCHIVO: MiProyecto.ino ==========

// 1. Primero: Activar debug (ANTES de incluir Debug.h)
#define DEBUG
#define DEBUGWIFI
#define DEBUGRTC

// 2. Incluir archivos de configuración
#include "Debug.h"
#include "Configuracion.h"

// 3. Incluir librerías
#include <WiFi.h>
#include <RTCManager.h>

// 4. Declarar objetos globales
RTCManager rtc;

// ========================================
// SETUP
// ========================================
void setup() {
    // Inicializar Serial con baudrate configurado
    Serial.begin(Config::Serial::BAUD_RATE);
    delay(1000);
    
    // Mostrar información de inicio
    DebugHelper::printSeparator();
    DBG("Proyecto: " + Config::Project::NAME);
    DBG("Versión: " + Config::Project::VERSION);
    DebugHelper::printSystemInfo();
    DebugHelper::printSeparator();
    
    // Conectar WiFi usando configuración
    conectarWiFi();
    
    // Configurar NTP usando configuración
    configurarNTP();
    
    // Inicializar RTCManager
    DBG("Inicializando RTCManager...");
    rtc.begin();
    
    DBG("Sistema listo!");
}

// ========================================
// LOOP
// ========================================
void loop() {
    // Mostrar hora cada segundo
    static unsigned long ultimoDisplay = 0;
    if (millis() - ultimoDisplay >= 1000) {
        DebugHelper::printCurrentTime();
        ultimoDisplay = millis();
    }
    
    // Tu código aquí...
}

// ========================================
// FUNCIONES AUXILIARES
// ========================================

void conectarWiFi() {
    DBG_WIFI("Conectando a WiFi: " + Config::WiFi::SSID);
    WiFi.begin(Config::WiFi::SSID, Config::WiFi::PASSWORD);
    
    int intentos = 0;
    while (WiFi.status() != WL_CONNECTED && intentos < 20) {
        delay(500);
        DBG_WIFI_PRINT(".");
        intentos++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        DBG_WIFI("\n✓ WiFi conectado!");
        DebugHelper::printWiFiStatus();
    } else {
        DBG_WIFI("\n✗ ERROR: No se pudo conectar a WiFi");
    }
}

void configurarNTP() {
    DBG_RTC("Configurando sincronización NTP...");
    DBG_RTC_PRINTF("Zona horaria: GMT%+ld", Config::Time::GMT_OFFSET_SEC / 3600);
    
    configTime(Config::Time::GMT_OFFSET_SEC,
               Config::Time::DAYLIGHT_OFFSET_SEC,
               Config::Time::NTP_SERVER1.c_str(),
               Config::Time::NTP_SERVER2.c_str(),
               Config::Time::NTP_SERVER3.c_str());
    
    delay(2000);  // Esperar sincronización inicial
    
    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
        DBG_RTC("✓ NTP sincronizado correctamente");
        DebugHelper::printCurrentTime();
    } else {
        DBG_RTC("⚠ Advertencia: NTP no sincronizado aún");
    }
}
```

---

### Paso 4: Usar en Múltiples Archivos (Proyectos Grandes)

Si tu proyecto tiene **múltiples archivos** `.cpp` y `.h`:

#### Estructura del Proyecto Grande:
```
MiProyectoGrande/
├── MiProyectoGrande.ino    ← Sketch principal
├── Configuracion.h          ← Configuración compartida
├── Debug.h                  ← Debug compartido
├── pantalla.cpp
├── pantalla.h
├── sensores.cpp
├── sensores.h
├── wifi_manager.cpp
└── wifi_manager.h
```

#### Incluir en Cada Archivo que lo Necesite:

**Archivo: `wifi_manager.h`**
```cpp
#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>
#include "Debug.h"           // ← Incluir Debug
#include "Configuracion.h"   // ← Incluir Config

class WiFiManager {
public:
    void conectar();
    void verificarConexion();
};

#endif
```

**Archivo: `wifi_manager.cpp`**
```cpp
#include "wifi_manager.h"

void WiFiManager::conectar() {
    DBG_WIFI("Iniciando conexión WiFi...");  // ← Usar macros
    WiFi.begin(Config::WiFi::SSID, Config::WiFi::PASSWORD);  // ← Usar config
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        DBG_WIFI_PRINT(".");
    }
    DBG_WIFI("\nConectado!");
}

void WiFiManager::verificarConexion() {
    if (WiFi.status() == WL_CONNECTED) {
        DBG_WIFI_PRINTF("WiFi OK - IP: %s", WiFi.localIP().toString().c_str());
    } else {
        DBG_WIFI("WiFi desconectado - Reconectando...");
        conectar();
    }
}
```

**Archivo: `pantalla.cpp`**
```cpp
#include "pantalla.h"
#include "Debug.h"           // ← También aquí
#include "Configuracion.h"

void Pantalla::mostrarHora() {
    DBG("Actualizando pantalla con hora actual");  // ← Debug general
    
    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
        // Mostrar en pantalla...
        DBG_RTC_PRINTF("Mostrando: %02d:%02d:%02d", 
                       timeinfo.tm_hour, 
                       timeinfo.tm_min, 
                       timeinfo.tm_sec);
    }
}
```

---

## 🎯 Ventajas de Este Sistema

### ✅ Centralización
- **Antes**: WiFi credentials dispersas en varios archivos
- **Ahora**: Todo en `Configuracion.h`, se modifica una vez

### ✅ Debug Controlado
- **Desarrollo**: Activa `DEBUG`, `DEBUGWIFI`, `DEBUGRTC`
- **Producción**: Comenta todo → binario más pequeño y rápido

### ✅ Mantenimiento Fácil
- Cambiar zona horaria: editar 1 línea en `Configuracion.h`
- Cambiar WiFi: editar 2 líneas en `Configuracion.h`
- Todos los archivos usan automáticamente los nuevos valores

### ✅ Type-Safe
- El compilador detecta errores de tipo
- Autocompletado en IDEs funciona perfectamente

### ✅ Sin Colisiones
- Namespaces evitan conflictos: `Config::Time::GMT_OFFSET_SEC`
- Claro de dónde viene cada valor

---

## 📝 Ejemplos Prácticos

### Ejemplo 1: Reloj Digital Simple

```cpp
#define DEBUG
#define DEBUGRTC
#include "Debug.h"
#include "Configuracion.h"
#include <WiFi.h>
#include <RTCManager.h>

RTCManager rtc;

void setup() {
    Serial.begin(Config::Serial::BAUD_RATE);
    
    // Conectar WiFi
    WiFi.begin(Config::WiFi::SSID, Config::WiFi::PASSWORD);
    while (WiFi.status() != WL_CONNECTED) delay(500);
    DBG("WiFi conectado");
    
    // Configurar NTP
    configTime(Config::Time::GMT_OFFSET_SEC, 
               Config::Time::DAYLIGHT_OFFSET_SEC,
               Config::Time::NTP_SERVER1.c_str());
    delay(2000);
    
    DebugHelper::printCurrentTime();
}

void loop() {
    DebugHelper::printCurrentTime();
    delay(1000);
}
```

---

### Ejemplo 2: Proyecto Multi-Archivo (Reloj con Display)

**Estructura del proyecto:**
```
RelojDisplay/
├── RelojDisplay.ino       ← Main
├── Configuracion.h        ← Config compartida
├── Debug.h                ← Debug compartido
├── display.cpp
├── display.h
├── ntp_sync.cpp
└── ntp_sync.h
```

**Archivo: `RelojDisplay.ino`**
```cpp
#define DEBUG
#define DEBUGWIFI
#define DEBUGRTC

#include "Debug.h"
#include "Configuracion.h"
#include "display.h"
#include "ntp_sync.h"

Display pantalla;
NTPSync sincronizador;

void setup() {
    Serial.begin(Config::Serial::BAUD_RATE);
    
    DBG("Iniciando " + Config::Project::NAME);
    DebugHelper::printSystemInfo();
    
    // Inicializar componentes
    pantalla.inicializar();
    sincronizador.conectarYSincronizar();
}

void loop() {
    sincronizador.mantenerSincronizado();
    pantalla.actualizarHora();
    delay(100);
}
```

**Archivo: `ntp_sync.h`**
```cpp
#ifndef NTP_SYNC_H
#define NTP_SYNC_H

#include <WiFi.h>
#include "Debug.h"
#include "Configuracion.h"

class NTPSync {
public:
    void conectarYSincronizar();
    void mantenerSincronizado();
    
private:
    void conectarWiFi();
    void configurarNTP();
    unsigned long ultimaSync = 0;
};

#endif
```

**Archivo: `ntp_sync.cpp`**
```cpp
#include "ntp_sync.h"

void NTPSync::conectarYSincronizar() {
    conectarWiFi();
    configurarNTP();
}

void NTPSync::conectarWiFi() {
    DBG_WIFI("Conectando a: " + Config::WiFi::SSID);
    WiFi.begin(Config::WiFi::SSID, Config::WiFi::PASSWORD);
    
    int intentos = 0;
    while (WiFi.status() != WL_CONNECTED && intentos < 30) {
        delay(500);
        DBG_WIFI_PRINT(".");
        intentos++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        DBG_WIFI("\nWiFi conectado!");
        DebugHelper::printWiFiStatus();
    } else {
        DBG_WIFI("\nERROR: WiFi no conectado");
    }
}

void NTPSync::configurarNTP() {
    DBG_RTC("Configurando NTP...");
    
    configTime(Config::Time::GMT_OFFSET_SEC,
               Config::Time::DAYLIGHT_OFFSET_SEC,
               Config::Time::NTP_SERVER1.c_str(),
               Config::Time::NTP_SERVER2.c_str(),
               Config::Time::NTP_SERVER3.c_str());
    
    delay(3000);
    
    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
        DBG_RTC("NTP sincronizado correctamente");
        DebugHelper::printCurrentTime();
    }
    
    ultimaSync = millis();
}

void NTPSync::mantenerSincronizado() {
    // Re-sincronizar cada hora (Config::Time::NTP_SYNC_INTERVAL_MS)
    if (millis() - ultimaSync >= Config::Time::NTP_SYNC_INTERVAL_MS) {
        DBG_RTC("Re-sincronizando NTP...");
        configurarNTP();
    }
}
```

**Archivo: `display.h`**
```cpp
#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include "Debug.h"

class Display {
public:
    void inicializar();
    void actualizarHora();
    
private:
    unsigned long ultimaActualizacion = 0;
};

#endif
```

**Archivo: `display.cpp`**
```cpp
#include "display.h"

void Display::inicializar() {
    DBG("Inicializando display...");
    // Configurar pines, I2C, SPI, etc.
    DBG("Display listo");
}

void Display::actualizarHora() {
    // Actualizar cada segundo
    if (millis() - ultimaActualizacion >= 1000) {
        struct tm timeinfo;
        if (getLocalTime(&timeinfo)) {
            // Mostrar en display
            DBG_PRINTF("Display: %02d:%02d:%02d", 
                      timeinfo.tm_hour, 
                      timeinfo.tm_min, 
                      timeinfo.tm_sec);
        }
        ultimaActualizacion = millis();
    }
}
```

---

### Ejemplo 3: Sistema de Alarmas (Proyecto Complejo)

**Estructura:**
```
SistemaAlarmas/
├── SistemaAlarmas.ino
├── Configuracion.h          ← Añade namespace Alarmas
├── Debug.h
├── alarma.cpp
├── alarma.h
├── wifi_manager.cpp
├── wifi_manager.h
├── display_manager.cpp
├── display_manager.h
└── storage_manager.cpp
```

**Personalizar `Configuracion.h` añadiendo:**
```cpp
namespace Config {
    // ... namespaces existentes ...
    
    // Añadir configuración específica del proyecto
    namespace Alarmas {
        constexpr int MAX_ALARMAS = 5;
        constexpr int PIN_BUZZER = 25;
        constexpr int PIN_LED_ALARMA = 2;
        inline String ARCHIVO_ALARMAS = "/alarmas.json";
    }
}
```

**Usar en tu código:**
```cpp
#include "Configuracion.h"
#include "alarma.h"

void Alarma::cargarDesdeArchivo() {
    File file = SPIFFS.open(Config::Alarmas::ARCHIVO_ALARMAS, "r");
    // ... leer alarmas ...
}

void Alarma::activarBuzzer() {
    pinMode(Config::Alarmas::PIN_BUZZER, OUTPUT);
    digitalWrite(Config::Alarmas::PIN_BUZZER, HIGH);
}
```

---

## 💡 Mejores Prácticas

### ✅ Para Debug.h

**Durante Desarrollo:**
```cpp
#define DEBUG          // ← Activa para ver todo
#define DEBUGWIFI      // ← Útil si hay problemas de conexión
#define DEBUGRTC       // ← Útil si NTP no sincroniza
```

**Antes de Subir a GitHub:**
```cpp
//#define DEBUG        // ← Comenta para reducir spam en logs
#define DEBUGWIFI      // ← Deja solo lo necesario
//#define DEBUGRTC
```

**Para Producción Final:**
```cpp
//#define DEBUG        // ← Todo comentado
//#define DEBUGWIFI    // ← Binario más pequeño
//#define DEBUGRTC     // ← Mayor velocidad
```

### ✅ Para Configuracion.h

**Valores Fijos (usar `constexpr`):**
```cpp
constexpr int PIN = 25;                  // ← Nunca cambia
constexpr long GMT_OFFSET_SEC = 3600;    // ← Fijo para tu ubicación
constexpr unsigned long BAUD_RATE = 115200; // ← Estándar
```

**Valores Variables (usar `inline`):**
```cpp
inline String SSID = "MiWiFi";           // ← Puede cambiar en runtime
inline String NTP_SERVER1 = "pool.ntp.org"; // ← Puede personalizarse
```

**Organización:**
```cpp
// ✅ BIEN: Agrupado por funcionalidad
namespace Config {
    namespace WiFi {
        inline String SSID = "...";
        inline String PASSWORD = "...";
        constexpr unsigned long TIMEOUT = 10000;
    }
}

// ❌ MAL: Todo mezclado sin organizar
inline String WIFI_SSID = "...";
constexpr int LED_PIN = 2;
inline String NTP_SERVER = "...";
```

---

## 🔧 Personalización Avanzada

### Añadir Namespace Propio en Configuracion.h

Si tu proyecto necesita configuración adicional:

```cpp
namespace Config {
    // ... namespaces existentes (Project, Serial, WiFi, Time) ...
    
    // Tu namespace personalizado
    namespace MiModulo {
        // Pines
        constexpr int PIN_SENSOR = 34;
        constexpr int PIN_ACTUADOR = 25;
        
        // Configuración
        constexpr unsigned long INTERVALO_LECTURA_MS = 5000;
        constexpr float UMBRAL_TEMP = 25.5;
        
        // Valores runtime
        inline String NOMBRE_SENSOR = "Sensor Principal";
        inline bool ACTIVADO = true;
    }
}

// Usar:
pinMode(Config::MiModulo::PIN_SENSOR, INPUT);
if (temperatura > Config::MiModulo::UMBRAL_TEMP) {
    // ...
}
```

---

## 🆘 Resolución de Problemas

### Error: "Configuracion.h: No such file or directory"

**Problema:** El archivo no está en la carpeta correcta.

**Solución:**
```bash
# Verifica que Configuracion.h esté junto a tu .ino
ls MiProyecto/
# Debe mostrar: MiProyecto.ino  Configuracion.h  Debug.h
```

---

### Error: "WiFi.status() was not declared in this scope"

**Problema:** Falta incluir `<WiFi.h>`.

**Solución:**
```cpp
#include <WiFi.h>          // ← Añadir ANTES de usar WiFi
#include "Debug.h"
#include "Configuracion.h"
```

---

### Advertencia: "NTP no sincronizado"

**Problema:** WiFi no conectado o servidores NTP bloqueados.

**Solución:**
```cpp
// 1. Verificar WiFi
#define DEBUGWIFI          // ← Activar debug WiFi
DebugHelper::printWiFiStatus(); // ← Ver estado

// 2. Probar servidores NTP locales
Config::Time::NTP_SERVER1 = "es.pool.ntp.org";  // España
// Config::Time::NTP_SERVER1 = "ar.pool.ntp.org";  // Argentina
// Config::Time::NTP_SERVER1 = "mx.pool.ntp.org";  // México

// 3. Aumentar delay después de configTime()
configTime(...);
delay(5000);  // ← Aumentar a 5 segundos
```

---

### Debug no Muestra Nada

**Problema:** Serial no inicializado o debug no activado.

**Solución:**
```cpp
// 1. Activar debug ANTES de incluir
#define DEBUG              // ← ANTES
#include "Debug.h"         // ← DESPUÉS

// 2. Inicializar Serial en setup()
void setup() {
    Serial.begin(Config::Serial::BAUD_RATE);
    delay(1000);           // ← Esperar inicialización
    DBG("Ahora debería funcionar");
}

// 3. Verificar baudrate en Serial Monitor (115200)
```

---

## 📚 Recursos Adicionales

- 📖 [Documentación Completa RTCManager](../README.md)
- 💻 [Ejemplos de RTCManager](../examples/)
- 🐙 [Repositorio Principal](https://github.com/Jusaba/ESP32-Jusaba-Librerias)

---

## 📄 Licencia

MIT License - Ver LICENSE en el repositorio principal

---

## 📞 Soporte

¿Problemas usando estos archivos? Abre un issue en:
- [GitHub Issues](https://github.com/Jusaba/ESP32-Jusaba-Librerias/issues)

---

**Última actualización:** 29 de noviembre de 2025  
**Autor:** Julian Salas Bartolomé
