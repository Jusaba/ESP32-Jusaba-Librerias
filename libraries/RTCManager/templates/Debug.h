/**
 * @file Debug.h
 * @brief Sistema de debug específico para proyectos con RTCManager
 * 
 * @details Macros condicionales de debug optimizadas para usar con RTCManager.
 *          Proporciona logging específico para operaciones de tiempo, WiFi y RTC.
 *          En producción, todo el código de debug se elimina (cero overhead).
 * 
 * **CARACTERÍSTICAS:**
 * - Debug general del sistema
 * - Debug específico de WiFi (necesario para NTP)
 * - Debug específico de RTC/NTP (sincronización de tiempo)
 * - Macros condicionales (sin impacto en producción)
 * - Helpers para información del sistema
 * 
 * **CÓMO FUNCIONA:**
 * - Si defines DEBUG, se activan los mensajes de debug generales
 * - Si defines DEBUGWIFI, se activan mensajes de conexión WiFi
 * - Si defines DEBUGRTC, se activan mensajes de sincronización NTP/RTC
 * - Si comentas todos los #define, el código de debug se elimina completamente
 * 
 * **USO:**
 * 1. Copia este archivo a tu proyecto
 * 2. Comenta/descomenta los #define según necesites
 * 3. Usa las macros DBG_XXX en tu código
 * 4. En producción final, comenta todos los #define
 * 
 * @example
 * @code
 * #define DEBUG              // Debug general
 * #define DEBUGWIFI          // Debug WiFi
 * #define DEBUGRTC           // Debug RTC/NTP
 * #include "Debug.h"
 * #include <RTCManager.h>
 * 
 * void setup() {
 *     Serial.begin(115200);
 *     DBG("Sistema iniciado");
 *     DBG_WIFI("Conectando a WiFi...");
 *     DBG_RTC("Sincronizando con NTP...");
 * }
 * @endcode
 * 
 * @author Julian Salas Bartolomé
 * @date 2025-11-29
 * @version 1.0.0
 * 
 * @note Ideal para desarrollo y troubleshooting
 * @note Comentar TODOS los #define para compilar versión de producción
 * @warning El debug usa memoria y reduce velocidad - solo para desarrollo
 */

#ifndef DEBUG_H
#define DEBUG_H

#include <Arduino.h>

// ============================================================================
// CONFIGURACIÓN DE DEBUG - Comentar/descomentar según necesidades
// ============================================================================

// Debug general - Mensajes básicos del sistema
// Actívalo durante el desarrollo inicial
#define DEBUG

// Debug WiFi - Información de conexión WiFi
// Muy útil cuando usas NTP (necesitas WiFi para sincronización)
//#define DEBUGWIFI

// Debug RTC/NTP - Información de sincronización de tiempo
// Útil para ver si NTP funciona y cómo se actualiza la hora
//#define DEBUGRTC

// ============================================================================
// MACROS DE DEBUG - NO MODIFICAR (usar tal cual)
// ============================================================================

// --- Debug General ---
#ifdef DEBUG
    /**
     * @brief Imprime mensaje de debug con salto de línea
     * @param msg Mensaje a imprimir
     */
    #define DBG(msg) Serial.println(String("[DEBUG] ") + msg)
    
    /**
     * @brief Imprime mensaje de debug sin salto de línea
     * @param msg Mensaje a imprimir
     */
    #define DBG_PRINT(msg) Serial.print(String("[DEBUG] ") + msg)
    
    /**
     * @brief Imprime mensaje de debug con formato printf
     * @param fmt Formato estilo printf
     * @param ... Argumentos variables
     */
    #define DBG_PRINTF(fmt, ...) Serial.printf("[DEBUG] " fmt "\n", ##__VA_ARGS__)
#else
    #define DBG(msg)
    #define DBG_PRINT(msg)
    #define DBG_PRINTF(fmt, ...)
#endif

// --- Debug WiFi ---
#ifdef DEBUGWIFI
    /**
     * @brief Imprime mensaje de debug WiFi con salto de línea
     * @param msg Mensaje a imprimir
     */
    #define DBG_WIFI(msg) Serial.println(String("[WIFI] ") + msg)
    
    /**
     * @brief Imprime mensaje de debug WiFi sin salto de línea
     * @param msg Mensaje a imprimir
     */
    #define DBG_WIFI_PRINT(msg) Serial.print(String("[WIFI] ") + msg)
    
    /**
     * @brief Imprime mensaje de debug WiFi con formato printf
     * @param fmt Formato estilo printf
     * @param ... Argumentos variables
     */
    #define DBG_WIFI_PRINTF(fmt, ...) Serial.printf("[WIFI] " fmt "\n", ##__VA_ARGS__)
#else
    #define DBG_WIFI(msg)
    #define DBG_WIFI_PRINT(msg)
    #define DBG_WIFI_PRINTF(fmt, ...)
#endif

// --- Debug RTC/NTP ---
#ifdef DEBUGRTC
    /**
     * @brief Imprime mensaje de debug RTC con salto de línea
     * @param msg Mensaje a imprimir
     */
    #define DBG_RTC(msg) Serial.println(String("[RTC] ") + msg)
    
    /**
     * @brief Imprime mensaje de debug RTC sin salto de línea
     * @param msg Mensaje a imprimir
     */
    #define DBG_RTC_PRINT(msg) Serial.print(String("[RTC] ") + msg)
    
    /**
     * @brief Imprime mensaje de debug RTC con formato printf
     * @param fmt Formato estilo printf
     * @param ... Argumentos variables
     */
    #define DBG_RTC_PRINTF(fmt, ...) Serial.printf("[RTC] " fmt "\n", ##__VA_ARGS__)
#else
    #define DBG_RTC(msg)
    #define DBG_RTC_PRINT(msg)
    #define DBG_RTC_PRINTF(fmt, ...)
#endif

// ============================================================================
// FUNCIONES AUXILIARES DE DEBUG
// ============================================================================

/**
 * @brief Clase helper con funciones útiles de debug
 * 
 * @details Proporciona métodos estáticos para mostrar información del sistema,
 *          memoria y otros datos útiles durante el desarrollo.
 *          Solo se compilan si DEBUG está definido.
 */
class DebugHelper {
public:
    /**
     * @brief Muestra memoria libre disponible en el heap
     * @note Útil para detectar fugas de memoria
     */
    static void printFreeHeap() {
        #ifdef DEBUG
        Serial.printf("[DEBUG] Memoria libre: %d bytes\n", ESP.getFreeHeap());
        #endif
    }
    
    /**
     * @brief Muestra información completa del sistema ESP32
     * @note Incluye frecuencia CPU, tamaño flash, heap, chip ID
     */
    static void printSystemInfo() {
        #ifdef DEBUG
        Serial.println("[DEBUG] ========== INFO SISTEMA ==========");
        Serial.printf("[DEBUG] CPU Freq: %d MHz\n", ESP.getCpuFreqMHz());
        Serial.printf("[DEBUG] Flash Size: %d bytes\n", ESP.getFlashChipSize());
        Serial.printf("[DEBUG] Free Heap: %d bytes\n", ESP.getFreeHeap());
        Serial.printf("[DEBUG] Chip ID: %08X\n", (uint32_t)ESP.getEfuseMac());
        Serial.println("[DEBUG] ====================================");
        #endif
    }
    
    /**
     * @brief Muestra estado de la conexión WiFi
     * @note Útil para troubleshooting de conexión WiFi/NTP
     */
    static void printWiFiStatus() {
        #ifdef DEBUGWIFI
        Serial.println("[WIFI] ========== ESTADO WiFi ==========");
        Serial.printf("[WIFI] Estado: %s\n", 
                     WiFi.status() == WL_CONNECTED ? "Conectado" : "Desconectado");
        if (WiFi.status() == WL_CONNECTED) {
            Serial.printf("[WIFI] SSID: %s\n", WiFi.SSID().c_str());
            Serial.printf("[WIFI] IP: %s\n", WiFi.localIP().toString().c_str());
            Serial.printf("[WIFI] RSSI: %d dBm\n", WiFi.RSSI());
        }
        Serial.println("[WIFI] ====================================");
        #endif
    }
    
    /**
     * @brief Imprime la hora actual del sistema
     * @note Útil para verificar sincronización NTP
     */
    static void printCurrentTime() {
        #ifdef DEBUGRTC
        struct tm timeinfo;
        if (!getLocalTime(&timeinfo)) {
            Serial.println("[RTC] ERROR: No se pudo obtener la hora");
            return;
        }
        
        char buffer[64];
        strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", &timeinfo);
        Serial.printf("[RTC] Hora actual: %s\n", buffer);
        #endif
    }
    
    /**
     * @brief Imprime un separador visual en los logs
     */
    static void printSeparator() {
        #ifdef DEBUG
        Serial.println("[DEBUG] =======================================");
        #endif
    }
    
    /**
     * @brief Imprime contenido de un array
     * 
     * @tparam T Tipo de datos del array
     * @param name Nombre descriptivo del array
     * @param array Puntero al array
     * @param size Tamaño del array
     */
    template<typename T>
    static void printArray(const char* name, T* array, size_t size) {
        #ifdef DEBUG
        Serial.printf("[DEBUG] Array %s [%d]: ", name, size);
        for (size_t i = 0; i < size; i++) {
            Serial.printf("%d ", array[i]);
        }
        Serial.println();
        #endif
    }
};

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
