/**
 * @file Configuracion.h
 * @brief Sistema centralizado de configuración usando namespaces
 * 
 * @details Plantilla de configuración modular para proyectos ESP32 que organiza
 *          todas las constantes y parámetros en namespaces lógicos.
 * 
 * **VENTAJAS:**
 * - Organización clara por módulos
 * - Evita colisiones de nombres
 * - Fácil de mantener y escalar
 * - Valores constexpr (evaluados en compilación)
 * - Type-safe (tipado fuerte)
 * 
 * **USO:**
 * 1. Copia este archivo a tu proyecto
 * 2. Personaliza los valores según tus necesidades
 * 3. Añade/elimina namespaces según tus módulos
 * 4. Accede con Config::Namespace::CONSTANTE
 * 
 * @example
 * @code
 * #include "Configuracion.h"
 * 
 * void setup() {
 *     pinMode(Config::Pins::LED, OUTPUT);
 *     WiFi.begin(Config::WiFi::SSID, Config::WiFi::PASSWORD);
 *     Serial.begin(Config::Serial::BAUD_RATE);
 * }
 * @endcode
 * 
 * @author Julian Salas Bartolomé
 * @date 2025-11-28
 * @version 1.0.0
 * 
 * @note Este es un archivo plantilla - personaliza según tu proyecto
 * @note Usa constexpr para valores conocidos en compilación
 * @note Usa inline para variables que pueden cambiar en runtime
 */

#ifndef CONFIGURACION_H
#define CONFIGURACION_H

#include <Arduino.h>

namespace Config {

    // ==================== INFORMACIÓN DEL PROYECTO ====================
    namespace Project {
        inline String NAME = "Mi Proyecto ESP32";
        inline String VERSION = "1.0.0";
        inline String AUTHOR = "Tu Nombre";
    }

    // ==================== HARDWARE - PINES ====================
    namespace Pins {
        // Pines digitales
        constexpr int LED_BUILTIN = 2;
        constexpr int BUTTON = 0;
        
        // Pines PWM
        constexpr int PWM1 = 25;
        constexpr int PWM2 = 26;
        
        // Pines analógicos
        constexpr int ANALOG1 = 34;
        constexpr int ANALOG2 = 35;
        
        // Comunicación I2C
        constexpr int SDA = 21;
        constexpr int SCL = 22;
        
        // Comunicación SPI
        constexpr int MISO = 19;
        constexpr int MOSI = 23;
        constexpr int SCK = 18;
        constexpr int SS = 5;
        
        // Añade más pines según tu proyecto
    }

    // ==================== COMUNICACIÓN SERIAL ====================
    namespace Serial {
        constexpr unsigned long BAUD_RATE = 115200;
        constexpr int RX_BUFFER_SIZE = 256;
        constexpr int TX_BUFFER_SIZE = 256;
    }

    // ==================== WIFI ====================
    namespace WiFi {
        // Credenciales (cámbialas por las tuyas)
        inline String SSID = "TU_SSID";
        inline String PASSWORD = "TU_PASSWORD";
        
        // Configuración
        constexpr unsigned long CONNECT_TIMEOUT_MS = 10000;  // 10 segundos
        constexpr int MAX_RECONNECT_ATTEMPTS = 5;
        constexpr unsigned long RECONNECT_INTERVAL_MS = 30000;  // 30 segundos
        
        // Modo AP (Access Point)
        inline String AP_SSID = "ESP32-Config";
        inline String AP_PASSWORD = "12345678";
        constexpr int AP_CHANNEL = 1;
        constexpr int AP_MAX_CONNECTIONS = 4;
    }

    // ==================== SERVIDOR WEB ====================
    namespace WebServer {
        constexpr int PORT = 80;
        constexpr int WS_PORT = 8080;  // WebSocket
        constexpr unsigned long REQUEST_TIMEOUT_MS = 5000;
        
        // Autenticación básica
        inline String AUTH_USER = "admin";
        inline String AUTH_PASSWORD = "admin123";
    }

    // ==================== NTP / RTC ====================
    namespace Time {
        // Servidores NTP
        inline String NTP_SERVER1 = "pool.ntp.org";
        inline String NTP_SERVER2 = "time.google.com";
        inline String NTP_SERVER3 = "time.cloudflare.com";
        
        // Zona horaria
        constexpr long GMT_OFFSET_SEC = 3600;        // GMT+1 (España)
        constexpr int DAYLIGHT_OFFSET_SEC = 3600;    // Horario de verano
        
        // Sincronización
        constexpr unsigned long NTP_SYNC_INTERVAL_MS = 3600000;  // 1 hora
        constexpr unsigned long NTP_TIMEOUT_MS = 15000;          // 15 segundos
    }

    // ==================== OTA (Over-The-Air Updates) ====================
    namespace OTA {
        inline String HOSTNAME = "esp32-device";
        inline String PASSWORD = "otapassword";
        
        // GitHub Releases
        inline String GITHUB_OWNER = "usuario";
        inline String GITHUB_REPO = "repositorio";
        inline String FIRMWARE_VERSION = "1.0.0";
        
        // Configuración
        constexpr bool AUTO_UPDATE_ENABLED = false;
        constexpr unsigned long CHECK_INTERVAL_HOURS = 24;
        constexpr size_t MAX_FIRMWARE_SIZE = 1310720;  // 1.25 MB
    }

    // ==================== MQTT ====================
    namespace MQTT {
        inline String BROKER = "mqtt.example.com";
        constexpr int PORT = 1883;
        inline String CLIENT_ID = "ESP32-Client";
        inline String USER = "mqtt_user";
        inline String PASSWORD = "mqtt_password";
        
        // Topics
        inline String TOPIC_STATUS = "esp32/status";
        inline String TOPIC_COMMAND = "esp32/command";
        inline String TOPIC_DATA = "esp32/data";
        
        // Configuración
        constexpr unsigned long RECONNECT_INTERVAL_MS = 5000;
        constexpr int QOS = 1;
        constexpr bool RETAIN = false;
    }

    // ==================== TELEGRAM BOT ====================
    namespace Telegram {
        inline String BOT_TOKEN = "123456789:ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        inline String CHAT_ID = "123456789";
        
        constexpr unsigned long CHECK_INTERVAL_MS = 2000;  // 2 segundos
        constexpr int MAX_MESSAGE_LENGTH = 4096;
        constexpr bool SILENT_NOTIFICATIONS = false;
        
        // Flags de notificaciones (personaliza según necesites)
        inline bool NOTIFY_STARTUP = true;
        inline bool NOTIFY_ERRORS = true;
        inline bool NOTIFY_WARNINGS = false;
    }

    // ==================== SENSORES ====================
    namespace Sensors {
        // Intervalos de lectura
        constexpr unsigned long READ_INTERVAL_MS = 5000;  // 5 segundos
        constexpr int SAMPLES_FOR_AVERAGE = 10;
        
        // Umbrales
        constexpr float TEMP_MIN = -10.0;
        constexpr float TEMP_MAX = 50.0;
        constexpr float HUMIDITY_MIN = 0.0;
        constexpr float HUMIDITY_MAX = 100.0;
    }

    // ==================== ALMACENAMIENTO ====================
    namespace Storage {
        // SPIFFS
        constexpr bool FORMAT_ON_FAIL = true;
        inline String CONFIG_FILE = "/config.json";
        inline String LOG_FILE = "/log.txt";
        constexpr size_t MAX_LOG_SIZE = 10240;  // 10 KB
        
        // Preferencias (NVS)
        inline String PREFS_NAMESPACE = "app_config";
    }

    // ==================== TIMERS / DELAYS ====================
    namespace Timing {
        constexpr unsigned long LOOP_DELAY_MS = 100;
        constexpr unsigned long WATCHDOG_TIMEOUT_MS = 60000;  // 1 minuto
        constexpr unsigned long DEBOUNCE_DELAY_MS = 50;
    }

    // ==================== PWM ====================
    namespace PWM {
        constexpr int FREQUENCY = 5000;  // 5 kHz
        constexpr int RESOLUTION = 8;    // 8 bits (0-255)
        constexpr int CHANNEL_0 = 0;
        constexpr int CHANNEL_1 = 1;
    }

    // ==================== BLUETOOTH ====================
    namespace Bluetooth {
        inline String DEVICE_NAME = "ESP32-BLE";
        constexpr bool ENABLE_BLE = false;
        constexpr unsigned long SCAN_DURATION_SEC = 5;
    }

    // ==================== DEBUG ====================
    namespace Debug {
        constexpr bool ENABLED = true;
        constexpr bool VERBOSE = false;
        constexpr unsigned long HEARTBEAT_INTERVAL_MS = 10000;  // 10 segundos
    }

    // ==================== LÍMITES Y VALIDACIONES ====================
    namespace Limits {
        constexpr int MAX_RETRIES = 3;
        constexpr unsigned long OPERATION_TIMEOUT_MS = 5000;
        constexpr size_t MAX_BUFFER_SIZE = 1024;
    }

} // namespace Config

// ============================================================================
// INSTRUCCIONES DE USO
// ============================================================================

/*
 * CÓMO USAR ESTE ARCHIVO EN TU PROYECTO:
 * 
 * 1. COPIAR A TU PROYECTO:
 *    - Copia este archivo Configuracion.h a tu proyecto
 * 
 * 2. PERSONALIZAR VALORES:
 *    - Cambia los valores por defecto según tus necesidades
 *    - Elimina namespaces que no uses
 *    - Añade nuevos namespaces para tus módulos
 * 
 * 3. USAR EN TU CÓDIGO:
 *    #include "Configuracion.h"
 *    
 *    void setup() {
 *        Serial.begin(Config::Serial::BAUD_RATE);
 *        pinMode(Config::Pins::LED_BUILTIN, OUTPUT);
 *        WiFi.begin(Config::WiFi::SSID, Config::WiFi::PASSWORD);
 *    }
 * 
 * 4. VALORES CONSTANTES VS VARIABLES:
 *    - constexpr: Para valores fijos en compilación (pines, baudrate, etc.)
 *    - inline: Para variables que pueden cambiar en runtime (SSID, tokens, etc.)
 * 
 * 5. AÑADIR MÓDULOS:
 *    namespace Config {
 *        namespace MiModulo {
 *            constexpr int MI_CONSTANTE = 42;
 *            inline String MI_VARIABLE = "valor";
 *        }
 *    }
 * 
 * 6. ACCEDER A CONFIGURACIÓN:
 *    int pin = Config::Pins::LED_BUILTIN;
 *    String ssid = Config::WiFi::SSID;
 *    
 * 7. MODIFICAR EN RUNTIME:
 *    Config::WiFi::SSID = "NuevoSSID";
 *    Config::Telegram::NOTIFY_ERRORS = false;
 * 
 * VENTAJAS:
 * ✅ Todo centralizado en un archivo
 * ✅ Organización clara por módulos
 * ✅ No hay colisiones de nombres
 * ✅ Fácil de mantener
 * ✅ Type-safe
 * ✅ Valores evaluados en compilación cuando es posible
 */

#endif // CONFIGURACION_H
