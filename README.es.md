# ESP32 Jusaba Librerías

**[English](README.md) | Español**

Colección de librerías reutilizables para proyectos ESP32 desarrolladas por Jusaba.

## 📚 Librerías Disponibles

### RTCManager
Gestión avanzada de sincronización temporal NTP con múltiples servidores y validación de fechas.

- ✅ Sincronización NTP con redundancia (3 servidores)
- ✅ Validación automática de fechas
- ✅ Timeout configurable
- ✅ Manejo de zona horaria y horario de verano
- ✅ Fallback automático entre servidores

[Ver documentación completa →](libraries/RTCManager/README.es.md)

## 🚀 Instalación

### Método 1: Arduino IDE
1. Descarga el repositorio como ZIP
2. En Arduino IDE: `Sketch` → `Include Library` → `Add .ZIP Library`
3. Selecciona la carpeta `libraries/RTCManager`

### Método 2: Manual
1. Clona el repositorio:
   ```bash
   git clone https://github.com/Jusaba/ESP32-Jusaba-Librerias.git
   ```
2. Copia la carpeta `libraries/RTCManager` a tu carpeta de librerías de Arduino:
   - Windows: `Documents/Arduino/libraries/`
   - Linux: `~/Arduino/libraries/`
   - macOS: `~/Documents/Arduino/libraries/`

### Método 3: PlatformIO
Añade al `platformio.ini`:
```ini
lib_deps = 
    https://github.com/Jusaba/ESP32-Jusaba-Librerias.git#libraries/RTCManager
```

## 📖 Uso Rápido

```cpp
#include <RTCManager.h>

void setup() {
    Serial.begin(115200);
    WiFi.begin("SSID", "password");
    
    // Esperar conexión WiFi
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
    
    // Sincronizar con múltiples servidores NTP
    if (RTC::beginConMultiplesServidores()) {
        Serial.println("✅ Sincronización NTP exitosa");
        Serial.println("Hora: " + RTC::getTimeStr());
    }
}

void loop() {
    if (RTC::isNtpSync()) {
        Serial.println(RTC::getTimeStr());
    }
    delay(1000);
}
```

## 🛠️ Próximas Librerías

- [ ] TelegramService - Cliente Telegram con comandos personalizados
- [ ] OTAService - Actualización OTA desde GitHub Releases
- [ ] WebServerManager - Servidor web asíncrono con WebSockets
- [ ] SPIFFSManager - Gestión simplificada de archivos SPIFFS

## 🤝 Contribuir

¿Tienes una librería útil para ESP32? Ver [CONTRIBUTING.md](docs/contributing.md)

## 📄 Licencia

MIT License - Ver [LICENSE](LICENSE) para más detalles

## 👤 Autor

**Julián Salas Bartolomé**
- GitHub: [@Jusaba](https://github.com/Jusaba)

## 📝 Changelog

### v1.0.0 (2025-11-28)
- ✨ Primera versión
- ✨ RTCManager: Sincronización NTP con múltiples servidores
