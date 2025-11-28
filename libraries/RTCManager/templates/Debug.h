/**
 * @file Debug.h
 * @brief Sistema centralizado de debug modular para proyectos ESP32
 * 
 * @details Sistema de macros condicionales para debug que permite activar/desactivar
 *          logging por módulos específicos. Optimiza el tamaño del binario eliminando
 *          todo el código de debug en producción.
 * 
 * **CARACTERÍSTICAS:**
 * - Debug modular por componentes
 * - Macros condicionales (cero overhead en producción)
 * - Soporte printf con formato
 * - Helper para información del sistema
 * 
 * **USO:**
 * 1. Copia este archivo a tu proyecto
 * 2. Comenta/descomenta los #define según necesites
 * 3. Usa las macros DBG_XXX en tu código
 * 4. En producción, comenta todos los #define
 * 
 * @example
 * @code
 * #define DEBUG              // Habilitar debug general
 * #define DEBUGWIFI          // Habilitar debug WiFi
 * #include "Debug.h"
 * 
 * void setup() {
 *     Serial.begin(115200);
 *     DBG("Sistema iniciado");
 *     DBG_WIFI("Conectando a WiFi...");
 * }
 * @endcode
 * 
 * @author Julian Salas Bartolomé
 * @date 2025-11-28
 * @version 1.0.0
 * 
 * @note Para proyectos pequeños, solo necesitas #define DEBUG
 * @note Para proyectos grandes, activa solo los módulos que necesites debugear
 * @warning Comentar TODOS los #define antes de compilar versión de producción
 */

#ifndef DEBUG_H
#define DEBUG_H

#include <Arduino.h>

// ============================================================================
// CONFIGURACIÓN DE DEBUG - Comentar/descomentar según necesidades
// ============================================================================

// Debug general del sistema (siempre útil durante desarrollo)
#define DEBUG

// Debug específico por módulos - Personaliza según tu proyecto
//#define DEBUGWIFI                   // Debug de conexión WiFi
//#define DEBUGSERVER                 // Debug del servidor web
//#define DEBUGOTA                    // Debug de actualizaciones OTA
//#define DEBUGRTC                    // Debug de sincronización NTP/RTC
//#define DEBUGSENSOR                 // Debug de sensores
//#define DEBUGI2C                    // Debug de comunicación I2C
//#define DEBUGSPI                    // Debug de comunicación SPI
//#define DEBUGBLE                    // Debug de Bluetooth
//#define DEBUGMQTT                   // Debug de MQTT
//#define DEBUGDATABASE               // Debug de base de datos
//#define DEBUGFILE                   // Debug de sistema de archivos
//#define DEBUGTELEGRAM               // Debug de bot Telegram
//#define DEBUGCUSTOM1                // Debug personalizado 1
//#define DEBUGCUSTOM2                // Debug personalizado 2

// ============================================================================
// MACROS DE DEBUG - Simplifican el uso de debug condicional
// ============================================================================

// --- Debug General ---
#ifdef DEBUG
    #define DBG(msg) Serial.println(String("[DEBUG] ") + msg)
    #define DBG_PRINT(msg) Serial.print(String("[DEBUG] ") + msg)
    #define DBG_PRINTF(fmt, ...) Serial.printf("[DEBUG] " fmt "\n", ##__VA_ARGS__)
#else
    #define DBG(msg)
    #define DBG_PRINT(msg)
    #define DBG_PRINTF(fmt, ...)
#endif

// --- Debug WiFi ---
#ifdef DEBUGWIFI
    #define DBG_WIFI(msg) Serial.println(String("[WIFI] ") + msg)
    #define DBG_WIFI_PRINT(msg) Serial.print(String("[WIFI] ") + msg)
    #define DBG_WIFI_PRINTF(fmt, ...) Serial.printf("[WIFI] " fmt "\n", ##__VA_ARGS__)
#else
    #define DBG_WIFI(msg)
    #define DBG_WIFI_PRINT(msg)
    #define DBG_WIFI_PRINTF(fmt, ...)
#endif

// --- Debug Servidor ---
#ifdef DEBUGSERVER
    #define DBG_SRV(msg) Serial.println(String("[SRV] ") + msg)
    #define DBG_SRV_PRINT(msg) Serial.print(String("[SRV] ") + msg)
    #define DBG_SRV_PRINTF(fmt, ...) Serial.printf("[SRV] " fmt "\n", ##__VA_ARGS__)
#else
    #define DBG_SRV(msg)
    #define DBG_SRV_PRINT(msg)
    #define DBG_SRV_PRINTF(fmt, ...)
#endif

// --- Debug OTA ---
#ifdef DEBUGOTA
    #define DBG_OTA(msg) Serial.println(String("[OTA] ") + msg)
    #define DBG_OTA_PRINT(msg) Serial.print(String("[OTA] ") + msg)
    #define DBG_OTA_PRINTF(fmt, ...) Serial.printf("[OTA] " fmt "\n", ##__VA_ARGS__)
#else
    #define DBG_OTA(msg)
    #define DBG_OTA_PRINT(msg)
    #define DBG_OTA_PRINTF(fmt, ...)
#endif

// --- Debug RTC ---
#ifdef DEBUGRTC
    #define DBG_RTC(msg) Serial.println(String("[RTC] ") + msg)
    #define DBG_RTC_PRINT(msg) Serial.print(String("[RTC] ") + msg)
    #define DBG_RTC_PRINTF(fmt, ...) Serial.printf("[RTC] " fmt "\n", ##__VA_ARGS__)
#else
    #define DBG_RTC(msg)
    #define DBG_RTC_PRINT(msg)
    #define DBG_RTC_PRINTF(fmt, ...)
#endif

// --- Debug Sensores ---
#ifdef DEBUGSENSOR
    #define DBG_SENSOR(msg) Serial.println(String("[SENSOR] ") + msg)
    #define DBG_SENSOR_PRINT(msg) Serial.print(String("[SENSOR] ") + msg)
    #define DBG_SENSOR_PRINTF(fmt, ...) Serial.printf("[SENSOR] " fmt "\n", ##__VA_ARGS__)
#else
    #define DBG_SENSOR(msg)
    #define DBG_SENSOR_PRINT(msg)
    #define DBG_SENSOR_PRINTF(fmt, ...)
#endif

// --- Debug I2C ---
#ifdef DEBUGI2C
    #define DBG_I2C(msg) Serial.println(String("[I2C] ") + msg)
    #define DBG_I2C_PRINT(msg) Serial.print(String("[I2C] ") + msg)
    #define DBG_I2C_PRINTF(fmt, ...) Serial.printf("[I2C] " fmt "\n", ##__VA_ARGS__)
#else
    #define DBG_I2C(msg)
    #define DBG_I2C_PRINT(msg)
    #define DBG_I2C_PRINTF(fmt, ...)
#endif

// --- Debug SPI ---
#ifdef DEBUGSPI
    #define DBG_SPI(msg) Serial.println(String("[SPI] ") + msg)
    #define DBG_SPI_PRINT(msg) Serial.print(String("[SPI] ") + msg)
    #define DBG_SPI_PRINTF(fmt, ...) Serial.printf("[SPI] " fmt "\n", ##__VA_ARGS__)
#else
    #define DBG_SPI(msg)
    #define DBG_SPI_PRINT(msg)
    #define DBG_SPI_PRINTF(fmt, ...)
#endif

// --- Debug Bluetooth ---
#ifdef DEBUGBLE
    #define DBG_BLE(msg) Serial.println(String("[BLE] ") + msg)
    #define DBG_BLE_PRINT(msg) Serial.print(String("[BLE] ") + msg)
    #define DBG_BLE_PRINTF(fmt, ...) Serial.printf("[BLE] " fmt "\n", ##__VA_ARGS__)
#else
    #define DBG_BLE(msg)
    #define DBG_BLE_PRINT(msg)
    #define DBG_BLE_PRINTF(fmt, ...)
#endif

// --- Debug MQTT ---
#ifdef DEBUGMQTT
    #define DBG_MQTT(msg) Serial.println(String("[MQTT] ") + msg)
    #define DBG_MQTT_PRINT(msg) Serial.print(String("[MQTT] ") + msg)
    #define DBG_MQTT_PRINTF(fmt, ...) Serial.printf("[MQTT] " fmt "\n", ##__VA_ARGS__)
#else
    #define DBG_MQTT(msg)
    #define DBG_MQTT_PRINT(msg)
    #define DBG_MQTT_PRINTF(fmt, ...)
#endif

// --- Debug Database ---
#ifdef DEBUGDATABASE
    #define DBG_DB(msg) Serial.println(String("[DB] ") + msg)
    #define DBG_DB_PRINT(msg) Serial.print(String("[DB] ") + msg)
    #define DBG_DB_PRINTF(fmt, ...) Serial.printf("[DB] " fmt "\n", ##__VA_ARGS__)
#else
    #define DBG_DB(msg)
    #define DBG_DB_PRINT(msg)
    #define DBG_DB_PRINTF(fmt, ...)
#endif

// --- Debug File System ---
#ifdef DEBUGFILE
    #define DBG_FILE(msg) Serial.println(String("[FILE] ") + msg)
    #define DBG_FILE_PRINT(msg) Serial.print(String("[FILE] ") + msg)
    #define DBG_FILE_PRINTF(fmt, ...) Serial.printf("[FILE] " fmt "\n", ##__VA_ARGS__)
#else
    #define DBG_FILE(msg)
    #define DBG_FILE_PRINT(msg)
    #define DBG_FILE_PRINTF(fmt, ...)
#endif

// --- Debug Telegram ---
#ifdef DEBUGTELEGRAM
    #define DBG_TELEGRAM(msg) Serial.println(String("[TELEGRAM] ") + msg)
    #define DBG_TELEGRAM_PRINT(msg) Serial.print(String("[TELEGRAM] ") + msg)
    #define DBG_TELEGRAM_PRINTF(fmt, ...) Serial.printf("[TELEGRAM] " fmt "\n", ##__VA_ARGS__)
#else
    #define DBG_TELEGRAM(msg)
    #define DBG_TELEGRAM_PRINT(msg)
    #define DBG_TELEGRAM_PRINTF(fmt, ...)
#endif

// --- Debug Personalizado 1 ---
#ifdef DEBUGCUSTOM1
    #define DBG_CUSTOM1(msg) Serial.println(String("[CUSTOM1] ") + msg)
    #define DBG_CUSTOM1_PRINT(msg) Serial.print(String("[CUSTOM1] ") + msg)
    #define DBG_CUSTOM1_PRINTF(fmt, ...) Serial.printf("[CUSTOM1] " fmt "\n", ##__VA_ARGS__)
#else
    #define DBG_CUSTOM1(msg)
    #define DBG_CUSTOM1_PRINT(msg)
    #define DBG_CUSTOM1_PRINTF(fmt, ...)
#endif

// --- Debug Personalizado 2 ---
#ifdef DEBUGCUSTOM2
    #define DBG_CUSTOM2(msg) Serial.println(String("[CUSTOM2] ") + msg)
    #define DBG_CUSTOM2_PRINT(msg) Serial.print(String("[CUSTOM2] ") + msg)
    #define DBG_CUSTOM2_PRINTF(fmt, ...) Serial.printf("[CUSTOM2] " fmt "\n", ##__VA_ARGS__)
#else
    #define DBG_CUSTOM2(msg)
    #define DBG_CUSTOM2_PRINT(msg)
    #define DBG_CUSTOM2_PRINTF(fmt, ...)
#endif

// ============================================================================
// FUNCIONES AUXILIARES DE DEBUG
// ============================================================================

/**
 * @brief Clase helper para funciones avanzadas de debug
 * 
 * @details Proporciona métodos estáticos para mostrar información del sistema,
 *          memoria, arrays, etc. Solo se compilan si DEBUG está definido.
 */
class DebugHelper {
public:
    /**
     * @brief Muestra memoria libre disponible
     */
    static void printFreeHeap() {
        #ifdef DEBUG
        Serial.printf("[DEBUG] Memoria libre: %d bytes\n", ESP.getFreeHeap());
        #endif
    }
    
    /**
     * @brief Muestra información completa del sistema
     */
    static void printSystemInfo() {
        #ifdef DEBUG
        Serial.println("[DEBUG] === INFORMACIÓN DEL SISTEMA ===");
        Serial.printf("[DEBUG] CPU Freq: %d MHz\n", ESP.getCpuFreqMHz());
        Serial.printf("[DEBUG] Flash Size: %d bytes\n", ESP.getFlashChipSize());
        Serial.printf("[DEBUG] Free Heap: %d bytes\n", ESP.getFreeHeap());
        Serial.printf("[DEBUG] Chip ID: %08X\n", (uint32_t)ESP.getEfuseMac());
        Serial.println("[DEBUG] ================================");
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
    
    /**
     * @brief Imprime separador visual en logs
     */
    static void printSeparator() {
        #ifdef DEBUG
        Serial.println("[DEBUG] =====================================");
        #endif
    }
};

// ============================================================================
// INSTRUCCIONES DE USO
// ============================================================================

/*
 * CÓMO USAR ESTE ARCHIVO EN TU PROYECTO:
 * 
 * 1. COPIAR A TU PROYECTO:
 *    - Copia este archivo Debug.h a la carpeta de tu proyecto
 * 
 * 2. PERSONALIZAR MÓDULOS:
 *    - Descomenta los #define de los módulos que necesites
 *    - Añade más módulos copiando el patrón existente
 * 
 * 3. USAR EN TU CÓDIGO:
 *    #include "Debug.h"
 *    
 *    void setup() {
 *        Serial.begin(115200);
 *        DBG("Sistema iniciado");
 *        DebugHelper::printSystemInfo();
 *    }
 *    
 *    void loop() {
 *        DBG_WIFI("Conectando...");
 *        DBG_PRINTF("Valor: %d", miVariable);
 *    }
 * 
 * 4. COMPILAR PRODUCCIÓN:
 *    - Comenta TODOS los #define al principio del archivo
 *    - El compilador eliminará todo el código de debug
 *    - Resultado: binario más pequeño y rápido
 * 
 * 5. AÑADIR MÓDULOS PERSONALIZADOS:
 *    - Copia el bloque de un módulo existente
 *    - Cambia el nombre (ej: DEBUGCUSTOM3)
 *    - Define las macros DBG_CUSTOM3, etc.
 */

#endif // DEBUG_H
