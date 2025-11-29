/**
 * @file Debug.h
 * @brief Debug template for RTCManager library
 * 
 * This is an EXAMPLE template. Copy this file to your project and customize it.
 * See templates/README.md for detailed instructions.
 * 
 * @author Julian Salas Bartolomé
 * @date 2025-11-29
 */

#ifndef DEBUG_H
#define DEBUG_H

#include <Arduino.h>

// ============================================================================
// DEBUG CONFIGURATION
// ============================================================================

// Master debug switch
#define DEBUG_ENABLED 1

// RTCManager debug flag
#define DEBUG_RTC 1

// ============================================================================
// RTCMANAGER DEBUG MACROS
// ============================================================================

#if DEBUG_ENABLED && DEBUG_RTC
    #define DBG_RTC(x) Serial.println(String("[RTC] ") + x)
    #define DBG_RTC_PRINT(x) Serial.print(String("[RTC] ") + x)
    #define DBG_RTC_PRINTF(fmt, ...) Serial.printf("[RTC] " fmt "\n", ##__VA_ARGS__)
#else
    #define DBG_RTC(x)
    #define DBG_RTC_PRINT(x)
    #define DBG_RTC_PRINTF(fmt, ...)
#endif

// ============================================================================
// DEBUG HELPER CLASS - RTCManager Extensions
// ============================================================================

class DebugHelper {
public:
    /**
     * @brief Print current ESP32 time info
     */
    static void printCurrentTime() {
        struct tm timeinfo;
        if (!getLocalTime(&timeinfo)) {
            Serial.println("[RTC] ERROR: Failed to obtain time");
            return;
        }
        
        Serial.println("\n========== CURRENT TIME ==========");
        Serial.printf("Date: %04d-%02d-%02d\n", 
                     timeinfo.tm_year + 1900, 
                     timeinfo.tm_mon + 1, 
                     timeinfo.tm_mday);
        Serial.printf("Time: %02d:%02d:%02d\n", 
                     timeinfo.tm_hour, 
                     timeinfo.tm_min, 
                     timeinfo.tm_sec);
        Serial.printf("Weekday: %d (0=Sun)\n", timeinfo.tm_wday);
        Serial.printf("Year day: %d\n", timeinfo.tm_yday);
        Serial.println("==================================\n");
    }
    
    /**
     * @brief Print current time in custom format
     * @param format strftime format string
     */
    static void printFormattedTime(const char* format = "%d/%m/%Y %H:%M:%S") {
        struct tm timeinfo;
        if (!getLocalTime(&timeinfo)) {
            Serial.println("[RTC] ERROR: Failed to obtain time");
            return;
        }
        
        char buffer[64];
        strftime(buffer, sizeof(buffer), format, &timeinfo);
        Serial.printf("[RTC] %s\n", buffer);
    }
    
    /**
     * @brief Print WiFi status (needed for NTP sync)
     */
    static void printWiFiStatus() {
        Serial.println("\n========== WIFI STATUS ==========");
        Serial.printf("Status: %s\n", 
                     WiFi.status() == WL_CONNECTED ? "Connected" : "Disconnected");
        if (WiFi.status() == WL_CONNECTED) {
            Serial.printf("SSID: %s\n", WiFi.SSID().c_str());
            Serial.printf("IP: %s\n", WiFi.localIP().toString().c_str());
            Serial.printf("RSSI: %d dBm\n", WiFi.RSSI());
        }
        Serial.println("=================================\n");
    }
    
    /**
     * @brief Print free heap memory
     */
    static void printFreeHeap() {
        Serial.printf("[DEBUG] Free Heap: %u bytes\n", ESP.getFreeHeap());
    }
    
    /**
     * @brief Print separator line
     */
    static void printSeparator(char c = '=', int length = 50) {
        for (int i = 0; i < length; i++) {
            Serial.print(c);
        }
        Serial.println();
    }
};

#endif // DEBUG_H

// ============================================================================
// INSTRUCCIONES DE USO
// ============================================================================

/*
 * GUÍA RÁPIDA DE USO CON RTCMANAGER:
 * 
 * 1. COPIAR ESTE ARCHIVO:
 *    - Copia Debug.h a la carpeta de tu proyecto Arduino
 * 
 * 2. ACTIVAR DEBUG SEGÚN NECESIDADES:
 *    
 *    A) Durante desarrollo inicial:
 *       #define DEBUG          // Activa mensajes generales
 *       #define DEBUGWIFI      // Ver estado WiFi
 *       #define DEBUGRTC       // Ver sincronización NTP
 *    
 *    B) Para depurar solo WiFi:
 *       //#define DEBUG        // Comentado
 *       #define DEBUGWIFI      // Solo WiFi
 *       //#define DEBUGRTC     // Comentado
 *    
 *    C) Para producción (compilación final):
 *       //#define DEBUG        // TODO comentado
 *       //#define DEBUGWIFI
 *       //#define DEBUGRTC
 * 
 * 3. INCLUIR EN TU SKETCH:
 *    #include "Debug.h"
 *    #include "Configuracion.h"
 *    #include <RTCManager.h>
 * 
 * 4. USAR MACROS DE DEBUG:
 *    
 *    void setup() {
 *        Serial.begin(115200);
 *        
 *        // Mensajes generales
 *        DBG("Sistema iniciando...");
 *        DebugHelper::printSystemInfo();
 *        
 *        // Debug WiFi
 *        DBG_WIFI("Conectando a WiFi...");
 *        WiFi.begin(Config::WiFi::SSID, Config::WiFi::PASSWORD);
 *        
 *        while (WiFi.status() != WL_CONNECTED) {
 *            delay(500);
 *            DBG_WIFI_PRINT(".");
 *        }
 *        DBG_WIFI("\nConectado!");
 *        DebugHelper::printWiFiStatus();
 *        
 *        // Debug RTC/NTP
 *        DBG_RTC("Configurando NTP...");
 *        configTime(Config::Time::GMT_OFFSET_SEC, 
 *                   Config::Time::DAYLIGHT_OFFSET_SEC,
 *                   Config::Time::NTP_SERVER1.c_str());
 *        
 *        delay(2000);  // Esperar sincronización
 *        DebugHelper::printCurrentTime();
 *    }
 *    
 *    void loop() {
 *        // Usar macros según necesites
 *        DBG("Loop ejecutándose");
 *        DBG_PRINTF("Valor: %d, Estado: %s", miValor, miEstado);
 *    }
 * 
 * 5. MACROS DISPONIBLES:
 *    
 *    DEBUG GENERAL:
 *    - DBG(msg)                    // Mensaje con salto de línea
 *    - DBG_PRINT(msg)              // Mensaje sin salto de línea
 *    - DBG_PRINTF(fmt, ...)        // Mensaje con formato printf
 *    
 *    DEBUG WiFi:
 *    - DBG_WIFI(msg)               // Mensaje WiFi con salto
 *    - DBG_WIFI_PRINT(msg)         // Mensaje WiFi sin salto
 *    - DBG_WIFI_PRINTF(fmt, ...)   // Mensaje WiFi con formato
 *    
 *    DEBUG RTC:
 *    - DBG_RTC(msg)                // Mensaje RTC con salto
 *    - DBG_RTC_PRINT(msg)          // Mensaje RTC sin salto
 *    - DBG_RTC_PRINTF(fmt, ...)    // Mensaje RTC con formato
 *    
 *    HELPERS:
 *    - DebugHelper::printFreeHeap()      // Memoria libre
 *    - DebugHelper::printSystemInfo()    // Info completa sistema
 *    - DebugHelper::printWiFiStatus()    // Estado WiFi
 *    - DebugHelper::printCurrentTime()   // Hora actual
 *    - DebugHelper::printSeparator()     // Línea separadora
 * 
 * 6. EJEMPLO COMPLETO:
 *    
 *    #define DEBUG
 *    #define DEBUGWIFI
 *    #define DEBUGRTC
 *    #include "Debug.h"
 *    #include "Configuracion.h"
 *    #include <WiFi.h>
 *    #include <RTCManager.h>
 *    
 *    void setup() {
 *        Serial.begin(Config::Serial::BAUD_RATE);
 *        delay(1000);
 *        
 *        DebugHelper::printSeparator();
 *        DBG("Proyecto: " + Config::Project::NAME);
 *        DBG("Versión: " + Config::Project::VERSION);
 *        DebugHelper::printSystemInfo();
 *        DebugHelper::printSeparator();
 *        
 *        // Conectar WiFi
 *        DBG_WIFI("Conectando a: " + Config::WiFi::SSID);
 *        WiFi.begin(Config::WiFi::SSID, Config::WiFi::PASSWORD);
 *        
 *        int intentos = 0;
 *        while (WiFi.status() != WL_CONNECTED && intentos < 20) {
 *            delay(500);
 *            DBG_WIFI_PRINT(".");
 *            intentos++;
 *        }
 *        
 *        if (WiFi.status() == WL_CONNECTED) {
 *            DBG_WIFI("\n¡WiFi conectado!");
 *            DebugHelper::printWiFiStatus();
 *            
 *            // Configurar NTP
 *            DBG_RTC("Sincronizando con NTP...");
 *            configTime(Config::Time::GMT_OFFSET_SEC,
 *                       Config::Time::DAYLIGHT_OFFSET_SEC,
 *                       Config::Time::NTP_SERVER1.c_str(),
 *                       Config::Time::NTP_SERVER2.c_str(),
 *                       Config::Time::NTP_SERVER3.c_str());
 *            
 *            delay(3000);
 *            DebugHelper::printCurrentTime();
 *        } else {
 *            DBG_WIFI("\nERROR: No se pudo conectar WiFi");
 *        }
 *        
 *        DebugHelper::printSeparator();
 *    }
 * 
 * VENTAJAS:
 * ✅ Debug específico para RTCManager
 * ✅ Fácil activar/desactivar por categorías
 * ✅ Cero overhead en producción (se elimina al compilar)
 * ✅ Mensajes bien categorizados ([DEBUG], [WIFI], [RTC])
 * ✅ Helpers útiles para troubleshooting
 * 
 * IMPORTANTE:
 * ⚠️  Comentar TODOS los #define antes de compilar versión final
 * ⚠️  El debug consume memoria y reduce velocidad
 * ⚠️  Solo usar durante desarrollo y troubleshooting
 */

#endif // DEBUG_H
