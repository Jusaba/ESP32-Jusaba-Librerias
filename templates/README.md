# Templates / Plantillas

**[English](#english) | [Español](#español)**

---

## English

### 📁 Generic Project Templates

This folder contains **generic/minimal** template files for any ESP32 project. These are the base files that every project should start with.

#### Available Files:

- **`Configuracion.h`** - Minimal configuration (Project info, Serial)
- **`Debug.h`** - Basic debug macros (DBG, DBG_PRINT, DBG_PRINTF)

#### How to Use:

1. **Copy these files to your project:**
   ```bash
   cp templates/Configuracion.h MyProject/
   cp templates/Debug.h MyProject/
   ```

2. **Customize according to your needs**

3. **Add library-specific configuration:**
   - Each library has its own `templates/` folder
   - Example: `libraries/RTCManager/templates/`
   - Copy and merge the namespaces/macros you need

### 🔧 Library-Specific Templates

Each library in `libraries/` has its own `templates/` folder with **specific** files:

```
libraries/
└── RTCManager/
    └── templates/
        ├── Configuracion.h    ← RTCManager-specific (WiFi, Time namespaces)
        ├── Debug.h            ← RTCManager-specific (DEBUGWIFI, DEBUGRTC, helpers)
        └── README.md          ← Detailed usage instructions
```

### 📖 Workflow:

1. **Start** with generic templates from `templates/`
2. **Add** specific configurations from `libraries/LibraryName/templates/`
3. **Merge** by copying namespaces/macros into your project files
4. **Customize** for your specific project needs

### Example:

**Step 1:** Copy generic files
```bash
cp templates/Configuracion.h MyProject/
cp templates/Debug.h MyProject/
```

**Step 2:** Add RTCManager configuration

Open `MyProject/Configuracion.h` and add from `libraries/RTCManager/templates/Configuracion.h`:

```cpp
namespace Config {
    // Generic (already here)
    namespace Project { ... }
    namespace Serial { ... }
    
    // Add from RTCManager templates
    namespace WiFi {
        inline String SSID = "MyWiFi";
        inline String PASSWORD = "mypassword";
    }
    
    namespace Time {
        inline String NTP_SERVER1 = "pool.ntp.org";
        constexpr long GMT_OFFSET_SEC = 3600;
        constexpr int DAYLIGHT_OFFSET_SEC = 3600;
    }
}
```

**Step 3:** Add RTCManager debug macros

Open `MyProject/Debug.h` and add from `libraries/RTCManager/templates/Debug.h`:

```cpp
// Generic (already here)
#define DEBUG

// Add from RTCManager templates
#define DEBUGWIFI
#define DEBUGRTC

#ifdef DEBUGWIFI
    #define DBG_WIFI(msg) Serial.println(String("[WIFI] ") + msg)
    // ... more macros ...
#endif

#ifdef DEBUGRTC
    #define DBG_RTC(msg) Serial.println(String("[RTC] ") + msg)
    // ... more macros ...
#endif

// Add helpers from RTCManager templates
class DebugHelper {
    static void printWiFiStatus() { ... }
    static void printCurrentTime() { ... }
    // ... more helpers ...
};
```

### 💡 Key Concepts:

- **Generic templates** = Minimal, essential, no dependencies
- **Library templates** = Specific, complete, with helpers
- **Your project files** = Fusion of generic + needed libraries
- **DebugHelper functions** = Project-specific, not in generic templates

---

## Español

### 📁 Plantillas Genéricas de Proyecto

Esta carpeta contiene archivos **genéricos/mínimos** para cualquier proyecto ESP32. Son los archivos base con los que todo proyecto debería empezar.

#### Archivos Disponibles:

- **`Configuracion.h`** - Configuración mínima (Info proyecto, Serial)
- **`Debug.h`** - Macros de debug básicas (DBG, DBG_PRINT, DBG_PRINTF)

#### Cómo Usar:

1. **Copia estos archivos a tu proyecto:**
   ```bash
   cp templates/Configuracion.h MiProyecto/
   cp templates/Debug.h MiProyecto/
   ```

2. **Personaliza según tus necesidades**

3. **Añade configuración específica de librerías:**
   - Cada librería tiene su propia carpeta `templates/`
   - Ejemplo: `libraries/RTCManager/templates/`
   - Copia y fusiona los namespaces/macros que necesites

### 🔧 Plantillas Específicas de Librerías

Cada librería en `libraries/` tiene su propia carpeta `templates/` con archivos **específicos**:

```
libraries/
└── RTCManager/
    └── templates/
        ├── Configuracion.h    ← Específico RTCManager (WiFi, Time)
        ├── Debug.h            ← Específico RTCManager (DEBUGWIFI, DEBUGRTC, helpers)
        └── README.md          ← Instrucciones detalladas de uso
```

### 📖 Flujo de Trabajo:

1. **Empezar** con plantillas genéricas de `templates/`
2. **Añadir** configuraciones específicas de `libraries/NombreLibreria/templates/`
3. **Fusionar** copiando namespaces/macros a los archivos de tu proyecto
4. **Personalizar** para las necesidades específicas de tu proyecto

### Ejemplo:

**Paso 1:** Copiar archivos genéricos
```bash
cp templates/Configuracion.h MiProyecto/
cp templates/Debug.h MiProyecto/
```

**Paso 2:** Añadir configuración de RTCManager

Abre `MiProyecto/Configuracion.h` y añade desde `libraries/RTCManager/templates/Configuracion.h`:

```cpp
namespace Config {
    // Genérico (ya está)
    namespace Project { ... }
    namespace Serial { ... }
    
    // Añadir desde templates de RTCManager
    namespace WiFi {
        inline String SSID = "MiWiFi";
        inline String PASSWORD = "micontraseña";
    }
    
    namespace Time {
        inline String NTP_SERVER1 = "pool.ntp.org";
        constexpr long GMT_OFFSET_SEC = 3600;
        constexpr int DAYLIGHT_OFFSET_SEC = 3600;
    }
}
```

**Paso 3:** Añadir macros de debug de RTCManager

Abre `MiProyecto/Debug.h` y añade desde `libraries/RTCManager/templates/Debug.h`:

```cpp
// Genérico (ya está)
#define DEBUG

// Añadir desde templates de RTCManager
#define DEBUGWIFI
#define DEBUGRTC

#ifdef DEBUGWIFI
    #define DBG_WIFI(msg) Serial.println(String("[WIFI] ") + msg)
    // ... más macros ...
#endif

#ifdef DEBUGRTC
    #define DBG_RTC(msg) Serial.println(String("[RTC] ") + msg)
    // ... más macros ...
#endif

// Añadir helpers desde templates de RTCManager
class DebugHelper {
    static void printWiFiStatus() { ... }
    static void printCurrentTime() { ... }
    // ... más helpers ...
};
```

### 💡 Conceptos Clave:

- **Templates genéricos** = Mínimo, esencial, sin dependencias
- **Templates de librerías** = Específicos, completos, con helpers
- **Archivos de tu proyecto** = Fusión de genérico + librerías necesarias
- **Funciones DebugHelper** = Específicas del proyecto, no en templates genéricos

---

## 📋 Structure / Estructura

```
ESP32-Jusaba-Librerias/
├── templates/                          ← GENERIC TEMPLATES / PLANTILLAS GENÉRICAS
│   ├── Configuracion.h                 ← Minimal config / Config mínima
│   ├── Debug.h                         ← Basic macros / Macros básicas
│   └── README.md                       ← This file / Este archivo
│
└── libraries/
    └── RTCManager/
        └── templates/                  ← LIBRARY-SPECIFIC / ESPECÍFICO LIBRERÍA
            ├── Configuracion.h         ← WiFi, Time, NTP config
            ├── Debug.h                 ← DEBUGWIFI, DEBUGRTC, DebugHelper
            └── README.md               ← Detailed guide / Guía detallada
```

---

## ✅ Best Practices / Mejores Prácticas

### Generic Templates / Templates Genéricos

✅ **DO / HACER:**
- Keep minimal and essential / Mantener mínimo y esencial
- No library dependencies / Sin dependencias de librerías
- Basic debug macros only / Solo macros de debug básicas
- Project metadata / Metadatos del proyecto

❌ **DON'T / NO HACER:**
- Add library-specific code / Añadir código específico de librerías
- Include helper functions / Incluir funciones helper
- Add WiFi, NTP, or module config / Añadir config WiFi, NTP, módulos

### Library Templates / Templates de Librerías

✅ **DO / HACER:**
- Complete configuration for library / Configuración completa para la librería
- Specific debug macros / Macros de debug específicas
- Helper functions if needed / Funciones helper si es necesario
- Detailed documentation / Documentación detallada

❌ **DON'T / NO HACER:**
- Include generic config / Incluir configuración genérica
- Duplicate basic macros / Duplicar macros básicas

---

## 🆘 FAQ

### Q: ¿Qué archivo debo copiar primero?
**A:** Siempre empieza con los archivos genéricos de `templates/`, luego añade lo específico de las librerías que uses.

### Q: ¿Puedo modificar los templates genéricos?
**A:** Sí, pero mantén solo lo esencial. Lo específico va en templates de librerías.

### Q: ¿Dónde van las funciones DebugHelper?
**A:** En el `Debug.h` de tu proyecto, copiadas desde el `Debug.h` de las librerías que uses.

### Q: ¿Puedo combinar varias librerías?
**A:** Sí, fusiona los namespaces/macros de cada librería en tus archivos de proyecto.

### Q: Where should helper functions go?
**A:** In your project's `Debug.h`, copied from the `Debug.h` of the libraries you use.

### Q: Can I combine multiple libraries?
**A:** Yes, merge the namespaces/macros from each library into your project files.

---

## 📄 License / Licencia

MIT License - See [LICENSE](../LICENSE)

---

**Author / Autor:** Julián Salas Bartolomé  
**Date / Fecha:** 2025-11-29
