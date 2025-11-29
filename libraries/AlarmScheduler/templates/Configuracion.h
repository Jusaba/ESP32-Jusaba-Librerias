/**
 * @file Configuracion.h
 * @brief Configuration template for AlarmScheduler library
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
// PROJECT INFORMATION
// ============================================================================
namespace Config {
    namespace Project {
        const char NAME[]    = "MyAlarmProject";
        const char VERSION[] = "1.0.0";
        const char AUTHOR[]  = "Your Name";
    }

    // ========================================================================
    // SERIAL CONFIGURATION
    // ========================================================================
    namespace Serial {
        const unsigned long BAUD_RATE = 115200;
    }

    // ========================================================================
    // ALARMSCHEDULER CONFIGURATION
    // ========================================================================
    namespace Alarms {
        // JSON persistence file path
        const char JSON_FILE[] = "/customizable_alarms.json";
        
        // Load default alarms if JSON file doesn't exist
        const bool LOAD_DEFAULTS = true;
        
        // Auto-save on modifications
        const bool AUTO_SAVE = true;
        
        // Maximum customizable alarms (system uses AlarmScheduler::MAX_ALARMS = 16)
        const uint8_t MAX_CUSTOMIZABLE = 10;
    }

    // ========================================================================
    // SPIFFS CONFIGURATION
    // ========================================================================
    namespace Storage {
        const bool FORMAT_ON_FAIL = true;
        const size_t MAX_OPEN_FILES = 5;
    }

    // ========================================================================
    // NTP/RTC CONFIGURATION (if using RTCManager)
    // ========================================================================
    namespace Time {
        const char NTP_SERVER1[] = "pool.ntp.org";
        const char NTP_SERVER2[] = "time.nist.gov";
        const char NTP_SERVER3[] = "time.google.com";
        
        const long GMT_OFFSET_SEC = 3600;        // GMT+1 (Spain)
        const int  DAYLIGHT_OFFSET_SEC = 3600;   // DST +1 hour
        
        const char TIMEZONE[] = "CET-1CEST,M3.5.0,M10.5.0/3";  // Europe/Madrid
    }

    // ========================================================================
    // ALARM ACTION TYPES (customize for your project)
    // ========================================================================
    namespace AlarmTypes {
        const char TYPE_BELL[]        = "BELL";
        const char TYPE_LIGHT[]       = "LIGHT";
        const char TYPE_NOTIFICATION[] = "NOTIFICATION";
        const char TYPE_CUSTOM[]      = "CUSTOM";
        const char TYPE_SYSTEM[]      = "SYSTEM";
    }

    // ========================================================================
    // ALARM PARAMETERS (example constants)
    // ========================================================================
    namespace AlarmParams {
        // Bell durations (in seconds)
        const uint16_t BELL_SHORT  = 5;
        const uint16_t BELL_MEDIUM = 10;
        const uint16_t BELL_LONG   = 30;
        
        // Light zones
        const uint16_t LIGHT_ZONE_1 = 1;
        const uint16_t LIGHT_ZONE_2 = 2;
        const uint16_t LIGHT_ZONE_ALL = 0;
    }
}

#endif // CONFIGURACION_H
