/**
 * @file Configuracion.h
 * @brief Configuración centralizada para uso de RTCManager
 * 
 * @details Este archivo contiene la configuración específica necesaria para usar
 *          la librería RTCManager en tu proyecto. Proporciona valores por defecto
 *          que puedes personalizar según tus necesidades.
 * 
 * **PARA QUÉ SIRVE:**
 * - Configurar conexión WiFi para sincronización NTP
 * - Definir servidores NTP y zona horaria
 * - Establecer baudrate del puerto serial
 * - Configurar información del proyecto
 * 
 * **CÓMO USAR:**
 * 1. Copia este archivo a la carpeta de tu proyecto
 * 2. Personaliza los valores (WiFi, zona horaria, NTP, etc.)
 * 3. Incluye en tu sketch: #include "Configuracion.h"
 * 4. Úsalo con RTCManager para gestión de tiempo
 * 
 * @example
 * @code
 * #include "Configuracion.h"
 * #include <RTCManager.h>
 * 
 * void setup() {
 *     Serial.begin(Config::Serial::BAUD_RATE);
 *     WiFi.begin(Config::WiFi::SSID, Config::WiFi::PASSWORD);
 *     
 *     // Esperar conexión WiFi
 *     while (WiFi.status() != WL_CONNECTED) {
 *         delay(500);
 *     }
 *     
 *     // Configurar NTP con valores de Config
 *     configTime(Config::Time::GMT_OFFSET_SEC, 
 *                Config::Time::DAYLIGHT_OFFSET_SEC,
 *                Config::Time::NTP_SERVER1.c_str(),
 *                Config::Time::NTP_SERVER2.c_str(),
 *                Config::Time::NTP_SERVER3.c_str());
 * }
 * @endcode
 * 
 * @author Julian Salas Bartolomé
 * @date 2025-11-29
 * @version 1.0.0
 * 
 * @note Personaliza los valores según tu ubicación y red
 * @note Los valores constexpr se evalúan en tiempo de compilación
 * @note Los valores inline pueden modificarse en runtime si es necesario
 */

#ifndef CONFIGURACION_H
#define CONFIGURACION_H

#include <Arduino.h>

namespace Config {

    // ==================== INFORMACIÓN DEL PROYECTO ====================
    /**
     * @brief Información general del proyecto
     * @note Personaliza estos valores para identificar tu proyecto
     */
    namespace Project {
        inline String NAME = "Mi Proyecto ESP32";      // Nombre de tu proyecto
        inline String VERSION = "1.0.0";               // Versión actual
        inline String AUTHOR = "Tu Nombre";            // Tu nombre
    }

    // ==================== COMUNICACIÓN SERIAL ====================
    /**
     * @brief Configuración del puerto serial
     * @note RTCManager usa Serial para mensajes de debug
     */
    namespace Serial {
        constexpr unsigned long BAUD_RATE = 115200;    // Velocidad estándar para ESP32
    }

    // ==================== WIFI ====================
    /**
     * @brief Configuración WiFi necesaria para sincronización NTP
     * @note RTCManager requiere WiFi para obtener hora desde Internet
     */
    namespace WiFi {
        // IMPORTANTE: Cambia estos valores por tus credenciales WiFi
        inline String SSID = "TU_SSID";                   // Nombre de tu red WiFi
        inline String PASSWORD = "TU_PASSWORD";           // Contraseña de tu WiFi
        
        // Configuración de conexión
        constexpr unsigned long CONNECT_TIMEOUT_MS = 10000;  // Timeout de conexión (10 seg)
        constexpr int MAX_RECONNECT_ATTEMPTS = 5;            // Intentos de reconexión
        constexpr unsigned long RECONNECT_INTERVAL_MS = 30000; // Intervalo entre reconexiones
    }

    // ==================== NTP / RTC ====================
    /**
     * @brief Configuración de sincronización de tiempo
     * @note Esta es la configuración PRINCIPAL para RTCManager
     */
    namespace Time {
        // Servidores NTP (Network Time Protocol)
        // Estos servidores proporcionan la hora exacta desde Internet
        inline String NTP_SERVER1 = "pool.ntp.org";         // Servidor primario
        inline String NTP_SERVER2 = "time.google.com";      // Servidor secundario
        inline String NTP_SERVER3 = "time.cloudflare.com";  // Servidor terciario
        
        // Zona horaria - IMPORTANTE: Ajusta según tu ubicación
        // GMT_OFFSET_SEC = diferencia con GMT en segundos
        // España (península): GMT+1 = 3600 segundos
        // Canarias: GMT+0 = 0 segundos
        // Argentina: GMT-3 = -10800 segundos
        // México (centro): GMT-6 = -21600 segundos
        constexpr long GMT_OFFSET_SEC = 3600;              // GMT+1 (España península)
        
        // Horario de verano (DST - Daylight Saving Time)
        // En España: +1 hora (3600 seg) en verano, 0 en invierno
        // Si tu país no usa horario de verano, pon 0
        constexpr int DAYLIGHT_OFFSET_SEC = 3600;          // +1 hora en verano
        
        // Intervalos de sincronización
        constexpr unsigned long NTP_SYNC_INTERVAL_MS = 3600000;  // Sincronizar cada 1 hora
        constexpr unsigned long NTP_TIMEOUT_MS = 15000;          // Timeout NTP: 15 segundos
    }

} // namespace Config

// ============================================================================
// INSTRUCCIONES DE USO
// ============================================================================

/*
 * GUÍA RÁPIDA DE USO:
 * 
 * 1. COPIAR ESTE ARCHIVO:
 *    - Copia Configuracion.h a la carpeta de tu proyecto Arduino
 * 
 * 2. PERSONALIZAR CONFIGURACIÓN:
 *    
 *    A) WiFi (OBLIGATORIO para NTP):
 *       Config::WiFi::SSID = "TuRedWiFi";
 *       Config::WiFi::PASSWORD = "TuContraseña";
 *    
 *    B) Zona Horaria (ajusta según tu ubicación):
 *       - España (península): GMT_OFFSET_SEC = 3600 (GMT+1)
 *       - Canarias: GMT_OFFSET_SEC = 0 (GMT+0)
 *       - Argentina: GMT_OFFSET_SEC = -10800 (GMT-3)
 *       - México (centro): GMT_OFFSET_SEC = -21600 (GMT-6)
 *       - Chile: GMT_OFFSET_SEC = -14400 (GMT-4)
 *    
 *    C) Horario de Verano:
 *       Si tu país usa horario de verano: DAYLIGHT_OFFSET_SEC = 3600
 *       Si NO usa horario de verano: DAYLIGHT_OFFSET_SEC = 0
 *    
 *    D) Información del Proyecto (opcional):
 *       Config::Project::NAME = "Mi Reloj Digital";
 *       Config::Project::VERSION = "1.0.0";
 *       Config::Project::AUTHOR = "Tu Nombre";
 * 
 * 3. INCLUIR EN TU SKETCH:
 *    #include "Configuracion.h"
 *    #include <RTCManager.h>
 * 
 * 4. USAR EN TU CÓDIGO:
 *    
 *    void setup() {
 *        // Inicializar Serial con baudrate configurado
 *        Serial.begin(Config::Serial::BAUD_RATE);
 *        
 *        // Conectar WiFi
 *        WiFi.begin(Config::WiFi::SSID, Config::WiFi::PASSWORD);
 *        
 *        // Esperar conexión
 *        while (WiFi.status() != WL_CONNECTED) {
 *            delay(500);
 *            Serial.print(".");
 *        }
 *        Serial.println("\nWiFi conectado");
 *        
 *        // Configurar NTP con los valores de Config
 *        configTime(Config::Time::GMT_OFFSET_SEC, 
 *                   Config::Time::DAYLIGHT_OFFSET_SEC,
 *                   Config::Time::NTP_SERVER1.c_str(),
 *                   Config::Time::NTP_SERVER2.c_str(),
 *                   Config::Time::NTP_SERVER3.c_str());
 *        
 *        // Usar RTCManager para gestionar tiempo
 *        // ... tu código aquí ...
 *    }
 * 
 * 5. ACCEDER A VALORES:
 *    String ssid = Config::WiFi::SSID;
 *    long offset = Config::Time::GMT_OFFSET_SEC;
 *    unsigned long baud = Config::Serial::BAUD_RATE;
 * 
 * 6. MODIFICAR EN RUNTIME (si es necesario):
 *    Config::WiFi::SSID = "NuevoSSID";
 *    Config::Time::NTP_SERVER1 = "es.pool.ntp.org";
 * 
 * TABLA DE ZONAS HORARIAS COMUNES:
 * ─────────────────────────────────────────────────────
 * País/Región              GMT      Offset (segundos)
 * ─────────────────────────────────────────────────────
 * España (península)       GMT+1    3600
 * Canarias                 GMT+0    0
 * Portugal                 GMT+0    0
 * Reino Unido              GMT+0    0
 * Francia                  GMT+1    3600
 * Alemania                 GMT+1    3600
 * Argentina                GMT-3    -10800
 * Chile                    GMT-4    -14400
 * México (centro)          GMT-6    -21600
 * Colombia                 GMT-5    -18000
 * Perú                     GMT-5    -18000
 * Venezuela                GMT-4    -14400
 * Brasil (Brasilia)        GMT-3    -10800
 * Nueva York (EST)         GMT-5    -18000
 * California (PST)         GMT-8    -28800
 * ─────────────────────────────────────────────────────
 * 
 * NOTAS:
 * - Los offsets pueden variar con horario de verano
 * - Verifica el offset actual de tu zona en: timeanddate.com
 * - RTCManager facilita trabajar con estos valores
 */

#endif // CONFIGURACION_H
