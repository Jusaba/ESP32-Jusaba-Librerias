/**
 * @file Debug.h
 * @brief Sistema de debug genérico para proyectos ESP32
 * 
 * @details Este es un archivo GENÉRICO con macros de debug básicas.
 *          Proporciona solo lo esencial: debug general del sistema.
 *          
 *          Las librerías específicas pueden tener sus propios archivos
 *          Debug.h en sus carpetas templates/ con macros adicionales
 *          (ej: DEBUGWIFI, DEBUGRTC) y helpers específicos que puedes
 *          copiar y fusionar según necesites.
 * 
 * **CARACTERÍSTICAS:**
 * - Debug general del sistema (DBG, DBG_PRINT, DBG_PRINTF)
 * - Cero overhead en producción (se elimina al compilar)
 * - Fácil de extender con macros de librerías específicas
 * 
 * **USO:**
 * 1. Copia este archivo a tu proyecto
 * 2. Define o comenta #define DEBUG según necesites
 * 3. Si usas librerías específicas, copia sus macros adicionales desde
 *    libraries/NombreLibreria/templates/Debug.h
 * 
 * @author Julian Salas Bartolomé
 * @date 2025-11-29
 * @version 1.0.0
 */

#ifndef DEBUG_H
#define DEBUG_H

#include <Arduino.h>

// ============================================================================
// CONFIGURACIÓN DE DEBUG
// ============================================================================

// Debug general - Comentar para desactivar en producción
#define DEBUG

// ============================================================================
// MACROS DE DEBUG BÁSICAS
// ============================================================================

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

// ============================================================================
// CLASE HELPER DE DEBUG - Funciones universales
// ============================================================================

/**
 * @brief Clase helper con funciones útiles de debug universales
 * 
 * @details Proporciona métodos estáticos para mostrar información del sistema.
 *          Solo incluye funciones UNIVERSALES que funcionan en cualquier ESP32
 *          sin dependencias externas (WiFi.h, time.h, etc.).
 *          
 *          Las librerías específicas pueden extender esta clase con funciones
 *          adicionales en sus propios templates/Debug.h
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
 * GUÍA RÁPIDA:
 * 
 * 1. ACTIVAR/DESACTIVAR DEBUG:
 *    
 *    Durante desarrollo:
 *    #define DEBUG              // ← Activado
 *    
 *    Para producción:
 *    //#define DEBUG            // ← Desactivado (comentado)
 * 
 * 2. USAR EN TU CÓDIGO:
 *    
 *    #include "Debug.h"
 *    
 *    void setup() {
 *        Serial.begin(115200);
 *        DBG("Sistema iniciado");
 *        DBG_PRINTF("Valor: %d", miValor);
 *    }
 * 
 * 3. AÑADIR MACROS DE LIBRERÍAS ESPECÍFICAS:
 *    
 *    Si usas RTCManager, ve a libraries/RTCManager/templates/Debug.h
 *    y copia las secciones que necesites:
 *    
 *    ```cpp
 *    // Añadir después de las macros básicas:
 *    
 *    // Debug WiFi (copiado de RTCManager/templates/Debug.h)
 *    //#define DEBUGWIFI
 *    
 *    #ifdef DEBUGWIFI
 *        #define DBG_WIFI(msg) Serial.println(String("[WIFI] ") + msg)
 *        #define DBG_WIFI_PRINT(msg) Serial.print(String("[WIFI] ") + msg)
 *        #define DBG_WIFI_PRINTF(fmt, ...) Serial.printf("[WIFI] " fmt "\n", ##__VA_ARGS__)
 *    #else
 *        #define DBG_WIFI(msg)
 *        #define DBG_WIFI_PRINT(msg)
 *        #define DBG_WIFI_PRINTF(fmt, ...)
 *    #endif
 *    
 *    // Debug RTC (copiado de RTCManager/templates/Debug.h)
 *    //#define DEBUGRTC
 *    
 *    #ifdef DEBUGRTC
 *        #define DBG_RTC(msg) Serial.println(String("[RTC] ") + msg)
 *        #define DBG_RTC_PRINT(msg) Serial.print(String("[RTC] ") + msg)
 *        #define DBG_RTC_PRINTF(fmt, ...) Serial.printf("[RTC] " fmt "\n", ##__VA_ARGS__)
 *    #else
 *        #define DBG_RTC(msg)
 *        #define DBG_RTC_PRINT(msg)
 *        #define DBG_RTC_PRINTF(fmt, ...)
 *    #endif
 *    ```
 * 
 * 4. AÑADIR TUS PROPIAS MACROS:
 *    
 *    Crea macros personalizadas para tus módulos:
 *    
 *    ```cpp
 *    // Debug de tu módulo específico
 *    //#define DEBUG_MIMODULO
 *    
 *    #ifdef DEBUG_MIMODULO
 *        #define DBG_MIMODULO(msg) Serial.println(String("[MIMODULO] ") + msg)
 *    #else
 *        #define DBG_MIMODULO(msg)
 *    #endif
 *    ```
 * 
 * 5. MACROS Y FUNCIONES DISPONIBLES:
 *    
 *    MACROS BÁSICAS:
 *    - DBG(msg)                    // Mensaje con salto de línea
 *    - DBG_PRINT(msg)              // Mensaje sin salto de línea
 *    - DBG_PRINTF(fmt, ...)        // Mensaje con formato printf
 *    
 *    FUNCIONES HELPER UNIVERSALES:
 *    - DebugHelper::printFreeHeap()      // Memoria libre
 *    - DebugHelper::printSystemInfo()    // Info completa sistema
 *    - DebugHelper::printSeparator()     // Línea separadora
 *    - DebugHelper::printArray(name, arr, size)  // Contenido array
 * 
 * VENTAJAS:
 * ✅ Mínimo y esencial - fácil de entender
 * ✅ Cero overhead en producción
 * ✅ Fácil de extender con macros de librerías
 * ✅ Incluye helpers universales (sin dependencias externas)
 * ✅ Helpers específicos de librerías van en sus templates/
 * 
 * 6. USAR FUNCIONES HELPER:
 *    
 *    void setup() {
 *        Serial.begin(115200);
 *        
 *        DebugHelper::printSeparator();
 *        DBG("Sistema iniciado");
 *        DebugHelper::printSystemInfo();
 *        DebugHelper::printFreeHeap();
 *        DebugHelper::printSeparator();
 *    }
 * 
 * IMPORTANTE:
 * - Este archivo tiene macros BÁSICAS y helpers UNIVERSALES
 * - Helpers universales = Solo ESP32 nativo (sin WiFi, time, Wire, etc.)
 * - Para helpers específicos (printWiFiStatus, printCurrentTime, etc.)
 *   usa el Debug.h de cada librería y cópialos a tu proyecto
 */

#endif // DEBUG_H
