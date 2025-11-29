/**
 * @file Configuracion.h
 * @brief Configuration template for RTCManager library
 * 
 * This is an EXAMPLE template. Copy this file to your project and customize it.
 * See templates/README.md for detailed instructions.
 * 
 * @author Julian Salas Bartolomé
 * @date 2025-11-29
 */

#ifndef CONFIGURACION_H
#define CONFIGURACION_H

#include <Arduino.h>

// ============================================================================
// RTCMANAGER CONFIGURATION
// ============================================================================
namespace Config {
    namespace Time {
        // NTP servers (Network Time Protocol)
        const char NTP_SERVER1[] = "pool.ntp.org";
        const char NTP_SERVER2[] = "time.google.com";
        const char NTP_SERVER3[] = "time.cloudflare.com";
        
        // Timezone configuration
        // GMT_OFFSET_SEC = offset from GMT in seconds
        // Examples:
        //   Spain (mainland): GMT+1 = 3600
        //   Canary Islands: GMT+0 = 0
        //   Argentina: GMT-3 = -10800
        //   Mexico (central): GMT-6 = -21600
        const long GMT_OFFSET_SEC = 3600;        // GMT+1 (Spain)
        
        // Daylight Saving Time offset
        // Spain: +1 hour (3600 sec) in summer
        // If your country doesn't use DST, set to 0
        const int DAYLIGHT_OFFSET_SEC = 3600;    // +1 hour DST
        
        // Sync intervals
        const unsigned long NTP_SYNC_INTERVAL_MS = 3600000;  // Sync every 1 hour
        const unsigned long NTP_TIMEOUT_MS = 15000;          // NTP timeout: 15 seconds
    }
    
    namespace WiFi {
        // Connection configuration
        const unsigned long CONNECT_TIMEOUT_MS = 10000;        // 10 seconds
        const int MAX_RECONNECT_ATTEMPTS = 5;
        const unsigned long RECONNECT_INTERVAL_MS = 30000;     // 30 seconds
    }
}

#endif // CONFIGURACION_H

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
