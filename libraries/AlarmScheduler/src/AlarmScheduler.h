/**
 * @file AlarmScheduler.h
 * @brief Advanced alarm scheduling system for ESP32 with web management
 * 
 * @details Generic alarm system that allows executing specific actions at predetermined times,
 *          with complete support for web-based customizable alarm management:
 *          
 *          **IMPLEMENTED FEATURES:**
 *          - Programmable alarm system with day-of-week masks
 *          - Support for specific times and wildcards (any hour/minute)
 *          - Interval alarms that repeat every X minutes
 *          - Three action types: member methods, external functions with/without parameters
 *          - Duplicate execution prevention in same minute/day
 *          - Temporal cache system to avoid multiple triggers
 *          - Complete RTC integration and NTP synchronization
 *          - Detailed execution and alarm state debugging
 *          - **WEB MANAGEMENT:** Creation, editing, deletion via web interface
 *          - **JSON PERSISTENCE:** Automatic storage in SPIFFS
 *          - **UNIQUE IDS:** Independent web identification system
 *          - **DYNAMIC CALLBACKS:** Action configuration from external code
 *          
 *          **SUPPORTED ALARM TYPES:**
 *          1. **Fixed schedule:** Specific day + exact hour + minute
 *          2. **Wildcard:** Any hour (*) and/or any minute (*)
 *          3. **Interval:** Repetition every X minutes from anchor point
 *          4. **Multiple days:** Bit mask for days of the week
 *          5. **CUSTOMIZABLE:** Web-editable alarms with persistence
 *          6. **SYSTEM:** Predefined alarms not user-editable
 *          
 *          **WEB CUSTOMIZABLE ALARM MANAGEMENT:**
 *          - Dynamic creation with name, description and action type
 *          - Complete editing preserving configured callbacks
 *          - Safe deletion with automatic array reorganization
 *          - Individual enable/disable by web ID
 *          - JSON export for web interface (complete list + statistics)
 *          - Automatic persistence in /customizable_alarms.json
 *          - Automatic loading at system startup
 *          - Unique web IDs independent of array index
 *          
 *          **DUPLICATE PREVENTION:**
 *          - Cache by year day (lastYearDay) for daily alarms
 *          - Cache by minute (lastMinute) for same-day alarms
 *          - Epoch timestamp (lastExecution) for interval alarms
 * 
 * @note **TIME CONFIGURATION:**
 *       - 24-hour format (0-23 for tm_hour)
 *       - Minutes 0-59 (tm_min)
 *       - Days 0-6 where 0=Sunday, 6=Saturday (tm_wday)
 * 
 * @warning **CRITICAL DEPENDENCIES:**
 *          - time.h: System time functions (getLocalTime, time_t)
 *          - ArduinoJson.h: JSON serialization/deserialization for persistence
 *          - SPIFFS.h: File system for persistent storage
 * 
 * @warning **LIMITATIONS:**
 *          - Maximum 16 simultaneous alarms total (system + customizable)
 *          - Minimum resolution of 1 minute (no second support)
 *          - Cache not persistent (lost on reboot)
 *          - RTC verification required for operation
 *          - **SPIFFS:** Requires sufficient space for JSON file
 *          - **CALLBACKS:** Must be configured externally before creating alarms
 *          - **THREAD SAFETY:** Not thread-safe, use from main thread only
 * 
 * @author Julian Salas Bartolomé
 * @date 2025-11-29
 * @version 1.0.0
 * 
 * @since v1.0.0 - First release as independent library
 */

#ifndef ALARMSCHEDULER_H
#define ALARMSCHEDULER_H

#include <time.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>

// Debug configuration (uncomment to enable)
// #define ALARMSCHEDULER_DEBUG

#ifdef ALARMSCHEDULER_DEBUG
    #define DBG_ALM(x) Serial.println("[ALARM] " + String(x))
    #define DBG_ALM_PRINT(x) Serial.print("[ALARM] " + String(x))
    #define DBG_ALM_PRINTF(fmt, ...) Serial.printf("[ALARM] " fmt "\n", ##__VA_ARGS__)
#else
    #define DBG_ALM(x)
    #define DBG_ALM_PRINT(x)
    #define DBG_ALM_PRINTF(fmt, ...)
#endif

// Day masks (bit0 = Sunday ... bit6 = Saturday)
// Spanish names
enum : uint8_t {
    DOW_DOMINGO    = 1 << 0,
    DOW_LUNES      = 1 << 1,
    DOW_MARTES     = 1 << 2,
    DOW_MIERCOLES  = 1 << 3,
    DOW_JUEVES     = 1 << 4,
    DOW_VIERNES    = 1 << 5,
    DOW_SABADO     = 1 << 6,
    DOW_TODOS      = 0x7F
};

// English aliases
enum : uint8_t {
    DOW_SUNDAY     = DOW_DOMINGO,
    DOW_MONDAY     = DOW_LUNES,
    DOW_TUESDAY    = DOW_MARTES,
    DOW_WEDNESDAY  = DOW_MIERCOLES,
    DOW_THURSDAY   = DOW_JUEVES,
    DOW_FRIDAY     = DOW_VIERNES,
    DOW_SATURDAY   = DOW_SABADO,
    DOW_ALL        = DOW_TODOS
};

#define ALARMA_WILDCARD 255   // wildcard (*)
#define ALARM_WILDCARD  255   // English alias

class AlarmScheduler; // forward declaration

/**
 * @brief Alarm structure containing all alarm configuration and state
 */
struct Alarm {
    bool     enabled             = false;                       // Alarm enabled state
    uint8_t  dayMask             = DOW_ALL;                     // Day mask (bit0=Sunday ... bit6=Saturday)
    uint8_t  hour                = 0;                           // Hour (0-23 or ALARM_WILDCARD)
    uint8_t  minute              = 0;                           // Minute (0-59 or ALARM_WILDCARD)
    uint16_t intervalMin         = 0;                           // Interval (minutes)
    int16_t  lastYearDay         = -1;                          // Last year day
    uint8_t  lastMinute          = 255;                         // Last minute  
    uint8_t  lastHour            = 255;                         // Last executed hour (255 initial)
    time_t   lastExecution       = 0;                           // Last execution timestamp
    void     (AlarmScheduler::*action)(uint16_t) = nullptr;     // Member method
    void     (*externalAction)(uint16_t) = nullptr;             // External function with parameter
    void     (*externalAction0)() = nullptr;                    // External function without parameter
    uint16_t parameter           = 0;                           // Action parameter  
    
    // Fields for web customization
    char     name[50];                                          // Descriptive name
    char     description[100];                                  // Optional description  
    char     typeString[20];                                    // Generic type string
    bool     isCustomizable;                                    // true = editable via web, false = system
    int      webId = -1;                                        // Unique ID for web interface (-1 if not applicable)  
    
    // Constructor to initialize new fields
    Alarm() : isCustomizable(false), webId(-1) {
        name[0] = '\0';
        description[0] = '\0';
        strcpy(typeString, "SYSTEM");
    }    
};

/**
 * @brief Advanced alarm scheduler class with web management support
 */
class AlarmScheduler {
public:
    static constexpr uint8_t MAX_ALARMS = 16;
    struct tm t;

    // ========================================================================
    // PUBLIC METHODS - Available in Spanish and English
    // MÉTODOS PÚBLICOS - Disponibles en español e inglés
    // ========================================================================
    
    bool begin(bool loadDefaults = false);
    void check();
    
    // Add alarms (system alarms)
    uint8_t add(uint8_t dayMask,
                uint8_t hour,
                uint8_t minute,
                uint16_t intervalMin,
                void (AlarmScheduler::*action)(uint16_t),
                uint16_t parameter = 0,
                bool enabled = true);
                
    uint8_t addExternal(uint8_t dayMask,
                        uint8_t hour,
                        uint8_t minute,
                        uint16_t intervalMin,
                        void (*ext)(uint16_t),
                        uint16_t parameter = 0,
                        bool enabled = true);      
                        
    uint8_t addExternal0(uint8_t dayMask,
                         uint8_t hour,
                         uint8_t minute,
                         uint16_t intervalMin,
                         void (*ext0)(),
                         bool enabled = true);
    
    // Alarm management
    void disable(uint8_t idx);
    void enable(uint8_t idx);
    void clear();
    uint8_t count() const;
    const Alarm* get(uint8_t idx) const;
    Alarm* getMutable(uint8_t idx);
    void resetCache();
    
    // ========================================================================
    // WEB CUSTOMIZABLE ALARM MANAGEMENT
    // GESTIÓN WEB DE ALARMAS PERSONALIZABLES
    // ========================================================================
    
    // Spanish names
    uint8_t addPersonalizable(const char* nombre, const char* descripcion,
                              uint8_t mascaraDias, uint8_t hora, uint8_t minuto,
                              const char* tipoString, uint16_t parametro,
                              void (*callback)(uint16_t), bool habilitada = true);
    
    bool modificarPersonalizable(int idWeb, const char* nombre, const char* descripcion,
                                 uint8_t mascaraDias, uint8_t hora, uint8_t minuto,
                                 const char* tipoString, bool habilitada,
                                 void (*callback)(uint16_t), uint16_t parametro);
    
    bool eliminarPersonalizable(int idWeb);
    bool habilitarPersonalizable(int idWeb, bool estado);
    String obtenerPersonalizablesJSON();
    String obtenerEstadisticasJSON();
    bool cargarPersonalizablesDesdeJSON();
    bool guardarPersonalizablesEnJSON();
    
    // English aliases
    uint8_t addCustomizable(const char* name, const char* description,
                            uint8_t dayMask, uint8_t hour, uint8_t minute,
                            const char* typeString, uint16_t parameter,
                            void (*callback)(uint16_t), bool enabled = true);
    
    bool modifyCustomizable(int webId, const char* name, const char* description,
                            uint8_t dayMask, uint8_t hour, uint8_t minute,
                            const char* typeString, bool enabled,
                            void (*callback)(uint16_t), uint16_t parameter);
    
    bool deleteCustomizable(int webId);
    bool enableCustomizable(int webId, bool state);
    String getCustomizablesJSON();
    String getStatisticsJSON();
    bool loadCustomizablesFromJSON();
    bool saveCustomizablesToJSON();
    
    // Debug
    void printAllAlarms();

private:
    Alarm  _alarms[MAX_ALARMS];
    uint8_t _num = 0;
    int     _nextWebId = 1;

    // Helper methods
    static uint8_t _dayMaskFromWeekday(int weekday);
    uint8_t _findIndexByWebId(int webId);
    int     _generateNewWebId();
    String  _dayToString(int day);
    void    _createDefaultCustomizableAlarms();
};

#endif // ALARMSCHEDULER_H
